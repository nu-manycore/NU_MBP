
		    SimulinkモデルのXML対応のメモ

1. はじめに
===========

本メモは、XML出力結果確認用に提供する現在のプログラムです。

本ファイル以下の機能を有します

・Simulinkから抽出されたXMLを読み、RTWのコードを抽出して埋め込んだXMLファ
  イル(BLXML)を出力するプログラム
・BLXMLのRTWのコードの断片について、性能を計算し、XMLに埋め込むプログラム

以下は各ファイルの概要です。

  Simulink_XSD/SimulinkModel.m4        -- XMLスキーマのテンプレート
  Simulink_XSD/SimulinkXSD.xslx        -- スキーマの説明(Excelファイル)

  Simulink_XSD/xsd_driver.cxx          -- XMLスキーマのテストコード

  Simulink_XSD/SimulinkRTW.cxx         -- RTWのコード処理モジュール
  Simulink_XSD/SimulinkRTW.h           -- モジュールのヘッダファイル
  Simulink_XSD/rtw_test.cxx            -- RTWのコード処理のテストコード

  Simulink_XSD/SimulinkXML.cxx         -- RTWを埋め込んだXMLの処理支援コード
  Simulink_XSD/SimulinkXML.h           -- 上記用ヘッダ
  Simulink_XSD/xml_test.cxx            -- 上記テストコード

  Simulink_XSD/blxml2code.cxx          -- 計測コードを生成するプログラム

  Simulink_XSD/shim-schema.xsd         -- 4/14頃のSHIMスキーマ

  Simulink_XSD/SHIM.xml                -- v850計測でのSHIMサンプル

  Simulink_XSD/llvm-BLXMLPerf.cxx      -- BLXML用のLLVMのPassのソース
  Simulink_XSD/llvm-BLXMLPerf.h        -- 上記用ヘッダ
  Simulink_XSD/blxml_llvm.cxx          -- 上記のBLXMLアクセス用コード
  Simulink_XSD/blxml_llvm.h            -- 上記用ヘッダ
  Simulink_XSD/shim_llvm.cxx           -- 上記のSHIMアクセス用コード
  Simulink_XSD/shim_llvm.h             -- 上記用ヘッダ
  Simulink_XSD/boost_llvm.cxx          -- 外出ししたboostのソース

  Simulink_XSD/blxml-perf.sh           -- BLXMLを性能付きBLXMLにするスクリプト

  Simulink_XSD/models/pid/xx.xml       -- pid_controller1.xmlの書き換え
  Simulink_XSD/models/cell/xx.xml      -- vipcellcounting_win.xmlの書き換え
  Simulink_XSD/models/test1/xx.xml     -- testsys.xmlの書き換え
  Simulink_XSD/models/testmux/xx.xml   -- testmux3.xmlの書き換え

  Simulink_XSD/Makefile                -- Makefile


2. コンパイル環境
=================

コンパイルは Fedora 20(x64), CentOS6.5(x64)とSolaris11.1(x64)で確認して
います。


2.1. 必要なパッケージ
=====================

2.1.1. CodeSynthesis XSD
========================

C++のデータバインディングのコード生成のため、CodeSynthesisのXSDが必要で
す。Linuxではパッケージ化されているので、導入してください。
(CentOSではRedHatのEPELリポジトリから導入可能です)

また、各コマンドが実行できるよう、必要であれば PATH 環境変数の設定を行っ
てください。

2.1.2  Xerces-C++
=================

XSDはApache ProjectのXerces-C++が必要です。Linuxではパッケージ化されて
いるので導入してください。

また、pkg-configにて情報が取得できるよう、必要であれば PKG_CONFIG_PATH
環境変数等の設定を実施してください。

2.1.3. boostライブラリ
======================

GNU C++のregexは未実装のため、正規表現の処理にはboost-regexライブラリを
使用しています。また、文字列置換にboostライブラリのalgorithmを使用して
います。Linuxではパッケージ化されているのでboostライブラリを導入してく
ださい。

2.1.4. graphviz
===============

ブロックの接続情報をグラフ表示するためにはgraphvizが必要です。Linuxでは
パッケージ化されているので導入してください。

2.1.5. llvm3.4
==============

LLVM 3.4の開発環境が必要です。Linuxではパッケージ化されているので、導入
してください。(CentOSではRedHatのEPELリポジトリから導入可能です)

また、各コマンドが実行できるよう、必要であれば PATH 環境変数の設定を行っ
てください。


3. 必要なテストデータ
=====================

