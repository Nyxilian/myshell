CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
SRC = src/main.c src/parse.c src/execute.c src/builtin.c src/signals.c src/logger.c
OBJ = $(SRC:.c=.o)
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) myshell.log
