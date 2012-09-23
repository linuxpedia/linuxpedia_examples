/*  Simple driver example: 
 *
 * Registers a timer that is called after a user-defined
 * timeout. The timer function just prints a message and
 * re-arms itself.
 *
 * When unloading the driver we have to take special
 * precautions to correctly de-register the timer.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/timer.h>

/*
 * declare the timer itself
 */
static struct timer_list   my_timer;

/*
 * required synchronization mechanisms to disable
 * the timer on unload.
 *
 * This is a flag and a spinlock. We have to use a
 * spinlock because a mutex might call schedule()
 * on contention which is inappropriate in a timer
 * function.
 */
static spinlock_t   timer_lock;
static int          rearm_timer = 1;


/* 
 * declare the timeout as a module parameter
 */
static ulong timeout = 3;
module_param(timeout, ulong, 0);
MODULE_PARM_DESC(timeout, " interval in which the timer will be triggered");


/*
 * this is the timer function which will be called in
 * SoftIRQ context when the timer expires. We must no
 * do anything that eventually calls schedule() behind
 * our back because we have no process context
 * associated at that point.
 */
static void my_timer_func(unsigned long arg)
{
    unsigned long flags;
    /* print a message that we were invoked */
    printk(KERN_INFO "mytimer: woken up.(%ld)\n",jiffies);
    /* check whether we might re-arm ourselfes -- this
     * flag is changed by module unloading code, we do this
     * under the spinlock to avoid races between checking
     * rearm_timer and modifying the timer. */
    spin_lock_irqsave(&timer_lock, flags);
    if (rearm_timer)
        mod_timer(&my_timer, jiffies + timeout * HZ);
    spin_unlock_irqrestore(&timer_lock, flags);
}


/*
 * module init routine -- initializes and starts the timer
 */
static int __init my_timer_init(void)
{
    printk(KERN_INFO "mytimer: loading module...\n");
    /* initialize the timer and set the timer function */
    init_timer(&my_timer);
    my_timer.function = my_timer_func;
    /* initialize the spinlock */
    spin_lock_init(&timer_lock);
    /* set the first timer event */
    mod_timer(&my_timer, jiffies + timeout * HZ);
    printk(KERN_INFO "mytimer: module loaded.\n");
    return 0;
}

/*
 * module exit routine -- deregisters the timer carefully
 */
static void __exit my_timer_exit(void)
{
    unsigned long flags;
    printk(KERN_INFO "mytimer: unloading module...\n");
    /* lock the mutex to avoid a race with the timer function.
     * it might become running while we're in this section
     * and re-arm itself concurrently. We just change the
     * rearm-timer-flag so that the timer knows it should stop.
     */
    spin_lock_irqsave(&timer_lock, flags);
    rearm_timer = 0;
    spin_unlock_irqrestore(&timer_lock, flags);
    /* delete any next timer event and wait for the
     * timer function to complete. if we don't do that, we
     * might unmap the module code when it is still executing!
     */
    del_timer_sync(&my_timer);
    printk(KERN_INFO "mytimer: module unloaded.\n");
}


module_init(my_timer_init);
module_exit(my_timer_exit);
MODULE_LICENSE("GPL");

