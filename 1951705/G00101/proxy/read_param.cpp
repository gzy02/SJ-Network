#include"read_param.h"
/*****************
 * 传入：argc传入参数数量,argv传入main函数的参数，Param结构体传入需要的参数数量、名称、是否有值
 * 传出：Param结构体中result域传出所需参数的值
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
          if(index+1==argc){      //后续没有值了，出现错误
            printf("参数格式有误");
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
    if(i==param.num){            //参数格式不匹配
      printf("参数格式有误");
      return -1;
    }

  }
  return 1;
}