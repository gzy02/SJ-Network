src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
target = $(patsubst %.c, %, $(src))

common_path=../../common/
my_daemonsrc_name=my_daemon.c
my_daemonsrc_path=$(common_path)$(my_daemonsrc_name)
my_daemonobj_path=$(patsubst %.c, %.o, $(my_daemonsrc_path))

my_optargsrc_name=my_optarg.c
my_optargsrc_path=$(common_path)$(my_optargsrc_name)
my_optargobj_path=$(patsubst %.c, %.o, $(my_optargsrc_path))

CC = gcc
.SECONDARY: $(obj)

all: $(my_daemonobj_path) $(my_optargobj_path) $(target)

%: %.o
	$(CC) $< $(my_daemonobj_path) $(my_optargobj_path) -o $@

%.o: %.c
	$(CC) -c $< -o $@	

.PHONY: clean
clean:
	rm -rf $(obj) $(target) $(my_daemonobj_path) $(my_optargobj_path)
