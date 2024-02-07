#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
    //Заводим сокет
    int serverId = socket(AF_INET, SOCK_STREAM, 0);
    if(serverId == -1)
    {
        printf("server: error when tried to open a socket.\n");
        return -1;
    }

    //Даём параметры сокету
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;

    //Подключаем сокет к адресу
    if(bind(serverId, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        printf("server: error when tried to bind a socket.\n");
        return -1;
    }

    //Начинаем слушать сокет на 1 активное подключение
    if(listen(serverId, 1) == -1)
    {
        printf("server: error when tried to start to listen to socket.\n");
        return -1;
    }

    //Принимаем входящее подключение
    int connection;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    while(true)
    {
        printf("server: awaiting connection.\n");
        connection = accept(serverId, (struct sockaddr*)&clientAddr, &clientAddrLen);

        if(connection < 0)
        {
            printf("server: connection failed.\n");
            continue;
        }
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        printf("server: connecion established.\n");

        char message[4];
        memset(message, 0, sizeof(message));
        int len = recv(connection, message, 3, 0);
        message[len] = '\0';

        if(strlen(message) > 2 && atoi(message) % 32 == 0)
            printf("server: catched a valid message -> %s\n", message);
        else
            printf("server: catched a non-valid message.\n");
    }
    return 0;
}