#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#define MAX_SIZE 1024

#define IOC_MAGIC 'p'
#define MY_IOCTL_LEN _IO(IOC_MAGIC, 1)
#define MY_IOCTL_AVAIL _IO(IOC_MAGIC, 2)
#define MY_IOCTL_RESET _IO(IOC_MAGIC, 3)
unsigned char *pbuffer;
struct kfifo myfifo;

static long pseudo_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
int ret;
printk("Pseudo--ioctl method\n");
switch (cmd) {
case MY_IOCTL_LEN :
printk("ioctl--kfifo length is %d\n",
kfifo_len(&myfifo));
break;
case MY_IOCTL_AVAIL:
printk("ioctl--kfifo avail is %d\n",
kfifo_avail(&myfifo));
break;
case MY_IOCTL_RESET:
printk("ioctl--kfifo got reset\n");
kfifo_reset(&myfifo);
break;
}
return 0;
}

int pseudo_open(struct inode *inode, struct file *file) {
printk("Pseudo--open method\n");
return 0;
}
int pseudo_close(struct inode *inode, struct file *file) {
printk("Pseudo--release method\n");
return 0;
}
ssize_t pseudo_read(struct file *file, char __user *ubuf, size_t size,
loff_t *off) {

int rcount, ret;
char *tbuf;
printk("Pseudo--read method\n");

// Read method:-
if (kfifo_is_empty(&myfifo)) {
printk("buffer is empty\n");
return 0;
}
rcount = size;
if (rcount > kfifo_len(&myfifo))
rcount = kfifo_len(&myfifo);

tbuf = kmalloc(rcount, GFP_KERNEL);

kfifo_out(&myfifo, tbuf, rcount);
ret = copy_to_user(ubuf, tbuf, rcount);
// error handling
kfree(tbuf);
}

ssize_t pseudo_write(struct file *file, const char __user *buf, size_t size,
loff_t *off) {

int wcount;
printk("Pseudo--write method\n");
// Write method:-
if (kfifo_is_full(&myfifo)) {
printk("buffer is full\n");
return -ENOSPC;
}
wcount = size;
if (wcount > kfifo_avail(&myfifo))
wcount = kfifo_avail(&myfifo);
}
struct device *pdev; // global
struct class *pclass; // global
struct cdev cdev;
int ndevices = 1;
struct file_operations fops = {.open = pseudo_open,
.release = pseudo_close,
.write = pseudo_write,
.read = pseudo_read};

dev_t pdevid;
static int __init psuedo_init(void) {
int ret;
int i = 0;
ret = alloc_chrdev_region(&pdevid, 0, ndevices, "pseudo_sample");
if (ret) {
printk("Pseudo: Failed to register driver\n");
return -EINVAL;
}
cdev_init(&cdev, &fops);
kobject_set_name(&cdev.kobj, "pdevice%d", i);
ret = cdev_add(&cdev, pdevid, 1);
printk("Successfully registered,major=%d,minor=%d\n", MAJOR(pdevid),
MINOR(pdevid));
printk("Pseudo Driver Sample..welcome\n");
pclass = class_create(THIS_MODULE, "pseudo_class");
// alloc_chrdev_region, cdev_init, cdev_add
pdev = device_create(pclass, NULL, pdevid, NULL, "psample%d", i);
// pseudo_init
pbuffer = kmalloc(MAX_SIZE, GFP_KERNEL);
kfifo_init(&myfifo, pbuffer, MAX_SIZE);
// kfifo_alloc(&myfifo, MAX_SIZE, GFP_KERNEL); 

return 0;
}
static void __exit psuedo_exit(void) {
cdev_del(&cdev);
unregister_chrdev_region(pdevid, ndevices);
printk("Pseudo Driver Sample..Bye\n");
device_destroy(pclass, pdevid);
class_destroy(pclass);

kfifo_free(&myfifo);
}
module_init(psuedo_init);
module_exit(psuedo_exit);
