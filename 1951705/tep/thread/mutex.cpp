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
//�ú�������count��ֵ
void *creator(void *arg)
{
    cout << "creator add lock" << endl;
    pthread_mutex_lock(&mutex);

    count++;

    cout << "in creator count is : " << count << endl;
    //��������ʱ�����ź�
    if (count > 0)
    {

        pthread_cond_signal(&cond);
    }

    cout << "creator release lock" << endl;

    pthread_mutex_unlock(&mutex);

    return NULL;
}

//�ú�������count��ֵ
void *consumer(void *arg)
{
    cout << "consumer add lock" << endl;

    pthread_mutex_lock(&mutex);
    //������������ʱ�ȴ�
    while (count <= 0) //��ֹ��ٻ���
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
    //�����̣߳�һ���������߳�һ���������߳�
    pthread_t createthread, consumethread;

    pthread_create(&consumethread, NULL, consumer, NULL);
    sleep(2);
    pthread_create(&createthread, NULL, creator, NULL);

    //�����̵ȴ������߳̽���
    pthread_join(createthread, NULL);
    pthread_join(consumethread, NULL);
    return 0;
}