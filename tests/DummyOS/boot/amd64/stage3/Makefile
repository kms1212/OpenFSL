ROOTDIR =   ../../../
TARGET =	loader.bin
BINPATH =	$(ROOTDIR)/buildutils/cross/bin/

AS = 		  nasm
ASFLAG = 	  -I${CURDIR}/src 
CC = 		  $(BINPATH)/x86_64-elf-gcc
CCFLAG = 	  -c -m64 -ffreestanding
LD =		  $(BINPATH)/x86_64-elf-ld
LDFLAG = 	  -melf_x86_64 -T$(CURDIR)/ldscripts/x86_64-elf.x -nostdlib -e main -Ttext 0x310000
OBJCOPY = 	  $(BINPATH)/x86_64-elf-objcopy 
OBJCOPYFLAG = -j .text -j .data -j .rodata -j .bss -S -O binary

CSOURCES = $(wildcard src/*.c)
COBJECTS = $(subst tmp/main.o, , $(patsubst src/%.c, tmp/%.o, $(CSOURCES)))
CENTRYOBJECT = tmp/main.o
ASMSOURCES = $(wildcard src/*.asm)
ASMOBJECTS = $(patsubst src/%.asm, tmp/%.o, $(ASMSOURCES))

FILES = \
	tmp/* \
	dep/* \
	$(TARGET)
	
DIRSTRUCT = \
	tmp \
	dep

all : buildDirectoryStructure $(TARGET)

$(TARGET) : $(CENTRYOBJECT) $(COBJECTS) $(ASMOBJECTS)
	$(CC) $(CCFLAG) -MM $(CSOURCES) > dep/dependency.dep
	$(LD) $(LDFLAG) -o tmp/loader.elf $^
	$(OBJCOPY) $(OBJCOPYFLAG) tmp/loader.elf $@

tmp/%.o : src/%.asm
	$(AS) $(ASFLAG) -felf64 $< -o $@

tmp/%.o : src/%.c
	$(CC) $(CCFLAG) -c $< -o $@


buildDirectoryStructure : 
	mkdir -p $(DIRSTRUCT)

clean :
	rm -rf $(FILES)

ifeq (dep/dependency.dep, $(wildcard dep/dependency.dep))
include dep/dependency.dep
endif
