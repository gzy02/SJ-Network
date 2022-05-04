// mysql_demo.cpp
#include "../../include/my_optarg.h"
#include <iomanip>  // setw等
#include <iostream> // cin,cout等
#include <mysql.h>  // mysql特有
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

    /* 初始化 mysql 变量，失败返回NULL */
    if ((mysql = mysql_init(NULL)) == NULL)
    {
        cout << "mysql_init failed" << endl;
        return -1;
    }

    /* 连接数据库，失败返回NULL
       1、mysqld没运行
       2、没有指定名称的数据库存在 */
    if (mysql_real_connect(mysql, "localhost", "root", "root123", "demo", 0, NULL, 0) == NULL)
    {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 设置字符集，否则读出的字符乱码，即使/etc/my.cnf中设置也不行 */
    mysql_set_character_set(mysql, "gbk");

    /* 进行查询，成功返回0，不成功非0
       1、查询字符串存在语法错误
       2、查询不存在的数据表 */
    if (mysql_query(mysql, "select * from student"))
    {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
       注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL)
    {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    /* 打印当前查询到的记录的数量 */
    // cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* 循环读取所有满足条件的记录
      1、返回的列顺序与select指定的列顺序相同，从row[0]开始
      2、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
      3、根据自己的需要组织输出格式 */

    // while ((row = mysql_fetch_row(result)) != NULL)
    //{
    //    cout << setiosflags(ios::left);         //输出左对齐
    //    cout << "学号：" << setw(12) << row[0]; //宽度12位，左对齐
    //    cout << "姓名：" << setw(8) << row[1];  //    8
    //    cout << "性别：" << setw(4) << row[2];  //    4
    //    cout << "年龄：" << setw(4) << row[3];  //    4
    //    cout << "系部：" << setw(4) << row[4];  //    4
    //    cout << endl;
    // }

    /* 释放result */
    mysql_free_result(result);

    return 0;
}
int main(int argc, char *argv[])
{
    my_daemon(0, 1);          //切换目录至根目录，不关文件描述符
    setbuf(stdout, NULL);     //设置缓冲区为空，直接输出
    signal(SIGCHLD, SIG_IGN); //子进程状态改变时要给父进程一个信号
    MYSQL *mysql;
    Myoptarg arg;
    Set_Config(argc, argv, &arg);
    int fork_error = 0;
    for (int i = 1; i <= arg.fnum; ++i) // fork fnum个子进程
    {
        if (fork_error)
            break;
        int pid = fork();
        if (pid == -1)
        {
            printf("Fork error\n");
            fork_error = 1;
        }
        else if (pid == 0) //子进程
        {
            prctl(PR_SET_PDEATHSIG, SIGKILL); //父进程状态改变时要给子进程一个信号
            if (get_value(mysql) != 0)
            {
                printf("Mysql error\n");
            };
            while (1)
                sleep(1);
        }
        else //父进程
        {
            if (i % arg.pnum == 0)
                printf("\n已分裂%d个子进程", i);
        }
    } /* 关闭整个连接 */
    mysql_close(mysql);
    if (fork_error == 1)
        printf("Fork error,program exit!\n");
    while (1)
        sleep(1); //第六题貌似要sleep
    return 0;
}
// ./test8-1 --fnum 3000 --pnum 100
// ./test8-2 --fnum 3000 --pnum 100
