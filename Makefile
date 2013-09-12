CC := gcc

CFLAGS := --std=c99 $(INC)
CFLAGS+=-I$(PWD)/include

SOURCES := main.c \
		   object.c \
		   point.c \
		   person.c \
		   string.c

all: points run


points: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o point_test


run:
	./point_test

clean:
	rm point_test


