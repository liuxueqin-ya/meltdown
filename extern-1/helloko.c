#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
 
MODULE_LICENSE("GPL");

#define MAXNUM 4096
#define MAJOR_NUM 456 //主设备号 ,没有被使用

typedef struct {
	int pcid;
	unsigned long address;
	unsigned long long timestamp;
}SegFault;
SegFault *vmallocmem ;

// void print(SegFault *segfault){
//     printk("pcid:%d\n",vmallocmem->pcid);
// 	printk("address:%lu\n",vmallocmem->address);
// 	printk("timestamp:%llu\n", vmallocmem->timestamp);
// }



struct dev{
    struct cdev devm; //字符设备
    struct semaphore sem;
    wait_queue_head_t outq;//等待队列,实现阻塞操作
    int flag; //阻塞唤醒标志
    // char buffer[MAXNUM+1]; //字符缓冲区
    SegFault buffer[MAXNUM+1];//结构体缓冲区
    SegFault *rd,*wr,*end; //读,写,尾指针
}globalvar;
static struct class *my_class;
static char * devicename = "extern";
static char * classname = "extern_class";
int major=MAJOR_NUM;

// void read_segfault(void) {
// 	printk("ko read_segfault\n");
//     print(vmallocmem);
// }

void write_segfault(int pcid, unsigned long address, unsigned long long timestamp) {
	printk("write_segfault\n");
    if(down_interruptible(&globalvar.sem)) //P 操作
    {
        return -ERESTARTSYS;
    }
    if(globalvar.rd <= globalvar.wr)
        len = min(len,(size_t)(globalvar.end - globalvar.wr));
    else
        len = min(len,(size_t)(globalvar.rd-globalvar.wr-1));   



	globalvar.wr->address = address;
	globalvar.wr->pcid = pcid;
	globalvar.wr->timestamp = timestamp;




    globalvar.wr = globalvar.wr + sizeof(SegFault);
    if(globalvar.wr == globalvar.end)
        globalvar.wr = globalvar.buffer; //循环
    up(&globalvar.sem);
    // globalvar.flag=1;
    // wake_up_interruptible(&globalvar.outq); //唤醒读进程
}


static ssize_t globalvar_read(struct file *,char *,size_t ,loff_t *);
// static ssize_t globalvar_write(struct file *,const char *,size_t ,loff_t *);
static int globalvar_open(struct inode *inode,struct file *filp);
static int globalvar_release(struct inode *inode,struct file *filp);

struct file_operations globalvar_fops =
{
    //用来从设备中获取数据. 在这个位置的一个空指针导致 read 系统调用以 -EINVAL("Invalid argument") 失败. 一个非负返回值代表了成功读取的字节数( 返回值是一个 "signed size" 类型, 常常是目标平台本地的整数类型).
    .read=globalvar_read,
    //发送数据给设备. 如果 NULL, -EINVAL 返回给调用 write 系统调用的程序. 如果非负, 返回值代表成功写的字节数.
    // .write=globalvar_write,
    //尽管这常常是对设备文件进行的第一个操作, 不要求驱动声明一个对应的方法. 如果这个项是 NULL, 设备打开一直成功, 但是你的驱动不会得到通知.
    .open=globalvar_open,
    //当最后一个打开设备的用户进程执行close ()系统调用时，内核将调用驱动程序的release () 函数：release 函数的主要任务是清理未结束的输入/输出操作、释放资源、用户自定义排他标志的复位等。
    .release=globalvar_release,
};
/*
在大部分驱动程序中,open 应完成如下工作:
● 递增使用计数。--为了老版本的可移植性
● 检查设备特定的错误(诸如设备未就绪或类似的硬件问题)。
● 如果设备是首次打开,则对其进行初始化。
● 识别次设备号,并且如果有必要,更新 f_op 指针。
● 分配并填写被置于 filp->private_data 里的数据结构。
*/
static int globalvar_open(struct inode *inode,struct file *filp)
{
    try_module_get(THIS_MODULE);//模块计数加一
    // filp->private_data = vmallocmem;
    printk("This externdev is in open\n");
    return(0);
}
/*
release都应该完成下面的任务:
● 释放由 open 分配的、保存在 filp->private_data 中的所有内容。
● 在最后一次关闭操作时关闭设备。字符设备驱动程序
● 使用计数减 1。
如果使用计数不归0,内核就无法卸载模块。
并不是每个 close 系统调用都会引起对 release 方法的调用。
仅仅是那些真正释放设备数据结构的 close 调用才会调用这个方法,
因此名字是 release 而不是 close。内核维护一个 file 结构被使用多少次的计数器。
无论是 fork 还是 dup 都不创建新的数据结构(仅由 open 创建),它们只是增加已有结构中的计数。
*/
static int globalvar_release(struct inode *inode,struct file *filp)
{
    module_put(THIS_MODULE); //模块计数减一
    printk("This externdev is in release\n");
    return(0);
}
static ssize_t globalvar_read(struct file *filp,char *buf,size_t len,loff_t *off)
{
    // if(wait_event_interruptible(globalvar.outq,globalvar.flag!=0)) //不可读时 阻塞读进程
    // {
    //     return -ERESTARTSYS;
    // }
    /*
    使用down_interruptible来获取信号量的代码不应调用其他也试图获得该信号量的函数,否则就会陷入死锁。
    如果驱动程序中的某段程序对其持有的信号量释放失败的话(可能就是一次出错返回的结果),
    那么其他任何获取该信号量的尝试都将阻塞在那里。
    */
    if(down_interruptible(&globalvar.sem)) //P 操作
    {
        // printk("read444\n");
        return -ERESTARTSYS;
    }
    globalvar.flag = 0;
    // printk("the rd is %c\n",*globalvar.rd); //读指针
    
    if(globalvar.rd < globalvar.wr)
    {
        printk("the read len1 is %lu\n",(size_t)(globalvar.wr - globalvar.rd)); 
        len = min(len,(size_t)(globalvar.wr - globalvar.rd)); //更新读写长度
    }
    else{
        printk("the read len2 is %lu\n",(size_t)(globalvar.end - globalvar.rd)); 
        len = min(len,(size_t)(globalvar.end - globalvar.rd));
    }
    /*
    unsigned long copy_to_user(void *to, const void *from,unsigned long count);
    unsigned long copy_from_user(void *to, const void *from,unsigned long count);
    */
    if(copy_to_user(buf,vmallocmem,len))
    {
        printk(KERN_ALERT"copy failed\n");
        /*
        up递增信号量的值,并唤醒所有正在等待信号量转为可用状态的进程。
        必须小心使用信号量。被信号量保护的数据必须是定义清晰的,并且存取这些数据的所有代码都必须首先获得信号量。
        */
        up(&globalvar.sem);
        return -EFAULT;
    }
    // printk("the read buffer is %s\n",vmallocmem);
    globalvar.rd = globalvar.rd + len;
    if(globalvar.rd == globalvar.end)
        globalvar.rd = globalvar.buffer; //字符缓冲区循环
    up(&globalvar.sem); //V 操作
    return len;
}
// static ssize_t globalvar_write(struct file *filp,const char *buf,size_t len,loff_t *off)
// {
//     printk("write111\n");
//     if(down_interruptible(&globalvar.sem)) //P 操作
//     {
//         printk("write222\n");
//         return -ERESTARTSYS;
//     }
//     printk("write333\n");
//     if(globalvar.rd < globalvar.wr)
//     {
//         printk("the write len1 is %lu\n",(size_t)(globalvar.end - globalvar.wr)); 
//         len = min(len,(size_t)(globalvar.end - globalvar.wr)); //更新读写长度
//     }
//     else{
//         printk("the write len2 is %lu\n",(size_t)(globalvar.rd-globalvar.wr-1)); 
//         len = min(len,(size_t)(globalvar.rd-globalvar.wr-1));
//     }

