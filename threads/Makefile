.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

build:
	@gcc $(CFLAGS) threads_and_mutex.c -pthread -o threads_and_mutex

clean:
	@rm threads_and_mutex
