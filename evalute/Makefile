CC?=gcc
LD=$(CC)

CC_FLAGS += -c -I ./include
LD_FLAGS +=

ifeq ($(DEBUG),1)
	CC_FLAGS += -g3 -DDEBUG
	LD_FLAGS += -g3
endif


SOURCES = src/lexer.c src/synt.c src/utils.c src/object.c src/evalute.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)

.PHONY: all clean test


build/%.o : src/%.c
	$(CC) $(CC_FLAGS) $< -o $@


all: $(OBJECTS)

test: $(OBJECTS)
	$(CC) $(CC_FLAGS) main.c -o build/main.o
	$(LD) $(LD_FLAGS) build/main.o $^ -o test


clean:
	rm build/* -f
	rm test -f
