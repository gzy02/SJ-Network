CC = g++
CFLAGS =
.PHONY:all clean
all: proxy

proxy:proxy.cpp tcp_init.cpp read_param.cpp
	$(CC) $^ -o $@ $(CFLAGS)

%:%.cpp
	$(CC) $^ -o $@ $(CFLAGS)


clean:
	-rm -f proxy

test:
	./proxy --daemon --port 20013 --proxy_ip 192.168.1.236 --proxy_port 80 --logname /home/u1950638/G000101/proxy.log

