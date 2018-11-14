#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

// extern void read_segfault(void);
extern void writesegfault(int pcid, unsigned long address, unsigned long long timestamp);



static int hello_init(void)
{
	
	printk(KERN_ALERT "Hello, world, koko\n");
	writesegfault(1, 2333, 78276);
	writesegfault(2, 2333, 78276);
	mdelay(5000);
	writesegfault(3, 2333, 78276);
	writesegfault(4, 2333, 78276);
	return 0;
}
static void hello_exit(void)
{
	mdelay(3000);
	writesegfault(5, 2333, 78276);
	writesegfault(6, 2333, 78276);
	writesegfault(7, 3444, 89387);
	writesegfault(8, 3444, 89387);
	writesegfault(9, 3444, 89387);
	writesegfault(10, 3444, 89387);
	writesegfault(11, 3444, 89387);
	writesegfault(12, 3444, 89387);
	writesegfault(13, 3444, 89387);
	writesegfault(14, 3444, 89387);
	mdelay(5000);
	writesegfault(15, 3444, 89387);
	// vfree(vmallocmem);
	printk(KERN_ALERT "Goodbye, cruel world, koko\n");
}


module_init(hello_init);
module_exit(hello_exit);
