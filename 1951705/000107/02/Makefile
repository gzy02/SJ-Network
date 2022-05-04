dllsrc=test1.c
dllname=test2
dllfilename=lib$(dllname).so

target=test2
src=$(target).c
obj=$(target).o
dllFlag=-shared

CC=gcc

path=/usr/lib64/u1951705

all:$(target)

$(target):$(obj)
	$(CC) $(obj) -o $(target) -Wl,-rpath="$(path)" -l$(dllname) -L $(path) 

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm -f $(dllfilename)
	rm -f $(obj)
	rm -f $(target)

.PHONY:dynamic
dynamic:
	@if [ -e $(dllfilename) ];then \
		echo $(dllfilename) has existed!;\
	else \
		gcc $(dllFlag) -fPIC -o $(dllfilename) $(dllsrc);\
	fi
	
.PHONY:install
install:
	mkdir -p $(path)
	cp $(dllfilename) $(path)/$(dllfilename)
