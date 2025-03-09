ASM=nasm
CC=gcc
LD=ld
QEMU=qemu-system-x86_64

# Прапорці компілятора
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
LDFLAGS=-m elf_i386 -T link.ld
ASMFLAGS=-f elf32

# Директорії
SRC_DIR=src
INCLUDE_DIR=include

# Імена файлів
KERNEL_C=kernel.c
KERNEL_ENTRY=kernel_entry.asm
BOOTLOADER=bootloader.asm
BOOTLOADER_BIN=bootloader.bin

# Файли ядра
KERNEL_C_FILES=$(KERNEL_C) $(SRC_DIR)/screen.c $(SRC_DIR)/keyboard.c $(SRC_DIR)/filesystem.c $(SRC_DIR)/shell.c $(SRC_DIR)/port.c $(SRC_DIR)/memory.c $(SRC_DIR)/string.c
KERNEL_OBJ=$(KERNEL_C_FILES:.c=.o) kernel_entry.o
KERNEL_BIN=kernel.bin
OS_IMAGE=nexux.img

# Правила збірки
all: $(OS_IMAGE)

# Створення образу ОС
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $^ > $@

# Компіляція завантажувача
$(BOOTLOADER_BIN): $(BOOTLOADER)
	$(ASM) -f bin $< -o $@

# Компіляція ядра
$(KERNEL_BIN): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

# Компіляція вхідної точки ядра
kernel_entry.o: $(KERNEL_ENTRY)
	$(ASM) $(ASMFLAGS) $< -o $@

# Загальне правило для компіляції C-файлів
%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@

# Правило для компіляції C-файлів з директорії src
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $< -o $@

# Запуск в QEMU
run: $(OS_IMAGE)
	$(QEMU) -fda $<

# Очищення згенерованих файлів
clean:
	rm -f *.o $(SRC_DIR)/*.o *.bin *.img

.PHONY: all run clean