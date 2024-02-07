#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>

#define pipeName    "/tmp/fifo1"
#define maxBufSize  128

class Reader
{
public:
    bool checkData(const std::string &data) const;
    bool processData(std::string &data);
    void processExit();
private:
    int  m_fd;
};