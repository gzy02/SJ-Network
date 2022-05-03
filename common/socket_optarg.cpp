#include "../include/socket_optarg.h"
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int digit(int num) //判断位数
{
    int start = 1;
    while (num / 10)
    {
        start++;
        num /= 10;
    }
    return start;
}

int StrToInt(const char *str)
{
    int flags = (str[0] == '-');
    int abs = 2 * flags - 1;
    int result;
    sscanf(str, "%d", &result); //也可使用atoi
    if (!(result * abs < 0 && strlen(str + flags) == digit(result)))
    {
        result = flags ? INT_MIN : INT_MAX;
    }
    return result;
}

void Socket_optarg::Print_Config()
{
    /*
    string ip;
    int port, myport;
    int rbuf, wbuf;
    int readbyte, sendbyte; //每次读入的字节数
    int delay;
    bool debug;
    bool getrbuf, getwbuf;   //打印TCP收、发缓冲区的原始大小
    bool getnrbuf, getnwbuf; //打印TCP收、发缓冲区的设置后大小
    */
    cout << ip << endl;
}

void check(int value, int l, int r) //[l,r]
{
    if (value < l || value > r)
    {
        cerr << "The value is out of range!\n";
        cerr << "The range should be between [" << l << ", " << r << "]" << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket_optarg::Set_Config(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        // printf("%s", &argv[1][2]);
        if (strlen(argv[i]) >= 2 && argv[1][0] == '-' && argv[1][1] == '-')
        {
            // printf("%s", &argv[i][2]);
            if (!strcmp(&argv[i][2], "ip"))
            {
                if (i + 1 < argc)
                {
                    ip = argv[i + 1];
                    if (ip.find(".") == string::npos)
                    {
                        cerr << "Error: Please use --ip x.x.x.x" << endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else if (!strcmp(&argv[i][2], "port"))
            {
                if (i + 1 < argc)
                {
                    port = StrToInt(argv[i + 1]);
                    check(port, 0, 65535);
                }
            }
            else if (!strcmp(&argv[i][2], "myport"))
            {
                if (i + 1 < argc)
                {
                    myport = StrToInt(argv[i + 1]);
                    check(myport, 0, 65535);
                }
            }
            else if (!strcmp(&argv[i][2], "setrbuf"))
            {
                if (i + 1 < argc)
                {
                    rbuf = StrToInt(argv[i + 1]);
                    check(rbuf, 1, 16384);
                    rbuf *= 512; //单位KB 但设置时会乘以2
                }
            }
            else if (!strcmp(&argv[i][2], "setwbuf"))
            {
                if (i + 1 < argc)
                {
                    wbuf = StrToInt(argv[i + 1]);
                    check(wbuf, 1, 16384);
                    wbuf *= 512;
                }
            }
            else if (!strcmp(&argv[i][2], "readbyte"))
            {
                if (i + 1 < argc)
                {
                    readbyte = StrToInt(argv[i + 1]);
                    check(readbyte, 1, 65536);
                }
            }
            else if (!strcmp(&argv[i][2], "sendbyte") || !strcmp(&argv[i][2], "writebyte"))
            {
                if (i + 1 < argc)
                {
                    sendbyte = StrToInt(argv[i + 1]);
                    check(sendbyte, 1, 65536);
                }
            }
            else if (!strcmp(&argv[i][2], "delay"))
            {
                if (i + 1 < argc)
                {
                    delay = StrToInt(argv[i + 1]);
                    check(delay, 0, 5000000);
                }
            }
            else if (!strcmp(&argv[i][2], "debug"))
            {
                debug = true;
            }
            else if (!strcmp(&argv[i][2], "getrbuf"))
            {
                getrbuf = true;
            }
            else if (!strcmp(&argv[i][2], "getwbuf"))
            {
                getwbuf = true;
            }
            else if (!strcmp(&argv[i][2], "getnrbuf"))
            {
                getnrbuf = true;
            }
            else if (!strcmp(&argv[i][2], "getnwbuf"))
            {
                getnwbuf = true;
            }
            else if (!strcmp(&argv[i][2], "rdelay"))
            {
                if (i + 1 < argc)
                {
                    rdelay = StrToInt(argv[i + 1]);
                    check(rdelay, 0, 15);
                }
            }
            else if (!strcmp(&argv[i][2], "wdelay"))
            {
                if (i + 1 < argc)
                {
                    wdelay = StrToInt(argv[i + 1]);
                    check(wdelay, 0, 15);
                }
            }
        }
    }
}
