
		性能付きブロックレベル構造XMLについて

1. はじめに
===========

性能付きブロックレベル構造XMLの生成する機能について述べます。

本機能は、以下の三つのプログラム、モジュールからなります。

(1) blxml-perf.sh

    ブロックレベル構造XMLのコード断片の性能を算出し、性能付きブロックレ
    ベル構造XMLを出力する為の一連の処理を実行するシェルスクリプトです。

(2) blxml2code

    ブロックレベル構造XMLのコード断片から、コンパイル可能なCコードを生
    成するプログラムです。生成したコードをLLVMでコンパイルし、後述する
    LLVMのPass(llvm-BLXMLPerf.so)を通す事で、性能が算出されます。

(3) llvm-BLXMLPerf.so

    ブロックレベル構造XMLのブロック毎のコード(task,update,init)単位の性
    能を計測し、性能付きブロックレベル構造XMLを出力するために利用する
    LLVMのPassです。


2. 機能説明
===========

2.1. blxml-perf.sh
==================

ブロックレベル構造XMLのコード断片の性能を算出し、性能付きブロックレベル
構造XMLを出力する為の一連の処理を実行するシェルスクリプトです。

2.1.1. 実行
===========

  blxml-perf.sh [-v] [-o output] [-shim=SHIM] [-shim-schema=SCHEMA] \
        [-C clang-option] [-0 pass-option] [-S pass-module] INPUT-XML

コマンドオプションは以下の通りです。

  -v                  : (optional) verbose option
  -o output           : (optional) Output BLXML
  -shim=SHIM          : (optional) SHIM File included `CommonInstructionSet'
  -shim-schema=SCHEMA : (optional) SHIM Schema for validation
  -C clang-option     : (optional) add CLANG options as `-O', `-I', etc
  -O pass-option      : (optional) add LLVM pass options, see below
  -S pass-module      : (optional) use LLVM Pass Module

-Oオプションは、以下の四つのLLVMのPass(llvm-BLXMLPerf.so)のオプションの
他、任意のoptコマンドのオプションを指定できます。

  -O -MasterComponent=name      : Name of SHIM MasterComponent
  -O -CommonInstructionSet=name : Name of SHIM CommonInstructionSet
  -O -no-ret                    : Not count LLVM ReturnInstruction
  -O -no-br                     : Not count LLVM BranchInstruction
  -O -out-dot=basename          : basename of output GraphViz DOT file

例えばpidでは以下のように実行します。

  ./blxml-perf.sh -v \
      -o models/pid/test_blxml.xml \
      -shim=SHIM.xml \
      -C -Imodels/pid/pid_controller1_ert_rtw/ \
      -C -O \
      models/pid/yy.xml

　(1) model/pid/yy.xml からmodel/pid/test_blxml.xmlを生成します
  (2) SHIMとして、SHIM.xmlを使用します(schemaによるvalidationは行いません)
  (3) -Cはclangへのオプションで、blxml2codeによって生成されたコードが使
      用する rtwtypes.h のインクルードパス(-I)と、オプティマイズ(-O)を
      指定します

※ clangに-Oオプション(オプティマイズ)を行う場合とそうでない場合では、
   ループの構造に差異が出て、固定回数あるいは最大回数がわからないループ
   になるケースがあります。その場合、ループのイテレーションの処理量は計
   算されません。(-C -Oのある/なし)

※ optに-mem2regを指定した方が、ループの回数を適切に検出できるようです
   (オプションは -O -mem2reg)

環境変数として、以下のものを指定する事ができます。

  BLXML2CODE     -- blxml2codeのプログラム名。デフォルトではblxm2code
  CLANG          -- LLVM CLANGのプログラム名。デフォルトではclang
  LLVM_OPT       -- LLVMのoptのプログラム名。デフォルトではopt
  LLVM_BLXMLPERF -- 性能算出用LLVMのPass。フォルトは ./llvm-BLXMLPerf.so

※ BLXML2CODE,CLANG,LLVM_OPTを変更しない場合、各プログラムが実行できる
   ように PATH 環境変数を設定してください。特に、カレントディレクトリの
   blxml2codeを実行する必要がある場合、PATHに"."が含まれねばなりません。


2.1.2. 動作内容
===============

