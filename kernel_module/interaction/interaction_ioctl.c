#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

#include "interaction_ioctl.h"

struct fasync_struct *fasync;

ioctl_data_t global_data;

static long inter_mod_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	long ret;

	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);

	/* Check type and command number */
	if (_IOC_TYPE(cmd) != INTER_MOD_MAJOR_NUM)
		return -ENOTTY;

	switch(cmd)
	{
		case GET_INTER_MOD_DATA:
		{
			ioctl_data_t local_data;

			ret = copy_to_user(&local_data, argp, sizeof(ioctl_data_t));
			local_data = global_data;
			ret = copy_to_user(argp, &local_data, sizeof(ioctl_data_t));

			printk(KERN_INFO "%s: Get cmd flag=%d, buffer=\"%s\", ret = %ld\n", MODULE_NAME, global_data.flag, global_data.buffer, ret);

			break;
		}

		case SET_INTER_MOD_DATA:
		{
			ioctl_data_t local_data;

			ret = copy_from_user(&local_data, argp, sizeof(ioctl_data_t));
			global_data = local_data;

			printk(KERN_INFO "%s: Set cmd flag=%d, buffer=\"%s\", ret = %ld\n", MODULE_NAME, global_data.flag, global_data.buffer, ret);

			break;
		}

		default:
			return 1;
	}

	return 0;
}

static int inter_mod_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);
	try_module_get(THIS_MODULE);
	return 0;
}

static int inter_mod_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);
	module_put(THIS_MODULE);
	return 0;
}

static int inter_mod_fasync(int fd, struct file *file, int mode)
{
	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);
	return fasync_helper(fd, file, mode, &fasync);
}

static const struct file_operations inter_mod_fops =
{
	.owner           = THIS_MODULE,
	.unlocked_ioctl  = inter_mod_ioctl,
	.open            = inter_mod_open,
	.release         = inter_mod_release,
	.fasync          = inter_mod_fasync,
};

static int __init inter_mod_init(void)
{
	int ret = register_chrdev(INTER_MOD_MAJOR_NUM, INTER_MOD_DEV_NAME, &inter_mod_fops);

	if (ret < 0)
	{
		printk(KERN_INFO "%s: Regiseter character device fail (ret = %d)\n", MODULE_NAME, ret);
	}
	else
	{
		printk(KERN_INFO "%s: Regiseter character device ok\n", MODULE_NAME);
	}

	return ret;
}

static void __exit inter_mod_exit(void)
{
	unregister_chrdev(INTER_MOD_MAJOR_NUM, INTER_MOD_DEV_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Sennikov <sennikov.work@ya.ru>");
MODULE_DESCRIPTION("Basic ioctl module");
MODULE_VERSION("1.0");

module_init(inter_mod_init);
module_exit(inter_mod_exit);
