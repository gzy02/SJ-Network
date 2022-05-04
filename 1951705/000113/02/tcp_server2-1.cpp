#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //error
#include <iostream> //cout
using namespace std;
int main(int argc, char **argv)
{

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); // listen(fd,1)

    int cur_fd = cur_connect.AcceptClient(); //µ»Client¡¨Ω”
    cur_connect.Setaccept_fd_nonblock(cur_fd);
    ssize_t len = cur_connect.Read(cur_fd);
    cout << "len: " << len << endl;

    cout << errno << endl
         << strerror(errno) << endl;

    return 0;
}
