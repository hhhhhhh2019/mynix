CC?=gcc
LD=$(CC)

OUT=dbpm_eval

CC_FLAGS += -c -I ./include
LD_FLAGS +=

ifeq ($(DEBUG),1)
	CC_FLAGS += -g3 -DDEBUG
	LD_FLAGS += -g3
endif


SOURCES = src/main.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)


all: $(OBJECTS)
	$(LD) $(LD_FLAGS) $^ -o $(OUT)


build/%.o : src/%.c dirs
	$(CC) $(CC_FLAGS) $< -o $@


dirs:
	mkdir -p build


clean:
	rm build/* -f
	rm test -f
