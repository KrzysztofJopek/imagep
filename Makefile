CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
LDFLAGS = -lm
CFLAGS = -g
INC = -Iinclude/ 

all: imagep

imagep: $(OBJ)
	$(CC) $(PR) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(PR) -c $< -o $@ $(INC)

clean:
	rm -f $(OBJ) imagep
