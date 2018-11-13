#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

extern void read_segfault(void);
extern void write_segfault(int pcid, unsigned long address, unsigned long long timestamp);

// typedef struct {
// 	int pcid;
// 	unsigned long address;
// 	unsigned long long timestamp;
// }SegFault;
// SegFault *vmallocmem;

static int hello_init(void)
{
	
	printk(KERN_ALERT "Hello, world, koko\n");
	// read_segfault();
	write_segfault(1, 2333, 78276);
	// vmallocmem = (SegFault *)vmalloc(sizeof(SegFault));
	return 0;
}
static void hello_exit(void)
{
	// read_segfault();
	write_segfault(2, 3444, 89387);
	// vfree(vmallocmem);
	printk(KERN_ALERT "Goodbye, cruel world, koko\n");
}


module_init(hello_init);
module_exit(hello_exit);
