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
     cur_connect.Setsendbytes(10);
     cur_connect.Setsend_str("hello123!");
     while (1)
     {
          ssize_t len = cur_connect.Write();
          cout << "len: " << len << endl;
          if (len <= 0)
          {
               cout << errno << endl
                    << strerror(errno) << endl;
               // exit(EXIT_FAILURE);
               getchar();
          }
          /*if ((len < 0 && errno != EAGAIN) || len == 0)
          {
               cout << errno << endl
                    << strerror(errno) << endl;
               // exit(EXIT_FAILURE);
               getchar();
          }*/
          // sleep(1);
     }
     return 0;
}
