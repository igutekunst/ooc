CC := gcc
INC := -I./

CFLAGS := --std=c99 $(INC)

SOURCES := main.c \
		   object.c \
		   point.c \
		   person.c

all: points run


points: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o point_test


run:
	./point_test


