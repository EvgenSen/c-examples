.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror -g3 -rdynamic

all: build

build:
	@gcc $(CFLAGS) perf.c -pthread -o perf_test

clean:
	@rm perf_test
