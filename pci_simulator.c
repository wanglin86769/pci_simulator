/**
 * pci_simulator.c  --  File operation implementation for PCI simulator Linux driver
 *                      Ubuntu 20.04 LTS
 *                      Linux kernel 5.15.0
 *
 * Author: Lin Wang (CSNS)
 * Email:  wanglin@ihep.ac.cn
 * Date:   November 9, 2022
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>

#include "pci_simulator.h"

#define REGISTER_BUFFER_LENGTH 0x00100000 // 1MB

/* Read and write registers */
#define	   REG_READ8(addr)             (*(volatile UINT8 *)(addr))
#define    REG_WRITE8(addr, value)     (*((volatile UINT8 *)(addr))=(value))
#define	   REG_READ16(addr)            (*(volatile UINT16 *)(addr))
#define	   REG_WRITE16(addr, value)    (*((volatile UINT16 *)(addr))=(value))
#define	   REG_READ32(addr)            (*(volatile UINT32 *)(addr))
#define	   REG_WRITE32(addr, value)    (*((volatile UINT32 *)(addr))=(value))

/* Major number of character device driver  */
#define MYMAJOR 64

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lin Wang");
MODULE_DESCRIPTION("A simple example for PCI simulator file operations in a Linux kernel");

/* Simulator PCI device registers */
UINT8 register_buffer[REGISTER_BUFFER_LENGTH] = {0};


/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("pci_simulator - open was called!\n");
	return 0;
}


/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("pci_simulator - close was called!\n");
	return 0;
}


/**
 * @brief File operation: read()
 */
static ssize_t driver_read(struct file *file_p, char __user *buf, size_t length, loff_t *f_pos)  
{
    int ret;
    printk("pci_simulator - read was called!\n");  
  
    ret = copy_to_user(buf, register_buffer + 0, length);
    if(ret) {
		printk("driver_read - copy_to_user failed!\n"); 
	}

    return length;
}


/**
 * @brief File operation: write()
 */
static ssize_t driver_write(struct file *file_p, const char __user *buf, size_t length, loff_t *f_pos)  
{    
    int ret;
    printk("pci_simulator - write was called!\n");  
    
    ret = copy_from_user(register_buffer + 0, buf, length);
    if(ret) {
		printk("driver_write - copy_from_user failed!\n"); 
	}
      
    return length;  
}


/**
 * @brief File operation: ioctl()
 */ 
