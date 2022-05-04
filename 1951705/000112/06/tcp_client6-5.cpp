#include "../../include/socket_optarg.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main(int argc, char **argv)
{
    Socket_optarg arg;
    arg.Set_Config(argc, argv);
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
    cout << "This is client" << endl;

    // socket
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1)
    {
        cerr << "Error: socket" << endl;
        exit(EXIT_FAILURE);
    }

    // closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket
    int opt = 1;
    setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

    socklen_t optlen = sizeof(int);
    cout << arg.rbuf << ' ' << arg.wbuf << endl;
    int recvbuf_ori, recvbuf_end;
    getsockopt(client, SOL_SOCKET, SO_RCVBUF, (void *)&recvbuf_ori, &optlen); //默认值
    if (arg.getrbuf)
        cout << "recvbuf_ori_len=" << recvbuf_ori << endl;
    setsockopt(client, SOL_SOCKET, SO_RCVBUF, &arg.rbuf, optlen);
    getsockopt(client, SOL_SOCKET, SO_RCVBUF, (void *)&recvbuf_end, &optlen); //默认值
    if (arg.getnrbuf)
        cout << "recvbuf_end_len=" << recvbuf_end << endl;

    int sendbuf_ori, sendbuf_end;
    getsockopt(client, SOL_SOCKET, SO_SNDBUF, (void *)&sendbuf_ori, &optlen); //默认值
    if (arg.getwbuf)
        cout << "sendbuf_ori_len=" << sendbuf_ori << endl;
    setsockopt(client, SOL_SOCKET, SO_SNDBUF, &arg.wbuf, optlen);             //默认值
    getsockopt(client, SOL_SOCKET, SO_SNDBUF, (void *)&sendbuf_end, &optlen); //默认值
    if (arg.getnwbuf)
        cout << "sendbuf_end_len=" << sendbuf_end << endl;

    // bind
    if (arg.myport != ARG_PORT_DEFAULT) //输入了myport
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(arg.myport);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(client, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        {
            cerr << "Error: bind" << endl;
            exit(EXIT_FAILURE);
        }
    }

    // connect
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(arg.port);
    serverAddr.sin_addr.s_addr = inet_addr(arg.ip.c_str());
    if (serverAddr.sin_addr.s_addr == INADDR_NONE)
    {
        cout << "Not a valid IP address, maybe a domain name.\n";
        exit(EXIT_FAILURE);
    }

    if (connect(client, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cerr << "Error: connect" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "connect " << inet_ntoa(serverAddr.sin_addr) << ":" << ntohs(serverAddr.sin_port) << endl;
    int cnt = 0;
    const int n = 10000;
    char data[n + 1];
    for (int i = 0; i < n; ++i)
        data[i] = 'h';
    data[n] = '\0';
    while (1)
    {

        ssize_t len = write(client, data, n);
        cnt += len;
        cout << "cnt=" << cnt << endl;
        sleep(1);
    }

    close(client);
    return 0;
}
