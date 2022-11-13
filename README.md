# PCI Driver Simulator in Linux

This is a Linux driver in kernel space that simulates PCI device access. It allocates a memory buffer for register access, registers a character device and implements the file operations to be called by the user space application.

## Use Case
This simulator is useful when the user space application needs to be tested with Linux kernel driver but the PCI devices are not available or not at hand.

This simulator is suitable for simple devices that merely implements register access but does not need interrupt or DMA interface. 

## Implementation

* Only PCI register access is implemented, interrupt and DMA are not taken into account.
* PCI device registers are simulated by a buffer in memory.
* A character device is registered to be opened by the user space application.
* PCI register access interface is implemented in ioctl() file operation with different commands.
* The test module in user space is a simple command line shell that supports register read/write and waveform read commands. 

## Development Environment

* Ubuntu 20.04 LTS
* Linux kernel 5.15.0
* gcc 9.4.0

## Compile and Execute
```
$ cd pci_simulator
$ make
# make load
$ gcc test.c
$ ./a.out
```

![Alt text](docs/screenshots/make.png?raw=true "Title")

![Alt text](docs/screenshots/execute.png?raw=true "Title")

## Log Info
Logs printed in kernel can be displayed using "dmesg".
```
$ dmesg -wH
```

![Alt text](docs/screenshots/dmesg.png?raw=true "Title")

## Command Usage

### **help**

![Alt text](docs/screenshots/help.png?raw=true "Title")

### **read and write**

![Alt text](docs/screenshots/read_write.png?raw=true "Title")

### **waveform read**

![Alt text](docs/screenshots/waveform_read.png?raw=true "Title")

![Alt text](docs/screenshots/waveform_data.png?raw=true "Title")