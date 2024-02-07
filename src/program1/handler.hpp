#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>
#include <cstring>

#define pipeName "/tmp/fifo1"
#define maxBufSize  128

class Handler
{
public:
    std::string readData();
    bool        processData(const std::string &data);
private:
    int         m_fd;
    int         m_clientId;
};