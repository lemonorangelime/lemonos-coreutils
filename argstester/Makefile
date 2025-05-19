BUILD_DIR := build

CC := gcc
S := gcc
ASM := nasm
ASMFLAGS := -f elf32
CCFLAGS := -O3 -fno-stack-protector -D __LEMONOS__ -fPIE -msse -msse2 -mno-avx -mhard-float -static -m32 -fno-builtin -fno-builtin-function -fomit-frame-pointer -funsigned-char -falign-functions=16 -nostdlib -nostartfiles -funsigned-char -I../lemonos-libc/include -I../linked-lists/include -Iinclude
LD := ld
LDFLAGS := -m elf_i386 --no-dynamic-linker -pie
MAKE := make
AR := ar

SOURCES=$(wildcard src/*.c)
ASM_SOURCES=$(wildcard src/*.asm)

OBJS=$(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
ASM_OBJS=$(patsubst src/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

LINKED := ../linked-lists/linked.o
LIBC := ../lemonos-libc/libc.a
LINK_FILE := ../lemonos-libc/link.ld
OUTPUT := argstester

default: mkdir depends build

depends:
	ORIGIN="$$(pwd)"; \
	cd ..; \
	make depends; \
	cd $$ORIGIN

mkdir:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR} ${OUTPUT} ${GRUBDST}

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CCFLAGS) $^ -c -o $@

$(BUILD_DIR)/%.o: src/%.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

build: $(OBJS) $(ASM_OBJS)
	$(LD) $(LDFLAGS) -T $(LINK_FILE) $^ $(LINKED) $(LIBC) -o $(OUTPUT)
	strip $(OUTPUT)
