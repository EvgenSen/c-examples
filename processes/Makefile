.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

build:
	@gcc $(CFLAGS) shared_mem_and_semaphore.c -pthread -o shared_mem_and_semaphore

clean:
	@rm shared_mem_and_semaphore
