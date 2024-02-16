obj-m += message.o

CC = gcc
CFLAGS := -ggdb --static

KDIR := /lib/modules/6.6.16-pwnCTF-basicBOF/build
KFLAGS := -g
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) EXTRA_CFLAGS=$(KFLAGS) modules
	$(CC) $(CFLAGS) -o exp exp.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f exp
