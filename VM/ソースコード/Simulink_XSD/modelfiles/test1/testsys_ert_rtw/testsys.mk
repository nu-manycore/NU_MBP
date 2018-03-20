# Copyright 1994-2011 The MathWorks, Inc.
#
# File    : ert_lcc.tmf   $Revision: 1.26.4.50 $
#
# Abstract:
#       Template makefile for building a PC-based stand-alone embedded real-time 
#       version of Simulink model using generated C code and 
#			LCC compiler Version 2.4
#
#       This makefile attempts to conform to the guidelines specified in the
#       IEEE Std 1003.2-1992 (POSIX) standard. It is designed to be used
#       with GNU Make (gmake) which is located in matlabroot/bin/win32.
#
#       Note that this template is automatically customized by the build 
#       procedure to create "<model>.mk"
#
#       The following defines can be used to modify the behavior of the
#       build:
#         OPT_OPTS       - Optimization options. Default is none. To enable 
#                          debugging specify as OPT_OPTS=-g4. 
#         OPTS           - User specific compile options.
#         USER_SRCS      - Additional user sources, such as files needed by
#                          S-functions.
#         USER_INCLUDES  - Additional include paths 
#                          (i.e. USER_INCLUDES="-Iwhere-ever -Iwhere-ever2")
#                          (For Lcc, have a '/'as file seperator before the 
#                          file name instead of a '\' . 
#                          i.e.,  d:\work\proj1/myfile.c - reqd for 'gmake')
#       This template makefile is designed to be used with a system target
#       file that contains 'rtwgensettings.BuildDirSuffix' see ert.tlc


#------------------------ Macros read by make_rtw ------------------------------
#
# The following macros are read by the build procedure:
#
#  MAKECMD         - This is the command used to invoke the make utility
#  HOST            - What platform this template makefile is targeted for
#                    (i.e. PC or UNIX)
#  BUILD           - Invoke make from the build procedure (yes/no)?
#  SYS_TARGET_FILE - Name of system target file.

MAKECMD         = "%MATLAB%\bin\win32\gmake"
SHELL           = cmd
HOST            = PC
BUILD           = yes
SYS_TARGET_FILE = any
COMPILER_TOOL_CHAIN = lcc

MAKEFILE_FILESEP = /

#---------------------- Tokens expanded by make_rtw ----------------------------
#
# The following tokens, when wrapped with "|>" and "<|" are expanded by the
# build procedure.
#
#  MODEL_NAME          - Name of the Simulink block diagram
#  MODEL_MODULES       - Any additional generated source modules
#  MAKEFILE_NAME       - Name of makefile created from template makefile <model>.mk
#  MATLAB_ROOT         - Path to where MATLAB is installed.
#  MATLAB_BIN          - Path to MATLAB executable.
#  S_FUNCTIONS         - List of additional S-function modules.
#  S_FUNCTIONS_LIB     - List of S-functions libraries to link. 
#  NUMST               - Number of sample times
#  NCSTATES            - Number of continuous states
#  BUILDARGS           - Options passed in at the command line.
#  MULTITASKING        - yes (1) or no (0): Is solver mode multitasking
#  INTEGER_CODE        - yes (1) or no (0): Is generated code purely integer
#  MAT_FILE            - yes (1) or no (0): Should mat file logging be done,
#                        if 0, the generated code runs indefinitely
#  EXT_MODE            - yes (1) or no (0): Build for external mode
#  TMW_EXTMODE_TESTING - yes (1) or no (0): Build ext_test.c for external mode
#                        testing.
#  EXTMODE_TRANSPORT   - Index of transport mechanism (e.g. tcpip, serial) for extmode
#  EXTMODE_STATIC      - yes (1) or no (0): Use static instead of dynamic mem alloc.
#  EXTMODE_STATIC_SIZE - Size of static memory allocation buffer.
#  MULTI_INSTANCE_CODE - Is the generated code multi instantiable (1/0)?
#  PORTABLE_WORDSIZES  - Is this build intented for SIL simulation with portable word sizes (1/0)?
#  SHRLIBTARGET        - Is this build intended for generation of a shared library instead 
#                        of executable (1/0)?
#  MAKEFILEBUILDER_TGT - Is this build performed by the MakefileBuilder class
#                        e.g. to create a PIL executable?
#  STANDALONE_SUPPRESS_EXE - Build the standalone target but only create object code modules 
#                            and do not build an executable

