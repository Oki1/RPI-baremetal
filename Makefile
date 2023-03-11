CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCPATH = gnuAssembler\bin

all: clean kernel8.img

boot.o: boot.S
	$(GCCPATH)\arm-none-eabi-gcc.exe -c boot.S -o boot.o

%.o: %.c
	$(GCCPATH)\arm-none-aebi-gcc.exe -c $< -o $@

kernel8.img: boot.o $(OFILES)
	$(GCCPATH)\arm-none-aebi-ld.exe -nostdlib boot.o $(OFILES) -T link.ld -o kernel8.elf
	$(GCCPATH)\arm-none-aebi-objcopy.exe -O binary kernel8.elf kernel8.img

clean:
	