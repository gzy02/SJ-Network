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
        // �Դ���Ĳ�������ǿ������ת������������ָ���Ϊ������ָ�룬Ȼ���ٶ�ȡ
        arg *tid = (arg *)threadid;
        cout << "Hello Runoob! �߳� ID-a, " << tid->a << endl;
        cout << "Hello Runoob! �߳� ID-b, " << tid->b << endl;
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
            cout << "main() : �����߳�, " << i << endl;
            arg indexes[NUM_THREADS]; // ������������i��ֵ
            indexes[i] = {i, i + 1};  //�ȱ���i��ֵ
            // �����ʱ�����ǿ��ת��Ϊvoid* ���ͣ���������ָ��
            rc = pthread_create(&threads[i], NULL,
                                PrintHello, (void *)&(indexes[i]));
            if (rc)
            {
                cout << "Error:�޷������߳�," << rc << endl;
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