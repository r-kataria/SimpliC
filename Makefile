CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c
OBJ = $(SRC:.c=.o)
EXEC = simpliC

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)
