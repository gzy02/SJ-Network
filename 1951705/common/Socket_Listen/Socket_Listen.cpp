#include "../../include/Socket_Listen.h"
#include "../../include/local_ip_mac_info.h"
#include <arpa/inet.h> //inet
#include <cstdlib>     //exit
#include <cstring>
#include <errno.h>
#include <iostream>     //cerr
#include <net/if.h>     //ifconf
#include <netinet/in.h> //IPPROTO_IP
#include <string>       //strcmp
#include <sys/ioctl.h>  //ioctl
#include <sys/socket.h> //socket
using namespace std;
int check(string arg_ip)
{
    if (arg_ip == "0.0.0.0")
        return 0;
    char ip[IP_SIZE];
    int sockfd;
    struct ifconf ifconf;
    struct ifreq *ifreq;
    char buf[512];

    ifconf.ifc_len = 512; //Ĭ��ֵ�����Ա�ioctl����
    ifconf.ifc_buf = buf;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cerr << "Error: socket" << endl;
        exit(EXIT_FAILURE);
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);

    ifreq = (struct ifreq *)ifconf.ifc_buf;
    for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; --i)
    {
        if (ifreq->ifr_flags == AF_INET)
        {
            get_local_ip(ifreq->ifr_name, ip);
            // cout << "name:" << ifreq->ifr_name << ",IP:" << ip << endl;
            // if (strcmp(ip, arg_ip.c_str()) == 0)
            if (arg_ip == ip) //ֱ����string
                return 0;
            ifreq++;
        }
    }
    return -1;
}

Socket_Listen::Socket_Listen(Socket_optarg arg, int __listen_queue_len, BLOCK_STATUS __isblock, int __af,
                             int __type, int __protocol) : Socket_Base(arg, __isblock, __af, __type, __protocol)
{
    if (arg.port == ARG_PORT_DEFAULT)
    {
        cerr << "Error: Please use --port [0,65535]" << endl;
        exit(EXIT_FAILURE);
    }
    if (arg.ip.size() == 0) // ip����Ĭ��Ϊ0,��δ����--ip,Ĭ�ϰ�����
    {
        arg.ip = "0.0.0.0";
    }
    else if (check(arg.ip) == -1) //�������ֲ�������
    {
        cerr << "Error: IP " << arg.ip << " not found" << endl;
        exit(EXIT_FAILURE);
    }

    listen_queue_len = __listen_queue_len;

    addr.sin_family = address_family;
    addr.sin_port = htons(arg.port);
    addr.sin_addr.s_addr = inet_addr(arg.ip.c_str()); //ָ����Ӧ�������ĵ�ַ����һ��unsigned int
}
void Socket_Listen::Bind()
{
    // bind
    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cerr << socket_fd << endl;
        cerr << errno << endl
             << strerror(errno) << endl;
        cerr << "Error: bind" << endl;
        exit(EXIT_FAILURE);
    }
}
void Socket_Listen::Listen()
{
    // listen
    if (listen(socket_fd, listen_queue_len) == -1)
    {
        cerr << "Error: listen" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "listening..." << endl;
}
