OBJECTS = xfs_exceptions.o xfs_inode.o xfs_extent.o xfs_inode_entry.o xfs_parser.o linux_file.o
TARGET = lib_xfs_undelete.so
COMP ?= clang++
CONFIG ?= debug
COMP_FLAGS ?= -g -Og -fsanitize=address -Weverything -Wall -Wextra -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-weak-vtables -fPIC -std=c++17 -Wno-reserved-id-macro
override CDEFINES := $(CDEFINES) -DSPDLOG_FMT_EXTERNAL
ARCHV ?= $(shell getconf LONG_BIT)
OUTDIR = ../bin/$(ARCHV)/$(CONFIG)


all : $(OBJECTS)
	mkdir -p $(OUTDIR)
	# $(COMP) $^ $(COMP_FLAGS) -shared -fvisibility=hidden -o $(OUTDIR)/$(TARGET)  -ldl -lasan
	$(COMP) $^ $(COMP_FLAGS) -shared -o $(OUTDIR)/$(TARGET)  -ldl -lasan

clean:
	$(RM) $(OBJECTS)

%.o: %.cpp
	$(COMP) $(COMP_FLAGS) $(CDEFINES) $(INCLUDES) -c $<
