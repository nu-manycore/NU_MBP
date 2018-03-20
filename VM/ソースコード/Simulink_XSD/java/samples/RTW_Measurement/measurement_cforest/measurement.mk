CC = v850-elf-gcc
NM = v850-elf-nm
AS = v850-elf-gcc
OBJCOPY = v850-elf-objcopy

MACHINE = -mv850e2v4

# PLEASE CHANGE user.mk (AUX_SRC_DIR,AUX_C_SRCS,AUX_S_SRCS,TARGET_SRC)
include $(PWD)/user.mk

# depend sub/gdb-command.x
TARGET_NAME = $(PWD)/measure
TARGET_HEX = $(TARGET_NAME).hex

C_SRCS = $(TARGET_SRC)
C_SRCS += hwdep.c
C_SRCS += $(AUX_C_SRCS)

S_SRCS += boot.s
S_SRCS += $(AUX_S_SRCS)

C_DEPS = $(C_SRCS:%.c=$(PWD)/%.d)

vpath %.c $(PWD) lib $(AUX_SRC_DIR)
vpath %.s $(PWD) lib $(AUX_SRC_DIR)
vpath %.h $(PWD) lib $(AUX_SRC_DIR)

LDSCRIPT = lib/sample.ld

OBJS = $(addprefix $(PWD)/,$(C_SRCS:.c=.o) $(S_SRCS:.s=.o))

CPPFLAGS += -I. -Ilib -Iinclude $(addprefix -I,$(AUX_SRC_DIR)) -DCFOREST=1
CFLAGS += -g $(MACHINE)
ASFLAGS += -g $(MACHINE) -c
LDFLAGS += -g $(MACHINE) -Xlinker -T$(LDSCRIPT)

MAP_NAME = $(TARGET_NAME).map

LDLIBS += -lm

COPY_FILE = $(PWD)/MyEnv.sh $(PWD)/gdb.sh $(PWD)/cforest.sh $(PWD)/run.sh

.PHONY: all build clean depend

all: build

build: $(TARGET_HEX)

$(TARGET_HEX): $(TARGET_NAME)
	$(OBJCOPY) $(TARGET_NAME) $(TARGET_HEX) -O srec
$(MAP_NAME): $(TARGET_NAME)
	$(NM) -n $(TARGET_NAME) > $(MAP_NAME)
$(TARGET_NAME): $(LDSCRIPT)
$(TARGET_NAME): $(OBJS)
	$(LINK.o) $(filter %.o,$^) $(LOADLIBES) $(LDLIBS) -o $@

clean:
	@-rm -f $(TARGET_NAME) $(MAP_NAME) $(TARGET_HEX) $(OBJS)

distclean: clean
	@-rm -f $(C_DEPS)
ifneq ($(PWD),$(CURDIR))
	@-rm -f $(COPY_FILE)
endif

depend: $(C_DEPS)

ifeq ($(PWD),$(CURDIR))
copy:
else
copy: $(COPY_FILE)
$(PWD)/MyEnv.sh: $(CURDIR)/MyEnv.sh
	cp $(CURDIR)/MyEnv.sh $(PWD)/MyEnv.sh;
	echo MEASUREMENT_TOOL_DIR=$(CURDIR) >> $(PWD)/MyEnv.sh;
$(PWD)/%.sh: $(CURDIR)/%.sh
	ln -s $< $@
endif

$(PWD)/%.d: %.c
	$(COMPILE.c) -MM $(OUTPUT_OPTION) $<
$(PWD)/%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<
$(PWD)/%.o: %.s
	$(COMPILE.s) $(OUTPUT_OPTION) $<

-include $(C_DEPS)
