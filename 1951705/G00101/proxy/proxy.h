#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/prctl.h>
#include<time.h>

#define MAX_CONNECT 1024
#define BUF_SIZE 10240
#define MAX_EVENT 128
using namespace std;

string getIpAddr(int connfd);
int getParam(int argc, char **argv);
int epoll_server(int listenfd);
int connect_socket(int& connectfd,char* proxy_ip,int proxy_port);
int errorExit(const char *str);
int my_daemon(int closeFile, int ignoreCHLD, int changeDir);

