# -*- mode: makefile-gmake; coding: utf-8-unix; -*-

########################################################################
##
## 指定可能マクロ
##
## このファイルをincludeする前に次のものを定義できる。
## 説明のないものは、定義マクロを参照
##
## - NIS_TOP_DIR
## - DEP_EXT
##
## - ENABLE_DEBUG=[yn]
##
##   デバッグオプション(-g)を有効にする。デフォルトではy
##
## - USE_DMALLOC=[ny]
##
##   dmallocを使用する。デフォルトでは n
##
## - SIMULINK_XSD
## - SIMULINK_XSD_LIBNAME
## - SIMULINK_XSD_LIBFILE
## - SIMULINK_XSD_LIBPATH
## - BOOST_LIB_EXT
## - DLL_EXT
## - EXE_EXT
## - PIC_OPTION
## - CXX
## - CC
## - DOT
## - M4
## - PKG_CONFIG
## - CXXFLAGS
## - LDFLAGS
## - XSD
##
## MinGWの場合、さらに以下をあらかじめ定義できる
##
## - MINGW_XSD_DIR
##
##   Codesynthesis XSDのWindows用パッケージをインストールした場所
##
## - MINGW_XSD_INCLUDE_DIR
##
##   Codesynthesis XSDのヘッダファイルの場所
##
## - MINGW_XSD_COMMAND
##
##   Codesynthesis XSDのライブラリの場所
##

########################################################################
##
## 定義マクロ
##
## このファイルをincludeすると、次のものが定義される。
## なお、このファイル以外で使用する事を想定していないものを除く
##
## - NIS_TOP_DIR
##
##   nu-manycore/NIS の場所。デフォルトは .. の絶対パス
##
## - OS [$(NIS_TOP_DIR)/make.rules/os.mk]
##
##   OSの種別。Linux, Solaris, MINGW32_NT-6.1, MINGW64_NT-6.1 等
##
## - IS_MINGW [$(NIS_TOP_DIR)/make.rules/os.mk]
##
##   MinGWの環境では 32 または 64 が設定され、それ以外では n になる
##
## - DEP_EXT [$(NIS_TOP_DIR)/make.rules/pre.rules.mk
##
##   makeの依存関係のファイルの拡張子。デフォルトは .d
##
## - SIMULINK_XSD
##
##   Simulink_XSDのパス名
##
## - SIMULINK_XSD_LIBNAME
##
##   Simulink_XSDのライブラリ名。デフォルトは SimulinkXSD
##
## - SIMULINK_XSD_LIBFILE
##
##   Simulink_XSDのライブラリのファイル名。デフォルトは libSimulinkXSD.a
##
## - SIMULINK_XSD_LIBPATH
##
##   Simulink_XSDのライブラリのパスファイル名。
##   デフォルトは $(SIMULINK_XSD)/$(SIMULINK_XSD_LIBFILE)
##
## - SIMULINK_XSD_CPPFLAGS
##
##   Simulink_XSDの CPPFLAGS。基本的に CPPFLAGS に設定して使用する
##
## - SIMULINK_XSD_LDLIBS
##
##   Simulink_XSDのLDLIBS。基本的に LDLIBS に設定して使用する
##
## - SIMULINK_XSD_LDFLAGS
##
##   Simulink_XSDのLDFLAGS。基本的に LDFLAGS に設定して使用する
##
## - BOOST_LIBS
##
##   使用するBoostのライブラリのリスト。-lboost_regexが指定済み
##   (BOOST_LDLIBS 参照)
##
## - BOOST_LDLIBS
##
##   リンカに渡すBoostのライブラリのリスト。
##   MinGWではライブラリに-mtのポストフィクスがついているため、
##   BOOST_LDLIBS は BOOST_LIBS を元に設定される。従って、
##   BOOST_LIBSを直接使用するのではなく、BOOST_LDLIBS を使用すること
##   なお BOOST_LDLIBS は SIMULINK_XSD_LDLIBS に含まれている。
##
## - BOOST_LIB_EXT
##
##   MinGWではライブラリに-mtのポストフィクスがつく。それを保持する
##   
## - DLL_EXT
##
##   動的ライブラリの拡張子。MinGWでは .dll
##
## - EXE_EXT
##
##   実行プログラムの拡張子。MinGwでは .exe
##
## - PIC_OPTION
##
##   PICオプション。unix系は -fPIC で、MinGWではなし
##
## - CXX
##
##   C++コンパイラ。デフォルトは g++
##
## - CC
##
##   Cコンパイラ。デフォルトは gcc
##
## - DOT
##
##   graphvizのdotプログラム。デフォルトは dot
##
## - M4
##
##   GNU m4。デフォルトは m4
##
## - PKG_CONFIG
##
##   pkg-configコマンド。デフォルトは pkg-config
##
## - CXXFLAGS
##
##   C++コンパイラのコンパイラフラグ。
##
## - LDFLAGS
##
##   リンカのフラグ
##
## - XSD
##
##   Codesynthesis XSDのプログラム名
##
## - XERCES_LIBS
##
##   Xerces/C++ のライブラリ(pkg-config --libs xercec-c の結果)
##   SIMULINK_XSD_LDLIBS に含まれる。
##
## - XERCES_CFLAGS
##
##   Xerces/C++ のコンパイルフラグ(pkg-config --cflags xerces-c の結果)
##   SIMULINK_XSD_CPPFLAGS に含まれる
##
## - XERCES_LIBDIR
##
##   Xerces/C++ ライブラリのディレクトリ(pkg-config --libdir xerces-c の結果)
##
## MinGW対応での注意
##
## * mingw.conf.mk を適切に変更する
##
## * 以下が定義される
##
##   - DLL_EXT
##   - EXE_EXT
##   - PIC_OPTION
##   - BOOST_LIB_EXT
##   - MINGW_XSD_DIR
##   - MINGW_XSD_INCLUDE_DIR
##   - MINGW_XSD_COMMAND
##
## Solaris対応での注意
##
## * solaris.conf.mk を適切に変更する
##
## * 以下が定義される
##
##   - XSD_ROOT
##
##     Codesynthesis XSDのsolaris用パッケージをインストールした場所
##
##   - XSD_LIBDIR
##
##     Codesynthesis XSDのヘッダファイル、ライブラリの場所
##
########################################################################

