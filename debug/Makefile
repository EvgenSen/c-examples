.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror -g3 -rdynamic

all: build

build:
	@gcc $(CFLAGS) backtrace.c -pthread -o backtrace
	@gcc $(CFLAGS) array_sort.c -o array_sort

clean:
	@rm backtrace
