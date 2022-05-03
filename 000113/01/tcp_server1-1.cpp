#include "../../include/Socket_Accept.h"
#include "../../include/Socket_Listen.h"
#include "../../include/socket_optarg.h"
#include <cstdlib>  //exit
#include <cstring>  //strerror
#include <errno.h>  //error
#include <iostream> //cout
using namespace std;
int main(int argc, char **argv)
{

    Socket_optarg arg;
    arg.Set_Config(argc, argv);
    Socket_Accept cur_connect(arg, 1, BLOCK); //����notaccept������������ʼ��
    cur_connect.Setreadbytes(20);             //����Ϊ20�ֽ�
    while (true)
    {
        int cur_fd = cur_connect.AcceptClient(); //��Client����
        if (cur_fd == -1)
        {
            cout << errno << endl
                 << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
        ssize_t len = cur_connect.Read(cur_fd);
        cout << "len: " << len << endl;

        cout << errno << endl
             << strerror(errno) << endl;

        cout << "Recv_str:" << cur_connect.Getread_str() << endl;
    }
    return 0;
}
