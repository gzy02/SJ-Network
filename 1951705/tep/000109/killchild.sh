#! /bin/bash
string=`ps -ef | grep $1 | grep -v grep | awk '{print $2,$3}'`
array=($string)
# echo ${array[@]}
IFS=$OLD_IFS
temp=()
for(( i=1;i<${#array[@]};i+=2)) 
do
    if [ ${array[i]} -ne 1 ]#������id��Ϊ1
    then
        kill ${array[i-1]}
    fi
done
