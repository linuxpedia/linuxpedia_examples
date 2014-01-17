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
#include <linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void) {
        printk(KERN_ERR" Hello world!\n");
        printk(KERN_ERR" Printing current pid : %d \n", current->pid);
        return 0;
}

static void hello_exit(void) {
        printk(KERN_ERR" Printing current pid : %d \n", current->pid);
        printk(KERN_ERR "Bye world\n");
}

module_init(hello_init);
module_exit(hello_exit);
