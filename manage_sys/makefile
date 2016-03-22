CC=gcc
TARGET=main
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ -lpthread
clean:
	rm $(OBJS) $(TARGET)
	rm message user
