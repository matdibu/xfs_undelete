TARGETS = lib client
TARGETS_CLEAN = $(addsuffix .clean, $(TARGETS))
ARCHV ?= $(shell getconf LONG_BIT)
COMP ?= g++
CONFIG ?= debug
TYPE ?= all

CDEFINES = -DSPDLOG_FMT_EXTERNAL
CURRENT_DIR = $(shell pwd)

ifeq ($(CONFIG),debug)
	COMP_FLAGS = -Og -g
	CDEFINES := "${CDEFINES} -DPAN_LOG_LEVEL_DEBUG"
else ifeq ($(CONFIG),release)
	COMP_FLAGS = -O2
	CDEFINES := "${CDEFINES} -DPAN_LOG_LEVEL_RELEASE"
else
	$(error "Unknown config!")
endif

ifeq ($(ARCHV), 32)
	COMP_FLAGS := ${COMP_FLAGS} -m32
else
	COMP_FLAGS := ${COMP_FLAGS} -m64
endif

FMT_PKG_CONFIG_PATH := "/nix/store/zc8xnz48ca61zjplxc3zz1ha3zss046p-fmt-10.2.1-dev/lib/pkgconfig"
SPDLOG_PKG_CONFIG_PATH := "/nix/store/g80avs6dxaigm3k73f6dci5ihd94xmkl-spdlog-1.13.0-dev/lib/pkgconfig"
PKGCONFIG_FLAGS := $(shell PKG_CONFIG_PATH=${FMT_PKG_CONFIG_PATH}:${SPDLOG_PKG_CONFIG_PATH} pkg-config --cflags --libs spdlog)

COMP_FLAGS := \
	${COMP_FLAGS} \
	-std=c++17 \
	-fPIC \
	${PKGCONFIG_FLAGS}

ifeq ($(COMP),clang++)
	COMP_FLAGS := ${COMP_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-reserved-id-macro -Wno-weak-vtables
else ifeq ($(COMP),g++)
	COMP_FLAGS := ${COMP_FLAGS} -Werror -Wextra -Wno-unknown-pragmas
else
	$(error "Unknown compiler!")
endif

.PHONY: all $(TARGETS) $(TARGETS_CLEAN)

COMP_FLAGS := "${COMP_FLAGS}"

all: $(TARGETS) postbuild

clean: $(TARGETS_CLEAN)

postbuild:
	# rm ./scripts/debugprint.py

$(TARGETS):
	mkdir -p bin/$(ARCHV)/$(CONFIG)
	$(MAKE) -C $@ COMP=$(COMP) COMP_FLAGS=$(COMP_FLAGS) CDEFINES=$(CDEFINES) CONFIG=$(CONFIG)

$(TARGETS_CLEAN): %.clean:
	$(MAKE) -C $* clean
	rm -f -r bin/$(ARCHV)/$(CONFIG)

clangtidy:
	/usr/lib/llvm/12/share/clang/run-clang-tidy.py . > clangtidy.log

# project dependencies
client: lib
lib:

