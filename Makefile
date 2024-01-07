CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread

SRC = src/main.c src/server.c
OBJ = $(SRC:.c=.o)
EXEC = simpliC

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)
