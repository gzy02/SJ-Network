#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int http_create_socket(const char *ip, int port) //这里直接用阻塞形式了
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = inet_addr(ip);
    if (0 != connect(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)))
    {
        cerr << "Create socket error." << endl;
        cerr << errno << endl
             << strerror(errno) << endl;

        exit(EXIT_FAILURE);
    }
    return sockfd;
}

string http_recv_request(int sockfd)
{
    const int BUFFER_SIZE = 4096;
    char buffer_tep[BUFFER_SIZE + 1];
    string result;
    while (1)
    {
        memset(buffer_tep, 0, BUFFER_SIZE);
        int len = recv(sockfd, buffer_tep, BUFFER_SIZE, 0); //接受数据
        if (len == -1)
        {
            cerr << "Error: recv\n";
            cerr << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        else if (len == 0) // disconnect
        {
            break;
        }
        buffer_tep[len] = '\0'; //加尾零
        result += buffer_tep;   //把buffer_tep数据copy到result中
    }
    return result;
}

int http_send_request(int sockfd, string buffer)
{
    int len = send(sockfd, buffer.c_str(), buffer.length(), 0);
    if (len == -1)
    {
        cerr << "Error: send" << endl;
        cerr << errno << endl
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    return len;
}
