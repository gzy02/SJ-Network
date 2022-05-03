#include "../../include/Socket_Accept.h"
#include <algorithm>   //find
#include <arpa/inet.h> //inet_ntop
#include <cstdlib>     //exit
#include <cstring>
#include <error.h>
#include <iostream>     //cerr
#include <sys/socket.h> //socket
#include <unistd.h>     //close
using namespace std;

int Socket_Accept::AcceptClient()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int accept_fd = accept(socket_fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (accept_fd != -1)
    {
        accept_fds_is_block.push_back(BLOCK);
        accept_fds.push_back(accept_fd);
        clientAddrs.push_back(clientAddr);
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(address_family, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        cout << "connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << endl;
    }
    return accept_fd;
}
void Socket_Accept::Initsend_str(int __sendbytes)
{
    sendbytes = __sendbytes;
    send_str = (char *)malloc(sizeof(char) * (sendbytes + 1));
}
void Socket_Accept::Initread_str(int __readbytes)
{
    readbytes = __readbytes;
    read_str = (char *)malloc(sizeof(char) * (readbytes + 1));
}
void Socket_Accept::Setsendbytes(int __sendbytes)
{
    free(send_str);
    Initsend_str(__sendbytes);
}
void Socket_Accept::Setreadbytes(int __readbytes)
{
    free(read_str);
    Initread_str(__readbytes);
}
void Socket_Accept::Setsend_str(const char *__send_str)
{
    strcpy(send_str, __send_str);
}
const char *Socket_Accept::Getread_str() { return read_str; }
// const char *Socket_Accept::GetclientIP() { return clientIP; }
// int Socket_Accept::GetclientPort() { return ntohs(clientAddr.sin_port); }
int Socket_Accept::Find_accept_fd_pos(int accept_fd)
{
    vector<int>::iterator p = find(accept_fds.begin(), accept_fds.end(), accept_fd);
    if (p != accept_fds.end())
    {
        return p - accept_fds.begin();
    }
    else
        return -1;
}
void Socket_Accept::Setaccept_fd_nonblock(int accept_fd)
{
    int pos;
    if (-1 != (pos = Find_accept_fd_pos(accept_fd)))
        if (accept_fds_is_block[pos] == BLOCK)
            if (SetNonblocking(accept_fd) == -1)
            {
                cerr << "Error: Setaccept_fd_nonblock()" << endl;
                exit(EXIT_FAILURE);
            }
            else
                accept_fds_is_block[pos] = NONBLOCK;
}
//传入Socket_Listen对象，完成构造, 在BLOCK的情况下可以传入NoAccept防止阻塞，后面自行调用AcceptClient
Socket_Accept::Socket_Accept(Socket_optarg arg, int __listen_queue_len, BLOCK_STATUS __isblock, int __af, int __type, int __protocol) : Socket_Listen(arg, __listen_queue_len, __isblock, __af, __type, __protocol)
{
    Socket();
    Bind();
    Listen();
    Initsend_str(arg.sendbyte);
    Initread_str(arg.readbyte);
}
Socket_Accept::~Socket_Accept()
{
    free(send_str);
    free(read_str);
    for (int i = 0; i < accept_fds.size(); ++i)
        close(accept_fds[i]);
    accept_fds.clear();
    accept_fds_is_block.clear();
}
ssize_t Socket_Accept::Recv(int accept_fd, int flags = 0)
{
    if (Find_accept_fd_pos(accept_fd) == -1)
        return -2;
    ssize_t res = recv(accept_fd, read_str, readbytes, flags);
    if (res >= 0)
        read_str[res] = '\0';
    return res;
}

ssize_t Socket_Accept::Read(int accept_fd)
{
    if (Find_accept_fd_pos(accept_fd) == -1)
        return -2;
    ssize_t res = read(accept_fd, read_str, readbytes);
    if (res >= 0)
        read_str[res] = '\0';
    return res;
}
ssize_t Socket_Accept::Send(int accept_fd, int flags = 0)
{
    if (Find_accept_fd_pos(accept_fd) == -1)
        return -2;
    return send(accept_fd, send_str, sendbytes, flags);
}
ssize_t Socket_Accept::Write(int accept_fd)
{
    if (Find_accept_fd_pos(accept_fd) == -1)
        return -2;
    return write(accept_fd, send_str, sendbytes);
}
ssize_t Socket_Accept::Send(int accept_fd, const char *buf, int __n, int flags = 0)
{
    if (Find_accept_fd_pos(accept_fd) == -1)
        return -2;
    return send(accept_fd, buf, __n, flags);
}
ssize_t Socket_Accept::Write(int accept_fd, const char *buf, int __n)
{
    if (Find_accept_fd_pos(accept_fd) == -1)
        return -2;
    return write(accept_fd, buf, __n);
}
