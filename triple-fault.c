/*
 * triple-fault.c - Inject a triple fault at the beginning of any kernel
 * function to trigger a system reboot when the function is called
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 *
 * Copyright (C) 2020 Andrea Righi <andrea.righi@canonical.com>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/fdtable.h>
#include <linux/fs.h>

static char *function_name;
module_param(function_name, charp, 0);
MODULE_PARM_DESC(function_name, "Name of the fuction to trap");

static void __triple_fault(void)
{
	/* Load an invalid IDT */
	struct desc_ptr idt = {
		.address = (unsigned long) NULL,
		.size = 0,
	};
	load_idt(&idt);
	/*
	 * Breakpoint with the invalid IDT => this will trigger the
	 * triple-fault exception, causing the system to reboot.
	 */
	__asm__ __volatile__("int3");
}

static int wrapper(struct kprobe *p, struct pt_regs *regs)
{
	__triple_fault();
	return 0;
}

static struct kprobe kp = {
	.pre_handler = wrapper,
};

static int __init triple_fault_init(void)
{
	int ret;

	/* Use a kprobe to wrap target function */
	kp.symbol_name = function_name;
	ret = register_kprobe(&kp);
	if (ret < 0) {
		printk(KERN_INFO "%s: error %d\n", __func__, ret);
		return ret;
	}
	return 0;
}

static void __exit triple_fault_exit(void)
{
	unregister_kprobe(&kp);
}

module_init(triple_fault_init)
module_exit(triple_fault_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrea Righi <andrea.righi@canonical.com>");
MODULE_DESCRIPTION("Inject a triple-fault into a target kernel function");
