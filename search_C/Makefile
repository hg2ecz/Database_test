CC=gcc
CFLAGS=-O2 -Wall
LDFLAGS=-s
TARGET=../search_c
TARGET_GEN=indexgen_unordered_ordered

OBJS=search_fopen.o search_indexuse_ordered.o search_indexuse_unordered.o search_mmap.o benchmark.o

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
	$(CC) $(LDFLAGS) $(TARGET_GEN).c -o ../$(TARGET_GEN)

clean:
	rm -f $(OBJS) $(TARGET) ../$(TARGET_GEN)
