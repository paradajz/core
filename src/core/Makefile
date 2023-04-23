ifeq ($(shell uname), Linux)
    FIND      := find
    SHA256SUM := sha256sum
else ifeq ($(shell uname), Darwin)
    FIND      := gfind
    SHA256SUM := gsha256sum
else
    $(error Unsupported platform)
endif

ROOT_MAKEFILE_DIR         := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
FIND                      := $(FIND) 2>/dev/null
SCRIPTS_DIR               := $(ROOT_MAKEFILE_DIR)/../../scripts
MCU                       := $(shell basename $(shell $(FIND) $(ROOT_MAKEFILE_DIR)/arch -type f -name "*.yml" | sort | head -n 1) .yml)
MCU_YML_FILE              := $(shell find $(ROOT_MAKEFILE_DIR) -type f -name "*$(MCU).yml")
MCU_GENERATED_SUBDIR_BASE := generated
MCU_GEN_DIR               := $(ROOT_MAKEFILE_DIR)/$(MCU_GENERATED_SUBDIR_BASE)/$(MCU)
MCU_EXT_CLOCK_MHZ         :=
MCU_GEN_USB               := 0

ifeq ($(MCU_YML_FILE),)
    $(error YML config file not found for specified MCU or MCU unspecified)
endif

mcu:
	@$(SCRIPTS_DIR)/gen_mcu.sh \
	--yaml-file=$(MCU_YML_FILE) \
	--gen-dir=$(MCU_GEN_DIR) \
	--external-freq=$(MCU_EXT_CLOCK_MHZ) \
	--gen-usb=$(MCU_GEN_USB)

# Debugging
print-%:
	@echo '$($*)'

.PHONY: mcu