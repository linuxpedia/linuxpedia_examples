/*
 *  This program explains POSIX Message Queue APIs
 *
 *  http://www.linuxpedia.org/index.php?title=Linux_Character_Driver
 *
 *  Copyright (C) 2012  LinuxPedia.org (anoojgopi@linuxpedia.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

struct task_struct *my_kthread_task_struct;


int my_kthread_entry_function (void *data)
{
	int count;
        printk(KERN_ERR" In kthread function with arg : %x \n", (unsigned int)data);
	allow_signal(SIGTERM);

	for(count=0; count < 10; count++) {

		// Print tick messages
		printk(KERN_ERR" kthread function .. tick %lu \n", jiffies);

		msleep_interruptible(30000);//TODO need to findout a way to exit when kthread_stop is called
		/*
		 * When someone calls kthread_stop() on your kthread, it will be woken
		 * and kthread_should_stop() will return true.  You should then return from kthread, and your return
		 * value will be passed through to kthread_stop().
		*/
		if(kthread_should_stop())
		{
			printk(KERN_ERR" kthread_stop initiated exit at %lu \n", jiffies);
			return -1; //Exit from the thread. Return value will be passed to kthread_stop()
		}

	}
	// Exit from the thread
	printk(KERN_ERR" kthread function exits at %lu \n", jiffies);
	do_exit(0);
}

static int driver_init(void) {

        printk(KERN_ERR" In driver init \n");

	/**
	 * kthread_run - create and run a kthread.
	 * @threadfn: the function to run until signal_pending(current).
	 * @data: data ptr for @threadfn.
	 *
	 * Description: This helper function creates and names a kernel
	 * thread. When woken, the thread will run @threadfn() with @data as its
	 * argument. @threadfn() can either call do_exit() directly if it is a
	 * standalone thread for which no one will call kthread_stop(), or
	 * return when 'kthread_should_stop()' is true (which means
	 * kthread_stop() has been called).  The return value should be zero
	 * or a negative error number; it will be passed to kthread_stop().
	 *
	 * Returns a task_struct or ERR_PTR(-ENOMEM).
	 */
	my_kthread_task_struct = kthread_run(my_kthread_entry_function, (void *)0xdeadbeef, "my-kthread");

        return 0;
}

static void driver_exit(void) {
	int ret_val;

        printk(KERN_ERR" In driver exit called at %lu \n", jiffies);
	/* Stop the kthread we created */
	ret_val = kthread_stop(my_kthread_task_struct);
        printk(KERN_ERR" kthread_stop returned with %d at %lu \n", ret_val, jiffies);
}

MODULE_LICENSE("Dual BSD/GPL");
module_init(driver_init);
module_exit(driver_exit);
