CC=gcc
TG=server
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
$(TG):$(OBJS)
	$(CC) -o $@ $^ -lpthread

	rm *.o

clean:
	rm $(TG) $(OBJS)
