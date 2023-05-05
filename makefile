# target = echo_server
target = http_server
src = $(wildcard *.cc)
obj = $(patsubst %.cc, %.o, $(src))

CC = g++
CFLAGS = -Wall -g -c -O3 -DNDEBUG
LDFLAGS = -pthread


$(target):$(obj)
	$(CC) $(LDFLAGS) $(obj) -g -o $@

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean:
	rm *.o