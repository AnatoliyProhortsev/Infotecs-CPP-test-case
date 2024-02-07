#include "reader.hpp"

bool Reader::checkData(const std::string &data) const
{
    return (data.length() <= 64 &&
     !data.empty() &&
     std::find_if(
        data.begin(), 
        data.end(),
         [](unsigned char c)
            { return !std::isdigit(c); }
                ) == data.end() );
}

bool Reader::processData(std::string &data)
{
    //Производим замену
    auto is_even = [](char ch)
    {return std::isdigit(static_cast<unsigned char>(ch)) && ((ch - '0') % 2) == 0;};
    std::string_view data_view = "KB";
    auto top = std::find_if(data.begin(), data.end(), is_even) - data.begin();
    for (std::string::size_type pos{};
         (pos = (std::find_if(
            data.begin() + pos, data.end(), is_even) - data.begin())) < data.length();
         pos += data_view.length() - 1)
    {
        data.replace(pos, 1, data_view.data());
    }
    printf("R: data to write to buf -> %s\n", data.c_str());

    //Открываем общий буфер
    m_fd = open(pipeName, O_WRONLY | O_NDELAY);
    if(m_fd < 0)
        return false;

    char dataToSend[maxBufSize];
    strcpy(dataToSend, data.c_str());

    //Записываем данные
    if(write(m_fd, dataToSend, strlen(dataToSend)) == strlen(dataToSend))
    {
        printf("R: pipe write succeded.\n");
        close(m_fd);
        return true;
    } 
    else
    {
        printf("R: pipe write failed.\n");
        close(m_fd);
        return false;
    }
    
}

void Reader::processExit()
{
    //Открываем общий буфер
    m_fd = open(pipeName, O_WRONLY | O_NDELAY);
    if(m_fd < 0)
        return;

    //Записываем команду выхода
    if(write(m_fd, "exit", 5) == 5)
    {
        printf("R: exit pipe write succeded.\n");
        close(m_fd);
    } 
    else
    {
        printf("R: exit pipe write failed.\n");
        close(m_fd);
    }
}