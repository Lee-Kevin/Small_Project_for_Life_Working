TARGET = list_test
CFLAGS += -g

all: $(TARGET) install
list_test:main.o list.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	
install:

 
.PHONY: clean
clean:
	rm -rf *.o $(TARGET) $(CFG_TARGET) $(LED_TARGET) bin/


