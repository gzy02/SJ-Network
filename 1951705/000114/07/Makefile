src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = $(patsubst %.cpp, %, $(src))

common_path=../../common
my_daemonsrc_name=my_daemon.cpp
my_daemonsrc_path=$(common_path)/$(my_daemonsrc_name)
my_daemonobj_path=$(patsubst %.cpp, %.o, $(my_daemonsrc_path))

socket_optarg_src_name=socket_optarg.cpp
socket_optarg_src_path=$(common_path)/$(socket_optarg_src_name)
socket_optarg_obj_path=$(patsubst %.cpp, %.o, $(socket_optarg_src_path))

lock_set_name=lock_set.cpp
lock_set_src_path=$(common_path)/$(lock_set_name)
lock_set_obj_path=$(patsubst %.cpp, %.o, $(lock_set_src_path))

need=$(my_daemonobj_path) $(lock_set_obj_path) $(socket_optarg_obj_path)#∆‰À˚.o“¿¿µ

CC = g++
.SECONDARY: $(obj)

all: $(need) $(target) 

%: %.o
	$(CC) $< $(need)  -o $@

%.o: %.cpp
	$(CC) -c $< -o $@	

.PHONY: clean
clean: 
	rm -rf $(obj) $(target) $(need)

.PHONY: cleandir
cleandir: 
	rm -rf $(obj) $(target) 
