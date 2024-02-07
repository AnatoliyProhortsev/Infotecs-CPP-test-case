#include "handler.hpp"

std::string Handler::readData()
{
    m_fd = open(pipeName, O_RDONLY);
    if(m_fd < 0)
        return "error";
    
    char msg[maxBufSize] = {0};
    read(m_fd, msg, maxBufSize);

    std::string data{msg};
    return data;
}

bool Handler::processData(const std::string &data)
{
    //Подсчёт суммы
    unsigned short sum = 0;
    for(char c: data)
    {
        if(std::isdigit(c))
            sum += c - '0';
    }
        
    printf("H: sum of last data -> %hu\n", sum);

    //Открытие сокета
    m_clientId = socket(AF_INET, SOCK_STREAM, 0);
    if(m_clientId == -1)
    {
        printf("H: error when try to init socket.\n");
        return false;
    }

    //Параметры подключения
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Подключение к сокету
    if(connect(m_clientId, (struct sockaddr*)&serverAddr, sizeof(serverAddr))<0)
    {
        printf("H: error when try to connect via socket.\n");
        return false;
    }
    printf("H: connection established.\n");

    //Отправка сообщения
    char dataToSend[3];
    sprintf(dataToSend, "%hu", sum);
    send(m_clientId, dataToSend, strlen(dataToSend), 0);
    close(m_clientId);

    return true;
}