BUILD_DIR := build

CC := i386-gcc-lemonos
ASM := nasm
ASMFLAGS := -f elf32
CCFLAGS := -O3 -msse -msse2 -mno-avx -Iinclude
LD := i386-ld-lemonos
LDFLAGS := -msse -msse2 -mno-avx

SOURCES=$(wildcard src/*.c)
ASM_SOURCES=$(wildcard src/*.asm)

OBJS=$(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
ASM_OBJS=$(patsubst src/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))

OUTPUT := screenshot

default: mkdir build

mkdir:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR} ${OUTPUT}

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CCFLAGS) $^ -c -o $@

$(BUILD_DIR)/%.o: src/%.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

build: $(OBJS) $(ASM_OBJS)
	$(LD) $(LDFLAGS) $^ -o $(OUTPUT)
	strip $(OUTPUT)
