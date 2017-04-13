CC=gcc
CFLAGS=-O2 -Wall
LDFLAGS=-s -lsqlite3

OBJS1 = insert.o
OBJS2 = select.o

TARGET1=../insert_sqlite
TARGET2=../select_sqlite

all: insert select

insert: $(OBJS1)
	$(CC) $(OBJS1) $(LDFLAGS) -o $(TARGET1)

select: $(OBJS2)
	$(CC) $(OBJS2) $(LDFLAGS) -o $(TARGET2)

clean:
	rm -f $(OBJS1) $(OBJS2) $(TARGET1) $(TARGET2)
