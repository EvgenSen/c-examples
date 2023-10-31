.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror -O0 -ggdb

all: build

build:
	@gcc $(CFLAGS) main.c wave.c dsp.c -o dtmf_detector -lm

clean:
	@rm dtmf_detector
