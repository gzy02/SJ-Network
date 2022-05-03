src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
#obj = $(src:%.c=%.o)
target = test
CC = gcc

all: $(target) 

$(target): $(obj)
	$(CC) $(obj) -o $(target) -static -L./

%.o: %.c
	$(CC) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
