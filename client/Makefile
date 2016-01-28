CC=gcc
TG=client
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
$(TG):$(OBJS)
	$(CC) -o $@ $^ -lpthread

	rm *.o

clean:
	rm $(TG) $(OBJS)
