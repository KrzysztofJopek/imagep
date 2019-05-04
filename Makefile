CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
LDFLAGS = -lm
CFLAGS = -g
INC = -Iinclude/ 

all: imagep

imagep: $(OBJ)
	$(CC) $(PR) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(PR) $(CFLAGS) -c $< -o $@ $(INC)

clean:
	rm -f $(OBJ) imagep
