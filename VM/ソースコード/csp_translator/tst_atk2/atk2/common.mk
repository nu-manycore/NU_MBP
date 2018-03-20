# -*- mode: makefile-gmake; coding: utf-8-unix; -*-

#
# 共通Makefile
#

# common.mk,src,include,toolsがあるトップディレクトリを指定する。
# ターゲットのMakefileで、commin.mkをincludeする前に設定しておく。
# この設定はデフォルト(../atk2)
TOP_DIR ?= ../atk2

# Windows環境では .exe を指定することになる(Windows環境では動作未確認)
# ターゲットのMakefileで、commin.mkをincludeする前に設定しておく。
# この設定はデフォルト(なし)
EXE_POSTFIX ?=

# ABREXのYAMLファイル
# ターゲットのMakefileで、commin.mkをincludeする前に設定しておく。
# この設定はデフォルト(testout.yaml)
ATK2_ABREX_YAML ?= testout.yaml

# CSPトランスレータの生成ファイル
# ターゲットのMakefileで、commin.mkをincludeする前に設定しておく。
# この設定はデフォルト(testout.c)
ATK2_SRCS ?= testout.c

# 実行プログラム名(ファイル名ではなく、makeのターゲット)
# 実際のファイル名は ATK2_PROGRAM_FILE で設定される
# ターゲットのMakefileで、commin.mkをincludeする前に設定しておく。
# この設定はデフォルト(testout)
ATK2_PROGRAM ?= testout

# 実行トレース用の生成ヘッダファイル名指定
# ターゲットのMakefileで、commin.mkをincludeする前に設定しておく。
# この設定はデフォルト(testout_tick.h)
ATK2_TRACE_HDR ?= testout_trace.h

# main関数のソース
# この設定はデフォルト (ambp_main.c)
ATK2_SRC_MAIN ?= ambp_main.c

# その他のソース
# この設定はデフォルト (counter_handler.c)
ATK2_SRC_OTHER ?= counter_handler.c

#
#
# 以下は特に変更は不要であるが、初期値の設定が可能なものがある
#
# 設定可能なものは以下:
#
#    ATK2_SRCS: コンパイル対象のファイルを追加
#

# 実行プログラムのファイル名
ATK2_PROGRAM_FILE = $(ATK2_PROGRAM)$(EXE_POSTFIX)

# include, src, tools の各ディレクトリ
ATK2_INC_DIR = $(TOP_DIR)/include
ATK2_SRC_DIR = $(TOP_DIR)/src
ATK2_TOOL_DIR = $(TOP_DIR)/tools
ATK2_STATIC_DIR = $(TOP_DIR)/static
ATK2_DEBUG_DIR = $(TOP_DIR)/support

# $(ATK2_ABREX) が生成するコンフィグレーションファイル
ATK2_CFG_HDR = Os_Lcfg.h
ATK2_CFG_SRC = Os_Lcfg.c
ATK2_CFG_FILES = $(ATK2_CFG_SRC) $(ATK2_CFG_HDR)

# コンパイル対象のファイル
ATK2_SRCS += Os_Lcfg.c os_wrapper.c $(ATK2_SRC_MAIN) $(ATK2_SRC_OTHER)
ATK2_OBJS = $(ATK2_SRCS:.c=.o)
ATK2_DEPS = $(ATK2_SRCS:.c=.d)

# YAMLを処理してコンフィグレーションファイルを生成する
ATK2_ABREX = $(ATK2_TOOL_DIR)/abrexyaml2pthread-c.pl

# testout_tick.h によるタスクの起床・スリープ時刻のトレース出力を解析する
ATK2_TRACECHECK = $(ATK2_TOOL_DIR)/tracecheck.pl

# phony
ATK2_PHONY_TARGETS += all abrex program clean realclean distclean depend

# clean対象ファイル
ATK2_CLEAN_FILES += $(ATK2_PROGRAM_FILE) $(ATK2_OBJS)
ATK2_DISTCLEAN_FILES += $(ATK2_CFG_FILES) $(ATK2_TRACE_HDR) $(ATK2_DEPS)

CPPFLAGS += -I. -I$(ATK2_STATIC_DIR) -I$(ATK2_DEBUG_DIR) -I$(ATK2_INC_DIR)

# ATK2_MODEL_DIRのディレクトリをincludeするようにする
ifneq ($(ATK2_MODEL_DIR),)
CPPFLAGS += $(patsubst %,-I%,$(ATK2_MODEL_DIR))
endif

# pthread関連ライブラリ
LDLIBS += -lpthread -lrt

# os_wrapper.cやモデルのソース
vpath %.c $(ATK2_STATIC_DIR) $(ATK2_SRC_DIR) $(ATK2_MODEL_DIR)
vpath %.yaml $(ATK2_STATIC_DIR)

all: program

SUFFIXES += .d .yaml
.SUFFIXES: .d .yaml

.PHONY: $(ATK2_PHONY_TARGETS)

abrex: $(ATK2_CFG_FILES)
$(ATK2_CFG_FILES): basic.yaml $(ATK2_ABREX_YAML) $(ATK2_ABREX)
	$(ATK2_ABREX) $(filter %.yaml,$^)

ifneq ($(ATK2_TRACE_HDR),)
$(ATK2_TRACE_HDR): $(ATK2_ABREX_YAML) $(ATK2_ABREX)
	$(ATK2_ABREX) -n -t $(ATK2_TRACE_HDR) $(filter %.yaml,$^)
endif

program: $(ATK2_PROGRAM)

$(ATK2_PROGRAM): $(ATK2_OBJS)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f $(ATK2_CLEAN_FILES)

realclean distclean: clean
	rm -f $(ATK2_DISTCLEAN_FILES)

$(ATK2_OBJS): $(ATK2_CFG_HDR) $(ATK2_TRACE_HDR)

depend: $(ATK2_DEPS)

$(ATK2_DEPS): $(ATK2_CFG_HDR) $(ATK2_TRACE_HDR)

%.d: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM $(OUTPUT_OPTION) $<

-include $(wildcard *.d)
