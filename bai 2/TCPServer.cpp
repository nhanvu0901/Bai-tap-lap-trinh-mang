



// MultithreadServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    ​
        SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    // Nhan du lieu tu client va in ra man hinh
    while (1)
    {
        ​
            ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
    }​
        closesocket(client);
}​
int main()
{
    ​
        // Khoi tao thu vien
        WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    while (1)
    {
        ​
            SOCKET client = accept(listener, NULL, NULL);
        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }​
}​





