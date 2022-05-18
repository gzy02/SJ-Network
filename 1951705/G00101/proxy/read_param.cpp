#include"read_param.h"
/*****************
 * ���룺argc�����������,argv����main�����Ĳ�����Param�ṹ�崫����Ҫ�Ĳ������������ơ��Ƿ���ֵ
 * ������Param�ṹ����result�򴫳����������ֵ
 * ***************/

int read_param(int argc,char** argv,Param param){
  if(argc==1){
    return 1;
  }
  int index=1;
  int i;
  while(index<argc){

    for(i=0;i<param.num;i++){
      if(!strcmp(argv[index],param.name[i])){
        if(param.need_value[i]){
          if(index+1==argc){      //����û��ֵ�ˣ����ִ���
            printf("������ʽ����");
            return -1;
          }
          param.result[i]=strdup(argv[index+1]);
          index+=2;
          break;
        }
        else{
          param.result[i]=strdup("true");
          index++;
          break;
        }
      }
    }
    if(i==param.num){            //������ʽ��ƥ��
      printf("������ʽ����");
      return -1;
    }

  }
  return 1;
}