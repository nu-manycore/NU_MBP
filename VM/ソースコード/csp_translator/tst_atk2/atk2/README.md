<!-- -*- mode: markdown; coding: utf-8-unix; -*- -->
<style type="text/css">
table { border-collapse: collapse; }
table, th, tr, td { border: 1px solid black; }
code { color: darkgreen; }
pre { background-color: gainsboro; }
</style>

# CSPトランスレータ生成コードのTOPPERS/ATK2用pthreadエミュレーション環境

# はじめに

これは**CSPトランスレータ**の
__TOPPERS/ATK2__
用コード生成に対応した**pthreadエミュレーション環境**です。

**pthreadエミュレーション環境**は
__TOPPERS/ATK2__
のAPIを標準的な **pthread** ライブラリを用いてエミュレーションし、
実行する環境を提供します。

ここでは以下について説明します。

- [利用方法](#Usage)
    - [Makefile.tmpl](#MakefileTmpl)
	- [プログラムの実行](#ExecuteProgram)
- その他ファイル
	- [common.mk](#CommonMk)
	- include  
	  エミュレーション向けTOPPERS/ATK2のヘッダーファイル  
	  *Os.h* および *t\_syslog.h* 以外は、ほぼ空のファイル
		- *Os.h*  
		  TOPPERS/ATK2の *Os.h* のエミュレーション用ヘッダ  
		  各種マクロ、APIラッパー用のマクロを含む
		- *prc\_sil.h*  
		  TOPPERS/ATK2の *Os.h* のエミュレーション用ヘッダ  
		  使用する関数をダミーかするマクロを含む
		- *t\_stdlib.h*  
		  TOPPERS/ATK2の *t\_stdlib.h* のエミュレーション用ヘッダ  
		  stdlib.hをインクルードする
		- *t\_syslog.h*
		  TOPPERS/ATK2の *t\_syslog.h* のエミュレーション用ヘッダ  
		  LOGレベルのマクロを含む
		- *target\_hw\_counter.h*  
		  TOPPERS/ATK2の *target\_hw\_counter.h* のエミュレーション用ヘッダ  
		  実態として空
		- *target\_serial.h*  
		  TOPPERS/ATK2の *target\_serial.h* のエミュレーション用ヘッダ  
		  実態として空
		- *target\_sysmod.h*  
		  TOPPERS/ATK2の *target\_sysmod.h* のエミュレーション用ヘッダ  
		  実態として空
		- *sysmod*  
		  TOPPERS/ATK2の *sysmod* のエミュレーション用ヘッダのディレクトリ
			- *banner.h*  
			  TOPPERS/ATK2の *banner.h* のエミュレーション用ヘッダ  
			  実態として空
			- *serial.h*  
			  TOPPERS/ATK2の *serial.h* のエミュレーション用ヘッダ  
			  実態として空
			- *syslog.h*  
			  TOPPERS/ATK2の *syslog.h* のエミュレーション用ヘッダ  
			  実態として空
	- src  
	  TOPPERS/ATK2の **pthreadエミュレーション環境** 向けAPIライブラリ
		- *os\_wrapper.c*  
		  TOPPERS/ATK2のAPIのエミュレーション
	- static  
	  **CSPトランスレータ**
	  生成コード向けに用意しているTOPPERS/ATK2用の静的なファイル  
		- *ambp\_main.c*  
		  TOPPERS/ATK2のmain関数とスタートフック
		- *aos.h*  
		  TOPPERS/ATK2のインクルードするヘッダファイルをまとめたヘッダファイル
		- *basic.yaml*  
		  自動生成に無関係な、静的なABREXの設定ファイル
		- *counter\_handler.c*  
		  Nios用ソフトウェアカウンタの初期化およびハンドラの定義
		- *counter\_handler.h*  
		  Nios用ソフトウェアカウンタの初期化およびハンドラの宣言
	- support  
	  **CSPトランスレータ**
	  生成コード向けに用意しているデバッグ支援機能ヘッダファイルのディレクトリ
		- *event\_debug.h*  
		  TOPPERS/ATK2のイベントAPIのデバッグ用ヘッダファイル
	- tools  
	  **pthreadエミュレーション環境** 向けツール
		- [abrexyaml2pthread-c.pl](#ToolAbrexyaml2pthreadC)  
		  ABREXのYAMLファイルから **pthreadエミュレーション環境**
		  向け各種ファイル生成を行うスクリプト
		- [tracecheck.pl](#ToolTraceCheck)  
		  実行トレースを読み込み時系列に表示するスクリプト

なお **pthreadエミュレーション環境** を利用する場合、
お使いのコンパイラおよびプラットフォームが、
少なくとも以下の標準・規格を満たしている必要があります。

1. C99
2. POSIX.1-2001

---

# <a id="Usage">利用方法</a>

## 概要

**CSPトランスレータ** の **TOPPERS/ATK2** 用コード生成では、
以下のファイルが生成されます。

1. &lt;モデル名&gt;-app.yaml  
  生成されたabrex用YAMLファイル
2. &lt;モデル名&gt;-app.h  
  上記YAMLファイルがインクルードする設定用ヘッダファイル
3. &lt;モデル名&gt;-output.c  
  生成されたモデルのCソースコード

**pthreadエミュレーション環境** では、上記1\(および2\)から、
__TOPPERS/ATK2__ のように *Os\_Lcfg.c* および *Os\_Lcfg.h*
を生成し、それらと上記3および **TOPPERS/ATK2**
のAPIをエミュレーションするライブラリとリンクし、
実行可能ファイルを生成します。

この、実行可能ファイルを簡便に生成し、実行する手順は、
次のようになります。

1. Makefileテンプレート[Makefile.tmpl](#MakefileTmpl)
   を生成したファイルがあるディレクトリに
   *Makefile* としてコピーし、編集する
2. make を実行する
3. [ビルドされた実行形式ファイルを実行する](#ExecuteProgram)

---

## <a id="MakefileTmpl">Makefile.tmpl</a>

*Makefile.tmpl* は以下のような構成になっていますので、
必要に応じて、コピーした *Makefile* の変更を行ってください。

1. [コンパイラおよび各種フラグの設定](#CompilerSettings)
2. [トップディレクトリの指定](#TopDir)
3. 各変数の設定
	- [モデルのコードのディレクトリの指定](#Atk2ModelDir)
	- [ABREXのYAMLファイルの指定](#Atk2AbrexYaml)
	- [main関数のソースファイルの指定](#Atk2SrcMain)
	- [その他のATK2ソースファイルの指定](#Atk2SrcOther)
	- [CSPトランスレータの生成したソースの指定](#Atk2Srcs)
	- [実行プログラム名の指定](#Atk2Program)
	- [実行トレース用ヘッダファイル名の指定](#Atk2TraceHdr)
	- [cleanで削除するファイルの追加](#Atk2CleanFiles)
	- [distcleanで削除するファイルの追加](#Atk2DistcleanFiles)
	- [makeの擬似ターゲットの追加](#Atk2PhonyTargets)
	- [プログラムのビルド前に実行するターゲット設定](#Atk2MakeGoalAll)
	- [makeのデフォルトゴールの設定](#Atk2MakeChangeDefault)
4. *include $\(TOP\_DIR\)/common.mk*  
   \(これは各種定義やルールを記述した
   *common.mk* を読み込む設定で変更は不要です\)
5. [利用者によるルールの追加](#Atk2MakeAfter)

*common.mk* によって提供されるmakeのターゲットは次の通りです。

 - [all](#Atk2TargetAll)  
   makeのデフォルトゴール
 - [abrex](#Atk2TargetAbrex)  
   ABREXの実行
 - [program](#Atk2TargetProgram)  
   実行ファイルのビルド
 - [clean](#Atk2TargetClean)  
   実行ファイルとコンパイルしたオブジェクトの削除
 - [realclean](#Atk2TargetRealclean)  
   生成した全てのファイルの削除
 - [distclean](#Atk2TargetRealclean)  
   [realclean](#Atk2TargetRealclean) と同じ
 - [depend](#Atk2TargetDepend)
   コンパイル対象のソースコードの依存関係の抽出

---

### <a id="CompilerSettings">コンパイラおよび各種フラグの設定</a>

		CC = gcc
		CFLAGS = -g -Wall -O2
		LDFLAGS = -g -Wall
		
		# 明示的に POSIX.1-2001 を有効にする必要がある場合、以下を有効にする
		#
		# CPPFLAGS = -DPOSIX_C_SOURCE=200112L

上記は **GNU Compiler Collection** を想定した設定です。

**pthreadエミュレーション環境** を利用するには、
コンパイラおよび開発環境が、
少なくとも以下の標準・規格を満たしている必要があります。

1. C99
2. POSIX.1-2001

ご利用の環境によりますが、POSIX.1-2001に対応するに、以下のように
*CPPFLAGS* を設定しなければならない場合があります。

		CPPFLAGS = -DPOSIX_C_SOURCE=200112L

### <a id="TopDir">トップディレクトリの指定</a>

		# common.mk,src,include,toolsがあるトップディレクトリを指定する。
		#
		TOP_DIR = /opt/MBP/atk2

*TOP\_DIR* は **pthreadエミュレーション環境**
の全てが配置されているディレクトリを指定します。

### <a id="Atk2ModelDir">モデルのコードのディレクトリの指定</a>

		# カレントディレクトリに、Embedded Coderが生成したファイルがないなら、
		# Embedded Coderが生成したモデルのコードのパスを指定する
		# (これを元にインクルードパスの追加やソースを探すvpath設定が行われる)
		#
		# ATK2_MODEL_DIR =

カレントディレクトリに、Embedded Coderが生成したファイルがないなら、
コメントを外し、
Embedded Coderが生成したモデルのコードのパスを指定してください。

### <a id="Atk2AbrexYaml">ABREXのYAMLファイルの指定</a>

		# ABREXのYAMLファイル(未定義の場合 testout.yaml になる)
		#
		# ATK2_ABREX_YAML =

**CSPトランスレータ** が生成したABREX用のYAMLファイル名を指定してください。  
なお、
__CSPトランスレータ__
が生成するデフォルトのファイル名は、"*&lt;モデル名&gt;-app.yaml*" です。

### <a id="Atk2SrcMain">main関数のソースファイルの指定</a>

		# ATK2のmain関数のソースファイルの指定
		# (未定義の場合は ambp_main.c になる)
		#
		# ATK2_SRC_MAIN =

通常は未定義としてください。  
未定義の場合 *$\(TOP\_DIR\)/static/ambp\_main.c* を使用します。

もし利用者が用意したTOPPERS/ATK2用main関数があれば、
そのファイルを指定してください。

なお、このTOPPERS/ATK2用のmain関数は、
**pthreadエミュレーション環境** が生成する実行可能プログラムの
main関数ではないことに注意してください。  
このmain関数は、プリプロセッサによって名前を置換され、
__pthreadエミュレーション環境__ の
**StartCore\(\)** から呼び出されるようになります。

### <a id="Atk2SrcOther">その他のATK2ソースファイルの指定</a>

		# その他のATK2のソース(未定義の場合は counter_handler.c になる)
		#
		# ATK2_SRC_OTHER =

通常は未定義としてください。  
未定義の場合 *$\(TOP\_DIR\)/static/couter\_handler.c* を使用します。

#### 注意

**pthreadエミュレーション環境** は、
ターゲットプラットフォームの動作をエミュレーションしません。  
目的は **ターゲットプラットフォームで実行した結果**
をエミュレーションすることであり、
従って定義される関数の中身は空であって構いません。

従って、本変数に指定するファイルは次のようになっていることに注意してください。

1. コンパイル時にコンパイルエラーにならない
2. リンク時にリンクエラーにならない
3. 実行時にエラーにならず、期待動作をする

### <a id="Atk2Srcs">CSPトランスレータの生成したソースの指定</a>

		# CSPトランスレータが生成したソースと、モデルに必要なソースがあれば追加
		# する(未定義の場合 testout.c になる)
		#
		# ATK2_SRCS =

**CSPトランスレータ** が生成したCソースのファイル名を指定してください。  
なお、
__CSPトランスレータ__
が生成するデフォルトのファイル名は、"*&lt;モデル名&gt;-output.c*" です。

また、Embedded Coderが生成した他のソースファイルを
コンパイル・リンクしたい場合は、以下のように、
この変数に追加してください。

		ATK2_SRCS = model-output.c rtGetInf.c rtGetNaN.c

Embedded Coderのファイルは、カレントディレクトリか、
または [ATK2\_MODEL\_DIR](#Atk2ModelDir)
で指定したディレクトリになければなりません。

### <a id="Atk2Program">実行プログラム名の指定</a>

		# 実行プログラム名。ただしファイル名ではなく、makeのターゲットである。
		# (未定義の場合は testout)
		#
		# ATK2_PROGRAM =

実行プログラム名を設定してください。  
未定義の場合は *testout* になります。

### <a id="Atk2TraceHdr">実行トレース用ヘッダファイル名の指定</a>

		# 実行トレース用の生成ヘッダファイル名指定(未定義の場合 testout_trace.h)
		# (空にすると生成されない)
		#
		# ATK2_TRACE_HDR =

実行トレース用ヘッダファイルを作成する場合は、以下のようにしてください。

- 未定義にする  
  *testout\_trace.h* が生成されます
- ファイル名を設定する  
  設定したファイル名で生成されます

実行トレース用ヘッダファイルを作成しない場合は、以下のように、
変数を **空** にしてください。

		ATK2_TRACE_HDR =

#### 注意

実行トレース用ヘッダファイルは、あらかじめ、
**CSPトランスレータ** の **-i** オプションで直接指定するか、
または指定したヘッダファイルから include するようにしてください。

また、生成されるヘッダファイルの詳細は、
[実行トレース用ヘッダファイル](#TraceHeader)
\([abrexyaml2pthread-c.pl](#ToolAbrexyaml2pthreadC)\)
を参照してください。

### <a id="Atk2CleanFiles">cleanで削除するファイルの追加</a>

		# clean で削除するファイルの追加
		# ($(ATK2_PROGRAM)$(EXE_POSTFIX), および対象の %.o は自動的に追加される)
		# 
		# ATK2_CLEAN_FILES =

makeのcleanターゲットで削除するファイルを追加します。  
通常は未定義のままで構いませんが、
make時に独自のルールで生成したファイルがあり、
clean時に削除したい場合は、それらのファイルを指定してください。

### <a id="Atk2DistcleanFiles">distcleanで削除するファイルの追加</a>

		# distclean/realclean で削除するファイルの追加
		# (未定義の場合 Os_Lcfg.[ch], $(ATK2_TRACE_HDR), *.d)
		#
		# ATK2_DISTCLEAN_FILES =

makeのdistclean,realcleanターゲットで削除するファイルを追加します。  
通常は未定義のままで構いませんが、
make時に独自のルールで生成したファイルがあり、
distclean/realclean時に削除したい場合は、
それらのファイルを指定してください。

### <a id="Atk2PhonyTargets">makeの擬似ターゲットの追加</a>

		# 擬似ターゲットの追加
		# (all abrex program clean realclean distclean depend は自動的に追加される)
		#
		# ATK2_PHONY_TARGETS =


利用者が用意するmakeの擬似ターゲットを追加してください。  
通常は未定義のままで構いませんが、
追加した独自ルールに擬似ターゲットがあれば、それらを追加してください。

擬似ターゲットは、実ファイルではないmakeのターゲットのことで、
makeはこのようなターゲットは存在の確認や日付の比較を行いません。

実際の使用例は、以下を参照してください。

- [プログラムのビルド前に実行するターゲット設定](#Atk2MakeGoalAll)
- [利用者によるルールの追加](#Atk2MakeAfter)

### <a id="Atk2MakeGoalAll">プログラムのビルド前に実行するターゲット設定</a>

		# プログラムのビルド前に実行したいターゲットがある場合、ここでそのター
		# ゲットをallの依存ターゲットとして追加する。
		# その場合、ATK2_PHONY_TARGETS にも利用者のターゲットを追加する。
		#
		# all:

makeのデフォルトゴールのallに依存ターゲットとして、
プログラムのビルド前に、実行するターゲットを追加します。

また、利用者が指定したターゲットが、makeが生成するファイルではない場合、
*ATK2\_PHONY\_TARGETS* に擬似ターゲットとして追加してください。

*$\(TOP\_DIR\)/common.mk* では *all*
が最初のターゲット\(すなわちデフォルトゴールのターゲット\)
として定義されているので、例えば準備作業として *my-preparation*
を先に実行したい場合は、以下のようにしてください。

		ATK2_PHONY_TARGETS += my-preparation
		
		all: my-preparation
		
		include $(TOP)/common.mk
		
		my-preparation: 利用者が用意するmakeのターゲット

なお、プログラムのビルド前に実行したいターゲットが、
プログラムそのものが依存している場合については、
[利用者によるルールの追加](#Atk2MakeAfter)
の設定例の方が望ましいやり方です。

また、プログラムのビルド後に実行したいターゲットがある場合も、
[利用者によるルールの追加](#Atk2MakeAfter)
を参照してください。

### <a id="Atk2MakeChangeDefault">makeのデフォルトゴールの変更</a>


		# もしもデフォルトゴールのallをどうしても使用できず、別の何かに変更する
		# 必要がある場合は、そのターゲットを .DEFAULT_GOAL に設定する。
		#
		# .DEFAULT_GOAL :=

デフォルトゴールとなっている*all*を使用せず、
他のターゲットをデフォルトゴールとして意場合、
そのターゲットを設定します。

例えば、デフォルトゴールのターゲットを、
*my-default* にするには次のようします。

		ATK2_PHONY_TARGETS += my-default my-preparation
		
		.DEFAULT_GOAL := my-default
		
		include $(TOP)/common.mk
		
		my-default: my-preparation all
		
		my-preparation: 利用者が必要とするmakeターゲット
		

### <a id="Atk2MakeAfter">利用者によるルールの追加</a>

		include $(TOP_DIR)/common.mk
		
		# プログラムそのものに関連し、プログラムのビルド前に実行する必要のある
		# ターゲットは、ここにprogramの依存ターゲットとして追加する。
		#
		# ただし、ATK2_PHONY_TARGETS への追加は、common.mkのインクルード前に行
		# う必要がある。
		#
		# program:
		
		# プログラムのビルド後に実行したいターゲットがある場合、ここでそのター
		# ゲットをallの依存ターゲットとして追加する。
		#
		# ただし、ATK2_PHONY_TARGETS への追加は、common.mkのインクルード前に行
		# う必要がある。
		#
		# all:

*include $\(TOP\_DIR\)/common.mk* より後に、
必要に応じてルールの追加や変数への値の追加等を行ってください。

例えば、プログラム自身に依存があり、
プログラムのビルド前に実行したいターゲットがある場合は、
以下のように、することが望ましいです。

		ATK2_PHONY_TARGETS += pre_build
		
		include $(TOP)/common.mk
		
		program: pre_build
		
		pre_build: 利用者が必要とするmakeターゲット


また、以下のようにプログラムを実行するターゲット *run*
を追加することで、プログラムをビルド後に実行させる、
というような事ができます。

		ATK2_PHONY_TARGETS += run
		
		include $(TOP)/common.mk
		
		all: run		
		run: program
			$(ATK2_PROGRAM) -s 15

---

## <a id="ExecuteProgram">プログラムの実行</a>

ビルドされたプログラムは、**pthread**を用いたマルチスレッドプログラムです。

### 実行方法

		プログラム名 [-s sleep]

プログラム名は
[ATK2\_PROGRAM](#Atk2Program)
で設定したプログラム名です。

### オプション

- __-s__ *sleep*  
  実行する時間を指定します。単位は秒です。  
  省略時は *3600\(1時間\)* となります。  
  プログラムは指定時間実行後、終了しますが、
  *0* を指定した場合はプログラムは終了しません。
  
#### 実行トレースを有効にしている場合

実行トレースが有効な場合、以下のタイミングで実行トレースを表示します。

- __-s__ *0* で実行した場合  
  実行トレースの格納領域に空きがなくなった場合に、
  実行トレースの情報を出力します。
- それ以外の場合  
  プログラム終了時に実行トレースの情報を出力します。

実行トレースについては、以下を参照してください。

- [実行トレース用ヘッダファイル名の指定](#Atk2TraceHdr)
- [実行トレース用ヘッダファイル](#TraceHeader)

---

# その他ファイル

## <a id="CommonMk">common.mk</a>

*common.mk* は、
**pthreadエミュレーション環境** の、
コンパイルに必要な変数設定およびルールをまとめたもので、
[Makefile.tmpl](#MakefileTmpl) からincludeして
利用することを想定しています。

従って利用方法については
[Makefile.tmpl](#MakefileTmpl)
を参照してください。

*common.mk* が提供するターゲットは次の通りです。

 - [all](#Atk2TargetAll)  
   makeのデフォルトゴール
 - [abrex](#Atk2TargetAbrex)  
   ABREXの実行
 - [program](#Atk2TargetProgram)  
   実行ファイルのビルド
 - [clean](#Atk2TargetClean)  
   実行ファイルとコンパイルしたオブジェクトの削除
 - [realclean](#Atk2TargetRealclean)  
   生成した全てのファイルの削除
 - [distclean](#Atk2TargetRealclean)  
   [realclean](#Atk2TargetRealclean) と同じ
 - [depend](#Atk2TargetDepend)
   コンパイル対象のソースコードの依存関係の抽出

### <a id ="Atk2TargetAll">all</a>

makeのデフォルトゴールです。

*common.mk* によって *all*
の依存ターゲットに、プログラムをビルドするターゲット
[program](#Atk2TargetProgram)
が設定されます。

従って、 *common.mk* をインクルードする前に *all*
に設定した依存ターゲットは、
[program](#Atk2TargetProgram) の前に実行されます。  
また、 *common.mk* をインクルードした後に *all*
に設定した依存ターゲットは
[program](#Atk2TargetProgram)
の後に実行されます。

### <a id="Atk2TargetAbrex">abrex</a>

ABREXを実行します。

### <a id="Atk2TargetProgram">program</a>

実行ファイルをビルドします。

[all](#Atk2TargetAll) の依存ターゲットです。

### <a id="Atk2TargetClean">clean</a>

実行ファイルとコンパイルしたオブジェクトを削除します。

### <a id="Atk2TargetRealclean">realclean</a>

生成した全てのファイルを削除します。

*realclean* と *distclean* は、同じことを行います。

### distclean

realcleanと同じです。

### <a id="Atk2TargetDepend">depend</a>

コンパイル対象のソースコードの依存関係を抽出し、
ソースファイル毎に %.d ファイルを生成します。

---

## <a id="ToolAbrexyaml2pthreadC">tools/abrexyaml2pthread-c.pl</a>

*abrexyaml2pthread-c.pl* は、ABREX用YAMLファイルを入力し、
**pthreadエミュレーション環境** 向け各種コード生成を行うスクリプトです。

このスクリプトは *make* の実行中に呼ばれます。

このスクリプトには以下の機能があります。

1. TOPPERS/ATK2同様に *Os\_Lcfg.c* と *Os\_Lcfg.h* を生成する
2. デバッグ/動作確認用の実行トレース用のヘッダファイルを生成する

### 使用方法

		abrexyaml2pthread-c.pl [-n] [-t trace.h] [-s tick size] [abrex.yaml...]

### オプション

- __-n__ 
  このオプションは *Os\_Lcfg.c* と *Os\_Lcfg.h* の生成を制御します。  
  このオプションを指定した場合、
  *Os\_Lcfg.c* と *Os\_Lcfg.h* を出力しません。  
  オプションを指定しない場合、
  *Os\_Lcfg.c* と *Os\_Lcfg.h* を出力します。  
  これは __-t__ オプションと併用します。  
  TOPPERS/ATK2のABREXは、AUTOSARの設定ファイルを生成するため、
  モデルのYAMLファイル以外に、他のYAMLファイルを入力する必要があります。  
  一方、 __-t__ オプションで生成する、
  実行トレース用のヘッダファイル生成には、
  モデル以外のYAMLファイルは不要です。  
  このように *Os\_Lcfg.c* と *Os\_Lcfg.h* の生成と、
  *trace.h* の生成を分けて行うのが適当な場合に
  __-n__ オプションを使用します。
- __-t__ *trace.h*  
  このオプションは実行トレース用のヘッダーファイルの生成を制御します。  
  このオプションを指定した場合、
  実行トレース用のヘッダーファイルを生成します。  
  なお前述の通り、入力するABREXのYAMLファイルに差異があり、
  *Os\_Lcfg.c* と *Os\_Lcfg.h* と同時に生成する事が適当でない場合、
  **-n** オプションと併用するのが適当です。  
  このオプションを指定しない場合、
  実行トレース用のヘッダーファイルを生成しません。
- **-s** _tick\_size_  
  このオプションは、実行トレースとして
  タスクの周期動作の開始と終了時刻を記録する数を指定します。  
  このオプションを省略した場合は _64_ 個になります。  
  pthreadによるエミュレーションでなく、実機動作の場合は、
  実際に確保可能なサイズを考えて指定する必要があります。  
  配列のサイズは以下の通りです。  
  ```
		NUM_TASK x 記録する数 x 2 x 4(byte)
  ```
- *abrex.yaml*  
  処理するabrexのYAMLファイルを指定します。  
  このパラメタは複数指定可能です。

### <a id="TraceHeader">実行トレース用ヘッダーファイル</a>

この生成ファイルは、CSPトランスレータが生成したTOPPERS/ATK2のコードに対し、
タスクの周期動作の開始と終了の時刻を各タスク毎の配列に記録する機能を
提供するヘッダファイルです。

#### Cマクロ

記録する機能を制御するマクロは以下の通りです。

- *OS\_WRAPPER\_\_\_ENABLE\_TASK\_TICK*  
  タスクの周期動作を記録する機能を使用するかどうか指定します。  
  *OS\_WRAPPER\_\_\_ENABLE\_TASK\_TICK* が *1* の場合、機能を使用します。  
  *OS\_WRAPPER\_\_\_ENABLE\_TASK\_TICK* が *0* の場合、機能を使用せん。  
  デフォルトでは *1* で、機能を使用します。  
  なお *OS\_WRAPPER\_\_\_ENABLE\_TASK\_TICK* は、コンパイル時に、
  `-DOS_WRAPPER___ENABLE_TASK_TICK=0`
  のようにすることで、機能を無効化できます。
- *OS\_WRAPPER\_\_\_ENABLE\_TTICK\_AUTO\_STOP*  
  タスクの周期動作の開始と終了時刻を記録は、
  記録する配列に空きがなくなるまで行います。
  \(空きがなければ記録されません\)  
  *OS\_WRAPPER\_\_\_ENABLE\_TTICK\_AUTO\_STOP* が *1* の場合、
  全てのタスクについて、
  記録する配列の空きがなくなると記録を自動停止し、
  実行トレースを出力します。  
  \(*OS\_WRAPPER\(\_dump\_task\_info\)\(\)* を参照 \)  
  *OS\_WRAPPER\_\_\_ENABLE\_TTICK\_AUTO\_STOP* が *0* の場合、
  実行トレースを出力しません。  
  なお *OS\_WRAPPER\_\_\_ENABLE\_TTICK\_AUTO\_STOP* は、コンパイル時に、
  `-DOS_WRAPPER___ENABLE_TTICK_AUTO_STOP=1`
  のようにすることでも変更可能です。

#### 注意

pthreadエミュレーションの環境では、
指定時間動作して停止させる事ができるため、
デフォルトでは自動停止は無効となり、
終了のタイミングで記録した情報を出力しますが、
実機では停止させるタイミングがないため、
配列に空きがなくなったタイミングで記録を停止し、
記録した情報を出力するために用いる機能です

そのため実機では、
`-DOS_WRAPPER___ENABLE_TTICK_AUTO_STOP=0`
のような、機能の無効化を行わない事を推奨します。

### CSPトランスレータの対応マクロ

実行トレース用ヘッダファイルでは、
**CSPトランスレータ** が生成するコードに出力されているマクロを使って、
実行トレースを取得するコードを埋め込みます。

以下は対応するマクロです。

- *TASK\_&lt;TASK\_ID&gt;\_DEFINITIONS*  
  CSPトランスレータの生成コードでは、
  タスクに対して任意の宣言・定義を配置するためのマクロです。  
  現在、このマクロに対しては何も設定を行いません。
- *TASK\_&lt;TASK\_ID&gt;\_LOOP\_TOP*  
  CSPトランスレータの生成コードでは、
  タスクの周期動作のループの先頭部に置かれるマクロです。  
  本機能では、このマクロに、
  タスク毎の周期動作の開始時刻を取得・記録する処理を設定します。
- *TASK\_&lt;TASK\_ID&gt;\_BEFORE\_SLEEP*  
  CSPトランスレータの生成コードでは、
  タスクの周期動作のループの最後尾に置かれるマクロです。  
  本機能では、このマクロに、
  タスク毎の周期動作の終了時刻を取得・記録する処理を設定します。  
  記録の自動停止が有効な場合、
  このマクロの中で自動停止するかどうかの判定と、
  自動停止した場合の記録した情報の出力を行います。

### 定義される関数

生成されたヘッダファイルでは関数が定義されます。
これらの関数は、特に利用することを意識するはありません。

実機では原則として、機能は全て **CSPトランスレータ** の生成コード中の
*TASK\_&lt;TASK\_ID&gt;\_LOOP\_TOP*
および
*TASK\_&lt;TASK\_ID&gt;\_BEFORE\_SLEEP*
マクロ内に隠蔽されるため、特にこれら関数の知識は不要です。

一方、pthreadエミュレーション環境では、
記録の自動停止が行われる場合とそうでない場合、
機能が有効か無効かで動作を変えるため、
*os\_wrapper.c* で *weak* シンボルとして宣言され、
そのシンボルがリンクされているかどうかで
*main\(\)* の動作を変えます。

これらの関数の実際の使われかたは *os\_wrapper.c* の
*OS\_WRAPPER\(\_check\_and\_dump\)\(\)*
および
*OS\_WRAPPER\(\_dump\_task\_info\)\(\)*
と
*main\(\)*
を参照してください。

- *OS\_WRAPPER\(\_check\_task\_ticks\_done\)*  
  周期動作の開始時刻・終了時刻を記録した配列が、
  全てのタスクについて、空きがない状況かを調べます。  
  *1* が返った場合は、記録する配列に空きがない事を示し、
  *0* が返った場合は空きがある事を示します。
- *OS\_WRAPPER\(\_dump\_task\_ticks\)*  
  タスク毎に周期動作の開始時刻、終了時刻を、
  TOPPERS/ATK2 のAPI *syslog\(\)* を使って表示します。  
  
### 実行トレースの出力

*OS\_WRAPPER\(\_dump\_task\_ticks\)* で、
タスク毎に周期動作の開始時刻、終了時刻を、
TOPPERS/ATK2 のAPI *syslog\(\)* を使って表示します。

フォーマットは、以下の通りです。

		開始時刻:終了時刻:タスク名

このフォーマットの出力は
[tracecheck.pl](#ToolTraceCheck)
のスクリプトで、
パースして実行状態を表示させることができます。

---
## <a id="ToolTraceCheck">tools/tracecheck.pl</a>

*tracecheck.pl* は、
出力された実行トレースを読み込み、時系列に表示するスクリプトです。

本スクリプトは、
[abrexyaml2pthread-c.pl](#ToolAbrexyaml2pthreadC)
が生成した実行トレース用ヘッダファイルを用いて、
取得・表示した実行トレースの情報を読み込み、
それらを時系列に並べ、タスクの周期と実行時刻の差異を表示します。

### 使用方法

		tracecheck.pl [-b base_rate] [-c hz] [-l max_load] [dumpfile]

### オプション

- **-b** *base\_rate*  
  タスクの周期のベースレートを秒で指定します。  
  デフォルトは *0.001\(1ms\)* です。  
  コマンドでは、タスクの識別子から抽出した、 *周期* 、 *オフセット* を
  *base\_rate* を用いて、実時間の周期とオフセットを変換します。  
  なお、タスクの識別子は実行トレースの表示では次のようになっています。  
  ```
		<モデル名>_Core<コア番号>_<周期>_<オフセット>_TASK
  ```
- **-c** *hz*  
  実行トレースの計時に用いた *MAIN\_HW\_COUNTER\_CORE0* の周波数を指定します。
  デフォルトは *60000000\(60MHz\)* です。  
  実行トレースの時刻はTOPPERS/ATK2の *GetCounterValue\(\)* で取得した、
  *MAIN\_HW\_COUNTER\_CORE0* の値を使用します。  
  この *MAIN\_HW\_COUNTER\_CORE0* の周波数を指定してください。
- **-l** *max\_load*  
  実行トレースの計時に用いた *MAIN\_HW\_COUNTER\_CORE0*
  の最大カウント値を指定します。  
  デフォルトでは *536870911\(0x1fffffff\)* です。  
  実行トレースの時刻は *MAIN\_HW\_COUNTER\_CORE0* の値ですが、
  カウンタは、最大カウント値の次は *0* に戻ります。  
  このため、 *0* に戻ったカウント値の時刻を補正するために、
  最大カウント値をコマンドに与える必要があります。
- *dumpfile*  
  入力ファイルを指定します。省略時は標準入力になります。  
  実行トレースの情報が出力されているファイルを指定してください。

### 表示の説明

#### 周期動作開始行

行頭が **+** の場合、周期動作を開始した事を示します。

		+    1.000455    1.000297 Core1_1000_0 (   0.030%)

**+** に続くフィールドの意味は次の通りです。

1. *1.000455*  
   このフィールドは、開始した時刻です。  
   単位は秒です。
2. *1.000297*  
   このフィールドは前回開始した時刻からの差異です。  
   単位は秒で、この期待値は周期です。
3. *Core1\_1000\_0*  
   このフィールドは、タスクの情報です。  
   この例では、コア番号1、周期1000、開始オフセット0のタスクを示します。
4. *\(   0.030%\)*  
   このフィールドは、上記の2と周期とのずれを示します。  
   単位はパーセンテージです。

#### 周期動作終了行

行頭が **-** の場合、周期動作を終了した事を示します。

		-    1.000466    0.000011 Core1_1000_0

**-** に続くフィールドの意味は次の通りです。

1. *1.000466*  
   このフィールドは、終了した時刻です。  
   単位は秒です。
2. *0.000011*  
   このフィールドは、開始した時刻からの差異です。  
   この回の周期動作に要した時間で、単位は秒で、期待値は、周期未満です。
3. *Core1\_1000\_0*  
   このフィールドは、タスクの情報です。  
   この例では、コア番号1、周期1000、開始オフセット0のタスクを示します。

#### レコード数

タスク毎の実行トレースの最終レコードに対しては、
行末に以下のように表示します。

		[[-- END(8)--]]

1. *8*  
   この例では、 *8* 個のレコードを表示した事を示します。