11/29ご提供のmodels.zipを models 以下に展開してください。
使用するのはRTW-ECのCソース、ヘッダ、パラメタ初期値のファイルです。
XMLは、当該ディレクトリのxx.xmlを使用します。

SHIMのスキーマ shim-schema.xsd が必要です。(同梱しています)


3.1. XMLファイルについて
========================

データバインディングを使用するにあたり、ネームスペースの対応が必要にな
ります。

先頭のblocksは以下のように変更する必要があります。

  <blocks>
  ↓
  <sm:blocks
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://example.com/SimulinkModel SimulinkModel.xsd"
    xmlns:sm="http://example.com/SimulinkModel"
    name="pid_controller1">        // これは pid 用の名前です

これでXMLスキーマの名前空間はプレフィクス xsi をつけることになります。
またblocksはグローバルタグなので、名前空間smで扱います。それ以外の
blocksにもプレフィクスとして sm をつける必要があります。

  <blocks> → <sm:blocks>

blocksの終了タグも以下のように sm を付けます。

  </blocks> → </sm:blocks>

変更状況については、models.zipを展開後に例えば以下を比較してみてくださ
い。

  models/pid/pid_controller1.xml
  models/pid/xx.xml


名前空間の識別子として "http://example.com/SimulinkModel" を使用してい
ます。これを変更する場合、MakefileのSIM_URLを変更します。


4. コンパイル
=============

以下でコンパイルされます

  $ make

名前空間の識別子として "http://example.com/SimulinkModel" を使用してい
ます。これを変更する場合、MakefileのSIM_URLを変更して make しなおしてく
ださい。

名前空間を変更すると以下のファイルが変更されます。

  SimulinkModel.xsd
  SimulinkModel_ns.h

上記により、以下のファイルが影響を受けます。

  xsd_driver.cxx    (SimulinkModel_ns.hをインクルード)
  SimulinkRTW.h     (SimulinkModel_ns.hをインクルード)
  SimulinkRTW.cxx   (SimulinkRTW.hに依存)
  SimulinkModel.cxx (SimulinkModel.xsdから生成)
  SimulinkModel.hxx (SimulinkModel.xsdから生成)
  

5. テスト
=========

以下で(デフォルトではpidが)テストできます。

  $ make test

テスト対象を変更する場合、TEST_TARGETマクロを設定します。

  $ make TEST_TARGET=test1

現状 pid, test1, cell, testmux が指定できますが、cellはエラーになります。

問題は以下の通りです。

・cellはXMLのoutputタグにconnectがない問題がありエラー
・ブロック名が一致しない問題

なお、性能情報付きブロックレベル構造XMLを出力するテストを単独で実行する
場合、以下のようにしてください。(6.1.に記載のyy.xmlが必要です)

  $ make test_blxml


6. 結果確認
===========

6.1. 出力XML
============

サンプルプログラム rtw_test によってRTW-ECのコードを抽出したXMLファイル
を生成します(TEST_TARGET毎)。アウトプットは以下のものです。

  Simulink_XSD/models/pid/yy.xml
  (Simulink_XSD/models/cell/yy.xml) -- 生成できない
  Simulink_XSD/models/test1/yy.xml
  Simulink_XSD/models/testmux/yy.xml

実行にあたっては、入力となるXMLファイルと同じディレクトリに
SimulinkModel.xsd を置く必要があります。

以下に簡単にXMLのタグの説明します。内容についてはSimulinkXSD.xslx、
SimulinkModel.xsdを参照してください。

<sm:blocks> ブロックをまとめるタグです。smは名前空間のprefixです。

<block> ブロックを示します。属性は以下の通りです
   blocktype - タイプ
   name -- 名前

<output> 出力のline/portを示します。

<input> 入力のline/portを示します。

  <connect> output/inputの接続先blockとportを示します。

<var> 変数情報です。modeで(外部)入出力を識別、typeで型、nameが名前です

<state> 状態変数です。nameで名前、typeで型、storageが構造体名です

<param> パラメタです。(状態変数と同様)

<const_param> 定数パラメタです(同上)

<code> C/Headerのコードを保持します。typeでコードの種別を示します。
  task -- タスク実行部となるコードです
  update -- UnitDelayのコードです
  init -- タスク実行部に関わる初期化コードです
  state -- 状態変数の初期化コードです
  param -- パラメタの初期値です
  const -- 定数パラメタの初期値です

