#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/gpio.h>

#define LED1	4

/*
 * The module commandline arguments ...
 */
static int 			 toggleSpeed = 420;
static int 			 ios[2] 	= { -1, -1 };
static int 			 arr_argc 		= 0;

module_param(toggleSpeed, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(toggleSpeed, "An integer");
module_param_array(ios, int, &arr_argc, 0000);
MODULE_PARM_DESC(ios, "An array of integers");

static struct timer_list blink_timer;
static long data=0;

/*
 * Timer function called periodically
 */
static void blink_timer_func(struct timer_list* t)
{
	printk(KERN_INFO "%s\n", __func__);

	gpio_set_value(LED1, data);
	data=!data; 
	
	/* schedule next execution */
	//blink_timer.data = !data;						// makes the LED toggle 
	blink_timer.expires = jiffies + (toggleSpeed*HZ); 		// 1 sec.
	add_timer(&blink_timer);
}

/*
 * Module init function
 */
static int __init clargmod_init(void)
{
	int i;

	int ret = 0;
	printk(KERN_INFO "myint is an integer: %d\n", toggleSpeed);

	for (i = 0; i < (sizeof ios/ sizeof (int)); i++)
	{
		printk(KERN_INFO "myintArray[%d] = %d\n", i, ios[i]);
	}

	printk(KERN_INFO "got %d arguments for myintArray.\n", arr_argc);



	printk(KERN_INFO "%s\n", __func__);

	// register, turn off 
	ret = gpio_request_one(LED1, GPIOF_OUT_INIT_LOW, "led1");

	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs: %d\n", ret);
		return ret;
	}

	/* init timer, add timer function */
	//init_timer(&blink_timer);
	 timer_setup(&blink_timer, blink_timer_func, 0);

	blink_timer.function = blink_timer_func;
	//blink_timer.data = 1L;							// initially turn LED on
	blink_timer.expires = jiffies + (toggleSpeed*HZ); 		// 1 sec.
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
	gpio_set_value(LED1, 0); 
	
	// unregister GPIO 
	gpio_free(LED1);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexander");
MODULE_DESCRIPTION("Basic Linux Kernel module taking command line arguments");

module_init(clargmod_init);
module_exit(clargmod_exit);
