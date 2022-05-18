#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <fcntl.h>

struct SocketOption{
	int readSize=0;
	bool getrbuf=false;
	bool getwbuf=false;
	int setrbuf=0;						//缓冲区大小默认为8
	int setwbuf=0;
	bool getnrbuf=false;
	bool getnwbuf=false;
	bool nonblock=false;

};



int tcp_server_init_listen(int& sock_S, char* S_IP, int S_port, SocketOption opt=SocketOption());

int print_peer_info(int connfd);