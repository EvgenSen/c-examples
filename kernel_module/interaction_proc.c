#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "interaction_proc.h"

static int inter_mod_proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Hello world from proc\n");
    printk(KERN_INFO "%s: Proc file was opened\n", MODULE_NAME);
    return 0;
}

static int inter_mod_proc_open(struct inode *inode, struct  file *file)
{
    return single_open(file, inter_mod_proc_show, NULL);
}

static const struct file_operations inter_mod_fops =
{
    .owner   = THIS_MODULE,
    .open    = inter_mod_proc_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};

static int __init inter_mod_init(void)
{
    proc_create(PROC_NAME, 0, NULL, &inter_mod_fops);
    return 0;
}

static void __exit inter_mod_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Sennikov <sennikov.work@ya.ru>");
MODULE_DESCRIPTION("Simple proc Hello world module");
MODULE_VERSION("1.0");

module_init(inter_mod_init);
module_exit(inter_mod_exit);