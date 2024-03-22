include subprojects

export PROJECT_PATH=$(PWD)

.PHONY: all build clean code_style $(targets)

all: $(targets)

$(targets):
	@echo -e "\n  $@: make build\n"
	@make -C $@ -f Makefile build

build clean:
	@for i in $(targets); do \
		echo -e "\n  $$i: make $@\n" ; \
		make -C $$i -f Makefile $@; \
	done \

code_style:
	@for i in $(targets); do \
		echo -e "\n  Check $$i ..." ; \
		clang-format $$i/*.[ch] -i --verbose --Werror ; \
	done \
