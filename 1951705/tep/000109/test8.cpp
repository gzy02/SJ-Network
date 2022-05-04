// mysql_demo.cpp
#include "../../include/my_optarg.h"
#include <iomanip>  // setw��
#include <iostream> // cin,cout��
#include <mysql.h>  // mysql����
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>
using namespace std;
void my_daemon(int nochdir, int noclose);

int get_value(MYSQL *mysql)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    /* ��ʼ�� mysql ������ʧ�ܷ���NULL */
    if ((mysql = mysql_init(NULL)) == NULL)
    {
        cout << "mysql_init failed" << endl;
        return -1;
    }

    /* �������ݿ⣬ʧ�ܷ���NULL
       1��mysqldû����
       2��û��ָ�����Ƶ����ݿ���� */
    if (mysql_real_connect(mysql, "localhost", "root", "root123", "demo", 0, NULL, 0) == NULL)
    {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* �����ַ���������������ַ����룬��ʹ/etc/my.cnf������Ҳ���� */
    mysql_set_character_set(mysql, "gbk");

    /* ���в�ѯ���ɹ�����0�����ɹ���0
       1����ѯ�ַ��������﷨����
       2����ѯ�����ڵ����ݱ� */
    if (mysql_query(mysql, "select * from student"))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* ����ѯ����洢���������ִ����򷵻�NULL
       ע�⣺��ѯ���ΪNULL�����᷵��NULL */
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    /* ��ӡ��ǰ��ѯ���ļ�¼������ */
    // cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* ѭ����ȡ�������������ļ�¼
      1�����ص���˳����selectָ������˳����ͬ����row[0]��ʼ
      2���������ݿ�����ʲô���ͣ�C�ж��������ַ��������д�������б�Ҫ����Ҫ�Լ�����ת��
      3�������Լ�����Ҫ��֯�����ʽ */

    // while ((row = mysql_fetch_row(result)) != NULL)
    //{
    //    cout << setiosflags(ios::left);         //��������
    //    cout << "ѧ�ţ�" << setw(12) << row[0]; //���12λ�������
    //    cout << "������" << setw(8) << row[1];  //    8
    //    cout << "�Ա�" << setw(4) << row[2];  //    4
    //    cout << "���䣺" << setw(4) << row[3];  //    4
    //    cout << "ϵ����" << setw(4) << row[4];  //    4
    //    cout << endl;
    // }

    /* �ͷ�result */
    mysql_free_result(result);

    return 0;
}
int main(int argc, char *argv[])
{
    my_daemon(0, 1);          //�л�Ŀ¼����Ŀ¼�������ļ�������
    setbuf(stdout, NULL);     //���û�����Ϊ�գ�ֱ�����
    signal(SIGCHLD, SIG_IGN); //�ӽ���״̬�ı�ʱҪ��������һ���ź�
    MYSQL *mysql;
    Myoptarg arg;
    Set_Config(argc, argv, &arg);
    int fork_error = 0;
    for (int i = 1; i <= arg.fnum; ++i) // fork fnum���ӽ���
    {
        if (fork_error)
            break;
        int pid = fork();
        if (pid == -1)
        {
            printf("Fork error\n");
            fork_error = 1;
        }
        else if (pid == 0) //�ӽ���
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //������״̬�ı�ʱҪ���ӽ���һ���ź�
            if (get_value(mysql) != 0)
            {
                printf("Mysql error\n");
            };
            while (1)
                sleep(1);
        }
        else //������
        {
            if (i % arg.pnum == 0)
                printf("\n�ѷ���%d���ӽ���", i);
        }
    } /* �ر��������� */
    mysql_close(mysql);
    if (fork_error == 1)
        printf("Fork error,program exit!\n");
    while (1)
        sleep(1); //������ò��Ҫsleep
    return 0;
}
// ./test8-1 --fnum 3000 --pnum 100
// ./test8-2 --fnum 3000 --pnum 100
