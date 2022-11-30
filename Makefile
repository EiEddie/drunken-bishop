CC = gcc
C_FLAGS = -Wall

target = drunken_bishop
c_srcs = drunken_bishop.c

ifdef DEBUG
	C_FLAGS += -g
else
	C_FLAGS += -O2
endif


.PHONY: all clean mkdir


all: $(target)


mkdir:
	-mkdir bin


.c.o: mkdir
	$(CC) $(C_FLAGS) -c $< -o bin/$@


objs = $(target).o
$(target): mkdir $(objs)
	$(CC) -o bin/$@ $(patsubst %,bin/%,$(objs))


clean:
	-rm bin/*
