src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = $(patsubst %.cpp, %, $(src))

common_path=../../common
socket_optarg_src_name=socket_optarg.cpp
socket_optarg_src_path=$(common_path)/$(socket_optarg_src_name)
socket_optarg_obj_path=$(patsubst %.cpp, %.o, $(socket_optarg_src_path))

get_local_ip_src_name=get_local_ip.cpp
get_local_ip_src_path=$(common_path)/$(get_local_ip_src_name)
get_local_ip_obj_path=$(patsubst %.cpp, %.o, $(get_local_ip_src_path))

my_daemonsrc_name=my_daemon.cpp
my_daemonsrc_path=$(common_path)/$(my_daemonsrc_name)
my_daemonobj_path=$(patsubst %.cpp, %.o, $(my_daemonsrc_path))

socket_path=Socket_Base Socket_Listen Socket_Accept Socket_Connect
socket_class_src=$(foreach i,$(socket_path),$(wildcard $(common_path)/$(i)/*.cpp))
socket_class_obj=$(patsubst %.cpp, %.o, $(socket_class_src))


client_path=/mnt/client
CC = g++
.SECONDARY: $(obj)

all: $(socket_optarg_obj_path) $(my_daemonobj_path) $(get_local_ip_obj_path) $(socket_class_obj) $(target)
#	all做完后把client端的可执行文件移过去
	@for i in `ls -l|awk '$$1~/[-rwx]*x$$/  {print $$NF}' | awk '$$1~/client/'`;do\
		echo cp $$i $(client_path)/u1951705_$$i;\
		cp $$i $(client_path)/u1951705_$$i ;\
	done;

%: %.o
	$(CC) $< $(socket_optarg_obj_path) $(my_daemonobj_path) $(get_local_ip_obj_path) $(socket_class_obj) -o $@

%.o: %.cpp
	$(CC) -c $< -o $@	

.PHONY: clean
clean: 
	rm -rf $(obj) $(target) $(socket_optarg_obj_path) $(my_daemonobj_path) $(socket_class_obj) $(get_local_ip_obj_path)

.PHONY: cleandir
cleandir: 
	rm -rf $(obj) $(target)
