#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

#include "handler.hpp"
#include "reader.hpp"

int main(int argc, char** argv)
{
    //Удаляем именованный канал, если вызникала ошибка (канал был не удалён)
    unlink(pipeName);

    //Создаём именованный канал
    if( mkfifo(pipeName, S_IRWXO | S_IRWXG | S_IRWXU) )
    {
        perror("client: Failed to create pipe. Exit...\n");
        return -1;
    }
    printf("client: Pipe created.\n");

    if(fork())
    {
        //Работа потока 2
        Handler handler;
        std::string data;

        while(data != "exit")
        {
            data = handler.readData();

            if(data != "exit")
                std::cout<<"H: catched data -> "<<data<<'\n';
            else
            {
                printf("H: catched exit command.\n");
                exit(0);
            }
            
            //Отправляем данные в программу 2
            handler.processData(data);
        }        
    }
    else
    {
        //Работа потока 1
        Reader reader;
        std::string data;

        while(data != "exit")
        {
            printf("R: enter a data ->");
            std::cin >> data;
            if(data == "error")
            {
                printf("R: error when tried to read from console.\n");
                sleep(1);
                continue;
            }
            if(data != "exit" && !reader.checkData(data))
            {
                printf("R: error - string is not valid.\n");
                sleep(1);
                continue;
            }
            if(data == "exit")
                reader.processExit();
            else
                //Отправялем данные в канал
                reader.processData(data);
            sleep(1);
        }
        
        int handler_rv;
        wait(&handler_rv);

        unlink(pipeName);
        exit(0);
    } 
}