MODEL                = testsys
MODULES              = testsys_data.c 
MAKEFILE             = testsys.mk
MATLAB_ROOT          = C:/Program Files/MATLAB/R2012a
ALT_MATLAB_ROOT      = C:/PROGRA~1/MATLAB/R2012a
MATLAB_BIN           = C:/Program Files/MATLAB/R2012a/bin
ALT_MATLAB_BIN       = C:/PROGRA~1/MATLAB/R2012a/bin
MASTER_ANCHOR_DIR    = 
START_DIR            = C:/Users/ryusei/Documents/MATLAB/models
S_FUNCTIONS          = 
S_FUNCTIONS_LIB      = 
NUMST                = 1
NCSTATES             = 0
BUILDARGS            =  GENERATE_REPORT=1 GENERATE_ASAP2=0
MULTITASKING         = 0
INTEGER_CODE         = 0
MAT_FILE             = 0
ONESTEPFCN           = 1
TERMFCN              = 1
B_ERTSFCN            = 0
MEXEXT               = mexw32
EXT_MODE             = 0
TMW_EXTMODE_TESTING  = 0
EXTMODE_TRANSPORT    = 0
EXTMODE_STATIC       = 0
EXTMODE_STATIC_SIZE  = 1000000
MULTI_INSTANCE_CODE  = 0
MODELREFS            = 
SHARED_SRC           = 
SHARED_SRC_DIR       = 
SHARED_BIN_DIR       = 
SHARED_LIB           = 
GEN_SAMPLE_MAIN      = 1
MEX_OPT_FILE         = -f C:/Users/ryusei/AppData/Roaming/MATHWO~1/MATLAB/R2012a/mexopts.bat
PORTABLE_WORDSIZES   = 0
SHRLIBTARGET         = 0
MAKEFILEBUILDER_TGT  = 0
STANDALONE_SUPPRESS_EXE = 0
OPTIMIZATION_FLAGS      = 
ADDITIONAL_LDFLAGS      = 

#--------------------------- Model and reference models -----------------------
MODELLIB                  = testsyslib.lib
MODELREF_LINK_LIBS        = 
MODELREF_LINK_RSPFILE     = testsys_ref.rsp
MODELREF_INC_PATH         = 
RELATIVE_PATH_TO_ANCHOR   = ..
# NONE: standalone, SIM: modelref sim, RTW: modelref coder target
MODELREF_TARGET_TYPE       = NONE
MODELREF_SFCN_SUFFIX       = _msf

#-- In the case when directory name contains space ---
ifneq ($(MATLAB_ROOT),$(ALT_MATLAB_ROOT))
MATLAB_ROOT := $(ALT_MATLAB_ROOT)
endif
ifneq ($(MATLAB_BIN),$(ALT_MATLAB_BIN))
MATLAB_BIN := $(ALT_MATLAB_BIN)
endif

#----------------------------- External mode -----------------------------------
# Uncomment -DVERBOSE to have information printed to stdout
# To add a new transport layer, see the comments in
#   <matlabroot>/toolbox/simulink/simulink/extmode_transports.m
ifeq ($(EXT_MODE),1)
  EXT_CC_OPTS = -DEXT_MODE -DWIN32 # -DVERBOSE
  ifeq ($(EXTMODE_TRANSPORT),0) #tcpip
    EXT_SRC = ext_svr.c updown.c ext_work.c rtiostream_interface.c rtiostream_tcpip.c
    EXT_LIB = $(MATLAB_ROOT)\sys\lcc\lib\wsock32.lib
  endif
  ifeq ($(EXTMODE_TRANSPORT),1) #serial_win32
    EXT_SRC  = ext_svr.c updown.c ext_work.c ext_svr_serial_transport.c
    EXT_SRC += ext_serial_pkt.c rtiostream_serial_interface.c rtiostream_serial.c
  endif
  ifeq ($(TMW_EXTMODE_TESTING),1)
    EXT_SRC     += ext_test.c
    EXT_CC_OPTS += -DTMW_EXTMODE_TESTING
  endif
  ifeq ($(EXTMODE_STATIC),1)
    EXT_SRC     += mem_mgr.c
    EXT_CC_OPTS += -DEXTMODE_STATIC -DEXTMODE_STATIC_SIZE=$(EXTMODE_STATIC_SIZE)
  endif
endif

#--------------------------- Tool Specifications -------------------------------

LCC = $(MATLAB_ROOT)\sys\lcc
include $(MATLAB_ROOT)\rtw\c\tools\lcctools.mak

# Determine if we are generating an s-function
SFCN = 0
ifeq ($(MODELREF_TARGET_TYPE),SIM)
  SFCN = 1
