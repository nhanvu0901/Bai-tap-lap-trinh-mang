#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

SOCKET clients[64];
int numClients = 0;
void RemoveClient(SOCKET client) {
    int i = 0;
    for (; i < numClients; i++)
        if (clients[i] == client) break;

    if (i < numClients - 1) {
        clients[i] = clients[numClients - 1];
        numClients--;
    }

}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    char user[32], pass[32], tmp[32];
    // Xu ly dang nhap
    const char* msg = "Nhap theo cu phap : Mssv,pass.\n";
    send(client, msg, strlen(msg), 0);
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            return 0;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
        ret = sscanf(buf, "%s %s %s", user, pass, tmp);
        if (ret != 2)
        {
            const char* msg = "Sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
        else
        {
            FILE* f = fopen("D:\\test\\test.txt", "r");
            int found = 0;
            while (!feof(f))
            {
                char line[256];
                fgets(line, sizeof(line), f);
                char userf[32], passf[32];
                sscanf(line, "%s %s", userf, passf);
                if (strcmp(user, userf) == 0 && strcmp(pass, passf) == 0)
                {
                    found = 1;
                    break;
                }
            }
            if (found == 1)
            {
                const char* msg = "Dang nhap thanh cong. Hay nhap lenh.\n";
                send(client, msg, strlen(msg), 0);


                FILE* fdiem = fopen("D:\\test\\receive.txt", "rb");
                int foundDiem = 0;
                while (!feof(f))
                {
                    char line[256];
                    char* diem;
                    fgets(line, sizeof(line), f);
                    char name[32], mark[32], mssv[256];
                    sscanf(line, "%s %s %s", name, mark, mssv);
                    for (int i = 0; i < numClients; i++)
                    {
                        if (strcmp(user, mssv) == 0) {
                            foundDiem = 1;
                            diem = mark;
                            break;
                        }
                    }
                    if (foundDiem == 1) {
                        char result[256];
                        sprintf(result, "%s %s", "OK", diem);
                        send(client, result, sizeof(result), 0);
                        fclose(fdiem);
                        break;
                    }
                    else {
                        const char* msg = "Khong tim thay diem so.\n";
                        send(client, msg, strlen(msg), 0);
                    }
                }

                fclose(f);
                break;
            }
            else
            {
                const char* msg = "Khong tim thay tai khoan. Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
            }

        }
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
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}