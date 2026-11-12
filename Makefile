###############################################################################
# Command-line build for the LAUNCHXL-F28P55X blink LED example
#
# Requires TI C2000 Code Generation Tools (cl2000), e.g. installed with CCS:
#   <ccs_install>/ccs/tools/compiler/ti-cgt-c2000_<version>
#
# Usage:
#   make CGT_ROOT=/path/to/ti-cgt-c2000_22.6.1.LTS            # FLASH build
#   make CGT_ROOT=/path/to/ti-cgt-c2000_22.6.1.LTS CONFIG=RAM # RAM build
#   make clean
###############################################################################

CGT_ROOT ?= /opt/ti/ti-cgt-c2000_22.6.1.LTS
CONFIG   ?= FLASH

CC := $(CGT_ROOT)/bin/cl2000

BUILD_DIR := build/$(CONFIG)
TARGET    := $(BUILD_DIR)/f28p55x_blinkled.out

ifeq ($(CONFIG),FLASH)
  CFG_DEFINES := --define=_FLASH
  LINKER_CMD  := cmd/28p55x_generic_flash_lnk.cmd
else ifeq ($(CONFIG),RAM)
  CFG_DEFINES := --define=RAM
  LINKER_CMD  := cmd/28p55x_generic_ram_lnk.cmd
else
  $(error CONFIG must be FLASH or RAM)
endif

CFLAGS := -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu1 \
          --abi=eabi --opt_level=off -g --gen_func_subsections=on \
          --diag_warning=225 --diag_suppress=10063 --display_error_number \
          --define=DEBUG --define=CPU1 --define=_LAUNCHXL_F28P55X $(CFG_DEFINES) \
          --include_path=$(CGT_ROOT)/include --include_path=. \
          --include_path=device --include_path=driverlib

LDFLAGS := -z --entry_point=code_start --stack_size=0x3F8 --heap_size=0x200 \
           --reread_libs --warn_sections --rom_model $(CFG_DEFINES) \
           -i$(CGT_ROOT)/lib -m$(BUILD_DIR)/f28p55x_blinkled.map

SRCS_C   := main.c device/device.c $(wildcard driverlib/*.c)
SRCS_ASM := device/f28p55x_codestartbranch.asm

OBJS := $(addprefix $(BUILD_DIR)/,$(SRCS_C:.c=.obj) $(SRCS_ASM:.asm=.obj))

.PHONY: all clean

all: $(TARGET)

$(BUILD_DIR)/%.obj: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) --obj_directory=$(dir $@) --output_file=$@ -c $<

$(BUILD_DIR)/%.obj: %.asm
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) --obj_directory=$(dir $@) --output_file=$@ -c $<

$(TARGET): $(OBJS) $(LINKER_CMD)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LINKER_CMD) -llibc.a

clean:
	rm -rf build
