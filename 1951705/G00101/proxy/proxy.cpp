#include "proxy.h"
#include "read_param.h"
#include "tcp_init.h"
using namespace std;

extern int reuse_addr;

const char *param_name[] = {
    "--daemon",
    "--port",
    "--proxy_ip",
    "--proxy_port",
    "--logname"};

bool need_value[] = {
    false,
    true,
    true,
    true,
    true};

bool isDeamon = false;
int port;
char *proxy_ip;
int proxy_port;
char *logname;

fstream logfout;

int main(int argc, char **argv)
{
  //获取参数
  getParam(argc, argv);
  logfout.open(logname, ios::out | ios::app);

  if (isDeamon)
  {
    my_daemon(1, 1, 1);
  }

  //初始化listen fd
  SocketOption opt;
  opt.nonblock = true;
  int listenfd;
  tcp_server_init_listen(listenfd, (char *)"0.0.0.0", port);

  epoll_server(listenfd);
}

int getParam(int argc, char **argv)
{
  Param param;
  char *result[5] = {};
  param.num = 5;
  param.name = param_name;
  param.need_value = need_value;
  param.result = result;

  read_param(argc, argv, param);
  //获取--daemon
  if (result[0])
  {
    isDeamon = true;
  }
  //获取--port
  if (result[1])
  {
    port = atoi(result[1]);
  }
  else
  {
    printf("未输入port\n");
    exit(-1);
  }
  if (port < 0 || port > 65535)
  {
    printf("port应该在[0,65535]之间\n");
    exit(-1);
  }
  //获取--proxy_ip
  if (result[2])
  {
    proxy_ip = strdup(result[2]);
  }
  else
  {
    printf("未输入proxy_ip\n");
    exit(-1);
  }
  //获取--proxy_port
  if (result[3])
  {
    proxy_port = atoi(result[3]);
  }
  else
  {
    printf("未输入proxy_port\n");
    exit(-1);
  }
  if (proxy_port < 0 || proxy_port > 65535)
  {
    printf("proxy_port应该在[0,65535]之间\n");
    exit(-1);
  }
  //获取--logname
  if (result[4])
  {
    logname = strdup(result[4]);
  }
  else
  {
    printf("未输入logname\n");
    exit(-1);
  }
  return 0;
}

int epoll_server(int listenfd)
{

  int epoll_fd = epoll_create(MAX_CONNECT);
  if (epoll_fd < 0)
  {
    errorExit("epoll_create");
  }
  struct epoll_event ev;
  struct epoll_event events[MAX_EVENT];
  int ev_num;
  int max_ev = MAX_EVENT;
  int ret;

  //注册事件
  ev.events = EPOLLIN;
  ev.data.fd = listenfd;
  ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev);
  if (ret < 0)
  {
    errorExit("epoll_ctl");
  }

  int i;
  bool flag = true;
  unsigned int client_sock_len = sizeof(struct sockaddr_in);
  struct sockaddr_in client_sock;
  char buf[BUF_SIZE];

  map<int, bool> writable;
  map<int, int> proxyPair;
  vector<int> waitForConnect;
  map<int, string> ipAddr;

  while (flag)
  {
    ev_num = epoll_wait(epoll_fd, events, max_ev, -1);
    if (ev_num == 0)
    {
      printf("timeout\n");
      continue;
    }
    else if (ev_num < 0)
    {
      errorExit("epoll_wait");
    }
    for (i = 0; i < ev_num; i++)
    {
      struct epoll_event &event = events[i];
      if (event.data.fd == listenfd && (event.events & EPOLLIN))
      {
        int newfd = accept(event.data.fd, (struct sockaddr *)&client_sock, &client_sock_len);
        if (newfd < 0)
        {
          perror("accept error");
          continue;
        }
        int old_flags = fcntl(newfd, F_GETFL, 0);
        fcntl(newfd, F_SETFL, old_flags | O_NONBLOCK);

        ev.events = EPOLLIN | EPOLLOUT;
        ev.data.fd = newfd;
        ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newfd, &ev);
        if (ret < 0)
        {
          errorExit("epoll_ctl");
        }
        // print_peer_info(newfd);

        int connectfd;
        connect_socket(connectfd, proxy_ip, proxy_port);
        old_flags = fcntl(connectfd, F_GETFL, 0);
        fcntl(connectfd, F_SETFL, old_flags | O_NONBLOCK);

        ev.events = EPOLLOUT;
        ev.data.fd = connectfd;
        ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connectfd, &ev);
        if (ret < 0)
        {
          errorExit("epoll_ctl");
        }
        proxyPair.insert(pair<int, int>(newfd, connectfd));
        proxyPair.insert(pair<int, int>(connectfd, newfd));
        writable[newfd] = true;
        writable[connectfd] = false;
        ipAddr[newfd] = getIpAddr(newfd);
        waitForConnect.push_back(connectfd);
        time_t tm;
        time(&tm);
        char tmp_time[64] = "";
        strftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d %H:%M:%S", localtime(&tm));

        logfout << "[" << tmp_time << "] " << ipAddr[newfd] << " server accept success" << endl;
      }
      else if (event.events & EPOLLIN)
      {
        int recvfd = event.data.fd;
        int sendfd = proxyPair[recvfd];
        int readCount = 0;
        int writeCount = 0;
        if (!writable[sendfd])
        {
          continue;
        }
        memset(buf, 0, BUF_SIZE);
        ret = read(recvfd, buf, BUF_SIZE);
        if (ret <= 0)
        {
          time_t tm;
          time(&tm);
          char tmp_time[64] = "";
          strftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d %H:%M:%S", localtime(&tm));
          logfout << "[" << tmp_time << "] " << ipAddr[recvfd] << " lost connection" << endl;
          logfout << "[" << tmp_time << "] " << ipAddr[sendfd] << " close connection" << endl;
          writable.erase(recvfd);
          writable.erase(sendfd);
          proxyPair.erase(recvfd);
          proxyPair.erase(sendfd);
          ipAddr.erase(recvfd);
          ipAddr.erase(sendfd);
          close(recvfd);
          close(sendfd);

          // printf("connection close\n");
          continue;
        }
        readCount = ret;

        ret = write(sendfd, buf, ret);
        if (ret <= 0)
        {
          errorExit("write");
        }
        writeCount = ret;

        time_t tm;
        time(&tm);
        char tmp_time[64] = "";
        strftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d %H:%M:%S", localtime(&tm));

        logfout << "[" << tmp_time << "] " << ipAddr[recvfd] << " receive " << readCount << " bytes" << endl;
        logfout << "[" << tmp_time << "] " << ipAddr[sendfd] << " send " << writeCount << " bytes" << endl;
      }
      else if (event.events & EPOLLOUT)
      {
        auto it = find(waitForConnect.begin(), waitForConnect.end(), event.data.fd);
        if (it != waitForConnect.end())
        {
          int optval = -1;
          socklen_t optlen = sizeof(optval);
          ret = getsockopt(event.data.fd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
          if (ret != 0 || optval != 0)
          {
            errorExit("connect");
          }
          ev.events = EPOLLIN | EPOLLOUT;
          ev.data.fd = event.data.fd;
          ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event.data.fd, &ev);
          if (ret < 0)
          {
            errorExit("epoll_ctl");
          }
          waitForConnect.erase(it);
          ipAddr[event.data.fd] = getIpAddr(event.data.fd);
          time_t tm;
          time(&tm);
          char tmp_time[64] = "";
          strftime(tmp_time, sizeof(tmp_time), "%Y-%m-%d %H:%M:%S", localtime(&tm));

          logfout << "[" << tmp_time << "] " << ipAddr[event.data.fd] << " connect success" << endl;
        }

        if (!writable[event.data.fd])
        {
          writable[event.data.fd] = true;
        }
      }
    }
  }
  return 0;
}

