#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>

#define DEVICE_NAME "ExChannel_module"

#define bram_gate_phy_addr 0xA8000000
#define bram0_phy_addr     0xA0000000
#define bram1_phy_addr     0xA2000000
#define bram2_phy_addr     0xA4000000
#define bram3_phy_addr     0xA6000000


MODULE_AUTHOR("Gerty");
MODULE_DESCRIPTION("ExChannel_driver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");


static int ExChannel_driver_major;
static struct class*  ExChannel_driver_class   = NULL;
static struct device* ExChannel_driver_device  = NULL;


unsigned long mem_vir_addr     =   0;
unsigned long mem_phy_addr     =   0;


extern unsigned long bram_gate_vir_addr;
extern unsigned long bram0_vir_addr   ;
extern unsigned long bram1_vir_addr   ;
extern unsigned long bram2_vir_addr   ;
extern unsigned long bram3_vir_addr   ;


static int ExChannel_open(struct inode * inode, struct file * filp)
{
  return 0;
}


static int ExChannel_release(struct inode * inode, struct file *filp)
{
  return 0;
}


ssize_t ExChannel_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long mem_data;	
	            
	mem_data = ioread64((unsigned long *)(mem_vir_addr));

	copy_to_user(buf, &mem_data, 8);	
    return 0;
}

ssize_t ExChannel_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	mem_phy_addr = arg;

	mem_vir_addr = (unsigned long)phys_to_virt(mem_phy_addr);
	
	printk("mem_phy_addr : %lx\r\n", mem_phy_addr);
	printk("mem_vir_addr : %lx\r\n", mem_vir_addr);
	
	printk("bram3_vir_addr+0x1000:%016lx\n",*(long *)((long)bram3_vir_addr+0x1000));
	printk("bram3_vir_addr+0x1008:%016lx\n",*(long *)((long)bram3_vir_addr+0x1008));	
	printk("bram3_vir_addr+0x1010:%016lx\n",*(long *)((long)bram3_vir_addr+0x1010));
	printk("bram3_vir_addr+0x1018:%016lx\n",*(long *)((long)bram3_vir_addr+0x1018));
	printk("bram3_vir_addr+0x1020:%016lx\n",*(long *)((long)bram3_vir_addr+0x1020));
	printk("bram3_vir_addr+0x1028:%016lx\n",*(long *)((long)bram3_vir_addr+0x1028));
	
	
	return 0;	
}


static struct file_operations ExChannel_driver_fops = {
	.owner   =    THIS_MODULE,
	.open    =    ExChannel_open,
	.release =    ExChannel_release,
	.read    =    ExChannel_read,
	.unlocked_ioctl = ExChannel_ioctl,
};


static int __init ExChannel_driver_module_init(void)
{
	ExChannel_driver_major = register_chrdev(0, DEVICE_NAME, &ExChannel_driver_fops);
	if(ExChannel_driver_major < 0){
		printk("failed to register device.\n");
		return -1;
	}
	
	
	ExChannel_driver_class = class_create(THIS_MODULE, "ExChannel_driver");
    if (IS_ERR(ExChannel_driver_class)){
        printk("failed to create ExChannel moudle class.\n");
        unregister_chrdev(ExChannel_driver_major, DEVICE_NAME);
        return -1;
    }
	
	
    ExChannel_driver_device = device_create(ExChannel_driver_class, NULL, MKDEV(ExChannel_driver_major, 0), NULL, "ExChannel_device");
    if (IS_ERR(ExChannel_driver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(ExChannel_driver_major, DEVICE_NAME);
        return -1;
    }	


	bram_gate_vir_addr = (unsigned long)ioremap(bram_gate_phy_addr, 32);
	printk("bram_gate_vir_addr : %lx\r\n", bram_gate_vir_addr);
	bram0_vir_addr = (unsigned long)ioremap(bram0_phy_addr, 0x20000);
	printk("bram0_vir_addr : %lx\r\n", bram0_vir_addr);
	bram1_vir_addr = (unsigned long)ioremap(bram1_phy_addr, 0x20000);
	printk("bram1_vir_addr : %lx\r\n", bram1_vir_addr);
	bram2_vir_addr = (unsigned long)ioremap(bram2_phy_addr, 0x20000);
	printk("bram2_vir_addr : %lx\r\n", bram2_vir_addr);
	bram3_vir_addr = (unsigned long)ioremap(bram3_phy_addr, 0x20000);
	printk("bram3_vir_addr : %lx\r\n", bram3_vir_addr);	
	iowrite32(0x01,(unsigned long *)bram_gate_vir_addr);
	iowrite32(0x02,(unsigned long *)bram_gate_vir_addr);
	iowrite32(0x04,(unsigned long *)bram_gate_vir_addr);
	iowrite32(0x08,(unsigned long *)bram_gate_vir_addr);	
	printk("Open all bram!");

	
	printk(" ExChannel driver initial successfully!\n");
	
    return 0;	
}


static void __exit ExChannel_driver_module_exit(void)
{
	printk("exit module.\n");
    device_destroy(ExChannel_driver_class, MKDEV(ExChannel_driver_major, 0));
    class_unregister(ExChannel_driver_class);
	class_destroy(ExChannel_driver_class);
	unregister_chrdev(ExChannel_driver_major, DEVICE_NAME);

	
    printk("ExChannel module exit.\n");	
}

module_init(ExChannel_driver_module_init);
module_exit(ExChannel_driver_module_exit);







