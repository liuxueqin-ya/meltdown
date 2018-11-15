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
	writesegfault(15, 2333, 78276);
	writesegfault(16, 2333, 78276);
	writesegfault(17, 3444, 89387);
	writesegfault(18, 3444, 89387);
	writesegfault(19, 3444, 89387);
	writesegfault(20, 3444, 89387);
	writesegfault(21, 3444, 89387);
	writesegfault(22, 3444, 89387);
	writesegfault(23, 3444, 89387);
	writesegfault(24, 3444, 89387);
	writesegfault(25, 2333, 78276);
	writesegfault(26, 2333, 78276);
	writesegfault(27, 3444, 89387);
	writesegfault(28, 3444, 89387);
	writesegfault(29, 3444, 89387);
	writesegfault(30, 3444, 89387);
	writesegfault(31, 3444, 89387);
	writesegfault(32, 3444, 89387);
	writesegfault(33, 3444, 89387);
	writesegfault(34, 3444, 89387);
	writesegfault(35, 2333, 78276);
	writesegfault(36, 2333, 78276);
	writesegfault(37, 3444, 89387);
	writesegfault(38, 3444, 89387);
	writesegfault(39, 3444, 89387);
	writesegfault(40, 3444, 89387);
	writesegfault(41, 3444, 89387);
	writesegfault(42, 3444, 89387);
	writesegfault(43, 3444, 89387);
	writesegfault(44, 3444, 89387);
	writesegfault(45, 2333, 78276);
	mdelay(5000);
	writesegfault(46, 2333, 78276);
	writesegfault(47, 3444, 89387);
	writesegfault(48, 3444, 89387);
	writesegfault(49, 3444, 89387);
	writesegfault(50, 3444, 89387);
	writesegfault(51, 3444, 89387);
	writesegfault(52, 3444, 89387);
	writesegfault(53, 3444, 89387);
	writesegfault(54, 3444, 89387);
	writesegfault(55, 2333, 78276);
	writesegfault(56, 2333, 78276);
	writesegfault(57, 3444, 89387);
	writesegfault(58, 3444, 89387);
	writesegfault(59, 3444, 89387);
	writesegfault(60, 3444, 89387);
	writesegfault(61, 3444, 89387);
	writesegfault(62, 3444, 89387);
	writesegfault(63, 3444, 89387);
	writesegfault(64, 3444, 89387);
	printk(KERN_ALERT "Goodbye, cruel world, koko\n");
}


module_init(hello_init);
module_exit(hello_exit);