#
# nu-manycore/NIS のトップディレクトリ
#
NIS_TOP_DIR ?= $(realpath ..)
NIS_TOP_DIR := $(strip $(NIS_TOP_DIR))

# OSの識別
include $(NIS_TOP_DIR)/make.rules/os.mk

# 共通の事前定義
include $(NIS_TOP_DIR)/make.rules/pre.rules.mk

#
# 各種オプションのデフォルト
#

# デバッグビルドをする/しない (デフォルトはデバッグビルド)
ENABLE_DEBUG ?= y

# dmallocを使用する/しない (デフォルトはdmallocを使用しない)
USE_DMALLOC ?= n

#
# Simulink_XSDのライブラリの情報
#

# Simulink_XSDのディレクトリパス
# 本来この設定は $(NIS_TOP_DIR)/make.rules/dir.map.mk にある
SIMULINK_XSD ?= $(NIS_TOP_DIR)/Simulink_XSD
SIMULINK_XSD := $(strip $(SIMULINK_XSD))

# Simulink_XSDのライブラリ名
SIMULINK_XSD_LIBNAME ?= SimulinkXSD
SIMULINK_XSD_LIBNAME:= $(strip $(SIMULINK_XSD_LIBNAME))

# Simulink_XSDのライブラリのベースファイル名
SIMULINK_XSD_LIBFILE ?= lib$(SIMULINK_XSD_LIBNAME).a
SIMULINK_XSD_LIBFILE := $(strip $(SIMULINK_XSD_LIBFILE))

# Simulink_XSDのライブラリのパス名
SIMULINK_XSD_LIBPATH ?= $(SIMULINK_XSD)/$(SIMULINK_XSD_LIBFILE)
SIMULINK_XSD_LIBPATH := $(strip $(SIMULINK_XSD_LIBPATH))

# Simulink_XSDのライブラリの追加
override SIMULINK_XSD_LDLIBS += $(SIMULINK_XSD_LIBPATH)

# Simulink_XSDのインクルードパスの追加。ただしトップディレクトリとする
override SIMULINK_XSD_CPPFLAGS += -I$(NIS_TOP_DIR)

ifneq ($(IS_MINGW),n)
include $(SIMULINK_XSD)/mingw.conf.mk
endif

BOOST_LIB_EXT ?=
DLL_EXT ?= .so
EXE_EXT ?=
PIC_OPTION ?= -fPIC

#
# 各種プログラム
#

CXX ?= g++
CC ?= gcc
DOT ?= dot
M4 ?= m4
PKG_CONFIG ?= pkg-config

#
# プログラムフラグ
#

CXXFLAGS ?= -Wall

ifeq ($(ENABLE_DEBUG),y)
override CXXFLAGS += -g
override LDFLAGS += -g
endif

CXXFLAGS_PREPROCESSING_ONLY ?= -E

#
# ビルド環境の確認
#

# GNUCCかどうか
GNU_CC := $(shell \
	$(CXX) $(CXXFLAGS_PREPROCESSING_ONLY) -dM - </dev/null 2>/dev/null |\
        awk '/__GNUC__/{v=$$3}END{if(v)print v; else exit 1}'\
        || echo n)