このプログラムは、以下のように動作します

  (1) blxml2codeにより性能測定用コードを生成する
  (2) clangで上記コードをLLVMのアセンブラにコンパイルする
  (3) llvm-BLXMLPerfを用い、コンパイルされたコードの性能値(サイクル数)
      を算出し、その性能値を性能付きブロックレベル構造XMLに出力する


2.2. blxml2code
===============

※このプログラムはblxml-perf.shから実行されます

ブロックレベル構造XMLのコード断片から、コンパイル可能なCコードを生成す
るプログラムです。

2.2.1. 実行
===========

  blxml2code [-o output] [input]

コマンドオプションは以下の通りです。

  -o output : (optional) output C code

-oが指定されない場合、標準出力へソースコードを出力します。inputが指定さ
れない場合、標準入力からXMLを読み込みます。

例えばpidでは以下のように実行します。

  ./blxml2code -o xxx.c models/pid/yy.xml

生成されたコードをコンパイルする為には rtwtypes.h が必要です。コンパイ
ルの際には、このファイルが存在するディレクトリをclangの-Iオプションで指
定してください。

clanでコンパイルする場合は、例えば次のようにします。

  clang -S -O -I<path fof rtwtype.h> -emit-llvm -o xxx.ll xxx.c


2.2.2. コードについて
=====================

本プログラムが生成するコードは、性能測定のためのコンパイル可能なコード
を生成する事が目的であり、元のモデルの機能を実現するものではありません。

また、各ブロックで使用する入力側変数は関数パラメタとして値渡しされ、
出力変数はポインタとして扱われるようにします。状態変数やパラメタは、
ファイルスコープの変数として定義されます。


2.3. llvm-BLXMLPerf.so
======================

※この処理はblxml-perf.shから実行されます

ブロックレベル構造XMLのブロック毎のコード(task,update,init)単位の性能を
計測し、性能付きブロックレベル構造XMLを出力するために利用するLLVMの
Passです。

2.3.1. 実行
===========

本Passは以下のように実行します。

  opt -load=<path for llvm-BLXMLPerf.so> -S -blxml-perf \
    [-shim=SHIM] [-shim-schema=SCHEMA] \
    [-MasterComponent=name] [-CommonInstructionSet=name] \
    [-no-ret] [-no-br] \
    [-in-xml=INPUT.XML] [-out-xml=OUTPUT.XML] \
    -o dummy_output.ll input.ll

llvm-BLXMLPerf.soのオプションは以下の通りです。

  -shim=SHIM                 : (optional) SHIM File
  -shim-schema=SCHEMA        : (optional) SHIM Schema for validation
  -MasterComponent=name      : (optional) Name of MasterComponent
  -CommonInstructionSet=name : (optional) Name of CommonInstructionSet
  -no-ret                    : (optional) Not count LLVM ReturnInstruction
  -no-br                     : (optional) Not count LLVM BranchInstruction
  -in-xml=INPUT.XML          : (required) input BLXML 
  -out-xml=OUTPUT.XML        : (optional) output BLXML
  -out-dot=basename          : (optional) basename of output GraphViz DOT file

-shim=-no とした場合、SHIMを入力せず、サイクル数1として性能を算出します。
その場合、実行時、命令毎にエラーを出力します。

-shim-schemaを指定しない場合、XMLのチェック(validation)は行いません。

命令のサイクル数を取得する、SHIMのCommonInstructionSetを特定するため、
MasterComponentとCommonInstructionSetの属性の名前を指定する事ができます。
これらを指定しない場合、最初に見つけた CommonInstructionSetを使用します。

-no-retは、性能算出の際、RET命令のサイクルを無視します。これはブロック
のコードの断片は、blxml2codeでは一つの関数として生成するのでRETのオーバ
ヘッドを無視する場合に使用します。

-no-brは、基本ブロック間の分岐命令のサイクルを無視します。(これが有効な
結果に繋がるケースはあまりないと思われますが、一応つける)

-in-xmlによる入力のブロックレベル構造XMLは、blxml2codeでコード生成に使
用したXMLファイルでなければなりません。

-out-xmlは省略可能で、省略した場合、性能付きブロックレベル構造XMLを標準
出力に出力します。

