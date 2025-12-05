CC = gcc
CFLAGS = -Wall -Wextra -g -I$(INC_DIR)
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
TARGET = myshell
SRC_DIR = src
INC_DIR = include
LOG = myshell.log

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) $(LOG)
