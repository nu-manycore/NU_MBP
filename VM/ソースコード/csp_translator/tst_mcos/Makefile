#TARGET=c_gmres
#TARGET=synrmmodel
#TARGET=rate_synrm
TARGET=jikken

#RATE=10

ifdef RATE
MCOS_THREAD_MAX_CYCLE=$(shell expr 1000000 / $(RATE))
else
MCOS_THREAD_MAX_CYCLE=1000000
endif

USE_PTHREAD=y

CC = gcc
CPPFLAGS += -std=c90 -pedantic
#CPPFLAGS += -DNO_DEBUG_PRINTF

NIS_TOP = ../../../NIS
SIMULINK_XSD = $(NIS_TOP)/Simulink_XSD

ifeq ($(TARGET),c_gmres)
TARGET_DIR = $(SIMULINK_XSD)/20150313/20150313_mpc/c_gmres_ert_rtw \
	$(SIMULINK_XSD)/0911/synrmmodel_mt_RTW_ert_rtw_140911/mtr
MAIN_SRCS = testout-c_gmres.c
RTW_SRCS = rtGetInf.c rtGetNaN.c rt_nonfinite.c
CPPFLAGS += -DCHECK_USER_CYCLE -DMCOS_THREAD_MAX_CYCLE=9000
else
ifeq ($(TARGET),synrmmodel)
TARGET_DIR = $(SIMULINK_XSD)/0911/synrmmodel_mt_RTW_ert_rtw_140911/mtr\
	$(SIMULINK_XSD)/0911/synrmmodel_mt_RTW_ert_rtw_140911/synrmmodel_mt_RTW_ert_rtw
MAIN_SRCS = testout-synrmmodel.c
RTW_SRCS = conswitchy.c currentcond.c currentcony.c dIpmsmd.c dIpmsmy.c \
	degfiltery.c myconswitch_wrapper.c mycurrentcon_wrapper.c\
	mydIpmsm_wrapper.c mydegfilter_wrapper.c myobserverab_wrapper.c \
	mypositionestiab_wrapper.c myvelocitycon_wrapper.c \
	myvelocityesti_wrapper.c ovserverabd.c ovserveraby.c \
	positionestiaby.c rtGetInf.c rtGetNaN.c rt_nonfinite.c \
	rt_zcfcn.c velocitycond.c velocitycony.c velocityestid.c
CPPFLAGS += -DCHECK_USER_CYCLE -DMCOS_THREAD_MAX_CYCLE=$(MCOS_THREAD_MAX_CYCLE)
RTW_SRCS_SEQ_ADD = synrmmodel_mt_RTW_data.c
SEQ_PROG_SRC = debug-synrm.c
SEQ_PROG = $(SEQ_PROG_SRC:.c=)
else
ifeq ($(TARGET),rate_synrm)
TARGET_DIR = $(SIMULINK_XSD)/0911/synrmmodel_mt_RTW_ert_rtw_140911/mtr\
	$(SIMULINK_XSD)/0911/synrmmodel_mt_RTW_ert_rtw_140911/synrmmodel_mt_RTW_ert_rtw
MAIN_SRCS = testout-rate_synrm.c
RTW_SRCS = conswitchy.c currentcond.c currentcony.c dIpmsmd.c dIpmsmy.c \
	degfiltery.c myconswitch_wrapper.c mycurrentcon_wrapper.c\
	mydIpmsm_wrapper.c mydegfilter_wrapper.c myobserverab_wrapper.c \
	mypositionestiab_wrapper.c myvelocitycon_wrapper.c \
	myvelocityesti_wrapper.c ovserverabd.c ovserveraby.c \
	positionestiaby.c rtGetInf.c rtGetNaN.c rt_nonfinite.c \
	rt_zcfcn.c velocitycond.c velocitycony.c velocityestid.c
CPPFLAGS += -DCHECK_USER_CYCLE -DMCOS_THREAD_MAX_CYCLE=1000000
else
ifeq ($(TARGET),jikken)
TARGET_DIR = $(NIS_TOP)/yamasefu_tmp/jikken_trigger_7
MAIN_SRCS = testout-jikken_7.c
RTW_SRCS = rtGetInf.c rtGetNaN.c rt_nonfinite.c rt_zcfcn.c
CPPFLAGS += -DCHECK_USER_CYCLE -DMCOS_THREAD_MAX_CYCLE=1000000
endif
endif
endif
endif

vpath %.c . .. $(TARGET_DIR)
vpath %.h $(TARGET_DIR)

OS := $(shell uname -s)

ifeq ($(OS),MINGW64_NT-6.1)
EXE_EXT = .exe
endif
ifeq ($(OS),MINGW32_NT-6.1)
EXE_EXT = .exe
endif
ifeq ($(OS),CYGWIN_NT-6.1)
EXE_EXT = .exe
endif

CFLAGS+= -fstack-usage

COPTFLAG =
COPTFLAG += -O2
COPTFLAG += -ffloat-store
CPPFLAGS += -I. $(patsubst %,-I%,$(TARGET_DIR))
CFLAGS += -Wall -Wno-unused-but-set-variable -Wno-unused-variable -g $(COPTFLAG)
LDFLAGS += -g
LDLIBS += $(RTW_LIBS) -lm
ifeq ($(USE_PTHREAD),y)
CPPFLAGS += -DUSE_PTHREAD
LDLIBS += -lpthread
endif

# test attributes
#CPPFLAGS += -D"CORE_TASK_DATA_ATTRIBUTE(X)=__attribute__((unused))"
#CPPFLAGS += -D"CORE_TASK_CODE_ATTRIBUTE(X)=__attribute__((unused))"
#CPPFLAGS += -D"TASK_CODE_ATTRIBUTE(X)=__attribute__((unused))"
#CPPFLAGS += -D"TASK_DATA_ATTRIBUTE(X)=__attribute__((unused))"

MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_SUS = $(MAIN_SRCS:.c=.su)
PROGRAM = $(MAIN_SRCS:.c=)
PROGRAM_FILE = $(PROGRAM)$(EXE_EXT)
RTW_OBJS = $(RTW_SRCS:.c=.o)
RTW_SUS = $(RTW_SRCS:.c=.su)

RESOURCE_SRC = $(MAIN_SRCS:.c=_res.c)
RESOURCE_OBJ = $(RESOURCE_SRC:.c=.o)
RESOURCE_SUS = $(RESOURCE_SRC:.c=.su)

SUS = $(MAIN_SUS) $(RTW_SUS) $(RESOURCE_SUS)

OBJS = $(MAIN_OBJS) $(RTW_OBJS)

.PHOY: all program clean

all : program

program: $(PROGRAM)

$(PROGRAM): $(MAIN_OBJS)
$(PROGRAM): $(RTW_OBJS)

$(MAIN_OBJS): $(RESOURCE_SRC)
$(RESOURCE_SRC): $(MAIN_SRCS) mcos/genres.pl
	perl mcos/genres.pl $< > $@

seqprog: $(SEQ_PROG)
$(SEQ_PROG): $(SEQ_PROG_SRC:.c=.o) $(RTW_OBJS) $(RTW_SRCS_SEQ_ADD:.c=.o)
$(SEQ_PROG_SRC:.c=.o): CPPFLAGS += -DSEQUENCIAL_PROGRAM=1

clean:
	rm -f $(PROGRAM_FILE) $(RESOURCE_SRC) $(OBJS) $(SUS)
