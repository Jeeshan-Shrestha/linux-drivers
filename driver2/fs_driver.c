#include "linux/minmax.h"
#include "linux/types.h"
#include "linux/uaccess.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#define D_MAJOR 90

/*
 * kernel buffer 
 */
static char buffer[255];
static size_t buffer_pointer;

static int driver_open(struct inode * node, struct file *fp)
{
	printk("opened the driver ");
	return 0;
}

static int driver_release(struct inode * node, struct file *fp)
{
	printk("released the driver ");
	return 0;
}

/*
* count is the number of bytes that the user want to read,
* offset is the pointer pointing at the location current location of the file
*/
static ssize_t driver_read(struct file *fp,
			   char __user *user_space_buffer, 
			   size_t count,
			   loff_t *offset)
{

	size_t remaining = buffer_pointer - *offset;

	/*
	 * we should only copy the amount of byte that the user asks in the count,
	 * also we should be wary of the fact that the bytes may run out at the end 
	 * lets say we have 25 bytes on our kernel buffer, and user count is 10
	 * so we go 10 , 10 and now we cant read 10 byte again we need to read only 5 byte 
	 * that would be given by len i.e 25 - offset where the pointer is pointing at 
	 * so after 2nd read the offset is 20 and len is 25 so we get 5, now it could be 
	 * either of those value and we can make use of the min function here 
	 */
	size_t bytes_to_copy = min(count ,remaining);

	/* returns the number of byte that failed to copy 
	 * example : if copy_to_user(..,..,10) and returned value is 
	 * 0 : all 10 bytes were successfully copied
	 * 4 : only 6 bytes were successfully copied and 4 bytes failed to be copied
	 * 10 : no bytes were copied
	 */

	/* buffer + *offset because we want to read from where we left off not from the 0 
	 * again, other wise it would just loop 
	 */
	size_t nbyte = copy_to_user(user_space_buffer,buffer + *offset,bytes_to_copy);
	size_t bytes_copied = bytes_to_copy - nbyte;
	/*
	 * its offset should increment by the number of bytes that were successfully copied,
	 * and not by the number of bytes that failed to copy
	 * */
	*offset += bytes_copied;
	printk("read performed");
	
	return bytes_copied;
}

static ssize_t driver_write(struct file *fp, 
			const char __user *user_space_buffer,
			size_t count, 
			loff_t * offset)
{
	/* the write size cant be larger than 
	 * the kernel buffer so we limit it its max value
	 */
	size_t bytes_to_copy = min(count,sizeof(buffer));	
	size_t nbyte = copy_from_user(buffer, user_space_buffer, bytes_to_copy);

	size_t bytes_copied = bytes_to_copy - nbyte;
	buffer_pointer = bytes_copied;
	return bytes_copied;
	
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_release,
	.read = driver_read,
	.write = driver_write

};

static int __init init_hello(void) 
{
	printk("Hello world from kernel\n");
	int status = register_chrdev(D_MAJOR, "nara-fs-driver", &f_ops);	
	if (status == 0)
		printk("nara-fs-driver registered with major : %d and minor : %d \n",D_MAJOR,0);
	else{
		printk(KERN_ERR "error registering the nara-fs-driver \n");
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
