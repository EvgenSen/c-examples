.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

build:
	@gcc $(CFLAGS) singly_linked_list.c -o singly_linked_list
	@gcc $(CFLAGS) doubly_linked_list.c -o doubly_linked_list
	@gcc $(CFLAGS) array_sort.c -o array_sort
	@gcc $(CFLAGS) array_search.c -o array_search

clean:
	@rm singly_linked_list
	@rm doubly_linked_list
	@rm array_sort
	@rm array_search
