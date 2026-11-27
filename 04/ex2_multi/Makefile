CFILES = main.c sub.c

obj-m += hello.o
hello-objs := $(CFILES:.c=.o)

KDIR ?= $(HOME)/kernel-src/WSL2-Linux-Kernel
PWD  := $(shell pwd)

all:
		$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
		$(MAKE) -C $(KDIR) M=$(PWD) clean