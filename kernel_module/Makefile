.PHONY: all build clean

ifeq ($(PROJECT_PATH),)
SUB_PROJECT_PATH=$(PWD)
else
SUB_PROJECT_PATH=$(PROJECT_PATH)/kernel_module
endif

obj-m += hello_world.o interaction_proc.o

all: build

build:
	@make -C /lib/modules/$(shell uname -r)/build M=$(SUB_PROJECT_PATH) modules

clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(SUB_PROJECT_PATH) clean
