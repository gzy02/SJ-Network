#pragma once
#include "Socket_Base.h"
#include "socket_optarg.h"
#include <netinet/in.h>
//用于Server端Bind的socket的后续处理
class Socket_Listen : public Socket_Base
{
protected:
    int listen_queue_len;
    sockaddr_in addr;

public:
    Socket_Listen(Socket_optarg arg, int __listen_queue_len = 1,
                  BLOCK_STATUS __isblock = BLOCK, int __af = AF_INET,
                  int __type = SOCK_STREAM, int __protocol = IPPROTO_IP);

    void Listen();
    void Bind();
};
