src = $(wildcard ./*.cpp)
obj = $(patsubst %.cpp, %.o, $(src))
target = mysql_demo
CC = g++

all: $(target) 

$(target): $(obj)
	$(CC) $(obj) -o $(target) -lmysqlclient -lpthread -lm -lssl -lcrypto -ldl -static -L./
%.o: %.cpp
	$(CC) -c $< -o $@ `mysql_config --cflags`

.PHONY: clean
clean:
	rm -rf $(obj) $(target)
