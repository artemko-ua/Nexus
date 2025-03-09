
[bits 32]           ; 32-бітний код
[global _start]     ; Глобальна мітка для лінкера
[extern kernel_main] ; Зовнішня функція з C-коду

_start:
    ; Підготовка стеку
    mov esp, stack_space    ; Вказуємо ESP на вершину стеку
    
    ; Виклик функції ядра
    call kernel_main
    
    ; На випадок, якщо функція kernel_main завершиться
    jmp $                   ; Нескінченний цикл

; Виділяємо пам'ять для стеку
section .bss
stack_space: resb 8192      ; 8 КБ для стеку