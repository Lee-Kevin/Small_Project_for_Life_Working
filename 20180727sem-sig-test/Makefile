TARGET = app
CC = gcc
CFLAGS = -w -g -Wall -O2
LIBS = -lpthread 
INCS = -I./ -I./include -I./log -I./json -I./src

DIRS = . ./include ./log ./json ./src

FILES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))

OBJS = $(patsubst %.c,%.o,$(FILES))

$(TARGET):$(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBS)

$(OBJS):%.o:%.c
		@$(CC) $(INCS) -c $< $(CFLAGS) -o $@

clean:
		-rm -f $(TARGET) $(OBJS)
