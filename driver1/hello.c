#include "asm-generic/errno-base.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"
#include "linux/types.h"
#include "linux/uaccess.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

static struct proc_dir_entry *proc_dir;

char buffer[200];	

static ssize_t nara_read(struct file *fp, 
			 char __user *user_space_buffer, 
			 size_t count,
                         loff_t *offset) 
{
	printk(KERN_INFO "nara-read is performed\n");
	size_t len = strlen(buffer);

	//initially the offset is zero and if it reaches higher value then 
	//the length of character in the buffer then we can safely say that 
	//we have read everything from the buffer and return 0 so userspace 
	//program and stop reading and avoid the inf loop 
	if (*offset >= len)
		return 0;

	ssize_t res = copy_to_user(user_space_buffer,buffer,len);	
	if (res){
		printk(KERN_ERR "Error in copy_to_user");
		return -EFAULT;
	}
	*offset += len;

	return len;
} 

static ssize_t	nara_write(struct file *fp, 
			  const char __user *user_space_buffer, 
			  size_t count, 
			  loff_t *offset)
{
	printk("Performing nara-write\n");
	if (count >= sizeof(buffer)){
		return -EINVAL;
	}

	ssize_t res = copy_from_user(buffer, user_space_buffer, count);	
	if (res){
		printk(KERN_ERR "Error in copy_to_user");
		return -EFAULT;
	}
	buffer[count] = '\0';

	return count;
}

static struct proc_ops nara_ops = {
	.proc_write = nara_write,
	.proc_read = nara_read,
};

static int init_hello(void) 
{
	printk("Hello world from kernel\n");
	proc_dir = proc_create("nara-proc", 0, NULL, &nara_ops);
	if (proc_dir == NULL) 
		printk(KERN_ERR "error creating the proc dir");
	return 0;
}

static void exit_hello(void) 
{
	printk("Bye bye cruel world\n");
	proc_remove(proc_dir);
	printk("removed the nara-proc");
}

module_init(init_hello);
module_exit(exit_hello);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nara");
MODULE_DESCRIPTION("Simple driver");
