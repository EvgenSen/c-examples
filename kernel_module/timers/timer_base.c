#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>

#define MODULE_NAME "timer_base"

struct timer_list test_timer;

static int counter = 5;

static void test_timer_func(unsigned long data)
{
	printk(KERN_INFO "%s: %s() started, counter = %d\n", MODULE_NAME, __FUNCTION__, counter);

	if(counter > 0)
	{
		test_timer.function    = &test_timer_func;
		test_timer.expires     = msecs_to_jiffies(10 + counter * 10 ); // jiffies + 0;
		add_timer(&test_timer);
		counter--;
	}
}

static int __init timer_base_init(void)
{
	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);

	printk(KERN_INFO "%s: %s() HZ = %d; 1/HZ = %d ms - minimal timer step\n", MODULE_NAME, __FUNCTION__, HZ, 1000/HZ);

	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 1, msecs_to_jiffies(1));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 2, msecs_to_jiffies(2));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 3, msecs_to_jiffies(3));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 4, msecs_to_jiffies(4));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 5, msecs_to_jiffies(5));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 6, msecs_to_jiffies(6));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 7, msecs_to_jiffies(7));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 8, msecs_to_jiffies(8));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 9, msecs_to_jiffies(9));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 10, msecs_to_jiffies(10));
	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 100, msecs_to_jiffies(100));

	init_timer(&test_timer);
	test_timer.function = &test_timer_func;
	test_timer.expires  = jiffies + msecs_to_jiffies(100);
	add_timer(&test_timer);

	return 0;
}

static void __exit timer_base_cleanup(void)
{
	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Sennikov <sennikov.work@ya.ru>");
MODULE_DESCRIPTION("Simple Timer module");
MODULE_VERSION("1.0");

module_init(timer_base_init);
module_exit(timer_base_cleanup);
