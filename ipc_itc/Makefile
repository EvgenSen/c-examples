.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

build:
	@make -C local_socket $@
	@gcc $(CFLAGS) select_and_pipes.c -pthread -o select_and_pipes
	@gcc $(CFLAGS) shared_mem_and_semaphore.c -pthread -o shared_mem_and_semaphore
	@gcc $(CFLAGS) threads_and_mutex.c -pthread -o threads_and_mutex

clean:
	@make -C local_socket $@
	@rm select_and_pipes
	@rm shared_mem_and_semaphore
	@rm threads_and_mutex
