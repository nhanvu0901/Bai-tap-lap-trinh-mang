// Tao client nhan file tu server.
//

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }



    char buf[2048];

    ret = recv(client, buf, sizeof(buf), 0);
    buf[ret] = 0;
    printf("%s\n", buf);


    char input[256];
    while (1) {
        printf("Nhap xau ky tu: ");
        fgets(input, sizeof(input), stdin);
        if (strncmp(input, "exit", 4) == 0)
            break;

        send(client, input, strlen(input), 0);

    }






    closesocket(client);
    WSACleanup();
}