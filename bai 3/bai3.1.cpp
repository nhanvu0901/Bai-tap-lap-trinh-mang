// GroupChatServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//BT1:



#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#define MAX_LINE 10000

//const MAX_CLIENT = 64;
int numClient = 0;
SOCKET clients[64];
char buf[256];
FILE* fp = NULL;
char line[MAX_LINE];
char** arrayUser = (char**)malloc(10000 * 200 * sizeof(char));
char receive[100];




DWORD WINAPI SendThread(LPVOID lpParam) {
    SOCKET client = *(SOCKET*)lpParam;

    for (int i = 0; i < numClient; i++)
    {

        if (clients[i] != client)
        {
            send(clients[i], buf, strlen(buf), 0);
        }
    }
    return 0;
}




DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    clients[numClient] = client;
    numClient++;


    int ret;
    // Nhan du lieu tu client va in ra man hinh
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Client %d sent: %s\n", client, buf);
        CreateThread(0, 0, SendThread, &client, 0, 0);
    }
    closesocket(client);
}
int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    int count = 0;

    fp = fopen("D:\\test\\test.txt", "r");

    while (fgets(line, 10000, fp)) {
        arrayUser[count] = strdup(line);
        count++;
    }
    fclose(fp);




    while (1)
    {

        SOCKET client = accept(listener, NULL, NULL);
        printf("Client moi ket noi: %d\n", client);

        const char* initial = "\nPlease input user id:\n";
        boolean flag = false;
        while (flag == false) {

            send(client, initial, strlen(initial), 0);
            int ret = recv(client, receive, sizeof(receive), 0);
            receive[ret] = 0;
            printf("\n%s\n", receive);

            for (int i = 0; i < count; i++) {
                if (strcmp(receive, arrayUser[i]) == 0) {
                    flag = true;
                    break;
                }
            }
        }
        printf("Dang nhap thanh cong\n");
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }


    for (int i = 0; i < count; ++i) free(arrayUser[i]);
    free(arrayUser);
}