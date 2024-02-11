.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

build:
	@gcc $(CFLAGS) client_dgram.c -o client_dgram
	@gcc $(CFLAGS) server_dgram.c -o server_dgram

clean:
	@rm client_dgram
	@rm server_dgram