<forward> -- サブブロック間の接続を解消した後方依存フローのブロックです
             また、依存する変数を内部に <var> タグとして含みます。
  block -- 接続するブロック名
  type -- 接続種別で、現在 data/event/port があります

<backward> -- サブブロック間の接続を解消した前方依存フローのブロックです
  (forwardと同じ)


6.2. graphviz (dot形式)
=======================

上記、forward/backwardを元にテストコード xml_test が xml_test.dot を生
成します(TEST_TARGET毎に生成します)。これを用いると変数・ブロックの接続
関係を視覚的に確認できます。

  Simulink_XSD/models/pid/test_xml.dot
  Simulink_XSD/models/pid/test_xml.png

  (Simulink_XSD/models/cell/test_xml.dot) -- 生成できない

  Simulink_XSD/models/test1/test_xml.dot
  Simulink_XSD/models/test1/test_xml.png

  Simulink_XSD/models/testmux/test_xml.dot
  Simulink_XSD/models/testmux/test_xml.png


6.3. 性能付きブロックレベル構造XML(BLXML)
=========================================

上記の出力XMLのブロックのRTWのコード断片の性能値(サイクル数)を出力した
性能付きBLXMLを生成します(TEST_TARGET毎に生成します)。

  Simulink_XSD/models/pid/test_blxml.xml
  Simulink_XSD/models/test1/test_blxml.xml
  Simulink_XSD/models/testmux/test_blxml.xml

性能値はブロック毎に<performance>タグの属性として記載されます。性能値は、
コードの種類毎(task,update,init)に計測されます

他のテストを実施せず、単独でのこの生成手順は make test_blxml とします。


7. ユーザガイド(超概略)
=======================

7.1. データバインディング(SimulinkModel)
========================================

XSDによってSimulinkModel.xsdから SimulinkModel.hxx, SimulinkModel.cxx
が生成されます。これらの使い方については生成コードおよびdoxygenを参照く
ださい。名前空間(namespace)は SimulinkModel::XSD です。

サンプルコードとして xsd_driver.cxx がありますので、参考にしてください。

実行にあたっては、入力となるXMLファイルと同じディレクトリに
SimulinkModel.xsd を置く必要があります。


7.2 SimulinkXML
===============

SimulinkXML.h, SimulinkXML.cxxはXSDにより生成されるデータバインディング
の利用を簡単に支援するモジュールです。連想配列を用いた以下の機能があり
ます。名前空間(namespace)は Simulink::XML です。

・ブロック名のサーチ
  Readerクラスではブロック名をキーとした連想配列(map)になっているため、
  ブロック名からブロック情報をサーチすることが可能です。

・input/outputの接続先のサーチ

  input/outputのブロック名、line名、port名はポート名をキーとした連想配
  列(multimap)になっています。これを利用したサーチ関数が用意されており、
  input/outputに付属するconnectのline,portを指定し、それに接続した相手
  のブロックのタグとoutput/inputのタグを探す事ができます。

※ Cの外部リンケージの関数は未実装です

サンプルコードとしては、xml_test.cxx です。これはblockと
forward/backwardを元にgraphvizのdotファイルを生成します

実行にあたっては、入力となるXMLファイルと同じディレクトリに
SimulinkModel.xsd を置く必要があります。


8. 制限事項
===========

・global statesには対応していません
・step関数で、Simulinkのブロックが結合・分離される等の結果、if文やルー
  プ内などのCの構文ブロックの中にSimulinkのブロックが入っている場合等、
  正しくコードが切り出せない場合があります。
・initialize関数では、初期化コードは、単一変数あるいは配列要素の初期化、
  ループによる配列の初期化、memset()による初期化に対応していますが、よ
  り複雑なコードの場合、初期化コードが正しく切り出せない場合があります。
・変数の型で、構造体を使うようなものは、構造体の型の切り出しは行いませ
  んので、Cのコード生成を行う場合、別途、構造体を定義するなどの対応が必
  要です。
・Demuxは p=n で、配列サイズと分割数が一致するもののみ取り扱います
・SimulinkのモデルでInport/Outport/Demux/Muxのみを認識し、それらをまた
  がる変数の伝播を追跡しますが、それ以外のブロックは<input>/<output>の
  情報を使用します。したがって、上記ブロックタイプ以外でブロックで信号
  や変数に変化がある場合、追跡できないケースが生じます。
・Constantは独立したブロックとして取り扱います(他のブロックと結合するこ
  とはない)。
・最も外側にあるInport/Outportは開始・終端のブロックとして使用します。


以上
