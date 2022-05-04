allsublist = $(shell ls -l |awk '/^d/ {print $$NF}')
allmake = $(foreach n,$(allsublist),$(wildcard ./$(n)/Makefile))
sublist = $(patsubst ./%/Makefile,%,$(allmake))
.PHONY:all
all:$(sublist)


.PHONY:clean
clean:$(sublist)

$(sublist):ECHO
	@echo start $@
	@-if [ `cat $@/Makefile | grep $(MAKECMDGOALS):|wc -L` -ne 0 ] ;then make $(MAKECMDGOALS) -C $@ ;else echo no this command;fi
	@echo end $@

ECHO:
	@echo start with $(sublist)
