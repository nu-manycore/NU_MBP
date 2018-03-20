# -*- mode: makefile-gmake; coding: utf-8-unix; -*-

########################################################################
##
## このファイルはSimulink_XSDのライブラリを使うためのルールが定義される
##
## - dmallocを使用する場合のコンパイルフラグとライブラリの設定
##
## - BOOST_LIBS から BOOST_LDLIBS を設定する処理
##
## - LINK.o の置き換え
##
## - .xsd から .cxx と .hxx を作るためのルール
##   (他のXSDを使用するとくに使用するが、基本的には不要)
##
## - 動的ライブラリを作るためのルール
##   (基本的には不要)
##
## - 最後に $(NIS_TOP_DIR)/make.rules/post.rules.mk をインクルードし、
##   各種コンパイルルール、depend, clean等のコマンドを使用可能にする
##
########################################################################

ifeq ($(USE_DMALLOC),y)
override CPPFLAGS += -DUSE_DMALLOC
override LDLIBS += -ldmallocxx -ldmalloc
endif

BOOST_LDLIBS = $(foreach lib,$(BOOST_LIBS),$(lib)$(BOOST_LIB_EXT))

LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

%.cxx %.hxx: %.xsd
	$(XSD) cxx-tree $(XSD_OPTIONS) $<

%$(DLL_EXT): %.o
	$(LINK.o) -shared $^ $(OUTPUT_OPTION) $(LDLIBS)

include $(NIS_TOP_DIR)/make.rules/post.rules.mk
