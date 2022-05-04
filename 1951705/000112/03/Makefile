src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = $(patsubst %.cpp, %, $(src))

common_path=../../common/
socket_optarg_src_name=socket_optarg.cpp
socket_optarg_src_path=$(common_path)$(socket_optarg_src_name)
socket_optarg_obj_path=$(patsubst %.cpp, %.o, $(socket_optarg_src_path))

get_local_ip_src_name=get_local_ip.cpp
get_local_ip_src_path=$(common_path)$(get_local_ip_src_name)
get_local_ip_obj_path=$(patsubst %.cpp, %.o, $(get_local_ip_src_path))

client_path=/mnt/client

CC = g++
.SECONDARY: $(obj)

all: $(socket_optarg_obj_path) $(get_local_ip_obj_path) $(target)
	@for i in `ls -l|awk '$$1~/[-rwx]*x$$/  {print $$NF}' | awk '$$1~/client/'`;do\
		echo cp $$i $(client_path)/u1951705_$$i;\
		cp $$i $(client_path)/u1951705_$$i ;\
	done;
%: %.o
	$(CC) $< $(get_local_ip_obj_path) $(socket_optarg_obj_path) -o $@
#	cp $@ $(client_path)/u1951705_$@

%.o: %.cpp
	$(CC) -c $< -o $@	

.PHONY: clean
clean:
	rm -rf $(obj) $(target) $(socket_optarg_obj_path)
