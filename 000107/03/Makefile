lib=test3
libsrc=test1.cpp
libobj=test1.o
libs=lib$(lib).a

target=test2
src=$(target).cpp
obj=$(target).o
CC=g++

path=./

all:$(target)

$(target):$(obj) $(libs)
	$(CC) $(obj) -o $(target) -l$(lib) -L $(path)

%.o:%.cpp
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
