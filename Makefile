TARGET := libwpv.a
OBJS := wpv.o

CFLAGS += -Wall

ifdef DEBUG
	CFLAGS := $(CFLAGS) -g3 -ggdb -DDEBUG
endif

.PHONY: build clean tests

build: $(TARGET)

clean:
	rm -f *~ *.o tests

clean-all: clean
	rm -f $(TARGET)

tests: tests.o $(TARGET)
	$(CC) -o tests $(CFLAGS) tests.o -L. -lwpv 
	./tests

%.a: $(OBJS)
	$(AR) rvs $@ $^

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<