# __cplusplus の値の取得
CPLUSPLUS := $(strip $(shell \
	d=`mktemp -d`; \
	echo __cplusplus > $$d/t.cc; \
	$(CXX) $(CXXFLAGS_PREPROCESSING_ONLY) $$d/t.cc |\
	sed -ne 's/^\([1-9][0-9]*\)L$$/\1/p' | tail -1; \
	rm -rf $$d >/dev/null 2>&1))

# C++コンパイラがC++11をサポートするかどうか
USE_CXX11 := $(strip $(shell \
	if [ $(CPLUSPLUS) -ge 201103 ];\
	then echo y; \
	else echo n; \
	fi ))

# M4がGNU m4かどうか
GNU_M4 := $(strip $(shell echo "ifdef(\`__gnu__',\`y',\`n')" | $(M4)))

# C++11 でなく GNU M4 でもない場合は処理できない
ifneq ($(USE_CXX11)$(GNU_M4),yy)
error_stop:
ifneq ($(USE_CXX11),y)
	$(error Your C++ Compiler is not support C++11)
endif
ifneq ($(GNU_M4),y)
	$(error Your m4 is not GNU m4)
endif
ERROR_STOP := y
.DEFAULT_GOAL := error_stop
NOINCLUDE_DEP_GOALS += error_stop
endif

# Codesynthesis XSDのプログラム名が xsdcxx かどうか
XSDCXX := $(strip $(shell if type xsdcxx 1>/dev/null 2>&1; \
	then echo y;\
	else echo n;\
	fi ))

# Linux
ifeq ($(OS),Linux)
ifeq ($(XSDCXX),y)
XSD ?= xsdcxx
else
XSD ?= xsd
endif
endif

# Xerces-C++
XERCES_LIBS := $(shell $(PKG_CONFIG) --libs xerces-c)
XERCES_CFLAGS := $(shell $(PKG_CONFIG) --cflags xerces-c)
XERCES_LIBDIR := $(shell $(PKG_CONFIG) --variable=libdir xerces-c)

#
# CodeSynthesis XSD の設定
#

# MinGW (MSYS2)
ifneq ($(IS_MINGW),n)
XSD ?= $(MINGW_XSD_COMMAND)
ifdef MINGW_XSD_INCLUDE_DIR
override SIMULINK_XSD_CPPFLAGS += -idirafter $(MINGW_XSD_INCLUDE_DIR)
endif
endif

# Solaris
ifeq ($(OS),SunOS)
include $(SIMULINK_XSD)/solaris.conf.mk
XSD_ROOT := $(strip $(XSD_ROOT))
XSD ?= $(XSD_ROOT)/bin/xsd
XSD_LIBDIR ?= $(XSD_ROOT)/libxsd
override SIMULINK_XSD_CPPFLAGS += -idirafter $(XSD_LIBDIR)
endif

# Boostのインクルードパスとライブラリの追加
override SIMULINK_XSD_CPPFLAGS += $(BOOST_CPPFLAGS)
override BOOST_LIBS += -lboost_regex
override SIMULINK_XSD_LDLIBS += $(BOOST_LDLIBS)
override SIMULINK_XSD_LDFLAGS += $(BOOST_LDFLAGS)

# XERCES-Cのインクルードパス追加
override SIMULINK_XSD_CPPFLAGS += $(XERCES_CFLAGS)
override SIMULINK_XSD_LDLIBS += $(XERCES_LIBS)

#
# 使用するsuffixの追加
#
SUFFIXES += .xsd .xml .yaml .m4
ifneq ($(DLL_EXT),)
override SUFFIXES += $(DLL_EXT)
endif

#
# SunOS用
#
ifeq ($(OS),SunOS)
ifneq ($(GNU_CC),n)
GCC_LIBDIR := $(realpath $(dir $(shell $(CXX) -print-file-name=libstdc++.so)))
ifneq ($(GCC_LIBDIR),$(XERCES_LIBDIR))
override PRIOR_LDFLAGS += -L$(GCC_LIBDIR)
override PRIOR_RPATH_DIRS += $(GCC_LIBDIR)
endif
override POSTERIOR_RPATH_DIRS += $(XERCES_LIBDIR)

# 優先/非優先のパスに含まれているものを取り除くフィルタ
# 通常このフィルタを経たものを POSTERIOR_RPATH_DIRS に追加する
# (その際には POSTERIOR_LDFLAGS にも追加する必要がある)
rpath_filter = $(filter-out $(PRIOR_RPATH_DIRS) $(POSTERIOR_RPATH_DIRS),$(1))

# リンカに渡す-rpathオプションを作るマクロ
make_rpath = $(foreach d,$(1),-Wl,-rpath,$(d))

override SIMULINK_XSD_LDFLAGS += $(PRIOR_LDFLAGS) \
	$(call make_rpath,$(PRIOR_RPATH_DIRS)) \
	$(POSTERIOR_LDFLAGS) \
	$(call make_rpath,$(POSTERIOR_RPATH_DIRS))
endif
endif