-out-dotは省略可能です。このオプションを指定した場合、
basename-<番号>.dot というGraphVizのファイルがループおよび関数毎に順に
生成します。


2.3.2. dotファイル
==================

dotファイルはGraphVizのファイルで、処理量を計算した際の経路探索結果です。
関数単位の処理としては最内ループから出力し、最後にコード全体(関数)のグ
ラフになります。

ノードは基本ブロックまたはまとめられた固定回回るループです。

・赤枠(red)かつ塗りつぶされているノード(plum)は、固定回回るループを一つ
  にまとめたもの
・青枠(blue)のノードは、最大回数がわかっているループのヘッド(基本ブロック)
・それ以外のノードは通常の基本ブロック
・boxのノードは、ループのヘッドへのbackedgeを示すためのダミーノード
・内部の数字は上から順に、そのノードのbest/typical/worstの処理サイクル
・最後の[]内の数字は、終端までの平均処理時間です(successorの各処理時間
  と等確率分岐によって平均化した処理時間)

エッジはsuccessorになります。

・緑(green)は最短経路
・青(blue)は最長経路。最長経路が最短経路と重なる場合は、最短経路の表示
  が優先
・赤(red)の破線はループのバックエッジ
・黒の破線はダミーノードへの接続を示す
・数字はヘッドのノードの処理時間を経路探索のための距離(distance)で、上
  から順にbest/typical/worstとなる


2.3.2. 処理概要
===============

本Passはつぎのようになっています

  (1) FunctionPassを継承しており、関数単位に処理を行う
  (2) 事前に LoopInfo Passと ScalarEvolution Passを実行する
  (3) 初期化時にSHIMとブロックレベル構造XMLを読み込む
  (4) 終了時に性能付きブロックレベル構造XMLを出力する

関数単位の処理は以下のようになっています。

  (1) 基本ブロックのポインタをキーとした連想配列を作成(付随情報の保持のため)
  (2) 基本ブロックのサイクル数(best/typical/worst)を算出し、連想配列に
      保存
  (3) LoopInfoとScalarEvolutionを元にループの処理を実施
  (4) 基本ブロックのsuccessorおよび基本ブロックをまとめたループの情報
      (LoopInfoによる連想配列)から、有向グラフを作成する(boostのグラフ
      ライブラリを使用する)
  (5) 最短経路/best、最長経路/worst、分岐確率50%/typicalによる経路探索
      を実施し、関数のサイクル数(best/typical/worst)を算出し、関数の情
      報として保存

ループの処理はつぎのように行います

  (1) LoopInfo最内ループから処理するよう再帰処理する
  (2) ScalarEvolutionの結果から固定回回るループなら、ループとして基本ブ
      ロックを一つのグラフとして、ループ内の基本ブロックのサイクル数を
      最短経路/best, 分岐確率50%/typical、最長経路/worst別に算出、
      best/worst/typicalともループ回数分加算する。
  (3) 最大回数がわかるループ(途中で抜け出すことがある)なら、ループとし
      て基本ブロックを一つのグラフとして、ループ内の基本ブロックのサイ
      クル数をbest/typical/worst別に加算、worstだけループ回数分加算する。
      best/typicalは一回だけ回るものとする。
  (4) 上記のループの基本ブロックの情報をLoopInfoのポインタをキーとした
      連想配列に保存する
  (5) 上記いずれでもないループは、基本ブロックをループとしてまとめない
      (つまり、一回だけ回るものとして取り扱うことになる)


ループ内の基本ブロックも関数全体についてもグラフ探索はつぎのように行い
ます。

  (1) best値をエッジに距離として設定しdijkstraのアルゴリズムで最短経路
      探索を行う(boostのアルゴリズムを使用)
  (2) worst値の負数をエッジに距離として設定し、bellman-fordのアルゴリズ
      ムで最短経路探索を行う(結果として最長経路になる)。(boostのアルゴ
      リズムを使用)
  (3) 分岐確率50%(同確率分岐)のtypicalの探索は、再帰的なdepth firstで行
      う。ループを検出(GRAY)した場合は探索を打ちきる(つまりその分岐は当
      たらない)。到達済みノード(BLACK)であれば、下位ノードの計算済みの
      サイクルを加算して行く。最後に分岐数で割ってサイクル数とする。

以上
