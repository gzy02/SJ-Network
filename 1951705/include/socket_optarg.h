#pragma once
#include <string>
#define ARG_PORT_DEFAULT -1
#define ARG_READBYTE_DEFAULT 1024
#define ARG_SENDBYTE_DEFAULT 1024
#define ARG_DELAY_DEFAULT 0
#define ARG_RBUF_DEFAULT 8
#define ARG_WBUF_DEFAULT 8

struct Socket_optarg
{
    std::string ip;
    std::string filename;
    int port, myport;
    int rbuf, wbuf;
    bool getrbuf, getwbuf;   //打印TCP收、发缓冲区的原始大小
    bool getnrbuf, getnwbuf; //打印TCP收、发缓冲区的设置后大小
    int readbyte, sendbyte;  //每次读入的字节数
    int delay;
    int rdelay, wdelay;
    bool debug;
    Socket_optarg()
    {
        Init_optarg();
    }
    Socket_optarg(int argc, char **argv)
    {
        Set_Config(argc, argv);
    }
    void Init_optarg();
    void Print_Config();
    void Set_Config(int argc, char **argv);
};
