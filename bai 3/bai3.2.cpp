// GroupChatServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//BT1:



#include <WinSock2.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
using namespace std;
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

#define MAX_LINE 10000

//const MAX_CLIENT = 64;
int numClient = 0;
SOCKET clients[64];
char buf[256];

char line[MAX_LINE];
char uname[21], passw[20];
char receive[100];
char receiveUser[100];
char receivePass[100];




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
    
    fstream fp;
    fp.open("D:\\test\\test.txt",  ios::in);

   
    
 




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
            
            int j;
            for (int i = 0; i < strlen(receive); i++) {
                if (receive[i] == ' ') {
                    receiveUser[i] = 0;
                    break;
                }
                receiveUser[i] = receive[i];
            }
           
            j = strlen(receiveUser) + 1;
            for (int k = 0; k < strlen(receive) - j - 1; k++) {
                receivePass[k] = receive[j + k];
            }
            receivePass[strlen(receive) - j - 1] = 0;
           

			
            while (!fp.eof()) {
                fp >> uname >> passw;
                if (strcmp(uname, receiveUser) == 0) {
                    
                    if (strcmp(passw, receivePass) == 0) {
                        flag = TRUE;
                        
                        break;
                    }
                }
            }
            if (fp.is_open()) {
                fp.clear();
                fp.seekg(0, ios::beg);
            }
        }
        printf("Dang nhap thanh cong\n");
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }


   
}