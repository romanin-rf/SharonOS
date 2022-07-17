LINKDIR = tmp/
LDOPTS = -Tlink.ld -build-id=none -b pei-i386
GCCOPTS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -nostdlib -c -I include

build: clean compile link

clean:
	@echo Clearing...
	@del tmp\* /s /q

compile:
	@echo [AS] boot.o
	@nasm -f elf32 loader/loader.asm -o $(LINKDIR)boot.o
	@echo [CC] kernel.o
	@gcc $(GCCOPTS) -o $(LINKDIR)kernel.o kernel/main.c
	@echo [CC] printf.o
	@gcc $(GCCOPTS) -o $(LINKDIR)printf.o common/printf.c
	@echo [CC] screen.o
	@gcc $(GCCOPTS) -o $(LINKDIR)screen.o common/screen.c
	@echo [CC] pci.o
	@gcc $(GCCOPTS) -o $(LINKDIR)pci.o common/pci.c
	@echo [CC] inlineasm.o
	@gcc $(GCCOPTS) -o $(LINKDIR)inlineasm.o common/inlineasm.c

.c.o:
	@echo [CC] $@
	@gcc $(GCCOPTS) -o $@ -c

link:
	@echo [LD] preelf.o
	@ld $(LINKDIR)boot.o $(LINKDIR)kernel.o $(LINKDIR)pci.o $(LINKDIR)printf.o $(LINKDIR)inlineasm.o $(LINKDIR)screen.o $(LDOPTS) -o $(LINKDIR)preelf.o
	@echo [OBJCOPY] out.kern
	@objcopy -O elf32-i386 $(LINKDIR)preelf.o out.kern

run:
	@qemu-system-i386 -kernel out.kern
