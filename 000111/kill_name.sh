kill `ps -ef| grep $1 | grep -v grep | awk '$9~/\[main/ {print $2}'`
