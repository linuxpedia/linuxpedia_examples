obj-m +=k_timer1.o k_timer2.o
KDIR= /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

modules clean distclean mrproper:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) $@
#clean:
#	rm -rf *.o *.ko *.mod.* .c* .t*
