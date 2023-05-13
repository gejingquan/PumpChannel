#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>

#define DEVICE_NAME "read_mem_module"



MODULE_AUTHOR("Gerty");
MODULE_DESCRIPTION("read_mem_driver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");


static int read_mem_driver_major;
static struct class*  read_mem_driver_class   = NULL;
static struct device* read_mem_driver_device  = NULL;


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

static int read_mem_open(struct inode * inode, struct file * filp)
{
  return 0;
}


static int read_mem_release(struct inode * inode, struct file *filp)
{
  return 0;
}


ssize_t mem_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long mem_data;	
	            
	mem_data = ioread64((unsigned long *)(mem_vir_addr));

	copy_to_user(buf, &mem_data, 8);

	printk("current->cpu=%lx",current->cpu);
	
    return 0;
}

ssize_t read_mem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

	mem_phy_addr = arg;

	mem_vir_addr = (unsigned long)phys_to_virt(mem_phy_addr);
			
	return 0;	
}


static struct file_operations read_mem_driver_fops = {
	.owner   =    THIS_MODULE,
	.open    =    read_mem_open,
	.release =    read_mem_release,
	.read    =    mem_read,
	.unlocked_ioctl = read_mem_ioctl,
};


static int __init read_mem_driver_module_init(void)
{
	read_mem_driver_major = register_chrdev(0, DEVICE_NAME, &read_mem_driver_fops);
	if(read_mem_driver_major < 0){
		printk("failed to register device.\n");
		return -1;
	}
	
	
	read_mem_driver_class = class_create(THIS_MODULE, "read_mem_driver");
    if (IS_ERR(read_mem_driver_class)){
        printk("failed to create read_mem moudle class.\n");
        unregister_chrdev(read_mem_driver_major, DEVICE_NAME);
        return -1;
    }
	
	
    read_mem_driver_device = device_create(read_mem_driver_class, NULL, MKDEV(read_mem_driver_major, 0), NULL, "read_mem_device");
    if (IS_ERR(read_mem_driver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(read_mem_driver_major, DEVICE_NAME);
        return -1;
    }	

	
	printk(" read_mem driver initial successfully!\n");
	
	unsigned char *_stext;
	unsigned char *_etext;

	_stext = (void *)kallsyms_lookup_name("_stext");
	_etext = (void *)kallsyms_lookup_name("_etext");
	printk("Virtual address of _stext: 0x%lx\n",(long)_stext);
	printk("Virtual address of _etext: 0x%lx\n",(long)_etext);	
	unsigned long stext_phy = virt_to_phys(_stext);
	unsigned long etext_phy = virt_to_phys(_etext);	
	printk("Physical address of _stext: 0x%lx\n",(long)stext_phy);
	printk("Physical address of _etext: 0x%lx\n",(long)etext_phy);
	printk("_stext:%016lx\n",*(long *)_stext);
	printk("_stext+8:%016lx\n",*(long *)((long)_stext+8));	
	printk("_stext+16:%016lx\n",*(long *)((long)_stext+16));
	printk("_stext+24:%016lx\n",*(long *)((long)_stext+24));
	printk("_stext+32:%016lx\n",*(long *)((long)_stext+32));
	printk("_stext+40:%016lx\n",*(long *)((long)_stext+40));
		
	printk("bram0_vir_addr : %lx\r\n", bram0_vir_addr);	
	printk("bram1_vir_addr : %lx\r\n", bram1_vir_addr);	
	printk("bram2_vir_addr : %lx\r\n", bram2_vir_addr);	
	printk("bram3_vir_addr : %lx\r\n", bram3_vir_addr);		
	printk("bram4_vir_addr : %lx\r\n", bram4_vir_addr);	
	printk("bram5_vir_addr : %lx\r\n", bram5_vir_addr);	
	printk("bram6_vir_addr : %lx\r\n", bram6_vir_addr);	
	printk("bram7_vir_addr : %lx\r\n", bram7_vir_addr);		
	printk("bram8_vir_addr : %lx\r\n", bram8_vir_addr);	
	printk("exchannel_pump_vir_addr : %lx\r\n", exchannel_pump_vir_addr);	
	printk("exchannel_switch_vir_addr : %lx\r\n", exchannel_switch_vir_addr);		
	printk("exchannel_map_vir_addr : %lx\r\n", exchannel_map_vir_addr);			
    return 0;	
}


static void __exit read_mem_driver_module_exit(void)
{
	printk("exit module.\n");
    device_destroy(read_mem_driver_class, MKDEV(read_mem_driver_major, 0));
    class_unregister(read_mem_driver_class);
	class_destroy(read_mem_driver_class);
	unregister_chrdev(read_mem_driver_major, DEVICE_NAME);

	
    printk("read_mem module exit.\n");	
}

module_init(read_mem_driver_module_init);
module_exit(read_mem_driver_module_exit);







