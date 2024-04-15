.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror -g3 -rdynamic

all: build

build:
	@gcc $(CFLAGS) example.c -o example

clean:
	@rm example