//     // if(globalvar.rd < globalvar.wr)
//     //     len = min(len,(size_t)(globalvar.end - globalvar.wr));
//     // else
//     //     len = min(len,(size_t)(globalvar.rd-globalvar.wr-1));
//     printk("the write len is %d\n",len);
//     if(copy_from_user(vmallocmem,buf,len))
//     {
//         up(&globalvar.sem); //V 操作
//         return -EFAULT;
//     }
//     // printk("the write buffer is %s\n",vmallocmem);
//     printk("the len of buffer is %d\n",len);
//     globalvar.wr = globalvar.wr + len;
//     if(globalvar.wr == globalvar.end)
//         globalvar.wr = vmallocmem; //循环
//     up(&globalvar.sem);
//     //V 操作
//     globalvar.flag=1; //条件成立,可以唤醒读进程
//     // wake_up_interruptible(&globalvar.outq); //唤醒读进程
//     return len;
// }



static int hello_init(void)
{
	printk(KERN_ALERT "++Hello, world\n");
	int result = 0;
    int err = 0;
	
    vmallocmem = (SegFault *)vmalloc(1024*sizeof(SegFault));
    // vmallocmem->pcid = 9;
    // vmallocmem->address = 999;
    // vmallocmem->timestamp = 99999;
    // strcpy()

	dev_t dev = MKDEV(major, 0);
	if(major)
    {
        //静态申请设备编号
        result = register_chrdev_region(dev, 1, "charmem");
    }
    else
    {
        //动态分配设备号
        result = alloc_chrdev_region(&dev, 0, 1, "charmem");
        major = MAJOR(dev);
    }
    if(result < 0)
        return result;
	//注册字符设备驱动，设备号和file_operations结构体进行绑定
    cdev_init(&globalvar.devm, &globalvar_fops);
	globalvar.devm.owner = THIS_MODULE;
    err = cdev_add(&globalvar.devm, dev, 1);
    if(err)
        printk(KERN_INFO "Error %d adding char_mem device", err);
    else
    {
        printk("globalvar register success\n");
        sema_init(&globalvar.sem,1); //初始化信号量
        init_waitqueue_head(&globalvar.outq); //初始化等待队列
        globalvar.rd = globalvar.buffer; //读指针
        globalvar.wr = globalvar.buffer; //写指针
        globalvar.end = globalvar.buffer + MAXNUM*sizeof(SegFault);//缓冲区尾指针
        // globalvar.rd = vmallocmem; //读指针
        // globalvar.wr = vmallocmem + sizeof(SegFault); //写指针
        // globalvar.end = vmallocmem + sizeof(SegFault);//缓冲区尾指针
        globalvar.flag = 0; // 阻塞唤醒标志置 0
    }
	//在驱动初始化的代码里调用class_create为该设备创建一个class,创建设备类
	my_class = class_create(THIS_MODULE, classname);
	//device_create创建对应的设备，省去了利用mknod命令手动创建设备节点，创建设备节点
    device_create(my_class, NULL, dev, NULL, devicename);

	
	printk(KERN_ALERT "--Hello, world\n");
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_ALERT "++Goodbye, cruel world\n");
	vfree(vmallocmem);
	device_destroy(my_class, MKDEV(major, 0));
    class_destroy(my_class);
    cdev_del(&globalvar.devm);
	unregister_chrdev_region(MKDEV(major, 0), 1);//注销设备
	printk(KERN_ALERT "--Goodbye, cruel world\n");
}





// EXPORT_SYMBOL(read_segfault);
EXPORT_SYMBOL(write_segfault);


module_init(hello_init);
module_exit(hello_exit);
