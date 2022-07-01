#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//该函数增加count数值
void *creator(void *arg)
{
    cout << "creator add lock" << endl;
    pthread_mutex_lock(&mutex);

    count++;

    cout << "in creator count is : " << count << endl;
    //条件满足时发送信号
    if (count > 0)
    {

        pthread_cond_signal(&cond);
    }

    cout << "creator release lock" << endl;

    pthread_mutex_unlock(&mutex);

    return NULL;
}

//该函数减少count数值
void *consumer(void *arg)
{
    cout << "consumer add lock" << endl;

    pthread_mutex_lock(&mutex);
    //当条件不满足时等待
    while (count <= 0) //防止虚假唤醒
    {
        cout << "begin wait" << endl;
        pthread_cond_wait(&cond, &mutex);
        cout << "end wait" << endl;
    }

    count--;

    cout << "in consumer count is " << count << endl;

    pthread_mutex_unlock(&mutex);

    cout << "consumer release lock" << endl;

    return NULL;
}

int main()
{
    //两个线程，一个生产者线程一个消费者线程
    pthread_t createthread, consumethread;

    pthread_create(&consumethread, NULL, consumer, NULL);
    sleep(2);
    pthread_create(&createthread, NULL, creator, NULL);

    //主进程等待两个线程结束
    pthread_join(createthread, NULL);
    pthread_join(consumethread, NULL);
    return 0;
}