obj-m := main.o

KDIR ?= /home/higor/kernel-src/WSL2-Linux-Kernel

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean