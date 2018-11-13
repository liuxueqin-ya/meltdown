obj-m:=helloko.o
KERNELBUILD := /lib/modules/`uname -r`/build
default:
	@echo "BUILD kmod"
	@make -C $(KERNELBUILD) M=$(shell pwd) modules
clean:
	@echo "CLEAN kmod"
	@rm -rf *.o *.ko
	@rm -rf .depend .*.cmd *.mod.c .tmp_versions *.symvers .*.d *.markers *.order
