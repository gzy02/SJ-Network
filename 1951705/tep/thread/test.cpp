#include <cstdlib>
#include <errno.h>
#include <exception>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <string>
using namespace std;

#define NUM_THREADS 1
struct arg
{
    int a;
    int b;
};
void *PrintHello(void *threadid)
{
    try
    {
        // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
        arg *tid = (arg *)threadid;
        cout << "Hello Runoob! 线程 ID-a, " << tid->a << endl;
        cout << "Hello Runoob! 线程 ID-b, " << tid->b << endl;
        string a = "123";
        // cout << a[10];
        cout << 4 / 0 << endl;
    }
    catch (exception &e)
    {
        cout << "errno: " << errno << endl
             << strerror(errno) << endl;
        // cout << e << endl;
    }
    pthread_exit(NULL);
}
int main()
{
    try
    {
        pthread_t threads[NUM_THREADS];
        int rc;
        int i;
        for (i = 0; i < NUM_THREADS; i++)
        {
            cout << "main() : 创建线程, " << i << endl;
            arg indexes[NUM_THREADS]; // 用数组来保存i的值
            indexes[i] = {i, i + 1};  //先保存i的值
            // 传入的时候必须强制转换为void* 类型，即无类型指针
            rc = pthread_create(&threads[i], NULL,
                                PrintHello, (void *)&(indexes[i]));
            if (rc)
            {
                cout << "Error:无法创建线程," << rc << endl;
                exit(-1);
            }
        }
        pthread_exit(NULL);
    }
    catch (const char *&e)
    {
        cout << "errno: " << errno << endl
             << strerror(errno) << endl;
        cout << e << endl;
    }
}