sublist= $(shell ls -l| grep ^d |awk '{print $$NF}' )#获得当前目录的子文件夹名
allmake=$(foreach n,$(sublist),$(wildcard ./$(n)/Makefile))
make_list=$(patsubst ./%/Makefile,%,$(allmake))
MAKE=make
CLEAN=make clean

all:$(make_list)

.PHONY: clean
clean:$(make_list)

$(make_list):ECHO
ifneq "$(MAKECMDGOALS)" 'clean'# 需用if-else 否则定义两次all/clean会有冲突
	$(MAKE) -C $@
#   上述代码等价于 cd $@ && $(MAKE)
else
	$(CLEAN) -C $@
endif

ECHO:
	@echo $(make_list)
