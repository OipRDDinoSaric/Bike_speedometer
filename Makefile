MODULE_NAME = STM32F407

STLINK=~/stlink

#Toolchain
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
LD = $(PREFIX)gcc
AR = $(PREFIX)ar
BIN = $(PREFIX)objcopy

LIB_C = 
LIB_GCC = 

HAL = Drivers/STM32F4xx_HAL_Driver
CMSIS = Drivers/CMSIS
MIDDLEWARES = Middlewares/ST

STARTUP = $(CMSIS)/Device/ST/STM32F4xx/Source/Templates/gcc_ride7

CFLAGS = -Wall \
         -mcpu=cortex-m4 \
         -mthumb \
         -mfloat-abi=softfp \
         -fomit-frame-pointer \
         -fno-strict-aliasing \
         -specs=nosys.specs \
         -specs=nano.specs 

# Assembler flags
AFLAGS = -x assembler-with-cpp

# Linker flags
LDFLAGS = -specs=nosys.specs -specs=nano.specs -mcpu=cortex-m4 -mthumb

INC =  -I $(HAL)/Inc
INC += -I $(CMSIS)/Include
INC += -I $(CMSIS)/Device/ST/STM32F4xx/Include
INC += -I $(MIDDLEWARES)/STM32_USB_Host_Library/Class/CDC/Inc
INC += -I $(MIDDLEWARES)/STM32_USB_Host_Library/Core/Inc
INC += -I Inc

CFLAGS += $(INC)
CFLAGS += -g -std=gnu11
CFLAGS += -D USE_STDPERIPH_DRIVER -D STM32F40xx -D USE_STM32F07_DISCO

BLACKLIST = $(SPL)/src/stm32f4xx_fmc.c

_SRC = $(wildcard Src/*.c)
_SRC += $(wildcard $(CMSIS)/Device/ST/STM32F4xx/Templates/*.c)
_SRC += $(wildcard $(HAL)/Src/*.c)
_SRC += $(wildcard $(MIDDLEWARES)/STM32_USB_Host_Library/Class/CDC/Src/*.c)
_SRC += $(wildcard $(MIDDLEWARES)/STM32_USB_Host_Library/Core/Src/*.c)
SRC  = $(filter-out $(BLACKLIST), $(_SRC))

ASRC = $(STARTUP)/startup_stm32f40xx.s

OBJ = $(patsubst %.c,%.o,$(SRC))
AOBJ = $(patsubst %.s,%.o,$(ASRC))

BINFLAGS = -O binary

LINKER_SCRIPT = STM32F407VGTx_FLASH.ld

.PRECIOUS: %.c %.o

all: app.bin

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled $< successfully!"

%.o : %.s 
	$(AS) $(CFLAGS) $(AFLAGS) -c $< -o $@
	@echo "$(MODULE_NAME): Compiled "$<" successfully!"

app.bin : app.elf
	@$(BIN) $< $(BINFLAGS) $@
	@echo "$(MODULE_NAME): Generated binary successfully!"

app.elf : $(AOBJ) $(OBJ)
	@$(LD) $(AOBJ) $(OBJ) \
	$(LDFLAGS) -o $@ \
	-lc  \
	-T$(LINKER_SCRIPT)
	@echo "$(MODULE_NAME): Linked app successfully!"

clean :
	rm -f app.elf app.bin
	rm $(OBJ)

burn: app.bin
	$(STLINK)/st-flash write app.bin 0x8000000