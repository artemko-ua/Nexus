ENTRY(_start)    /* Вхідна точка */

SECTIONS {
    . = 0x1000;  /* Завантажуємо ядро на 4KB після початку диска */

    .text : {
        *(.text)  /* Секція коду */
    }

    .data : {
        *(.data)  /* Секція ініціалізованих даних */
    }

    .bss : {
        *(.bss)   /* Секція неініціалізованих даних */
    }
}