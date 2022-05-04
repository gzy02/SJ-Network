src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = $(patsubst %.cpp, %, $(src))
CC = clang++
CFLAGS = -Wno-invalid-source-encoding

.SECONDARY: $(obj)

all: $(target)

%: %.o
	$(CC) $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@	

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
