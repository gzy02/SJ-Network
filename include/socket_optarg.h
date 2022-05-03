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
    int port, myport;
    int rbuf, wbuf;
    bool getrbuf, getwbuf;   //��ӡTCP�ա�����������ԭʼ��С
    bool getnrbuf, getnwbuf; //��ӡTCP�ա��������������ú��С
    int readbyte, sendbyte;  //ÿ�ζ�����ֽ���
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
    void Init_optarg()
    {
        ip.clear();
        port = myport = ARG_PORT_DEFAULT;
        readbyte = ARG_READBYTE_DEFAULT;
        sendbyte = ARG_SENDBYTE_DEFAULT;
        rdelay = wdelay = delay = ARG_DELAY_DEFAULT;
        rbuf = ARG_RBUF_DEFAULT;
        wbuf = ARG_WBUF_DEFAULT;
        debug = false;
        getrbuf = getwbuf = false;
        getnrbuf = getnwbuf = false;
    }
    void Print_Config();
    void Set_Config(int argc, char **argv);
};
