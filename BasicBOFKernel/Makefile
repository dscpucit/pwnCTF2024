obj-m += message.o

CC = gcc

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	$(CC) -o exp exp.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f exp