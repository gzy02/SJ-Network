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

     while (1)
     {
          ssize_t len = cur_connect.Write();
          cout << "len: " << len << endl;
          if (len <= 0)
          {
               cout << errno << endl
                    << strerror(errno) << endl;
               sleep(arg.wdelay);
          }
     }
     return 0;
}
