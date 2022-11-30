CC = gcc
C_FLAGS = -Wall

target = bishop
c_srcs = drunken_bishop.c
objs = $(subst .c,.o,$(c_srcs))

ifdef DEBUG
	C_FLAGS += -g
else
	C_FLAGS += -O2
endif


.PHONY: all clean mkdir


all: mkdir $(target)


mkdir:
	-mkdir bin


$(objs): %.o: %.c
	$(CC) $(C_FLAGS) -c $< -o bin/$@


$(target): $(objs)
	$(CC) -o bin/$@ $(patsubst %,bin/%,$(objs))


clean:
	-rm bin/*
