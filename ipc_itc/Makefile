.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

build:
	@make -C local_socket $@
	@gcc $(CFLAGS) shared_mem_and_semaphore.c -pthread -o shared_mem_and_semaphore
	@gcc $(CFLAGS) threads_and_mutex.c -pthread -o threads_and_mutex

clean:
	@make -C local_socket $@
	@rm shared_mem_and_semaphore
	@rm threads_and_mutex
