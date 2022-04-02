#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/version.h>

#define MODULE_NAME "timer_base"

struct timer_list test_timer;

static int counter = 5;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
static void test_timer_func(unsigned long data)
#else
static void test_timer_func(struct timer_list *t)
#endif
{
	printk(KERN_INFO "%s: %s() started, counter = %d\n", MODULE_NAME, __FUNCTION__, counter);

	if(counter > 0)
	{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
		test_timer.function = &test_timer_func;
		test_timer.expires  = msecs_to_jiffies(10 + counter * 10 ); // jiffies + 0;
		add_timer(&test_timer);
#else
		timer_setup(&test_timer, test_timer_func, 0);
		mod_timer(&test_timer, jiffies + msecs_to_jiffies(10 + counter * 10));
#endif
		counter--;
	}
}

static int __init timer_base_init(void)
{
	int i;

	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);

	printk(KERN_INFO "%s: %s() HZ = %d; 1/HZ = %d ms - minimal timer step\n", MODULE_NAME, __FUNCTION__, HZ, 1000/HZ);

	for(i = 0; i <= 10; i++)
		printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, i, msecs_to_jiffies(i));

	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 100, msecs_to_jiffies(100));

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	init_timer(&test_timer);
	test_timer.function = &test_timer_func;
	test_timer.expires  = jiffies + msecs_to_jiffies(100);
	add_timer(&test_timer);
#else
	timer_setup(&test_timer, test_timer_func, 0);
	mod_timer(&test_timer, jiffies + msecs_to_jiffies(100));
#endif

	return 0;
}

static void __exit timer_base_cleanup(void)
{
	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Sennikov <sennikov.work@ya.ru>");
MODULE_DESCRIPTION("Simple Timer module");
MODULE_VERSION("1.1");

module_init(timer_base_init);
module_exit(timer_base_cleanup);
