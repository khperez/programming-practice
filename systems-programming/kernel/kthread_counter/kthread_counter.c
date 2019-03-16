/* SPDX-License-Identifier: GPL-2.0 */
// 4.15.0-46-generic
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Katherine Perez <perezkh@gmail.com>");
MODULE_DESCRIPTION("Proc read with kernel thread");

#define ENTRY_NAME "counter"
#define FILE_PERMS 0644
#define PARENT NULL
static struct file_operations fops;

static struct task_struct *kthread;
static int counter;
static char *message;
static int read_p;

static int __init counter_init(void);
static void __exit counter_exit(void);
int counter_run(void *data);
int counter_proc_open(struct inode *sp_inode, struct file *sp_file);
ssize_t counter_proc_read(struct file *sp_file, char __user *buf, size_t size, \
                          loff_t *offset);
int counter_proc_release(struct inode *sp_inode, struct file *sp_file);


int counter_proc_open(struct inode *sp_inode, struct file *sp_file)
{
        printk(KERN_INFO "[ENTRY] counter_proc_open\n");
        message = kmalloc(sizeof(char) * 20, GFP_KERNEL);
        read_p = 1;
        if (message == NULL) {
                printk(KERN_ERR "Error! counter_proc_open\n");
                return -ENOMEM; // out of memory 
        }
        sprintf(message, "The counter is now at %d\n", counter);
        printk(KERN_INFO "[EXIT] counter_proc_open\n");
        return 0;
}

ssize_t counter_proc_read(struct file *sp_file, char __user *buf, size_t size, loff_t *offset)
{
        int len = strlen(message);
        printk(KERN_INFO "[ENTRY] counter_proc_read\n");

        read_p = !read_p;
        if (read_p)
                return 0;
        copy_to_user(buf, message, len);
        printk(KERN_INFO "[EXIT] counter_proc_read\n");
        return len;
}        

int counter_proc_release(struct inode *sp_inode, struct file *sp_file)
{
        printk(KERN_INFO "[ENTRY] counter_proc_release\n");
        kfree(message);
        printk(KERN_INFO "[EXIT] counter_proc_release\n");
        return 0;
}

int counter_run(void *data)
{
        printk(KERN_INFO "[ENTRY] counter_run\n");
        while (!kthread_should_stop()) {
                ssleep(1);
                counter += 1;
        }
        printk(KERN_INFO "[EXIT] counter_run\n");
        return 0;
}

static int __init counter_init(void)
{
        printk(KERN_INFO "[ENTRY] counter_init\n");

        kthread = kthread_run(counter_run, NULL, ENTRY_NAME);
        if (IS_ERR(kthread)) {
                printk(KERN_ERR "Error! kthread_run\n");
                return PTR_ERR(kthread);
        }

        fops.open = counter_proc_open;
        fops.read = counter_proc_read;
        fops.release = counter_proc_release;

        if (!proc_create(ENTRY_NAME, FILE_PERMS, NULL, &fops)) {
                printk(KERN_ERR "Error! proc_create\n");
                remove_proc_entry(ENTRY_NAME, NULL);
                return -ENOMEM; // out of memory 
        }
        printk(KERN_INFO "[EXIT] counter_init\n");
        return 0;
}

static void __exit counter_exit(void)
{
        int ret = kthread_stop(kthread);
        if (ret != -EINTR)
                printk(KERN_INFO "Counter thread has stopped\n");
        remove_proc_entry(ENTRY_NAME, NULL);
        printk(KERN_INFO "Removing /proc/%s\n", ENTRY_NAME);
}

module_init(counter_init);
module_exit(counter_exit);
