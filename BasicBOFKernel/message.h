#pragma once

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init_task.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kernel_read_file.h>
#include <linux/uaccess.h>

#ifndef BASIC_H
#define BASIC_H

#define DEVICE_NAME "messager"
#define CLASS_NAME "msnger"

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

#endif // BASIC_H