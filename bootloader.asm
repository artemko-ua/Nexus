;asm bootloader 
;for my unix os (Nexus or smth else)
;BIOS/UEFI

org 0x7C00          ; BIOS завантажує завантажувач за цією адресою
bits 16             ; Режим 16-біт (реальний режим)

start:
    ; Налаштувати сегменти
    xor ax, ax      ; AX = 0
    mov ds, ax      ; DS = 0 (сегмент даних)
    mov es, ax      ; ES = 0 (додатковий сегмент)

    ; Налаштувати стек
    mov ss, ax      ; SS = 0 (сегмент стеку)
    mov sp, 0x7C00  ; SP = 0x7C00 (вершина стеку)

    ; Вивести повідомлення
    mov si, msg     ; SI = адреса рядка
    call print_string

    ; Чекати натискання клавіші
    mov ah, 0x00    ; Функція BIOS: очікування клавіші
    int 0x16        ; Виклик переривання BIOS

    ; Завершити (зациклити)
    cli             ; Заборонити переривання
    hlt             ; Зупинити CPU

print_string:
    lodsb           ; Завантажити символ з [SI] в AL
    or al, al       ; Перевірити на NULL (кінець рядка)
    jz .done        ; Якщо NULL, завершити
    mov ah, 0x0E    ; Функція BIOS: вивести символ
    int 0x10        ; Виклик переривання BIOS
    jmp print_string
.done:
    ret             ; Повернутися з функції

msg db "Hi bootloaders!", 0x0D, 0x0A, 0 ; \r\n + NULL

; Заповнити сектор до 512 байт
times 510 - ($-$$) db 0

; Сигнатура MBR (обов’язково!)
dw 0xAA55

