NAME=triple-fault

ifndef KERNELRELEASE
ifndef KDIR
KDIR:=/lib/modules/`uname -r`/build
endif
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
clean:
	rm -f *.o *.ko *.mod* .*.cmd Module.symvers modules.order
	rm -rf .tmp_versions
else
	obj-m := $(NAME).o
endif
