obj-m += hw_probe.o

#KDIR=/usr/src/linux-4.16.8
KDIR=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
