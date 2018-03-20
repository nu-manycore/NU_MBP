DOT = dot
DOT_PNG = $(DOT) -Tpng

#TARGET_GENERATOR=kalray
TARGET_GENERATOR=mcos

TEST_TARGET=pid
#TEST_TARGET=JIKKEN
#TEST_TARGET=synrmmodel_mt_RTW

ifeq ($(TARGET_GENERATOR),kalray)
CODE_SUFFIX = .sc
else
ifeq ($(TARGET_GENERATOR),mcos)
CODE_SUFFIX = .c
else
CODE_SUFFIX = .xc
endif
endif

ifeq ($(TEST_TARGET),synrmmodel_mt_RTW)
#SAMPLE_BASE = $(SIMULINK_XSD)/0911/synrmmodel_mt_RTW_ert_rtw_140911
#SAMPLE_BASE = $(SIMULINK_XSD)/0925
SAMPLE_BASE = pe_test
SAMPLE_OPTIONS = 
else
ifeq ($(TEST_TARGET),JIKKEN)
SAMPLE_BASE = $(SIMULINK_XSD)/temp
SAMPLE_OPTIONS = 
else
SAMPLE_BASE = $(SIMULINK_XSD)/models/$(TEST_TARGET)
SAMPLE_OPTIONS =
endif
endif

SAMPLE_XML_CSP = $(SAMPLE_BASE)/yy.xml
#SAMPLE_XML_CSP = $(SAMPLE_BASE)/$(TEST_TARGET).xml
SAMPLE_XML_XSD = $(SAMPLE_BASE)/$(SIM_XML_SCHEMA)
OUTPUT_DEPEND = $(SAMPLE_XML_CSP)

TRANSLATOR_FLAGS = -v

# (must be set this option, t0 support not implemented yet)
TRANSLATOR_FLAGS += --create-t0=n

# to set `y', ignore ToWorkSpace, Terminator, Scope...
TRANSLATOR_FLAGS += --ignore-to-sink=y

# to set `y', ignore isolated block
TRANSLATOR_FLAGS += --ignore-isolated=y

# to set `y', ignore leaf blocks
# TRANSLATOR_FLAGS += --ignore-leaves=n

# depend kalray (not implemented yet for mcos)
# TRANSLATOR_FLAGS += --incorporate-io=n

# depend XMOS (no effect for kalray and mcos)
TRANSLATOR_FLAGS += --unshare-stat

# Defaults Values (should not change)
# TRANSLATOR_FLAGS += --add-timer=0
# TRANSLATOR_FLAGS += --input-multirate=n
# TRANSLATOR_FLAGS += --subsystem-multirate=n

TRANSLATOR_FLAGS += --xsd-validation

TRANSLATOR_TESTOUT = testout.dot
TRANSLATOR_TESTOUT_PNG = $(TRANSLATOR_TESTOUT:.dot=.png)
TRANSLATOR_TESTOUT_CSV = $(TRANSLATOR_TESTOUT:.dot=.csv)
TRANSLATOR_TESTOUT_LOG = $(TRANSLATOR_TESTOUT:.dot=.log)
TRANSLATOR_TESTOUT_CODE = $(TRANSLATOR_TESTOUT:.dot=$(CODE_SUFFIX))

TRANSLATOR_TESTOUT_FILES = $(TRANSLATOR_TESTOUT) $(TRANSLATOR_TESTOUT_PNG) \
	$(TRANSLATOR_TESTOUT_CSV) $(TRANSLATOR_TESTOUT_LOG) \
	$(TRANSLATOR_TESTOUT_CODE)

conv: prepare_translate $(TRANSLATOR_TESTOUT)

$(TRANSLATOR_TESTOUT_CODE) $(TRANSLATOR_TESTOUT):	$(TRANSLATOR_PROG) \
							$(OUTPUT_DEPEND)
ifeq ($(TARGET_GENERATOR),kalray)
	./$(TRANSLATOR_PROG) $(TRANSLATOR_FLAGS) \
		--target=sc \
		$(SAMPLE_OPTIONS) \
		--output-csp $(TRANSLATOR_TESTOUT) \
		--output-var $(TRANSLATOR_TESTOUT_CSV) \
		--output-code $(TRANSLATOR_TESTOUT_CODE) \
		--xml-csp $(SAMPLE_XML_CSP) \
		> $(TRANSLATOR_TESTOUT_LOG)
else
ifeq ($(TARGET_GENERATOR),mcos)
	./$(TRANSLATOR_PROG) $(TRANSLATOR_FLAGS) \
		--target=mcos \
		$(SAMPLE_OPTIONS) \
		--output-csp $(TRANSLATOR_TESTOUT) \
		--output-var $(TRANSLATOR_TESTOUT_CSV) \
		--output-code $(TRANSLATOR_TESTOUT_CODE) \
		--xml-csp $(SAMPLE_XML_CSP) \
		> $(TRANSLATOR_TESTOUT_LOG)
else
	./$(TRANSLATOR_PROG) $(TRANSLATOR_FLAGS) \
		--target=xc -N 64 \
		$(SAMPLE_OPTIONS) \
		--output-csp $(TRANSLATOR_TESTOUT) \
		--output-var $(TRANSLATOR_TESTOUT_CSV) \
		--output-code $(TRANSLATOR_TESTOUT_CODE) \
		--xml-csp $(SAMPLE_XML_CSP) \
		> $(TRANSLATOR_TESTOUT_LOG)
endif
endif

png: prepare_translate $(TRANSLATOR_TESTOUT_PNG)
$(TRANSLATOR_TESTOUT_PNG): $(TRANSLATOR_TESTOUT)
	$(DOT_PNG) -o $(TRANSLATOR_TESTOUT_PNG) $(TRANSLATOR_TESTOUT)

prepare_translate: $(SAMPLE_XML_XSD)
$(SAMPLE_XML_XSD): $(SIMULINK_XSD)/$(SIM_XML_SCHEMA)
	if test -L $@ -o -r $@; \
	then \
		if cmp -s $^ $@; \
		then :;\
		else \
			rm -f $@; \
			( dir=`dirname $@`; cd $$dir; \
			for i in .. ../.. ../../.. ../../../..; \
			do \
				if test -r $$i/$^; \
					then ln -s $$i/$^ .; \
				fi; \
			done \
			) \
		fi; \
	else \
		( dir=`dirname $@`; cd $$dir; \
		for i in .. ../.. ../../.. ../../../..; \
		do \
			if test -r $$i/$^; \
				then ln -s $$i/$^ .; \
			fi; \
		done \
		) \
	fi

TEST_CONF_SUFFIXES = $(CODE_SUFFIX)
TEST_CONF_PHONY = conv png prepare_translate

CLEAN_FILES += $(TRANSLATOR_TESTOUT_FILES)
