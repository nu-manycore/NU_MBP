# -*- mode: makefile-gmake; coding: utf-8-unix; -*-

# file suffixes
DLL_EXT ?= .dll
EXE_EXT ?= .exe

# cc/c++ pic option (not need)
PIC_OPTION ?=

# Boost library extension
BOOST_LIB_EXT ?= -mt

# Codesynthesis XSD
#   - you should change or define MINGW_XSD_DIR for your environment
MINGW_XSD_DIR ?= /c/ICF_AutoCapsule_disabled/xsd-4.0.0-i686-windows
MINGW_XSD_INCLUDE_DIR ?= $(MINGW_XSD_DIR)/libxsd
MINGW_XSD_COMMAND ?= $(MINGW_XSD_DIR)/bin/xsd
