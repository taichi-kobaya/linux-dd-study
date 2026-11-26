obj-m += hello.o

KDIR ?= $(HOME)/kernel-src/WSL2-Linux-Kernel
PWD  := $(shell pwd)

all:
		$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean