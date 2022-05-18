#include "HttpArg.h"
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
void HttpArg::clear()
{
    ip.clear();
    user.clear();
    passwd.clear();
    dstfile.clear();
    srcfile.clear();
    port = ARG_PORT_DEFAULT;
}
void HttpArg::Init_optarg()
{
    clear();
}
void HttpArg::Print_Config()
{
    cout << ip << endl;
    cout << user << endl;
    cout << passwd << endl;
    cout << dstfile << endl;
    cout << srcfile << endl;
    cout << port << endl;
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
void HttpArg::Check_Config()
{
    if (port == ARG_PORT_DEFAULT)
    {
        cerr << "Error: Please input port." << endl;
        exit(EXIT_FAILURE);
    }
    check(port, 0, 65535);
    if (ip.find(".") == string::npos)
    {
        cerr << "Error: Please use --ip x.x.x.x" << endl;
        exit(EXIT_FAILURE);
    }
    if (user.length() == 0)
    {
        cerr << "Error: Please input user." << endl;
        exit(EXIT_FAILURE);
    }
    if (passwd.length() == 0)
    {
        cerr << "Error: Please input passwd." << endl;
        exit(EXIT_FAILURE);
    }
    if (dstfile.length() == 0)
    {
        cerr << "Error: Please input dstfile." << endl;
        exit(EXIT_FAILURE);
    }
    if (srcfile.length() == 0)
    {
        cerr << "Error: Please input srcfile." << endl;
        exit(EXIT_FAILURE);
    }
}
void HttpArg::Set_Config(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i)
    {
        if (strlen(argv[i]) >= 2 && argv[1][0] == '-' && argv[1][1] == '-')
        {
            if (!strcmp(&argv[i][2], "ip"))
            {
                if (i + 1 < argc)
                {
                    ip = argv[i + 1];
                }
            }
            else if (!strcmp(&argv[i][2], "port"))
            {
                if (i + 1 < argc)
                {
                    port = StrToInt(argv[i + 1]);
                }
            }
            else if (!strcmp(&argv[i][2], "user"))
            {
                if (i + 1 < argc)
                {
                    user = argv[i + 1];
                }
            }
            else if (!strcmp(&argv[i][2], "passwd"))
            {
                if (i + 1 < argc)
                {
                    passwd = argv[i + 1];
                }
            }
            else if (!strcmp(&argv[i][2], "dstfile"))
            {
                if (i + 1 < argc)
                {
                    dstfile = argv[i + 1];
                }
            }
            else if (!strcmp(&argv[i][2], "srcfile"))
            {
                if (i + 1 < argc)
                {
                    srcfile = argv[i + 1];
                }
            }
        }
    }
    Check_Config();
}
