src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = $(patsubst %.cpp, %, $(src))

common_path=../../common
socket_optarg_src_name=socket_optarg.cpp
socket_optarg_src_path=$(common_path)/$(socket_optarg_src_name)
socket_optarg_obj_path=$(patsubst %.cpp, %.o, $(socket_optarg_src_path))

socket_base_path=Socket_Base
socket_base_src=$(wildcard $(common_path)/$(socket_base_path)/*.cpp)
socket_base_obj=$(patsubst %.cpp, %.o, $(socket_base_src))



client_path=/mnt/client
CC = g++
.SECONDARY: $(obj)

all: $(socket_optarg_obj_path) $(socket_base_obj) $(target)
#	all做完后把client端的可执行文件移过去
	@for i in `ls -l|awk '$$1~/[-rwx]*x$$/  {print $$NF}' | awk '$$1~/client/'`;do\
		echo cp $$i $(client_path)/u1951705_$$i;\
		cp $$i $(client_path)/u1951705_$$i ;\
	done;

%: %.o
	$(CC) $< $(socket_optarg_obj_path) -o $@

%.o: %.cpp
	$(CC) -c $< -o $@	

.PHONY: clean
clean: 
	rm -rf $(obj) $(target) $(socket_optarg_obj_path) $(socket_base_obj)
