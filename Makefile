obj-m += pci_simulator.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	insmod ./pci_simulator.ko
	mknod /dev/pci_simulator c 64 0
	chmod 666 /dev/pci_simulator
	
unload:
	rmmod pci_simulator
	rm /dev/pci_simulator
