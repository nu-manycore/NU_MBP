ifdef MEASUREMENT_TOOL_DIR
.PHONY: all build clean distclean depend copy

all: copy
	make -C $(MEASUREMENT_TOOL_DIR) -f measurement.mk $@

build clean distclean depend copy:
	make -C $(MEASUREMENT_TOOL_DIR) -f measurement.mk $@
endif
