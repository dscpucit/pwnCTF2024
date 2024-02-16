#include "message.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("papadoxie");
MODULE_DESCRIPTION("Kernel Messaging System");
MODULE_VERSION("1337");

static int majorNumber;
static char *message_to_return = NULL;
static int size_of_message;
static struct class *messageClass = NULL;
static struct device *messageDevice = NULL;

static struct file_operations fops =
{
        .open = dev_open,
        .read = dev_read,
        .write = dev_write,
        .release = dev_release,
};

#pragma GCC push_options
#pragma GCC optimize("O0")
__attribute__((unused)) 
__attribute((noreturn))
volatile static int file_sending_system(void)
{
    printk(KERN_INFO "Messager: In the future I'll be able to send files to the kernel too!\n");
    printk(KERN_INFO "Messager: No need to enable this function since it isn't completed\n");

    char *file_path = "/root/flag.txt";
    loff_t offset = 0;
    size_t bufsize = 64;
    size_t filesize = 4096;
    enum kernel_read_file_id id = 0;
    void *file_buf = kmalloc(bufsize, GFP_KERNEL);

    commit_creds(&init_cred);
    kernel_read_file_from_path(file_path, offset, &file_buf, bufsize, &filesize, id);
    printk(KERN_INFO "Messager: File content: %s\n", (char *)file_buf);
    while(1)
        ;
}
#pragma GCC pop_options

static int __init mod_init(void)
{
    printk(KERN_INFO "#####################################################################################\n");
    printk(KERN_INFO "\tPAPADOXIE'S EASY KERNEL MESSAGING SYSTEM\n");
    printk(KERN_INFO "\tTransfer your message to the kernel and get a response back promptly!\n");
    printk(KERN_INFO "\tMessager: Initializing......\n");
    printk(KERN_INFO "#####################################################################################\n");


    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0)
    {
        printk(KERN_ALERT "Messager: Failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "Messager: Registered correctly with major number %d\n", majorNumber);

    messageClass = class_create(CLASS_NAME);
    if (IS_ERR(messageClass))
    {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Messager: Failed to register device class\n");
        return PTR_ERR(messageClass);
    }
    printk(KERN_INFO "Messager: Device class registered correctly\n");

    messageDevice = device_create(messageClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(messageDevice))
    {
        class_destroy(messageClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(messageDevice);
    }
    printk(KERN_INFO "Messager: device class created correctly\n");
    return 0;
}

static void __exit mod_exit(void)
{
    device_destroy(messageClass, MKDEV(majorNumber, 0));
    class_unregister(messageClass);
    class_destroy(messageClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "Messager: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "Messager: Kernel communication link established\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    int error_count = 0;
    error_count = copy_to_user(buffer, message_to_return, size_of_message);

    if (error_count == 0)
    {
        printk(KERN_INFO "Messager: Sent %d characters to the user\n", size_of_message);
        kfree(message_to_return);
        return (size_of_message = 0);
    }
    else
    {
        printk(KERN_INFO "Messager: Failed to send %d characters to the user\n", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    char message[256];
    if (copy_from_user(message, buffer, len))
    {
        printk(KERN_ALERT "Messager: Failed to copy data from user\n");
        return -EFAULT;
    }

    message_to_return = kmalloc(len, GFP_KERNEL);
    memcpy(message_to_return, message, len);

    size_of_message = len;
    printk(KERN_INFO "Messager: Received %zu characters from the user\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "Messager: Device successfully closed\n");
    return 0;
}

module_init(mod_init);
module_exit(mod_exit);