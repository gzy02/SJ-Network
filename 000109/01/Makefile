src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
target = $(patsubst %.c, %, $(src))
CC = gcc
.SECONDARY: $(obj)

all: $(target)

%: %.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) -c $< -o $@	

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
