#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h>

/*
 * The module commandline arguments ...
 */
static int toggleSpeed = 1; 
static int cnt = 0;
static int edgePin = 21;
static int ioPins[2] = {20, 21};
static int arr_argc = 0;

module_param(toggleSpeed, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(toggleSpeed, "An integer");

module_param_array(ioPins, int, &arr_argc, 0000);
MODULE_PARM_DESC(ioPins, "An array of integers");

module_param(edgePin, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(edgePin, "An integer");

static struct timer_list blink_timer;
static long data = 0;
static long data2 = 0;

/*
 * Timer function called periodically
 */
static void blink_timer_func(struct timer_list *t)
{
	printk(KERN_INFO "%s\n", __func__);

	gpio_set_value(ioPins[0], data);
	gpio_set_value(ioPins[1], data2);
	data = !data;
	data2 = !data2;

	if (edgePin == ioPins[0] || edgePin == ioPins[1])
	{
		cnt++;
		printk(KERN_INFO "--- Count: %d --- (pin %d) ---\n", cnt, edgePin);
	}
	else
	{
		printk(KERN_INFO "Wrong pin detection (tried pin %d instead of %d or %d)", edgePin, ioPins[0], ioPins[1]);
		return;
	}

	/* schedule next execution */
	blink_timer.expires = jiffies + (toggleSpeed * HZ); // 1 sec.
	add_timer(&blink_timer);
}

/*
 * Module init function
 */
static int __init clargmod_init(void)
{
	int i;

	int ret = 0;
	printk(KERN_INFO "Togglespeed: %d\n", toggleSpeed);

	printk(KERN_INFO "Edge detection pin: %d\n", edgePin);

	for (i = 0; i < (sizeof ioPins / sizeof(int)); i++)
	{
		printk(KERN_INFO "myintArray[%d] = %d\n", i, ioPins[i]);
	}

	printk(KERN_INFO "got %d arguments for myintArray.\n", arr_argc);

	printk(KERN_INFO "%s\n", __func__);

	// register, turn off
	ret = gpio_request_one(ioPins[0], GPIOF_OUT_INIT_LOW, "ioPins[0]");
	ret = gpio_request_one(ioPins[1], GPIOF_OUT_INIT_LOW, "ioPins[1]");

	if (ret)
	{
		printk(KERN_ERR "Unable to request GPioPins: %d\n", ret);
		return ret;
	}

	/* init timer, add timer function */
	//init_timer(&blink_timer);
	timer_setup(&blink_timer, blink_timer_func, 0);

	blink_timer.function = blink_timer_func;
	//blink_timer.data = 1L;							// initially turn LED on
	blink_timer.expires = jiffies + (toggleSpeed * HZ); // 1 sec.
	add_timer(&blink_timer);

	return ret;
}

/*
 * Exit function
 */
static void __exit clargmod_exit(void)
{
	printk(KERN_INFO "%s\n", __func__);

	// deactivate timer if running
	del_timer_sync(&blink_timer);

	// turn LED off
	gpio_set_value(ioPins[0], 0);
	gpio_set_value(ioPins[1], 0);

	// unregister GPIO
	gpio_free(ioPins[0]);
	gpio_free(ioPins[1]);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexander");
MODULE_DESCRIPTION("Basic Linux Kernel module taking command line arguments");

module_init(clargmod_init);
module_exit(clargmod_exit);