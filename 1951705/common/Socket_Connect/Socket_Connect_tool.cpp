#include "../../include/Socket_Connect.h"
#include <cstdlib>      //exit
#include <cstring>      //strcpy
#include <errno.h>      //errorno
#include <iostream>     //cerr
#include <sys/socket.h> //socket
#include <unistd.h>     //write read
using namespace std;

void Socket_Connect::Initsend_str(int __sendbytes)
{
    sendbytes = __sendbytes;
    send_str = (char *)malloc(sizeof(char) * (sendbytes + 1));
}
void Socket_Connect::Initread_str(int __readbytes)
{
    readbytes = __readbytes;
    read_str = (char *)malloc(sizeof(char) * (readbytes + 1));
}
void Socket_Connect::Setsendbytes(int __sendbytes)
{
    free(send_str);
    Initsend_str(__sendbytes);
}
void Socket_Connect::Setreadbytes(int __readbytes)
{
    free(read_str);
    Initread_str(__readbytes);
}
void Socket_Connect::Setsend_str(const char *__send_str)
{
    strcpy(send_str, __send_str);
}
const char *Socket_Connect::Getread_str() { return read_str; }
const char *Socket_Connect::GetserverIP() { return serverIP; }
int Socket_Connect::GetserverPort() { return ntohs(dest_addr.sin_port); }
ssize_t Socket_Connect::Recv(int flags = 0)
{
    ssize_t res = recv(socket_fd, read_str, readbytes, flags);
    read_str[res] = '\0';
    return res;
}

ssize_t Socket_Connect::Read()
{
    ssize_t res = read(socket_fd, read_str, readbytes);
    read_str[res] = '\0';
    return res;
}
ssize_t Socket_Connect::Send(int flags = 0)
{
    return send(socket_fd, send_str, sendbytes, flags);
}
ssize_t Socket_Connect::Write()
{
    return write(socket_fd, send_str, sendbytes);
}
ssize_t Socket_Connect::Send(const char *buf, int __n, int flags = 0)
{
    return send(socket_fd, buf, __n, flags);
}
ssize_t Socket_Connect::Write(const char *buf, int __n)
{
    return write(socket_fd, buf, __n);
}
