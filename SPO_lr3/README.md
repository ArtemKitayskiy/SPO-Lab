### Задание на лабораторную работу
1. Получить адреса-границы анализируемой области памяти. Границы всего
виртуального адресного пространства (ВАП) процесса можно определить
при помощи функции GetSystemInfo(). Начальный адрес, если он
вводится пользователем, нужно округлить вниз до кратного размеру
страницы виртуальной памяти (определяемого GetSystemInfo()).
Аналогично следует округлять и одиночный адрес, вводимый пользователем.
2. Атрибуты страниц, а также начальные адреса и размеры областей (регионов)
памяти можно определить при помощи функции VirtualQuery().
Перебор всех страниц (или регионов) можно выполнить, продолжая вызывать
функцию VirtualQuery() для адресов, смещаемых каждый раз на размер
страницы (или региона)
### Результат работы программы
```
Addresses range:[0x10000, 0x7ffeffff]
Enter lower limit:0x20000
Enter upper limit:0x60000

Legend:
        State:  Free, Reserved, Committed;
        Access: Readable, Writable, eXecutable, Copy-on-write, Guard,
                No cache, write combine (Q), no access (-------);
                empty space indicates no access to page protection info.

   Address      Size    Used    State   Access  Type
   0x20000      4096    Yes     C       RW--   private
   0x21000      106496  Yes     R       RW--   private
   0x3b000      20480   No      F               unused
   0x40000      106496  Yes     C       R---    mapped
   0x5a000      24576   No      F               unused
