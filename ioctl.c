ssize_t pseudo_ioctl(struct file * file, unsigned int cmd, unsigned long arg) 
{
 pritnk("pseudo--ioctl method\n");
 return 0;
}
struct file_operations fops = {
 .open = pseudo_open,
 .release = pseudo_close,
 .write = pseudo_write,
 .read = pseudo_read,
 .unlocked_ioctl = pseudo_ioctl
}; 
