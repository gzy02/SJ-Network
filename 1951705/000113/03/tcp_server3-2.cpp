#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //error
#include <iostream> //cout
#include <unistd.h>
using namespace std;
int main(int argc, char **argv)
{
    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, NONBLOCK); // listen(fd,1)
    cur_connect.Setsendbytes(10);
    cur_connect.Setsend_str("hello123!");
    int cur_fd = -1;
    while (cur_fd == -1)
    {
        cur_fd = cur_connect.AcceptClient(); //µ»Client¡¨Ω”
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            continue;
        else
        {
            cerr << "Accept Error" << endl;
            cerr << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }
    cur_connect.Setaccept_fd_nonblock(cur_fd);

    while (true)
    {
        ssize_t len = cur_connect.Write(cur_fd);
        cout << "len: " << len << endl;
        if ((len < 0 && errno != EAGAIN) || len == 0)
        {
            cerr << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    return 0;
}
