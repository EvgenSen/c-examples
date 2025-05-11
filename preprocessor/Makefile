.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror
APP     = preprocessor

all: build

build:
	@gcc $(CFLAGS) $(APP).c -o $(APP)

clean:
	@rm $(APP)
