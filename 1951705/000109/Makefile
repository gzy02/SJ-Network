allsublist = $(shell ls -l |awk '/^d/ {print $$NF}')
allmake = $(foreach n,$(allsublist),$(wildcard ./$(n)/Makefile))
sublist = $(patsubst ./%/Makefile,%,$(allmake))
.PHONY:all
all:$(sublist)
	@for i in $(sublist);do for j in `ls $$i -F | grep "*" | awk '{print substr($$0,1,length($$0)-1)}'`;do cp $$i/$$j ./;done;done

.PHONY:clean
clean:$(sublist)
	@rm -f $(shell ls -F | grep "*" | awk '{print substr($$0,1,length($$0)-1)}')

$(sublist):ECHO
	@echo start $@
	@-if [ `cat $@/Makefile | grep $(MAKECMDGOALS):|wc -L` -ne 0 ] ;then make $(MAKECMDGOALS) -C $@ ;else echo no this command;fi
	@echo end $@

ECHO:
	@echo start with $(sublist)
