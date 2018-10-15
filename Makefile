include subprojects

export PROJECT_PATH=$(PWD)

.PHONY: all build clean $(targets)

all: $(targets)

$(targets):
	@echo "\n  $@: make build\n"
	@make -C $@ -f Makefile build

build clean:
	@for i in $(targets); do \
		echo "\n  $$i: make $@\n" ; \
		make -C $$i -f Makefile $@; \
	done \
