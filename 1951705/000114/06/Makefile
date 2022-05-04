src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = $(patsubst %.cpp, %, $(src))

common_path=../../common
my_daemonsrc_name=my_daemon.cpp
my_daemonsrc_path=$(common_path)/$(my_daemonsrc_name)
my_daemonobj_path=$(patsubst %.cpp, %.o, $(my_daemonsrc_path))

client_path=/mnt/client
CC = g++
.SECONDARY: $(obj)

all: $(my_daemonobj_path) $(target)
#	all做完后把client端的可执行文件移过去
	@for i in `ls -l|awk '$$1~/[-rwx]*x$$/  {print $$NF}' | awk '$$1~/client/'`;do\
		echo cp $$i $(client_path)/u1951705_$$i;\
		cp $$i $(client_path)/u1951705_$$i ;\
	done;

%: %.o
	$(CC) $< $(my_daemonobj_path) -o $@

%.o: %.cpp
	$(CC) -c $< -o $@	

.PHONY: clean
clean: 
	rm -rf $(obj) $(target) $(my_daemonobj_path)

.PHONY: cleandir
cleandir: 
	rm -rf $(obj) $(target)
