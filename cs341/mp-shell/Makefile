# directory to store object files
OBJS_DIR = .objs

# define the EXES
EXE_SHELL=shell
EXES_STUDENT=$(EXE_SHELL)

OBJS_SHELL=$(EXE_SHELL).o format.o $(EXE_SHELL)_main.o
OBJS_TEST=$(EXE_SHELL).o format.o $(shell find tests/ -name '*.c' | sed 's/\.c$$/.o/g')

# set up compiler
CC = clang
INCLUDES=-I./includes/
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter -Wmissing-declarations -Wmissing-variable-declarations
CFLAGS_COMMON = $(INCLUDES) $(WARNINGS) -std=c99 -c -MMD -MP -D_GNU_SOURCE
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g -DDEBUG

# set up linker
LD = clang
PROVIDED_LIBRARIES:=$(shell find libs/ -type f -name '*.a' 2>/dev/null)
PROVIDED_LIBRARIES:=$(PROVIDED_LIBRARIES:libs/lib%.a=%)
LDFLAGS = -Llibs/ $(foreach lib,$(PROVIDED_LIBRARIES),-l$(lib)) -lm

# the string in grep must appear in the hostname, otherwise the Makefile will
# not allow the assignment to compile
IS_VM=$(shell hostname | grep "cs341")
VM_OVERRIDE=$(shell echo $$HOSTNAME)
ifeq ($(IS_VM),)
ifneq ($(VM_OVERRIDE),cs241grader)
$(error This assignment must be compiled on the CS341 VMs)
endif
endif

.PHONY: all
all: release

# build types
# run clean before building debug so that all of the release executables
# disappear
.PHONY: release
.PHONY: debug
.PHONY: test

release: $(EXES_STUDENT)
debug: clean $(EXES_STUDENT:%=%-debug)
test: clean $(EXES_STUDENT:%=%-test)

# include dependencies
-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

$(OBJS_DIR)/%-test.o: %.c | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

# exes
$(EXE_SHELL)-debug: $(OBJS_SHELL:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_SHELL): $(OBJS_SHELL:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_SHELL)-test: $(OBJS_TEST:%.o=$(OBJS_DIR)/%-test.o)
	$(LD) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf .objs $(EXES_STUDENT) $(EXES_STUDENT:%=%-debug)
