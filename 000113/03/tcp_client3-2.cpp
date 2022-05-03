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
     cur_connect.Setrbuf(50); //»º³åÇø 100×Ö½Ú
     cur_connect.Setreadbytes(100);

     while (1)
     {
          ssize_t len = cur_connect.Read();
          cout << "len: " << len << endl;
          if ((len < 0 && errno != EAGAIN) || len == 0)
          {
               cout << errno << endl
                    << strerror(errno) << endl;
               exit(EXIT_FAILURE);
          }
          sleep(1);
     }
     return 0;
}
