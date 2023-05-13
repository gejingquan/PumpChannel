#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>

#define DEVICE_NAME "ExChannel_module"

#define bram0_phy_addr              0xA0000000
#define bram1_phy_addr              0xA1000000
#define bram2_phy_addr              0xA2000000
#define bram3_phy_addr              0xA3000000
#define bram4_phy_addr              0xA4000000
#define bram5_phy_addr              0xA5000000
#define bram6_phy_addr              0xA6000000
#define bram7_phy_addr              0xA7000000
#define bram8_phy_addr              0xA8000000
#define exchannel_pump_phy_addr     0xA9000000
#define exchannel_switch_phy_addr   0xAA000000
#define exchannel_map_phy_addr      0xAB000000


MODULE_AUTHOR("Gerty");
MODULE_DESCRIPTION("ExChannel_driver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");


static int ExChannel_driver_major;
static struct class*  ExChannel_driver_class   = NULL;
static struct device* ExChannel_driver_device  = NULL;


unsigned long mem_vir_addr     =   0;
unsigned long mem_phy_addr     =   0;


extern unsigned long bram0_vir_addr   ;
extern unsigned long bram1_vir_addr   ;
extern unsigned long bram2_vir_addr   ;
extern unsigned long bram3_vir_addr   ;
extern unsigned long bram4_vir_addr   ;
extern unsigned long bram5_vir_addr   ;
extern unsigned long bram6_vir_addr   ;
extern unsigned long bram7_vir_addr   ;
extern unsigned long bram8_vir_addr   ;
extern unsigned long exchannel_pump_vir_addr;
extern unsigned long exchannel_switch_vir_addr;
extern unsigned long exchannel_map_vir_addr;
extern unsigned long user_kernel_flag;
extern unsigned long kernel_user_flag;


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
		
	printk("Before pump in.....");
		
	printk("bram8_vir_addr+0x1000:%016lx\n",*(long *)((long)bram8_vir_addr+0x1000));	
	printk("bram8_vir_addr+0x1FF0:%016lx\n",*(long *)((long)bram8_vir_addr+0x1FF0));
	printk("bram8_vir_addr+0x3000:%016lx\n",*(long *)((long)bram8_vir_addr+0x3000));
	printk("bram8_vir_addr+0x3FF0:%016lx\n",*(long *)((long)bram8_vir_addr+0x3FF0));
	printk("bram0_vir_addr:%016lx\n",*(long *)((long)bram0_vir_addr));
	printk("bram0_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram0_vir_addr+0xFF0));
	printk("bram1_vir_addr:%016lx\n",*(long *)((long)bram1_vir_addr));
	printk("bram1_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram1_vir_addr+0xFF0));	
	printk("bram6_vir_addr:%016lx\n",*(long *)((long)bram6_vir_addr));
	printk("bram6_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram6_vir_addr+0xFF0));	
	printk("bram7_vir_addr:%016lx\n",*(long *)((long)bram7_vir_addr));
	printk("bram7_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram7_vir_addr+0xFF0));
	
	iowrite32(0,(unsigned long *)exchannel_pump_vir_addr);
	iowrite32(0xA8001000,(unsigned long *)(exchannel_pump_vir_addr+4));
	iowrite32(0x1000,(unsigned long *)(exchannel_pump_vir_addr+8));
	iowrite32(0xA8003000,(unsigned long *)(exchannel_pump_vir_addr+12));
	iowrite32(0x1000,(unsigned long *)(exchannel_pump_vir_addr+16));
	iowrite32(0xF0F0F0F0,(unsigned long *)(exchannel_pump_vir_addr+20));
	iowrite32(0,(unsigned long *)(exchannel_pump_vir_addr+20));

	printk("After pump in.....");
	
	printk("bram8_vir_addr+0x1000:%016lx\n",*(long *)((long)bram8_vir_addr+0x1000));	
	printk("bram8_vir_addr+0x1FF0:%016lx\n",*(long *)((long)bram8_vir_addr+0x1FF0));
	printk("bram8_vir_addr+0x3000:%016lx\n",*(long *)((long)bram8_vir_addr+0x3000));
	printk("bram8_vir_addr+0x3FF0:%016lx\n",*(long *)((long)bram8_vir_addr+0x3FF0));
	printk("bram0_vir_addr:%016lx\n",*(long *)((long)bram0_vir_addr));
	printk("bram0_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram0_vir_addr+0xFF0));
	printk("bram1_vir_addr:%016lx\n",*(long *)((long)bram1_vir_addr));
	printk("bram1_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram1_vir_addr+0xFF0));	
	printk("bram6_vir_addr:%016lx\n",*(long *)((long)bram6_vir_addr));
	printk("bram6_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram6_vir_addr+0xFF0));	
	printk("bram7_vir_addr:%016lx\n",*(long *)((long)bram7_vir_addr));
	printk("bram7_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram7_vir_addr+0xFF0));

	iowrite32(0,(unsigned long *)exchannel_pump_vir_addr);
	iowrite32(0xA6000000,(unsigned long *)(exchannel_pump_vir_addr+4));
	iowrite32(0x1000,(unsigned long *)(exchannel_pump_vir_addr+8));
	iowrite32(0xA7000000,(unsigned long *)(exchannel_pump_vir_addr+12));
	iowrite32(0x1000,(unsigned long *)(exchannel_pump_vir_addr+16));	
	iowrite32(0x0F0F0F0F,(unsigned long *)(exchannel_pump_vir_addr+20));
	iowrite32(0,(unsigned long *)(exchannel_pump_vir_addr+20));	

	printk("After pump out.....");
	
	printk("bram8_vir_addr+0x1000:%016lx\n",*(long *)((long)bram8_vir_addr+0x1000));	
	printk("bram8_vir_addr+0x1FF0:%016lx\n",*(long *)((long)bram8_vir_addr+0x1FF0));
	printk("bram8_vir_addr+0x3000:%016lx\n",*(long *)((long)bram8_vir_addr+0x3000));
	printk("bram8_vir_addr+0x3FF0:%016lx\n",*(long *)((long)bram8_vir_addr+0x3FF0));
	printk("bram0_vir_addr:%016lx\n",*(long *)((long)bram0_vir_addr));
	printk("bram0_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram0_vir_addr+0xFF0));
	printk("bram1_vir_addr:%016lx\n",*(long *)((long)bram1_vir_addr));
	printk("bram1_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram1_vir_addr+0xFF0));	
	printk("bram6_vir_addr:%016lx\n",*(long *)((long)bram6_vir_addr));
	printk("bram6_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram6_vir_addr+0xFF0));	
	printk("bram7_vir_addr:%016lx\n",*(long *)((long)bram7_vir_addr));
	printk("bram7_vir_addr+0xFF0:%016lx\n",*(long *)((long)bram7_vir_addr+0xFF0));
	
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

	bram0_vir_addr = (unsigned long)ioremap(bram0_phy_addr, 0x40000);
	printk("bram0_vir_addr : %lx\r\n", bram0_vir_addr);
	bram1_vir_addr = (unsigned long)ioremap(bram1_phy_addr, 0x40000);
	printk("bram1_vir_addr : %lx\r\n", bram1_vir_addr);
	bram2_vir_addr = (unsigned long)ioremap(bram2_phy_addr, 0x40000);
	printk("bram2_vir_addr : %lx\r\n", bram2_vir_addr);
	bram3_vir_addr = (unsigned long)ioremap(bram3_phy_addr, 0x40000);
	printk("bram3_vir_addr : %lx\r\n", bram3_vir_addr);	
	bram4_vir_addr = (unsigned long)ioremap(bram4_phy_addr, 0x40000);
	printk("bram4_vir_addr : %lx\r\n", bram4_vir_addr);
	bram5_vir_addr = (unsigned long)ioremap(bram5_phy_addr, 0x40000);
	printk("bram5_vir_addr : %lx\r\n", bram5_vir_addr);
	bram6_vir_addr = (unsigned long)ioremap(bram6_phy_addr, 0x40000);
	printk("bram6_vir_addr : %lx\r\n", bram6_vir_addr);
	bram7_vir_addr = (unsigned long)ioremap(bram7_phy_addr, 0x40000);
	printk("bram7_vir_addr : %lx\r\n", bram7_vir_addr);	
	bram8_vir_addr = (unsigned long)ioremap(bram8_phy_addr, 0x100000);
	printk("bram8_vir_addr : %lx\r\n", bram8_vir_addr);
	exchannel_pump_vir_addr = (unsigned long)ioremap(exchannel_pump_phy_addr, 0x1000);
	printk("exchannel_pump_vir_addr : %lx\r\n", exchannel_pump_vir_addr);	
	exchannel_switch_vir_addr = (unsigned long)ioremap(exchannel_switch_phy_addr, 0x1000);
	printk("exchannel_switch_vir_addr : %lx\r\n", exchannel_switch_vir_addr);	
	exchannel_map_vir_addr = (unsigned long)ioremap(exchannel_map_phy_addr, 0x1000);
	printk("exchannel_map_vir_addr : %lx\r\n", exchannel_map_vir_addr);		
	iowrite32(0x01,(unsigned long *)exchannel_switch_vir_addr);
	printk("Open bram0 and bram1!");	
	iowrite32(0x02,(unsigned long *)exchannel_switch_vir_addr);
	printk("Open bram2 and bram3!");
	iowrite32(0x04,(unsigned long *)exchannel_switch_vir_addr);
	printk("Open bram4 and bram5!");
	iowrite32(0x08,(unsigned long *)exchannel_switch_vir_addr);
	printk("Open bram6 and bram7!");	
	iowrite32(0x0f,(unsigned long *)exchannel_switch_vir_addr);
	printk("Open bram8!");
	user_kernel_flag=0xbeaf0000;
	printk("user_kernel_flag==0xbeaf0000!");
	kernel_user_flag=0xbeaf0000;
	printk("kernel_user_flag==0xbeaf0000!");
	printk("ExChannel driver initial successfully!\n");
	
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







