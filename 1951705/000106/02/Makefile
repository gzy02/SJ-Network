src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = test
CC = g++

all: $(target) 

$(target): $(obj)
	$(CC) $(obj) -o $(target) -static -L./

%.o: %.cpp
	$(CC) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
