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

    // bind
    if (arg.myport != ARG_PORT_DEFAULT) //输入了myport
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(arg.myport);
        addr.sin_addr.s_addr = INADDR_ANY;
        // closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket
        int opt = 1;
        setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

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

    getchar();
    cout << "Read begin" << endl;
    const int n = 10000;
    char buf[n + 1];
    int cnt = 0;
    while (1)
    {
        ssize_t len = recv(client, buf, n, 0);
        buf[n] = '\0';
        if (len == -1)
        {
            cerr << errno << endl;
            cerr << strerror(errno) << endl;
            break;
        }
        cnt += len;
        // cout << "buf:" << buf << endl;
        cout << "cnt=" << cnt << endl;
        sleep(3);
    }
    close(client);
    return 0;
}
