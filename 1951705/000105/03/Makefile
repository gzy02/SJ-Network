src = $(wildcard ./*.c)
obj = $(patsubst %.c, %.o, $(src))
target = $(patsubst %.c, %, $(src))
CC = clang
CFLAGS = -Wno-invalid-source-encoding

.SECONDARY: $(obj)

all: $(target)

%: %.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@	

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
