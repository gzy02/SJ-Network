#include "../../include/Socket_Base.h"
#include "../../include/socket_optarg.h"

#include <cstdlib>      //exit
#include <iostream>     //cerr
#include <netinet/in.h> //IPPROTO_IP
#include <sys/socket.h> //socket
#include <unistd.h>     //close

using namespace std;
void SetReuseAddr(int socket_fd)
{
    // closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket
    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
    {
        cerr << "Error: SetReuseAddr()" << endl;
        exit(EXIT_FAILURE);
    }
}

Socket_Base::Socket_Base(Socket_optarg arg, BLOCK_STATUS __isblock, int __af, int __type, int __protocol)
{
    isblock = __isblock;
    address_family = __af;
    type = __type;
    if (isblock == NONBLOCK)
        type = type | SOCK_NONBLOCK; //非阻塞
    protocol = __protocol;
    socket_fd = -1;
    rbuf = arg.rbuf;
    wbuf = arg.wbuf;
}
void Socket_Base::Socket()
{
    socket_fd = socket(address_family, type, protocol);
    if (socket_fd == -1)
    {
        cerr << "Error: socket()" << endl;
        exit(EXIT_FAILURE);
    }

    Setrbuf(rbuf);
    Setwbuf(wbuf);
    SetReuseAddr(socket_fd);
}
int Socket_Base::Getsocketfd()
{
    return socket_fd;
}
Socket_Base::~Socket_Base()
{
    close(socket_fd);
}
