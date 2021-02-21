.PHONY: all build clean

ifeq ($(PROJECT_PATH),)
SUB_PROJECT_PATH=$(PWD)
else
SUB_PROJECT_PATH=$(PROJECT_PATH)/kernel_module
endif

obj-m += interaction_proc.o
obj-m += interaction_ioctl.o

all: build

build:
	@make -C /lib/modules/$(shell uname -r)/build M=$(SUB_PROJECT_PATH) modules
	@gcc userspace_ioctl.c -o userspace_ioctl

clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(SUB_PROJECT_PATH) clean
	@rm userspace_ioctl
