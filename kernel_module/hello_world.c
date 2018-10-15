#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

static int __init hello_init(void)
{
    printk(KERN_INFO " #### hello_world module started\n");
    return 0; // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO " #### hello_world module stopped.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Sennikov <sennikov.work@ya.ru>");
MODULE_DESCRIPTION("Simple Hello world module");
MODULE_VERSION("1.0");

module_init(hello_init);
module_exit(hello_cleanup);
