CC = gcc
C_FLAGS = -Wall

target = bin/bishop
c_srcs = drunken_bishop.c

objs = $(patsubst %.c,tmp/%.o,$(c_srcs))

ifdef DEBUG
	C_FLAGS += -g
else
	C_FLAGS += -O2
endif


all: mkdir $(target)

$(target): $(objs)
	$(CC) -o $@ $^

$(objs): tmp/%.o: %.c
	$(CC) $(C_FLAGS) -c $^ -o $@

clean:
	-rm -rf tmp

mkdir:
	-mkdir tmp
	-mkdir bin

.PHONY: all clean mkdir

