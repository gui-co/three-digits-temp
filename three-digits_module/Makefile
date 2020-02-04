obj-m += three-digits.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C '$(LINUX_DIR)' M='$(PWD)' clean

