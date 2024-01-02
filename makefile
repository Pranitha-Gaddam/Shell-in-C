CC = gcc
CFLAGS = -Wall -Wextra
OBJS = newshell.o path.o
TARGET = newshell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

newshell.o: newshell.c
	$(CC) $(CFLAGS) -c newshell.c

path.o: path.c
	$(CC) $(CFLAGS) -c path.c

clean:
	rm -f $(TARGET) $(OBJS)

