#include<string.h>
#include<stdlib.h>
#include<stdio.h>


struct Param{
  int num;              //参数数量
  const char** name;          //参数名称
  bool* need_value;     //参数是否需要值
  char** result;        //参数的值(字符串形式)
};

/*****************
 * 传入：argc传入参数数量,argv传入main函数的参数，Param结构体传入需要的参数数量、名称、是否有值
 * 传出：Param结构体中result域传出所需参数的值
 * ***************/

int read_param(int argc,char** argv,Param param);