int errorExit(const char *str)
{
  printf("&s error: %d %s\n", str, errno, strerror(errno));
  exit(-1);
}

int connect_socket(int &connectfd, char *proxy_ip, int proxy_port)
{
  int ret;
  struct sockaddr_in server_sock;
  struct sockaddr_in proxy_sock;
  unsigned int addr = 0;
  char ipaddr[100];
  connectfd = socket(AF_INET, SOCK_STREAM, 0);
  if (connectfd < 0)
  {
    errorExit("socket");
  }
  int flags = fcntl(connectfd, F_GETFL, 0);
  fcntl(connectfd, F_SETFL, flags | O_NONBLOCK);

  setsockopt(connectfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));

  server_sock.sin_family = AF_INET;
  server_sock.sin_port = htons(INADDR_ANY);
  if (inet_pton(AF_INET, "0.0.0.0", (void *)&addr) != 1)
  {
    errorExit("inet_pton server addr");
  }
  server_sock.sin_addr.s_addr = addr;

  ret = bind(connectfd, (struct sockaddr *)&server_sock, sizeof(struct sockaddr_in));
  if (ret < 0)
  {
    errorExit("bind");
  }

  proxy_sock.sin_family = AF_INET;
  proxy_sock.sin_port = htons(proxy_port);
  if (inet_pton(AF_INET, proxy_ip, &addr) != 1)
  {
    errorExit("inet_pton proxy addr");
  }
  proxy_sock.sin_addr.s_addr = addr;

  ret = connect(connectfd, (const sockaddr *)&proxy_sock, sizeof(struct sockaddr_in));
  if (ret < 0 && errno != EINPROGRESS)
  {
    printf("connect error,%d %s\n", errno, strerror(errno));
    errorExit("connect");
  }
  return 0;
}

string getIpAddr(int connfd)
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

  string ipInfo = string(peer_addr_str) + string(":") + to_string(peeraddr.sin_port);
  // printf("Peer Info: IP = %s,port = %d \n", peer_addr_str, ntohs(peeraddr.sin_port));

  return ipInfo;
}

int my_daemon(int closeFile, int ignoreCHLD, int changeDir)
{
  umask(0); //设置文件掩码为0

  if (fork() > 0) //创建一个子进程，退出父进程
  {
    exit(0);
  }

  setsid(); //使用setsid函数，是子进程脱离父进程

  if (changeDir == 1)
  {
    chdir("/"); //将工作目录切换至根目录
  }

  if (closeFile == 1)
  {
    close(0); //关闭文件描述符，分别是stdin stdout stderr
    close(1);
    close(2);
  }

  if (ignoreCHLD == 1)
  {
    signal(SIGCHLD, SIG_IGN); //忽略SIGCHLD
  }

  if (fork() > 0)
  {
    exit(0);
  }
  return 0;
}
