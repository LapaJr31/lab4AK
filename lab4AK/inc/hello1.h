#ifndef HELLO1_H
#define HELLO1_H

#include <linux/ktime.h>
#include <linux/list.h>

struct print_count {
  struct list_head list;
  ktime_t timestamp;
  ktime_t print_duration;
};

void print_hello(unsigned int repeat_count);

#endif
