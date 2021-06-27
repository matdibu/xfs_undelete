OBJECTS = xfs_exceptions.o xfs_inode.o xfs_extent.o xfs_inode_entry.o xfs_undelete.o xfs_parser.o
TARGET = lib_xfs_undelete.so
COMP ?= g++
CONFIG ?= debug
COMP_FLAGS ?= -O0 -fsanitize=address -Wall -Wextra -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-weak-vtables -fPIC -std=c++20 -Wno-reserved-id-macro
override CDEFINES := $(CDEFINES) -DPAN_LOG_LEVEL_DEBUG
ARCHV ?= $(shell getconf LONG_BIT)
OUTDIR = ../bin/$(ARCHV)/$(CONFIG)

all : $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(COMP) $^ $(COMP_FLAGS) -shared -fvisibility=hidden -o $(OUTDIR)/$(TARGET)  -ldl

clean:
	$(RM) $(OBJECTS)

%.o: %.cpp
	$(COMP) $(COMP_FLAGS) $(CDEFINES) $(INCLUDES) -c $<
