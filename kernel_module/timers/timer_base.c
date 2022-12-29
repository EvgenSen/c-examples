#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/version.h>

#define MODULE_NAME "timer_base"

struct timer_common {
	struct timer_list timer;
	char timer_data_str[128];
};

static struct timer_common test_timer;
static int counter = 5;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
static void test_timer_func(unsigned long data)
{
	char * local_data_str = (char *) data;

	printk(KERN_INFO "%s: %s() started, counter = %d, data = %s\n", MODULE_NAME, __FUNCTION__, counter, local_data_str ? : "No data");

	if(counter > 0)
	{
		test_timer.timer.function = &test_timer_func;
		test_timer.timer.expires  = msecs_to_jiffies(10 + counter * 10 ); // jiffies + 0;
		add_timer(&test_timer.timer);
		counter--;
	}
}
#else
static void test_timer_func(struct timer_list *t)
{

	struct timer_common * test_timer = from_timer(test_timer, t, timer);
	char * local_data_str = test_timer->timer_data_str;

	printk(KERN_INFO "%s: %s() started, counter = %d, data = %s\n", MODULE_NAME, __FUNCTION__, counter, local_data_str ? : "No data");

	if(counter > 0)
	{
		timer_setup(&test_timer->timer, test_timer_func, 0);
		mod_timer(&test_timer->timer, jiffies + msecs_to_jiffies(10 + counter * 10));
		counter--;
	}
}
#endif

static int __init timer_base_init(void)
{
	int i;

	printk(KERN_INFO "%s: %s() started\n", MODULE_NAME, __FUNCTION__);

	printk(KERN_INFO "%s: %s() HZ = %d; 1/HZ = %d ms - minimal timer step\n", MODULE_NAME, __FUNCTION__, HZ, 1000/HZ);

	for(i = 0; i <= 10; i++)
		printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, i, msecs_to_jiffies(i));

	printk(KERN_INFO "%s: %s() msecs_to_jiffies(%d) = %lu\n", MODULE_NAME, __FUNCTION__, 100, msecs_to_jiffies(100));

	strcpy(test_timer.timer_data_str, "test_data_for_timer_cb");

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	init_timer(&test_timer.timer);
	test_timer.timer.function = &test_timer_func;
	test_timer.timer.expires  = jiffies + msecs_to_jiffies(100);
	test_timer.timer.data     = (unsigned long) test_timer.timer_data_str;
	add_timer(&test_timer.timer);
#else
	timer_setup(&test_timer.timer, test_timer_func, 0);
	mod_timer(&test_timer.timer, jiffies + msecs_to_jiffies(100));
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
MODULE_VERSION("1.2");

module_init(timer_base_init);
module_exit(timer_base_cleanup);