endif
ifeq ($(B_ERTSFCN),1)
  SFCN = 1
endif

#------------------------------ Include Path -----------------------------------

# Additional includes 

ADD_INCLUDES = \
	-I$(START_DIR)/testsys_ert_rtw \
	-I$(START_DIR) \


# see MATLAB_INCLUDES and COMPILER_INCLUDES from lcctool.mak

SHARED_INCLUDES =
ifneq ($(SHARED_SRC_DIR),)
SHARED_INCLUDES = -I$(SHARED_SRC_DIR) 
endif

INCLUDES = -I. -I$(RELATIVE_PATH_TO_ANCHOR) $(MATLAB_INCLUDES) $(ADD_INCLUDES) \
           $(COMPILER_INCLUDES) $(USER_INCLUDES) $(MODELREF_INC_PATH) \
           $(SHARED_INCLUDES)

#-------------------------------- C Flags --------------------------------------

# Optimization Options
OPT_OPTS = $(DEFAULT_OPT_OPTS)

# General User Options
OPTS =

# Compiler options, etc:
ifneq ($(OPTIMIZATION_FLAGS),)
CC_OPTS = $(OPTS) $(ANSI_OPTS) $(EXT_CC_OPTS) $(OPTIMIZATION_FLAGS)
MEX_OPT_OPTS = OPTIMFLAGS="$(OPTIMIZATION_FLAGS)"
else
CC_OPTS = $(OPT_OPTS) $(OPTS) $(ANSI_OPTS) $(EXT_CC_OPTS)
MEX_OPT_OPTS = $(OPT_OPTS)
endif

CPP_REQ_DEFINES = -DMODEL=$(MODEL) -DNUMST=$(NUMST) -DNCSTATES=$(NCSTATES) \
		  -DMAT_FILE=$(MAT_FILE) -DINTEGER_CODE=$(INTEGER_CODE) \
		  -DONESTEPFCN=$(ONESTEPFCN) -DTERMFCN=$(TERMFCN) \
		  -DHAVESTDIO -DMULTI_INSTANCE_CODE=$(MULTI_INSTANCE_CODE)

ifeq ($(MODELREF_TARGET_TYPE),SIM)
CPP_REQ_DEFINES += -DMDL_REF_SIM_TGT=1
else
CPP_REQ_DEFINES += -DMT=$(MULTITASKING)
endif

ifeq ($(PORTABLE_WORDSIZES),1)
CPP_REQ_DEFINES += -DPORTABLE_WORDSIZES
endif

CFLAGS = $(CC_OPTS) $(CPP_REQ_DEFINES) $(INCLUDES) -w -noregistrylookup 

ifeq ($(OPT_OPTS),$(DEFAULT_OPT_OPTS))
LDFLAGS = -s -L$(LIB)
else
LDFLAGS = -L$(LIB)
endif

ifneq ($(ADDITIONAL_LDFLAGS),)
MEX_LDFLAGS = LINKFLAGS="$$LINKFLAGS $(ADDITIONAL_LDFLAGS)"
else
MEX_LDFLAGS =
endif

#-------------------------- Additional Libraries ------------------------------

LIBS =


LIBS += $(EXT_LIB) $(S_FUNCTIONS_LIB)

ifeq ($(MODELREF_TARGET_TYPE),SIM)
LIBMWIPP = $(MATLAB_ROOT)\lib\win32\libippmwipt.lib
LIBS += $(LIBMWIPP)
endif

ifeq ($(SFCN),1)
LIBFIXPT = $(MATLAB_ROOT)\extern\lib\win32\lcc\libfixedpoint.lib
LIBS += $(LIBFIXPT)
endif

ifeq ($(MODELREF_TARGET_TYPE),SIM)
LIBMWMATHUTIL = $(MATLAB_ROOT)\extern\lib\win32\lcc\libmwmathutil.lib
LIBS += $(LIBMWMATHUTIL)
endif

ifeq ($(MODELREF_TARGET_TYPE),SIM)
LIBMWSL_FILEIO = $(MATLAB_ROOT)\extern\lib\win32\lcc\libmwsl_fileio.lib
LIBS += $(LIBMWSL_FILEIO)
endif


#----------------------------- Source Files ------------------------------------
ADD_SRCS =

ifeq ($(SFCN),0)
  SRCS = $(ADD_SRCS) $(S_FUNCTIONS)
  SRC_DEP =
  ifeq ($(MODELREF_TARGET_TYPE), NONE)
    ifeq ($(SHRLIBTARGET),1)
