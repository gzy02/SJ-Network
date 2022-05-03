#include "../../include/Socket_Connect.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //errno
#include <iostream> //cout
#include <unistd.h> //sleep
using namespace std;
int main(int argc, char **argv)
{
    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Connect cur_connect(arg);
    cur_connect.SetNonblock();
    ssize_t len = cur_connect.Read();
    cout << "len: " << len << endl;
    cout << errno << endl
         << strerror(errno) << endl;
    return 0;
}
