#include <linux/init.h>
#include<linux/delay.h>
#include<linux/syscalls.h>
#include<linux/module.h>
#include<linux/kthread.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/cdev.h>

static struct task_struct *task1;
static struct task_struct *task2;
int val=100;
const int max=10;
static DEFINE_SEMAPHORE(s1);
static DEFINE_SEMAPHORE(s2);

static int thread_one(void *pargs)
{
	int i;
	printk("---> Thread A Welcome<---consumer");
	down_interruptible(&s1);
	for(i=1;i<=max;i++)
	{
		val++;
		printk("Thread A");
		if(kthread_should_stop())
			break;
	}
	up(&s2);
	do_exit(0);
	return 0;
}

static int thread_two(void *pargs)
{
	int i;
	printk("---> Thread B Welcome<---Producer");
	down_interruptible(&s2);
	for(i=1;i<=max;i++)
	{
		val--;
		printk("Thread B");
		if(kthread_should_stop())
			break;
	}
	up(&s1);
	do_exit(0);
	return 0;
}

static int __init thread_init(void) 
{  
	sema_init(&s1,0);
	sema_init(&s2,1);
	task1=kthread_run(thread_one, NULL, "thread_A");
	//kthread_create + wake_up_process
	task2=kthread_run(thread_two, NULL, "thread_B");
	return 0;
}

static void __exit thread_exit(void)
{
	printk("VAL:%d",val);
	/*
	if(task1)
	{
		kthread_stop(task1);
	}
	if(task2)
	{
		kthread_stop(task2);
	}
	*/
	printk("----EXIT----");
}

module_init(thread_init);
module_exit(thread_exit);
