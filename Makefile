CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lncurses
SRC = main.c model.c view.c controller.c
OBJ = $(SRC:.c=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
