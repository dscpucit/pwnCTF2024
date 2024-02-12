#include "message.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("papadoxie");
MODULE_DESCRIPTION("My first kernel module!");
MODULE_VERSION("1337");

static int majorNumber;
static char message[256] = {0};
static short size_of_message;
static struct class *messageClass = NULL;
static struct device *messageDevice = NULL;


static struct file_operations fops =
{
        .open = dev_open,
        .read = dev_read,
        .write = dev_write,
        .release = dev_release,
};

static int __init mod_init(void)
{
    printk(KERN_INFO "Messager: Initializing\n");

    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0)
    {
        printk(KERN_ALERT "Messager: Failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "Messager: Registered correctly with major number %d\n", majorNumber);

    messageClass = class_create(THIS_MODULE, CLASS_NAME);
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
    printk(KERN_INFO "Messager: Device has been opened\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    int error_count = 0;
    error_count = copy_to_user(buffer, message, size_of_message);

    if (error_count == 0)
    {
        printk(KERN_INFO "Messager: Sent %d characters to the user\n", size_of_message);
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
    sprintf(message, "%s(%zu letters)", buffer, len);
    size_of_message = strlen(message);
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