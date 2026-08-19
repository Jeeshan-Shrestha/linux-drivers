#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>

#define VENDOR_ID 0x346d
#define PROD_ID 0x5678

/*
* its a usb table that tells the kernel which usb devices 
* can be handled by this drivers, it takes list of usb_drive_id 
*/
static struct usb_device_id pen_table[] = {
{USB_DEVICE(VENDOR_ID,PROD_ID)}, // vendor id : product id of s_diddy pendrive 
};

/* probe function 
* this function gets called when the device that u have defined in the pen_table 
* gets connected, if and only if other driver are not alrdy probing it,
*/
static int pen_probe(struct usb_interface *intf,
		      const struct usb_device_id *id)
{
	printk(KERN_INFO "Sitaula Diddy 16GB pendrive is plugged in\n");
	printk(KERN_INFO "VENDOR_ID: %04d PRODUCT_ID: %04d\n",id -> idVendor, id->idProduct);
	return 0;
}

static void disconnect_pen (struct usb_interface *intf)
{
	printk(KERN_INFO "Disconnected Sitaula Diddy Pendrive\n");
}

static struct usb_driver sd_pen_driver = {
	.name = "s_dd pen_driver",
	.id_table = pen_table,
	.probe = pen_probe,
	.disconnect = disconnect_pen,

};

static int __init init_mod(void)
{
	printk("sd_pendrive_driver initialized");
	int res = usb_register(&sd_pen_driver);
	printk("registered with status : %d",res);
	return 0;
}

static void __exit exit_mod(void)
{
	usb_deregister(&sd_pen_driver);
	printk("sd pen driver de-registered");
}

module_init(init_mod);
module_exit(exit_mod);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NARA");
MODULE_DESCRIPTION("a usb driver");

