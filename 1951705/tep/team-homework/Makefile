src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
#obj = $(src:%.c=%.o)

target = client
CC = g++

all: $(target) 

$(target): $(obj)
	$(CC) $(obj) -o $(target)

%.o: %.cpp
	$(CC) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
