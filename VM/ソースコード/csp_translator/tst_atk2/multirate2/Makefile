# -*- mode: makefile-gmake; coding: utf-8-unix; -*-

MULTIRATE2_DIR = ../../../nakano-file/multirate2

#
# Makefileのテンプレート
#

CC = gcc
CFLAGS = -g -Wall # -O2
LDFLAGS = -g -Wall

# common.mk,src,include,toolsがあるトップディレクトリを指定する。
# 未定義の場合 ../atk2 になるが、そもそも common.mk をインクルードする
# のが面倒なはず
TOP_DIR = ../atk2

# カレントディレクトリに、Embedded Coderが生成したファイルがないなら、
# Embedded Coderが生成したモデルのコードのパスを指定する
# (これを元にインクルードパスの追加やソースを探すvpath設定が行われる)
#
ATK2_MODEL_DIR = $(MULTIRATE2_DIR)/multirate2_ert_rtw

# Windows環境では .exe を指定することになる(Windows環境では動作未確認)
#
# EXE_POSTFIX =

# ABREXのYAMLファイル(未定義の場合 testout.yaml になる)
#
ATK2_ABREX_YAML = multirate2-app.yaml

# ATK2のmain関数のソースファイルの指定
# (未定義の場合は ambp_main.c になる)
#
# ATK2_SRC_MAIN =

# その他のATK2のソース(未定義の場合は counter_handler.c になる)
#
# ATK2_SRC_OTHER =

# CSPトランスレータが生成したソースと、モデルに必要なソースがあれば追加
# する(未定義の場合 testout.c になる)
#
ATK2_SRCS = multirate2-output.c

# 実行プログラム名。ただしファイル名ではなく、makeのターゲットである。
# (未定義の場合は testout)
#
# ATK2_PROGRAM =

# 実行トレース用の生成ヘッダファイル名指定(未定義の場合 testout_trace.h)
# (空にすると生成されない)
#
# ATK2_TRACE_HDR =

# distclean/realclean で削除するファイルの追加
# (未定義の場合 Os_Lcfg.[ch], $(ATK2_TRACE_HDR), *.d)
#
ATK2_DISTCLEAN_FILES += $(GEN_CODE)

# makeのデフォルトのターゲットがall以外であれば、ここにターゲットを置くこと。
# さもなければ all が使われる。
#
# all:

include emulate-def.mk
include $(TOP_DIR)/common.mk
include emulate-rule.mk
