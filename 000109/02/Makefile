src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
target = $(patsubst %.c, %, $(src))

common_path=../../common/
my_daemonsrc_name=my_daemon.c
my_daemonsrc_path=$(common_path)$(my_daemonsrc_name)
my_daemonobj_path=$(patsubst %.c, %.o, $(my_daemonsrc_path))

CC = gcc
.SECONDARY: $(obj)

all: $(target)

$(target): $(my_daemonobj_path) $(obj)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@	

.PHONY: clean
clean:
	rm -rf $(obj) $(target) $(my_daemonobj_path)