#--- Shared library target (.dll) ---
      PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)_win32.dll
      BIN_SETTING        = $(LD) $(LDFLAGS) $(ADDITIONAL_LDFLAGS) -dll -o $(PRODUCT) $(SYSLIBS) 
      BUILD_PRODUCT_TYPE = dynamically linked library
      SRCS               += $(MODULES) $(MODEL).c lcc_dll_main.c $(EXT_SRC) 
    else
      ifeq ($(MAKEFILEBUILDER_TGT),1)
        PRODUCT            = $(MODEL).exe
        BIN_SETTING        = $(LD) $(LDFLAGS) $(ADDITIONAL_LDFLAGS) -o $(PRODUCT) $(SYSLIBS) 
        BUILD_PRODUCT_TYPE = executable
        PREBUILT_OBJS       = $(MODULES:.c=.obj)
      else
        ifeq ($(STANDALONE_SUPPRESS_EXE),1)
#--- Build library only for top level model
          PRODUCT            = ObjectModulesOnly
          BUILD_PRODUCT_TYPE = object modules
          SRCS               += $(MODULES) $(MODEL).c $(EXT_SRC)
        else
#--- Stand-alone model (.exe) ---
          PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL).exe
          BIN_SETTING        = $(LD) $(LDFLAGS) $(ADDITIONAL_LDFLAGS) -o $(PRODUCT) $(SYSLIBS) 
          BUILD_PRODUCT_TYPE = executable
          SRCS               += $(MODULES) $(MODEL).c ert_main.c $(EXT_SRC)
        endif
      endif
    endif
  else
    # Model reference coder target
    SRCS               += $(MODULES) 
    PRODUCT            = $(MODELLIB)
    BUILD_PRODUCT_TYPE = library
  endif
else
  ifeq ($(MODELREF_TARGET_TYPE), SIM)
  PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)$(MODELREF_SFCN_SUFFIX).$(MEXEXT)
  RTW_SFUN_SRC       = $(MODEL)$(MODELREF_SFCN_SUFFIX).c
  SRCS               = $(MODULES) $(ADD_SRCS)
  else
  PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)_sf.$(MEXEXT)
  RTW_SFUN_SRC       = $(MODEL)_sf.c
  SRCS               = $(MODULES) $(ADD_SRCS) $(S_FUNCTIONS)
  BIN_SETTING        = $(MATLAB_BIN)\mex.bat $(MEX_OPT_OPTS) -win32 $(MEX_LDFLAGS) $(RTW_SFUN_SRC) $(MEX_OPT_FILE) $(INCLUDES) -outdir $(RELATIVE_PATH_TO_ANCHOR) 
  endif  
  BUILD_PRODUCT_TYPE = mex file
  ifeq ($(B_ERTSFCN),1)
    SRCS              += $(MODEL).c    
  endif
  SRC_DEP            = $(RTW_SFUN_SRC)
endif

USER_SRCS =

USER_OBJS       = $(USER_SRCS:.c=.obj)
LOCAL_USER_OBJS = $(notdir $(USER_OBJS))

OBJS      = $(SRCS:.c=.obj) $(USER_OBJS)
LINK_OBJS = $(SRCS:.c=.obj) $(LOCAL_USER_OBJS)

SHARED_SRC := $(wildcard $(SHARED_SRC))
SHARED_OBJS_NO_PATH = $(addsuffix .obj, $(basename $(notdir $(SHARED_SRC))))
SHARED_OBJS = $(addprefix $(join $(SHARED_BIN_DIR), /), $(SHARED_OBJS_NO_PATH))
FMT_SHARED_OBJS = $(subst /,\,$(SHARED_OBJS))

#--------------------------------- Rules ---------------------------------------
ifeq ($(MODELREF_TARGET_TYPE),NONE)
  ifeq ($(SHRLIBTARGET),1)
    $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(MODELREF_LINK_LIBS)
	$(BIN_SETTING) $(LINK_OBJS) @$(MODELREF_LINK_RSPFILE) $(SHARED_LIB) $(LIBS) $(MODEL).def
