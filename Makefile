PWD	:= $(shell pwd)
KERNEL_DIR	:=/lib/modules/$(shell uname -r)/build
obj-m	:=netlink.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules 
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	rm -rf Module.symvers Module.markers 
remake:
	make clean
	make
