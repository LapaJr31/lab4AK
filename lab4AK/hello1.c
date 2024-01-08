#include "hello1.h"            // Підключення власного заголовочного файлу модуля
#include <linux/errno.h>       // Для кодів помилок
#include <linux/init.h>        // Для макросів ініціалізації та очищення модуля
#include <linux/ktime.h>       // Для роботи з часом у ядрі
#include <linux/list.h>        // Для роботи зі списками ядра
#include <linux/module.h>      // Основний інклюд для роботи з модулями
#include <linux/moduleparam.h> // Для роботи з параметрами модуля
#include <linux/printk.h>      // Для функцій логування
#include <linux/slab.h>        // Для функцій управління пам'яттю

// Метадані модуля
MODULE_AUTHOR("Yaroslav Latypov <jarlat04@lapuas.xyz>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("hello1"); // Псевдонім модуля

// Статична змінна для голови списку
static LIST_HEAD(event_list);

// Основна функція модуля, яка виводить "Hello, world!"
void print_hello(unsigned int repeat_count) {
    int i;
    ktime_t print_end;  
    struct print_count *event;

    // Цикл, що повторюється вказану кількість разів
    for (i = 0; i < repeat_count; i++) {
        // Виділення пам'яті для зберігання часу виведення
        event = kmalloc(sizeof(*event), GFP_KERNEL);
        event->timestamp = ktime_get(); // Зберігання часу початку виведення
        list_add(&event->list, &event_list); // Додавання елемента до списку

        pr_info("Hello, world!\n"); // Виведення повідомлення

        print_end = ktime_get();  // Зберігання часу кінця виведення
        event->print_duration = ktime_sub(print_end, event->timestamp); // Обчислення тривалості виведення
    }
}

EXPORT_SYMBOL(print_hello); // Експортування функції для використання в інших модулях

// Функція ініціалізації модуля
static int __init hello1_init(void) {
  pr_info("hello1 module loaded\n"); // Повідомлення про завантаження модуля
  return 0;
}

// Функція очищення модуля
static void __exit hello1_exit(void) {
  struct print_count *event, *temp;

  // Прохід по списку з виведенням часу та тривалості виведення для кожного елемента
  list_for_each_entry_safe(event, temp, &event_list, list) {
    ktime_t duration = ktime_sub(ktime_get(), event->timestamp);

    pr_info("Time: %lld, Print Duration: %lld\n", ktime_to_ns(duration),
            ktime_to_ns(event->print_duration));

    list_del(&event->list); // Видалення елемента зі списку
    kfree(event); // Звільнення виділеної пам'яті
  }

  pr_info("Unloading hello1 module\n"); // Повідомлення про вивантаження модуля
}

module_init(hello1_init); // Реєстрація функції ініціалізації
module_exit(hello1_exit); // Реєстрація функції очищення
