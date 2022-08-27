.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror

all: build

APP = mtdctl

build:
	@gcc $(CFLAGS) $(APP).c -o $(APP)

clean:
	@rm $(APP)
