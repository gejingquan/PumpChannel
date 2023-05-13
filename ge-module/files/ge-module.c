#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/uaccess.h>



#define DEVICE_NAME "write_mem_module"


MODULE_AUTHOR("Gerty");
MODULE_DESCRIPTION("write_mem_driver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");


static int write_mem_driver_major;
static struct class*  write_mem_driver_class   = NULL;
static struct device* write_mem_driver_device  = NULL;




static int write_mem_open(struct inode * inode, struct file * filp)
{
  return 0;
}


static int write_mem_release(struct inode * inode, struct file *filp)
{
  return 0;
}


ssize_t write_mem_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{

    return 0;
}

ssize_t write_mem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	unsigned long shm_vir_addr   =   0;
	unsigned long shm_phy_addr   =   0;

	unsigned long mem_data;
	mem_data = 0x1234567890abcdef;
	printk("mem_data : %lx\r\n", mem_data);
	
	shm_phy_addr=arg;
	shm_vir_addr = (unsigned long)phys_to_virt(shm_phy_addr);
	
	// printk("shm_phy_addr : %lx\r\n", shm_phy_addr);
	// printk("shm_vir_addr : %lx\r\n", shm_vir_addr);
	
	iowrite64(mem_data,(unsigned long *)shm_vir_addr);
	printk("addr=0x%lx,data=0x%lx\n",shm_vir_addr,mem_data);
	

	
	return 0;	
}


static struct file_operations write_mem_driver_fops = {
	.owner   =    THIS_MODULE,
	.open    =    write_mem_open,
	.release =    write_mem_release,
	.read    =    write_mem_read,
	.unlocked_ioctl = write_mem_ioctl,
};


static int __init write_mem_driver_module_init(void)
{
	write_mem_driver_major = register_chrdev(0, DEVICE_NAME, &write_mem_driver_fops);
	if(write_mem_driver_major < 0){
		printk("failed to register device.\n");
		return -1;
	}
	
	
	write_mem_driver_class = class_create(THIS_MODULE, "write_mem_driver");
    if (IS_ERR(write_mem_driver_class)){
        printk("failed to create write_mem moudle class.\n");
        unregister_chrdev(write_mem_driver_major, DEVICE_NAME);
        return -1;
    }
	
	
    write_mem_driver_device = device_create(write_mem_driver_class, NULL, MKDEV(write_mem_driver_major, 0), NULL, "write_mem_device");
    if (IS_ERR(write_mem_driver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(write_mem_driver_major, DEVICE_NAME);
        return -1;
    }	

	
	printk(" write_mem driver initial successfully!\n");
	
    return 0;	
}


static void __exit write_mem_driver_module_exit(void)
{
	printk("exit module.\n");
    device_destroy(write_mem_driver_class, MKDEV(write_mem_driver_major, 0));
    class_unregister(write_mem_driver_class);
	class_destroy(write_mem_driver_class);
	unregister_chrdev(write_mem_driver_major, DEVICE_NAME);

	
    printk("write_mem module exit.\n");	
}

module_init(write_mem_driver_module_init);
module_exit(write_mem_driver_module_exit);


