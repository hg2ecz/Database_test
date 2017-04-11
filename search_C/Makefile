CC=gcc
CFLAGS=-O2 -Wall -DLOG2CHUNKSIZE=16
LDFLAGS=-s
TARGET=../search_c
TARGET_GEN=indexgen_unordered_ordered

OBJS = search_fopen.o search_mmap.o search_indexuse_unordered.o search_indexuse_partially.o
OBJS+= search_indexuse_partially_multi.o search_indexuse_ordered.o benchmark.o

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TARGET_GEN).c -o ../$(TARGET_GEN)

clean:
	rm -f $(OBJS) $(TARGET) ../$(TARGET_GEN)
