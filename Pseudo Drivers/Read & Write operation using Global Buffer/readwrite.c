#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

// Global

struct device *pdev; 

struct class *pclass; 

unsigned char *pbuffer;

#define MAX_SIZE 64

int rd_offset=0;

int wr_offset=0;

int buflen=0;

dev_t pdevid;

int ndevices=1;

int ret = 0;

int wcount = 0;

int rcount = 0;

struct cdev cdev;

int pseudo_open(struct inode* inode , struct file* file)

{
	printk("Pseudo--open method\n");
	return 0;
}
int pseudo_close(struct inode* inode , struct file* file)

{

	printk("Pseudo--release method\n");
	return 0;
	
}

ssize_t pseudo_read(struct file * file, char __user *ubuf , size_t size, loff_t * off)

{

	printk("Pseudo--read method\n");
	if(buflen == 0)

	{

	 	printk("buffer is empty\n");
		return 0;

	}
	rcount = size ;
	if(rcount > buflen)

		rcount =buflen;

	ret = copy_to_user(ubuf, pbuffer + rd_offset,rcount);

	if (ret)

	{

		printk ("copy to user failed\n");
		return -EFAULT;

	}

	rd_offset += rcount;

	printk("rd_offset=%d",rd_offset);

	printk("\nrcount=%d",rcount);

	buflen -= rcount;

	printk("buflen=%d",buflen);

	printk("\nrcount=%d",rcount);

	return rcount;

}

ssize_t pseudo_write(struct file * file, const char __user *ubuf , size_t size, loff_t * off)

 
{
	printk("Pseudo--write method\n");
	if (wr_offset >= MAX_SIZE)
	{

		printk("buffer is full\n");
		return -ENOSPC;

	}

	wcount = size ;
	if (wcount > MAX_SIZE - wr_offset)

		wcount = MAX_SIZE - wr_offset;

	ret = copy_from_user(pbuffer + wr_offset, ubuf, wcount);
	if (ret)

	{
		printk("copy from user failed\n");

		return -EFAULT;

	}

	wr_offset += wcount;

	printk("wr_offset=%d",wr_offset);

	printk("\nwcount=%d",wcount);

	buflen += wcount;

	printk("buflen=%d",buflen);

	printk("\nwcount=%d",wcount);

	return wcount;

}

struct file_operations fops =

{
	.open = pseudo_open,

	.release = pseudo_close,

	.write = pseudo_write,

	.read = pseudo_read

 
};

static int __init psuedo_init(void)

{

	int ret, i=0;

	pclass = class_create(THIS_MODULE, "pseudo_class");

	ret=alloc_chrdev_region(&pdevid, 0, ndevices, "pseudo_sample");

	pbuffer = kmalloc(MAX_SIZE, GFP_KERNEL);

	cdev_init(&cdev, &fops); 

	kobject_set_name(&cdev.kobj,"pdevice%d",0);

	ret = cdev_add(&cdev, pdevid, 1);

	pdev = device_create(pclass, NULL, pdevid, NULL, "psample%d",i);

	if(ret) 

	{
		printk("Pseudo: Failed to register driver\n");

		cdev_init(&cdev, &fops);

		return -EINVAL;

	}

	printk("Successfully registered,major=%d,minor=%d\n",

	MAJOR(pdevid), MINOR(pdevid));

	printk("Pseudo Driver Sample..welcome\n");

	return 0;
}

static void __exit psuedo_exit(void)


{	

	device_destroy(pclass, pdevid);

	cdev_del(&cdev);

	unregister_chrdev_region(pdevid, ndevices);

	class_destroy(pclass);

	kfree(pbuffer);

	printk("Pseudo Driver Sample..Bye\n");
}

module_init(psuedo_init);

module_exit(psuedo_exit);
