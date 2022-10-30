BOOT_ASM_SOURCES = $(wildcard boot/*.asm) kernel/kernel_entry.asm
KERNEL_ASM_SOURCES = $(wildcard hardware/*.asm)
C_SOURCES = $(wildcard kernel/*.c drivers/*.c lib/*.c hardware/*.c mem/*.c)

HEADERS = $(wildcard kernel/*.h drivers/*.h lib/*.h hardware/*.h mem/*.h)

C_OBJ = ${C_SOURCES:.c=.o}
ASM_OBJ = ${KERNEL_ASM_SOURCES:.asm=.o}
OBJ = ${C_OBJ} ${ASM_OBJ}

IMAGE = micros.img

CC = gcc
C_FLAGS = -Wno-narrowing -ffreestanding -m32 -fno-pie -mgeneral-regs-only -nostdlib

.PHONY: clean all

all: $(IMAGE)

$(IMAGE): boot/boot.bin kernel/kernel.bin
	cat $^ > $(IMAGE)

kernel/kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ -T kernel.ld $^ -m elf_i386 --nmagic

boot/boot.bin: $(wildcard boot/*.asm)
	nasm boot/boot.asm -f bin -o $@

%.o: %.c ${HEADERS}
	$(CC) $(C_FLAGS) -c $< -o $@ 

%.o: %.asm ${KERNEL_ASM_SOURCES}
	nasm $< -f elf -o $@

kernel/kernel_entry.o: kernel/kernel_entry.asm ${BOOT_ASM_SOURCES}
	nasm $< -f elf -o $@

clean:
	rm -f *.bin *.dis *.o *.img *.log
	rm -f kernel/*.o kernel/*.bin boot/*.bin drivers/*.o lib/*.o hardware/*.o mem/*.o
