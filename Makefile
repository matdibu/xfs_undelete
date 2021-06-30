TARGETS = lib client
TARGETS_CLEAN = $(addsuffix .clean, $(TARGETS))
ARCHV ?= $(shell getconf LONG_BIT)
COMP ?= clang++
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

COMP_FLAGS := ${COMP_FLAGS} -std=c++17 -fPIC

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

