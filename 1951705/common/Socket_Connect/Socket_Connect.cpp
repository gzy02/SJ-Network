#include "../../include/Socket_Connect.h"
#include <arpa/inet.h>  //inet
#include <cstdlib>      //exit
#include <cstring>      //strcpy
#include <errno.h>      //errno
#include <iostream>     //cerr
#include <netinet/in.h> //IPPROTO_IP
#include <sys/socket.h> //socket
using namespace std;
Socket_Connect::Socket_Connect(Socket_optarg arg, BLOCK_STATUS __isblock, int __af,
                               int __type, int __protocol) : Socket_Base(arg, __isblock, __af, __type, __protocol)
{
    if (arg.port == ARG_PORT_DEFAULT)
    {
        cerr << "Error: Please use --port [0,65535]" << endl;
        exit(EXIT_FAILURE);
    }
    if (arg.ip.size() == 0) // ip长度默认为0
    {
        cerr << "Error: Please use --ip x.x.x.x" << endl;
        exit(EXIT_FAILURE);
    }

    Socket(); //初始化socket_fd

    if (arg.myport != ARG_PORT_DEFAULT) //输入了myport
    {
        struct sockaddr_in myaddr;
        myaddr.sin_family = address_family;
        myaddr.sin_port = htons(arg.myport); //绑定某个端口
        myaddr.sin_addr.s_addr = INADDR_ANY;
        // bind
        if (bind(socket_fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
        {
            cerr << "Error: bind" << endl;
            exit(EXIT_FAILURE);
        }
    }

    dest_addr.sin_family = address_family;
    dest_addr.sin_port = htons(arg.port);
    dest_addr.sin_addr.s_addr = inet_addr(arg.ip.c_str()); //指定对应的网卡的地址，是一个unsigned int
    if (dest_addr.sin_addr.s_addr == INADDR_NONE)
    {
        cout << arg.ip << " Not a valid IP address, maybe a domain name.\n";
        exit(EXIT_FAILURE);
    }
    strcpy(serverIP, arg.ip.c_str());

    // connect
    do
    {
        if (connect(socket_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1)
        {
            if ((errno == EINPROGRESS || errno == EALREADY) && __isblock == NONBLOCK)
                continue;
            cerr << "Error: connect" << endl;
            exit(EXIT_FAILURE);
        }

    } while (__isblock == NONBLOCK && socket_fd == -1);

    cout << "connect " << inet_ntoa(dest_addr.sin_addr) << ":" << ntohs(dest_addr.sin_port) << endl;

    // get my socketname
    socklen_t clientAddrLen = sizeof(my_addr);
    if (getsockname(socket_fd, (struct sockaddr *)&my_addr, &clientAddrLen) != 0)
    {
        cerr << "Error: Getsockname" << endl;
        exit(EXIT_FAILURE);
    }
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(address_family, &my_addr.sin_addr, clientIP, INET_ADDRSTRLEN);
    cout << "myaddr: " << clientIP << ":" << ntohs(my_addr.sin_port) << endl;

    Initsend_str(arg.sendbyte);
    Initread_str(arg.readbyte);
}

Socket_Connect::~Socket_Connect()
{
    free(send_str);
    free(read_str);
}
