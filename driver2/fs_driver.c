#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#define D_MAJOR 90

static int driver_open(struct inode * node, struct file *fp)
{
	printk("opened the driver ");
	return 0;
}

static int driver_release(struct inode *device_file, struct file *instance)
{
	printk("released the driver");
	return 0;
}
static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_release,

};

static int __init init_hello(void) 
{
	printk("Hello world from kernel\n");
	int status = register_chrdev(D_MAJOR, "nara-fs-driver", &f_ops);	
	if (status == 0)
		printk("nara-fs-driver registered with major : %d and minor : %d \n",D_MAJOR,0);
	else if (status > 0){
		printk("nara-fs-driver registered with major : %d and minor : %d \n",status>>20,status&0xfffff);
	}else{
		printk("error registering the nara-fs-driver \n");
		return -1;
	}
	return 0;
}

static void __exit exit_hello(void) 
{
	unregister_chrdev(D_MAJOR, "nara-fs-driver");
	printk("Bye bye cruel world\n");
}

module_init(init_hello);
module_exit(exit_hello);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nara");
MODULE_DESCRIPTION("register a device number and implement some callback functions");
