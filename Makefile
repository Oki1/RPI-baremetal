CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCPATH = gcc-arm-10.3-2021.07-mingw-w64-i686-aarch64-none-elf/bin
all: clean kernel8.img

start.o: boot.S
	$(GCCPATH)/aarch64-none-elf-gcc.exe $(CFLAGS) -c boot.S -o boot.o

kernel8.img: boot.o
	$(GCCPATH)/aarch64-none-elf-ld.exe -nostdlib boot.o -T link.ld -o kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy.exe -O binary kernel8.elf kernel8.img

clean:


run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -d in_asm