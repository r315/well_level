
BUILD_DIR =build
APP_SRC_DIR =src
BSP_SRC_DIR =bsp/f030

TARGET =well_level
CSRCS = \
$(BSP_SRC_DIR)/board.c \
$(wildcard $(APP_SRC_DIR)/*.c) \

ASRCS = \
$(BSP_SRC_DIR)/startup_stm32f030f4px.s \

SYMBOLS = \
-DSTM32F030x6 \
-DDEBUG \

OPTIMIZATION =-O0 -g3
CPU =-mcpu=cortex-m0 -mfloat-abi=soft -mthumb
STD =-std=gnu11
OPTIONS =-ffunction-sections -fdata-sections -Wall #-fstack-usage
SPECS =--specs=nano.specs

CFLAGS =$(CPU) $(OPTIMIZATION) $(SPECS)
DEPS =-MMD -MP -MF"$<" -MT"$@"

LD_SCRIPT =$(BSP_SRC_DIR)/STM32F030F4PX_FLASH.ld

PREFIX =arm-none-eabi-
GCC =$(PREFIX)gcc
CPP =$(PREFIX)gcc
AS =$(PREFIX)gcc
LD =$(PREFIX)gcc
SIZE =$(PREFIX)size

OBJECTS =$(addprefix $(BUILD_DIR)/, $(notdir $(CSRCS:.c=.o))) $(addprefix $(BUILD_DIR)/, $(notdir $(ASRCS:.s=.o)))
INCLUDES =$(addprefix -I, $(dir $(CSRCS)))

VPATH +=$(dir $(CSRCS)) #$(dir $(ASRCS))

all: $(TARGET).elf size

size: $(TARGET).elf
	$(SIZE)	"$<" -A -x
	$(SIZE)	"$<" -B -x

clean:
	rm -rf $(BUILD_DIR) $(TARGET).elf $(TARGET).map

$(TARGET).cfg:
	@echo "Creating opencod configuration file"
	echo "interface jlink" >> $@
	echo "transport select swd" >> $@
	echo "source [find target/stm32f0x.cfg]" >> $@
	echo "adapter_khz 4000" >> $@

program: $(TARGET).elf $(TARGET).cfg
	openocd -f $(TARGET).cfg -c "program $(TARGET).elf verify reset exit"
test:
	@echo ""; $(foreach d, $(dir $(VPATH)) , echo $(d);)

$(BUILD_DIR):
	mkdir -p $@	

$(TARGET).elf : $(OBJECTS)
	$(LD) -o $@ $(OBJECTS) $(CPU) -T"$(LD_SCRIPT)" $(SPECS) --specs=nosys.specs -static -Wl,-Map="$(TARGET).map" -Wl,--gc-sections -nostdlib
#-Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(GCC) $(CFLAGS) $(INCLUDES) $(OPTIONS) -c "$<" -o "$@"

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
#	$(AS) -mcpu=cortex-m0 -mfloat-abi=soft -mthumb -g3 -c -x assembler-with-cpp --specs=nano.specs -o "$@" "$<"
	$(AS) $(CPU) -g3 -c -x assembler-with-cpp $(SPECS) -o "$@" "$<"

ifeq ($(GCC_COLORS), )
export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'
#unexport GCC_COLORS
endif
