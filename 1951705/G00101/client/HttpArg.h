#pragma once
#include <string>
#define ARG_PORT_DEFAULT -1

struct HttpArg
{
    std::string ip;
    std::string user;
    std::string passwd;
    std::string dstfile;
    std::string srcfile;
    int port;
    HttpArg()
    {
        Init_optarg();
    }
    HttpArg(int argc, char **argv)
    {
        Set_Config(argc, argv);
    }
    void clear();
    void Check_Config();
    void Init_optarg();
    void Print_Config();
    void Set_Config(int argc, char **argv);
};
