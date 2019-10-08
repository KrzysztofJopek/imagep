.DEFAULT_GOAL = imagep
OUTPUT_OPTION = -MMD -MP -o $@
CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
DEP = $(SRC:%.c=%.d)

LDFLAGS = -lm
CFLAGS = -g
INCLUDES = -Iinclude/ 
-include $(DEP)
CFLAGS += $(INCLUDES)

imagep: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

#------

tags: $(SRC)
	ctags -R

.PHONY: clean

clean:
	rm -f $(OBJ) $(DEP) imagep tags
