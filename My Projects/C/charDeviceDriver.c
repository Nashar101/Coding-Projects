/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <charDeviceDriver.h>
#include  <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include "ioctl.h"

MODULE_LICENSE("GPL");

/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */


DEFINE_MUTEX  (devLock);
static int counter = 0;
struct message{
	char message [BUF_LEN+2];
	};

struct message *limit[1000];
static long device_ioctl(struct file *file,	/* see include/linux/fs.h */
		 unsigned int ioctl_num,	/* number and param for ioctl */
		 unsigned long ioctl_param)
{

	/* 
	 * Switch according to the ioctl called 
	 */
	if (ioctl_num == RESET_COUNTER) {
	    counter = 0; 
	    /* 	    return 0; */
	    return 5; /* can pass integer as return value */
	}

	else {
	    /* no operation defined - return failure */
	    return -EINVAL;

	}
}


/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	
	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}


    	for(i = 0; i < maxmessages; i++){
    		limit[i] = kmalloc(sizeof(struct message), GFP_ATOMIC);
    		strcpy(limit[i]->message,"NULL");
    		if(limit[i] == NULL){
			printk(KERN_INFO "kmalloc failed you donut. \n");
			return -ENOMEM;
		}
    	}
    	printk(KERN_INFO "Buffer_Size: it is %d", BUF_LEN);
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
	
	sema_init(&sem, 1);
	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/*  Unregister the device */
	/*if(msgStore != NULL){
		kfree(msgStore);
		printk(KERN_INFO "memory freed");
	}
	msgStore = NULL;*/
	for(i = 0; i < maxmessages; i++){
		kfree(limit[i]);
		limit[i] = NULL;
	}
	down(&sem);
	up(&sem);
	unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
    mutex_lock (&devLock);
    if (Device_Open) {
	mutex_unlock (&devLock);
	return -EBUSY;
    }
    Device_Open++;
    mutex_unlock (&devLock);
    //sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    printk(KERN_INFO "Buffer_Size: current length, it is %d", BUF_LEN);
    printk(KERN_INFO "CharDeviceDriver: Buffer Opened");
    try_module_get(THIS_MODULE);
    
    
    return SUCCESS;
}

/* Called when a process closes the device file. */
static int device_release(struct inode *inode, struct file *file)
{
    mutex_lock (&devLock);
	Device_Open--;		/* We're now ready for our next caller */
	mutex_unlock (&devLock);
	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	module_put(THIS_MODULE);
	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	/* result of function calls */
	int result;
	down (&sem);

	if((stpointer == edpointer) && (strcmp(limit[edpointer]->message, "NULL") == 0)){
		up(&sem);
		buffer = NULL;
		return -EAGAIN;
	}
	else if(stpointer == edpointer && nummsgs == 1){
		strncpy(msg, limit[stpointer]->message, strlen(limit[stpointer]->message)-1);
		if (strlen(msg) + 1 < length){
		length = strlen(msg);
		}
		result = copy_to_user(buffer, limit[stpointer]->message, length);
		strcpy(limit[stpointer]->message, "NULL");
		nummsgs--;
		up(&sem);
		printk(KERN_INFO "current value of edpointer %d \n", edpointer);
		printk(KERN_INFO "current value of stpointer %d \n", stpointer);
		return length;
	}
	strncpy(msg, limit[stpointer]->message, strlen(limit[stpointer]->message)-1);
	if (strlen(msg) + 1 < length){
	length = strlen(msg);
	}
	result = copy_to_user(buffer, limit[stpointer]->message, length);
	strcpy(limit[stpointer]->message, "NULL");		
	if (result > 0)
		return -EFAULT;
	if(stpointer == maxmessages-1){
		stpointer = 0;
	}
	else{	
		stpointer++;
	}
	nummsgs --;
	up(&sem);
	printk(KERN_INFO "current value of edpointer %d \n", edpointer);
	printk(KERN_INFO "current value of stpointer %d \n", stpointer);	
	return length;
}

/* Called when a process writes to dev file: echo "hi" > /dev/hello  */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{	
	
	if (nummsgs == maxmessages){
		return -EBUSY;
	}
	if(len-1 > BUF_LEN){
		buff = NULL;
		return -EINVAL;
	}
	else {
		printk(KERN_INFO "Buffer_Msg: buffer msg %s, length is %zu", buff,len);
		down(&sem);
		strncpy(msg,buff, len);
		strcpy(limit[edpointer]->message,msg);
		printk(KERN_INFO "charDeivceDriver: length of message is %zu", len-1);
		//sprintf(limit[currmsg]->message, "%s", msg);
		if(edpointer == maxmessages-1){
			edpointer = 0;
		}
		else{
		edpointer ++;
		}
		printk(KERN_INFO "current value of edpointer %d \n", edpointer);
		printk(KERN_INFO "current value of stpointer %d \n", stpointer);
		buff = NULL;}
	nummsgs++;
	up(&sem);	
	return len;
}
