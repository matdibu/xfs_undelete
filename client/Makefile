OBJECTS = main.o utils.o
TARGET = xfs_undelete
COMP ?= clang++
CONFIG ?= debug
ARCHV ?= $(shell getconf LONG_BIT)
OUTDIR = ../bin/${ARCHV}/${CONFIG}

ifeq (${COMP_FLAGS},)
    COMP_FLAGS = -std=c++17 -fPIC

    ifeq ($(ARCHV), 32)
        COMP_FLAGS := ${COMP_FLAGS} -m32
    else ifeq ($(ARCHV), 64)
        COMP_FLAGS := ${COMP_FLAGS} -m64
    else
    	$(error "Unknown archv ("$(ARCHV)")!")
    endif
    
    ifeq ($(CONFIG), debug)
    	COMP_FLAGS := ${COMP_FLAGS} -g
    else ifeq ($(CONFIG), release)
    	COMP_FLAGS := ${COMP_FLAGS} -O2
    else
        $(error "Unknown config ("$(CONFIG)")!")
    endif
    
    ifeq ($(COMP), clang++)
    	COMP_FLAGS := ${COMP_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
    else ifeq ($(COMP), g++)
    	COMP_FLAGS := ${COMP_FLAGS} -Wall -Wextra
    else
    	$(error "Unknown comp ("$(COMP)")!")
    endif
endif

CDEFINES2 = ${CDEFINES} -DSPDLOG_FMT_EXTERNAL -I.. -lpthread -lfmt -lspdlog -ldl -lasan -l:libspdlog.so -L$(OUTDIR) -l:lib_xfs_undelete.so
# CDEFINES2 = ${CDEFINES}

LINK_FLAGS := ${LINK_FLAGS} -lpthread -lfmt -lspdlog -ldl -l:libspdlog.so -lasan -L$(OUTDIR) -l:lib_xfs_undelete.so

all: $(OBJECTS)
	mkdir -p $(OUTDIR)
	$(COMP) $(COMP_FLAGS) $(LINK_FLAGS) $(CDEFINES2) $^ -o$(OUTDIR)/$(TARGET)

clean:
	$(RM) $(OBJECTS) $(OUTDIR)/$(TARGET)
	# $(RM) -r $(OUTDIR)

%.o: %.c
	$(COMP) $(COMP_FLAGS) $(CDEFINES2) -o "$@" -c $<

%.o: %.cpp
	$(COMP) $(COMP_FLAGS) $(CDEFINES2) -o "$@" -c $<

tidy:
	/usr/share/clang/run-clang-tidy.py -header-filter=.* . > tidy.log

tidyfix:
	/usr/share/clang/run-clang-tidy.py -fix . > tidyfix.log

valgrind:
	valgrind --leak-check=full --track-origins=yes $(OUTDIR)/$(TARGET)
