#pragma once
#include "Socket_Listen.h"
#include "socket_optarg.h"
#include <netinet/in.h>
#include <string>
#include <vector>
//用于Server端Accept的socket的后续处理
class Socket_Accept : public Socket_Listen
{
protected:
    std::vector<int> accept_fds; // accept返回的fd
    std::vector<BLOCK_STATUS> accept_fds_is_block;
    std::vector<sockaddr_in> clientAddrs;
    // char clientIP[INET_ADDRSTRLEN];
    int sendbytes, readbytes;
    char *send_str;
    char *read_str;

    void Initread_str(int __readbytes);
    void Initsend_str(int __sendbytes);

public:
    Socket_Accept(Socket_optarg arg, int __listen_queue_len = 1, BLOCK_STATUS __isblock = BLOCK, int __af = AF_INET, int __type = SOCK_STREAM, int __protocol = IPPROTO_IP);
    ~Socket_Accept();
    int Find_accept_fd_pos(int accept_fd);
    std::vector<int> Getaccept_fds() { return accept_fds; };
    void Setsendbytes(int __sendbytes);
    void Setreadbytes(int __readbytes);
    void Setsend_str(const char *__send_str);
    const char *Getread_str();
    // const char *GetclientIP(int accept_fd);
    // int GetclientPort(int accept_fd);
    void Setaccept_fd_nonblock(int accept_fd);
    ssize_t Send(int accept_fd, int __flags);
    ssize_t Write(int accept_fd);
    ssize_t Send(int accept_fd, const char *__buf, int __n, int __flags);
    ssize_t Write(int accept_fd, const char *__buf, int __n);
    ssize_t Recv(int accept_fd, int __flags);
    ssize_t Read(int accept_fd);

    int AcceptClient();
};
