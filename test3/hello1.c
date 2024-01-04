#include "hello1.h"
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>

MODULE_AUTHOR("Yaroslav Latypov <jarlat04@lapuas.xyz>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("hello1");

static LIST_HEAD(event_list);

void print_hello(unsigned int repeat_count) {
    int i;
    ktime_t print_end;  
    struct print_count *event;

    for (i = 0; i < repeat_count; i++) {
        event = kmalloc(sizeof(*event), GFP_KERNEL);
        event->timestamp = ktime_get();
        list_add(&event->list, &event_list);

        pr_info("Hello, world!\n");

        print_end = ktime_get();  
        event->print_duration = ktime_sub(print_end, event->timestamp);
    }
}

EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void) {
  pr_info("hello1 module loaded\n");

  return 0;
}

static void __exit hello1_exit(void) {
  struct print_count *event, *temp;

  list_for_each_entry_safe(event, temp, &event_list, list) {
    ktime_t duration = ktime_sub(ktime_get(), event->timestamp);

    pr_info("Time: %lld, Print Duration: %lld\n", ktime_to_ns(duration),
            ktime_to_ns(event->print_duration));

    list_del(&event->list);
    kfree(event);
  }

  pr_info("Unloading hello1 module\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
