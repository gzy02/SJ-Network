#include "tcp_init.h"
int reuse_addr = 1;
int tcp_server_init_listen(int &sock_S, char *S_IP, int S_port, SocketOption opt)
{
	int ret = 0;
	unsigned int addr = 0;
	unsigned int client_sock_len = sizeof(struct sockaddr_in);
	struct sockaddr_in server_sock;
	struct sockaddr_in client_sock;
	char client_addr_str[15];

	sock_S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_S < 0)
	{
		printf("socket error,%d %s\n", errno, strerror(errno));
		return 0;
	}

	if (opt.nonblock)
	{
		int flags = fcntl(sock_S, F_GETFL, 0);
		fcntl(sock_S, F_SETFL, flags | O_NONBLOCK);
	}

	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(S_port); //必须使用htons转换
	/*将字符串的ip地址转化为网络字节序的整形*/
	if (inet_pton(AF_INET, S_IP, (void *)&addr) != 1)
	{
		printf("inet_pton addr error:%d %s\n", errno, strerror(errno));
		return 0;
	}
	server_sock.sin_addr.s_addr = addr;

	//设置重用ip地址
	if (reuse_addr)
	{
		setsockopt(sock_S, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
	}
	//设置接收低水位标记
	if (opt.readSize != 0)
	{
		setsockopt(sock_S, SOL_SOCKET, SO_RCVLOWAT, &(opt.readSize), sizeof(opt.readSize));
	}

	ret = bind(sock_S, (struct sockaddr *)&server_sock, sizeof(struct sockaddr_in));
	if (ret < 0)
	{
		printf("bind error,%d %s\n", errno, strerror(errno));
		return 0;
	}

	ret = listen(sock_S, 128);
	if (ret < 0)
	{
		printf("listen error,%d %s\n", errno, strerror(errno));
		return 0;
	}

	return 1;
}

int print_peer_info(int connfd)
{
	int n;
	struct sockaddr_in peeraddr;
	socklen_t peeraddrlen = sizeof(peeraddr);
	char peer_addr_str[100] = {};

	if (n = getpeername(connfd, (struct sockaddr *)&peeraddr, &peeraddrlen) < 0)
	{
		printf("getpeername error: %s(errno: %d)\n", strerror(errno), errno);
		exit(-1);
	}

	/*将网络字节序的整形转化为字符串格式的ip地址*/
	if (inet_ntop(AF_INET, (void *)&(peeraddr.sin_addr), peer_addr_str, sizeof(peer_addr_str)) == NULL)
	{
		printf("inet_ntop error,%d %s\n", errno, strerror(errno));
		return 0;
	}
	printf("Peer Info: IP = %s,port = %d \n", peer_addr_str, ntohs(peeraddr.sin_port));

	return 0;
}