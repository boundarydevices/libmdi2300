# O (Output) is the build directory
ifeq '$O' ''
O = .
endif
# V (Verbosity) is 0 (quiet) or 1 (verbose)
ifeq '$V' ''
V = 0
endif
# D (Debug) is 0 (optimize) or 1 (build debug version)
ifeq '$D' ''
D = 0
endif

# files
INC = include/mdi2300.h
SRC_LIB = $(wildcard src/*.c)
SRC_BIN = $(wildcard examples/*.c)
OBJS = $(OBJ_LIB) $(OBJ_BIN)
OBJ_LIB = $(SRC_LIB:%.c=$O/%.o)
OBJ_BIN = $(SRC_BIN:%.c=$O/%.o)
TARGET_LIB = $O/libmdi2300.so.0
TARGET_LN = $O/libmdi2300.so
TARGET_BIN = $O/mdi2300
BINS = $(TARGET_LIB) $(TARGET_LN) $(TARGET_BIN)

DESTDIR ?= /usr

# options
override CPPFLAGS += -Iinclude/
override CFLAGS += -fPIC -Wall -Wextra -Wno-unused-parameter -Wno-unused-result -Werror
override LDFLAGS +=

# first rule (default)
all:

# rules verbosity
ifneq '$V' '0'
P = @ true
E =
else
P = @ echo
E = @
endif

# rules
directories :
	$P '  MKDIR'
	$E mkdir -p $O/src
	$E mkdir -p $O/examples

$O/%.o : %.c directories
	$P '  CC      $(@F)'
	$E $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(TARGET_LIB) : $(OBJ_LIB)
	$P '  LD      $(@F)'
	$E $(CC) -shared $(LINKERFLAG) $(LDFLAGS) $^ -o $@

$(TARGET_LN): $(TARGET_LIB)
	$P '  LN      $(notdir $@)'
	$E - ln -fs $(<F) $@

$(TARGET_BIN): $(OBJ_BIN)
	$P '  LD      $(@F)'
	$E $(CC) $(LDFLAGS) $^ -L$O -lmdi2300 -o $@

.PHONY : all
all : $(TARGET_LN)

examples : $(TARGET_BIN)

.PHONY: clean
clean:
	$P '  RM      objs bins'
	$E rm -f $(OBJS) $(BINS)

.PHONY: install
install: all
	$P '  INSTALL DIRS'
	$E mkdir -p $(DESTDIR)/include
	$E mkdir -p $(DESTDIR)/lib/pkgconfig
	$P '  INSTALL FILES'
	$E install $(TARGET_LIB) $(DESTDIR)/lib
	$E install $(INC) $(DESTDIR)/include
	$E install -m644 libmdi2300.pc $(DESTDIR)/lib/pkgconfig
	$E cp -R $(TARGET_LN) $(DESTDIR)/lib

.PHONY: examples_install
examples_install: examples
	$P '  INSTALL DIRS'
	$E mkdir -p $(DESTDIR)/bin
	$P '  INSTALL FILES'
	$E install $(TARGET_BIN) $(DESTDIR)/bin

.PHONY: uninstall
uninstall:
	$P '  UNINSTALL'
	$E rm -f $(DESTDIR)/lib/pkgconfig/libmdi2300.pc
	$E rm -f $(DESTDIR)/lib/$(notdir $(TARGET_LN))
	$E rm -f $(DESTDIR)/lib/$(notdir $(TARGET_LIB))
	$E rm -f $(DESTDIR)/bin/$(notdir $(TARGET_BIN))
	$E rm -f $(DESTDIR)/include/$(notdir $(INC))
	$E rm -fr $(DESTDIR)/share/doc/libmdi2300

.PHONY: help
help:
	@echo
	@echo make [D=1] [V=1] [O=path]
	@echo "   D=1: build debug version (default: D=0)"
	@echo "   V=1: verbose output (default: V=0)"
	@echo "   O=path: build binary in path (default: O=.)"
	@echo

