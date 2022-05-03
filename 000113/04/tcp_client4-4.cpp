#include "../../include/Socket_Connect.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //errno
#include <iostream> //cout
#include <unistd.h> //sleep

#include <sys/select.h> //select
#include <sys/time.h>
#include <sys/types.h>
using namespace std;
int main(int argc, char **argv)
{
     Socket_optarg arg;
     arg.Set_Config(argc, argv);
     Socket_Connect cur_connect(arg, NONBLOCK);
     int cur_fd = cur_connect.Getsocketfd();
     const int MAXLEN = 1023;
     char buf[MAXLEN + 1];
     for (int i = 0; i < MAXLEN; i++)
          buf[i] = 'h';
     buf[MAXLEN] = '\0';
     cur_connect.Setsend_str(buf);

     fd_set wset;
     struct timeval t;
     t.tv_sec = 1;
     t.tv_usec = 0;
     while (1)
     {
          FD_ZERO(&wset);
          FD_SET(cur_fd, &wset);
          int ready_num = select(cur_fd + 1, NULL, &wset, NULL, &t); // Write
          if (ready_num < 0)
          {
               cerr << "Error: select" << endl;
               cout << errno << endl
                    << strerror(errno) << endl;
               exit(EXIT_FAILURE);
          }
          else if (ready_num == 0) //超时
          {
               cout << "Time out" << endl;
               cout << errno << endl
                    << strerror(errno) << endl;
               sleep(arg.wdelay);
          }
          else //有空闲
          {
               if (FD_ISSET(cur_fd, &wset))
               {
                    ssize_t len = cur_connect.Write();
                    cout << "len: " << len << endl;
                    if (len <= 0) // len=-1且errno=11可以等等，len=0为断开连接
                    {
                         cout << errno << endl
                              << strerror(errno) << endl;
                         sleep(arg.wdelay);
                    }
               }
               else
               {
                    cout << "Unwritable" << endl;
                    sleep(arg.wdelay);
               }
          }
          sleep(1);
     }
     return 0;
}
