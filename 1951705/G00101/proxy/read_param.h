#include<string.h>
#include<stdlib.h>
#include<stdio.h>


struct Param{
  int num;              //��������
  const char** name;          //��������
  bool* need_value;     //�����Ƿ���Ҫֵ
  char** result;        //������ֵ(�ַ�����ʽ)
};

/*****************
 * ���룺argc�����������,argv����main�����Ĳ�����Param�ṹ�崫����Ҫ�Ĳ������������ơ��Ƿ���ֵ
 * ������Param�ṹ����result�򴫳����������ֵ
 * ***************/

int read_param(int argc,char** argv,Param param);