static long int driver_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	int ret;
	int i;
	IO_VALUE _ioValue;
	IO_VALUE * ioValue = &_ioValue;
    IO_WAVEFORM _ioWaveform;
    IO_WAVEFORM * ioWaveform = &_ioWaveform;
    UINT32 offset;
    UINT32 length;
    char *buffer;
    ktime_t start_time, stop_time, elapsed_time;

	switch(cmd) {
		case RD_VALUE_8:
            ret = copy_from_user(ioValue, (IO_VALUE *)arg, sizeof(IO_VALUE));
            if(ret) {
                printk("driver_ioctl - RD_VALUE_8 - copy_from_user failed!\n"); 
            }
            offset = ioValue->offset;
            ioValue->value_8 = REG_READ8(register_buffer + offset);
            ret = copy_to_user((IO_VALUE *) arg, ioValue, sizeof(IO_VALUE));
            if(ret) {
                printk("driver_ioctl - RD_VALUE_8 - copy_to_user failed!\n"); 
            }
            printk("RD_VALUE_8    addr = 0x%08X    data = 0x%02X\n", ioValue->offset, ioValue->value_8); 
			break;

        case WR_VALUE_8:
			ret = copy_from_user(ioValue, (IO_VALUE *)arg, sizeof(IO_VALUE));
			if(ret) {
                printk("driver_ioctl - WR_VALUE_8 - copy_from_user failed!\n"); 
            }
            offset = ioValue->offset;
            REG_WRITE8(register_buffer + offset, ioValue->value_8);
            printk("WR_VALUE_8    addr = 0x%08X    data = 0x%02X\n", ioValue->offset, ioValue->value_8); 
			break;

        case RD_VALUE_16:
            ret = copy_from_user(ioValue, (IO_VALUE *)arg, sizeof(IO_VALUE));
            if(ret) {
                printk("driver_ioctl - RD_VALUE_16 - copy_from_user failed!\n"); 
            }
            offset = ioValue->offset;
            ioValue->value_16 = REG_READ16(register_buffer + offset);
            ret = copy_to_user((IO_VALUE *) arg, ioValue, sizeof(IO_VALUE));
            if(ret) {
                printk("driver_ioctl - RD_VALUE_16 - copy_to_user failed!\n"); 
            }
            printk("RD_VALUE_16    addr = 0x%08X    data = 0x%04X\n", ioValue->offset, ioValue->value_16); 
			break;

        case WR_VALUE_16:
			ret = copy_from_user(ioValue, (IO_VALUE *)arg, sizeof(IO_VALUE));
			if(ret) {
                printk("driver_ioctl - WR_VALUE_16 - copy_from_user failed!\n"); 
            }
            offset = ioValue->offset;
            REG_WRITE16(register_buffer + offset, ioValue->value_16);
            printk("WR_VALUE_16    addr = 0x%08X    data = 0x%04X\n", ioValue->offset, ioValue->value_16); 
			break;

        case RD_VALUE_32:
            ret = copy_from_user(ioValue, (IO_VALUE *)arg, sizeof(IO_VALUE));
            if(ret) {
                printk("driver_ioctl - RD_VALUE_32 - copy_from_user failed!\n"); 
            }
            offset = ioValue->offset;
            ioValue->value_32 = REG_READ32(register_buffer + offset);
            ret = copy_to_user((IO_VALUE *) arg, ioValue, sizeof(IO_VALUE));
            if(ret) {
                printk("driver_ioctl - RD_VALUE_32 - copy_to_user failed!\n"); 
            }
            printk("RD_VALUE_32    addr = 0x%08X    data = 0x%08X\n", ioValue->offset, ioValue->value_32); 
			break;

        case WR_VALUE_32:
			ret = copy_from_user(ioValue, (IO_VALUE *)arg, sizeof(IO_VALUE));
			if(ret) {
                printk("driver_ioctl - WR_VALUE_32 - copy_from_user failed!\n"); 
            }
            offset = ioValue->offset;
            REG_WRITE32(register_buffer + offset, ioValue->value_32);
            printk("WR_VALUE_32    addr = 0x%08X    data = 0x%08X\n", ioValue->offset, ioValue->value_32); 
			break;

        case RD_WAVEFORM:
            ret = copy_from_user(ioWaveform, (IO_WAVEFORM *)arg, sizeof(IO_WAVEFORM));
            if(ret) {
                printk("driver_ioctl - RD_WAVEFORM - copy_from_user failed!\n"); 
            }
            offset = ioWaveform->offset;
            length = ioWaveform->length;
            buffer = ioWaveform->buffer;
            
            start_time = ktime_get(); /* Time measurement */
			ret = copy_to_user(buffer, register_buffer + offset, length);
			stop_time = ktime_get(); /* Time measurement */
			elapsed_time = ktime_sub(stop_time, start_time);
            printk("driver_ioctl - RD_WAVEFORM - The elapsed time of copy_to_user() is %lld nano seconds\n", ktime_to_ns(elapsed_time));
                
			if(ret) {
                printk("driver_ioctl - RD_WAVEFORM - copy_to_user failed!\n"); 
            }
            printk("RD_WAVEFORM    addr = 0x%08X    length = %d\n", offset, length);
			break;
			
		case WR_WAVEFORM:
		    if(*(UINT16 *)(register_buffer + WAVEFORM_OFFSET) == 0) { // Not initialized yet
				for(i=0; i<WAVEFORM_NUMBER*WAVEFORM_POINT; i++) {
				    *((UINT16 *)(register_buffer + WAVEFORM_OFFSET) + i) = i + 1;
				}
		    } else {  // Already initialized
		        for(i=0; i<WAVEFORM_NUMBER*WAVEFORM_POINT; i++) {
				    *((UINT16 *)(register_buffer + WAVEFORM_OFFSET) + i) += 1;
				}
		    }
			break;

        default:
            printk("Unknown ioctl command!\n");
            break;
	}

	return OK;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.read    = driver_read,  
    .write   = driver_write, 
	.release = driver_close,
	.unlocked_ioctl = driver_ioctl
};


/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	int retval;
	printk("Hello, Kernel!\n");
	/* register device nr. */
	retval = register_chrdev(MYMAJOR, "pci_simulator", &fops);
	if(retval == 0) {
		printk("pci_simulator - registered Device number Major: %d, Minor: %d\n", MYMAJOR, 0);
	}
	else if(retval > 0) {
		printk("pci_simulator - registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
	}
	else {
		printk("Could not register device number!\n");
		return -1;
	}
	return 0;
}


/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	unregister_chrdev(MYMAJOR, "pci_simulator");
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


