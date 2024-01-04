#include "hello1.h"
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>

MODULE_AUTHOR("Yaroslav Latypov <jarlat04@lapuas.xyz>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("hello2");

static unsigned int print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Times to print 'Hello, world!'");

static int __init hello2_init(void) {
  pr_info("hello1 module loaded\n");

  if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
    pr_warn("Warning: print_count is 0 or between 5 and 10\n");
  } else if (print_count > 10) {
    pr_err("Error: print_count is greater than 10\n");
    return -EINVAL;
  }

  print_hello(print_count);

  return 0;
}

static void __exit hello2_exit(void) { pr_info("Unloading hello2 module\n"); }

module_init(hello2_init);
module_exit(hello2_exit);
