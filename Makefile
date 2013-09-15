CC := clang

CFLAGS := --std=c99 $(INC)
CFLAGS+=-I$(PWD)/include

SOURCES := hashmap.c \
		   main.c \
		   object.c \
		   point.c \
		   person.c \
		   string.c \
		   Int.c 

all: points run

NAME:=point_test

points: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o point_test


run:
	./point_test

clean:
	rm point_test


