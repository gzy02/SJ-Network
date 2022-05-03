lib=test1
libsrc=$(lib).c
libobj=$(lib).o
libs=lib$(lib).a

target=test2
src=$(target).c
obj=$(target).o
CC=gcc

path=./

all:$(target)

$(target):$(obj) $(libs)
	$(CC) $(obj) -o $(target) -l$(lib) -L $(path)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm -f $(libobj)
	rm -f $(libs)
	rm -f $(obj)
	rm -f $(target)

.PHONY:static
static:$(libobj)
	@if [ -e $(libs) ];then \
		echo $(libs) has existed!;\
	else \
		ar -cr $(libs) $(libobj);\
	fi

$(libs):ECHO

ECHO:
	@if [ ! -e $(libs) ];then \
		echo Please use \"make static\" to build $(libs).;\
	fi