#--- Comment out the next line to retain .lib and .exp files ---
	@del $(RELATIVE_PATH_TO_ANCHOR)\$(MODEL)_win32.lib $(RELATIVE_PATH_TO_ANCHOR)\$(MODEL)_win32.exp
	@cmd /C "echo ### Created $(BUILD_PRODUCT_TYPE): $@"
  else
    ifeq ($(MAKEFILEBUILDER_TGT),1)
      $(PRODUCT) : $(PREBUILT_OBJS) $(OBJS) $(MODELLIB) $(SHARED_LIB) $(LIBS) $(SRC_DEP) $(MODELREF_LINK_LIBS)
	$(BIN_SETTING) $(PREBUILT_OBJS) $(LINK_OBJS) $(MODELLIB) @$(MODELREF_LINK_RSPFILE) $(SHARED_LIB) $(LIBS)
	@cmd /C "echo ### Created $(BUILD_PRODUCT_TYPE): $@"
    else
      ifeq ($(STANDALONE_SUPPRESS_EXE), 1)
        .PHONY: $(PRODUCT)
        $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(SRC_DEP) $(MODELREF_LINK_LIBS)
	@cmd /C "echo ### Created $(BUILD_PRODUCT_TYPE): $@"
      else
        $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(SRC_DEP) $(MODELREF_LINK_LIBS)
	$(BIN_SETTING) $(LINK_OBJS) @$(MODELREF_LINK_RSPFILE) $(SHARED_LIB) $(LIBS) 
	@cmd /C "echo ### Created $(BUILD_PRODUCT_TYPE): $@"
      endif
    endif
  endif
else
 ifeq ($(MODELREF_TARGET_TYPE),SIM)
  $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(SRC_DEP)
	@if exist $(MODELLIB) del $(MODELLIB)
	$(LIBCMD) /out:$(MODELLIB) $(LINK_OBJS)
	@cmd /C "echo ### Created $(MODELLIB)"
	$(MATLAB_BIN)\mex.bat -win32 $(MEX_LDFLAGS) -c $(RTW_SFUN_SRC) $(MEX_OPT_FILE) $(INCLUDES) 
	$(MATLAB_BIN)\mex.bat -win32 $(MEX_OPT_FILE) $(MEX_LDFLAGS) -outdir $(RELATIVE_PATH_TO_ANCHOR) $(MODEL)$(MODELREF_SFCN_SUFFIX).obj $(MODELLIB) @$(MODELREF_LINK_RSPFILE) $(SHARED_LIB) $(LIBS) 
	@cmd /C "echo ### Created $(BUILD_PRODUCT_TYPE): $@"
 else
  $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(SRC_DEP)
	@if exist $(MODELLIB) del $(MODELLIB)
	$(LIBCMD) /out:$(MODELLIB) $(LINK_OBJS)
	@cmd /C "echo ### Created $(MODELLIB)"
	@cmd /C "echo ### Created $(BUILD_PRODUCT_TYPE): $@"
 endif
endif

%.obj : %.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : %.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

# compile ert_main.c from rtw/c/ert if one was not generated
ifeq ($(GEN_SAMPLE_MAIN),0)
%.obj : $(MATLAB_ROOT)/rtw/c/ert/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<
endif

# compile lcc_dll_main.c
ifeq ($(SHRLIBTARGET),1)
%.obj : $(MATLAB_ROOT)/rtw/c/ert/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<
endif

%.obj : $(MATLAB_ROOT)/rtw/c/ert/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/rtiostream/rtiostreamtcpip/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/rtiostream/rtiostreamtcpip/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/serial/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/serial/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<



%.obj : $(MATLAB_ROOT)/rtw/c/src/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<



%.obj : $(MATLAB_ROOT)/simulink/src/%.c
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

%.obj : $(MATLAB_ROOT)/simulink/src/%.C
	$(CC) -c -Fo$(@F) $(CFLAGS) $<

# Libraries:





#----------------------------- Dependencies ------------------------------------

$(OBJS) : $(MAKEFILE) rtw_proj.tmw

$(SHARED_OBJS) : $(SHARED_BIN_DIR)/%.obj : $(SHARED_SRC_DIR)/%.c  
	$(CC) -c -Fo$@ $(CFLAGS) $<

$(SHARED_LIB) : $(SHARED_OBJS)
	@cmd /C "echo ### Creating $@"
	@if exist $@ del $@
	$(LIBCMD) /out:$@ $(FMT_SHARED_OBJS)
	@cmd /C "echo ### $@ Created"

#--------- Miscellaneous rules to purge, clean and lint (sol2 only) ------------

purge : clean
	@cmd /C "echo ### Deleting the generated source code for $(MODEL)"
	@del $(MODEL).c $(MODEL).h $(MODEL)_types.h $(MODEL)_data.c $(MODEL).rtw \
	        $(MODEL)_private.h $(MODULES) rtw_proj.tmw $(MAKEFILE)

clean :
	@cmd /C "echo ### Deleting the objects and $(PROGRAM)"
	@del $(LINK_OBJS) ..\$(MODEL).exe

# EOF: ert_lcc.tmf
