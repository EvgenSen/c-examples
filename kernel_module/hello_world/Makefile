.PHONY: all build clean

ifeq ($(PROJECT_PATH),)
SUB_PROJECT_PATH=$(PWD)
else
SUB_PROJECT_PATH=$(PROJECT_PATH)/kernel_module
endif

# For example: https://github.com/raspberrypi/linux
KERNEL_SRC_PATH=

obj-m += hello_world.o

all: build

build:
	@make -C /lib/modules/$(shell uname -r)/build M=$(SUB_PROJECT_PATH) modules

build_arm64:
	@make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KERNEL_SRC_PATH) M=$(SUB_PROJECT_PATH) modules

clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(SUB_PROJECT_PATH) clean
