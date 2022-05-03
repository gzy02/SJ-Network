src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
dynamic_target=./test8-1
static_target=./test8-2
target = $(dynamic_target) $(static_target)
CC = g++


common_path=../../common/
my_daemonsrc_name=my_daemon.c
my_daemonsrc_path=$(common_path)$(my_daemonsrc_name)
my_daemonobj_path=$(patsubst %.c, %.o, $(my_daemonsrc_path))

my_optargsrc_name=my_optarg.c
my_optargsrc_path=$(common_path)$(my_optargsrc_name)
my_optargobj_path=$(patsubst %.c, %.o, $(my_optargsrc_path))

.PHONY:
all:$(my_daemonobj_path) $(my_optargobj_path) $(target) 
$(dynamic_target): $(obj)
	$(CC) $(my_daemonobj_path) $(my_optargobj_path) $(obj) -o $@ `mysql_config --libs`
$(static_target): $(obj)
	$(CC) $(my_daemonobj_path) $(my_optargobj_path)  $(obj) -o $@ -lmysqlclient -lpthread -lm -lssl -lcrypto -ldl -static -L./
%.o: %.cpp
	$(CC) -c $< -o $@ `mysql_config --cflags`
%.o: %.c
	$(CC) -c $< -o $@
$(my_daemonobj_path):ECHO
	$(CC) -c $(my_daemonsrc_path) -o $@
$(my_optargobj_path):ECHO
	$(CC) -c $(my_optargsrc_path) -o $@
.PHONY: clean
clean:
	rm -rf $(obj) $(target) $(my_daemonobj_path) $(my_optargobj_path)
ECHO:
	@echo
