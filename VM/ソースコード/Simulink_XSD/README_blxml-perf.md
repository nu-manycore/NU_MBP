<!-- -*- mode: markdown; coding: utf-8-unix; -*- -->

<style type="text/css">
table { border-collapse: collapse; }
table, th, tr, td { border: 1px solid black; }
code { color: darkgreen; }
pre { background-color: gainsboro; }
</style>

# <a id="BlxmlPerfSh">blxml-perf.shの使用方法</a>

# はじめに

**blxml-perf.sh** は __rtw\_test__ の出力した
**コード情報付きブロックレベル構造XML\(BLXML\)** と、
ターゲットアーキテクチャを記述した __SHIM__ を入力し、  
ブロックのコードの性能を算出し、ブロックに性能情報を付加した
**性能情報付きBLXML** を出力します。

**SHIM** についての詳細は以下を参照してください。  
<https://www.multicore-association.org/workgroup/shim.php>

また、SHIMのスキーマの入手や、ツールの情報は以下を参照してください。  
<https://github.com/openshim/shim>

なお、本コマンドを構成するプログラム及びモジュールの個別情報については、
[プログラムの構成](#Program) を参照してください。

---

# <a id="Synopsis">使用方法</a>

コマンドの書式は以下の通りです。

		blxml-perf.sh [-v] [-o output] [-shim=SHIM] [-shim-schema=SCHEMA] \
				[-C clang-option] [-O pass-option] [-S pass-module] INPUT-XML

## オプション

- **-v**  
  verboseオプションです。  
  オプションを指定した場合、プログラムの実行を表示します
- **-o** _output_  
  出力する **性能情報付きBLXML** のファイル名を指定します。  
  オプションを省略した場合、標準出力に出力します
- **-shim**=_SHIM_  
  性能の算出に用いる **SHIM** のファイル名を指定します。
  オプションを省略した場合や **-no** を指定した場合、  
  命令のレイテンシが取得できない場合、  
  命令のレイテンシにはダミー値を使用します。
- **-shim-schema**=_SCHEMA_  
  **SHIM** ファイルの整合性確認\(バリデーション\)を行う **SHIMのスキーマ**
  を指定します。  
  オプションを省略した場合、整合性確認を行いません
- **-C** _clang-option_  
  コンパイラとして使用する **clang** に与えるオプション\(_-O_,_-I_ やその他\)
　を指定するために用います。  
  なお **-C** に続けて **clang** のオプションを指定しますが、
  元々複数のコマンドパラメタで構成される場合は、
  それぞれについて **-C** を使用する必要があります。  
  \(例えばオプション指定が _-A_ _B_ の場合は **-C** _-A_ **-C** _B_ とする\)
- **-O** _pass-option_  
  **LLVM** の **opt** のpassのオプションを指定するために用います。  
  **opt** がロードした **計測用passモジュール** のオプションや、
  他の機能のオプションが指定可能です。  
  **計測用passモジュール** のオプションの詳細は
  [llvm-BLXMLPerf.so](#LLVMBLXMLPerf) の[オプション](#LLVMBLXMLPerfOption)
  を参照してください。  
  なお **-O** に続けて **opt** のオプションを指定しますが、
  元々複数のコマンドパラメタで構成される場合は、
  それぞれについて **-O** を使用する必要があります。  
  \(例えばオプション指定が _-A_ _B_ の場合は **-O** _-A_ **-O** _B_ とする\)
- **-S** _pass-module_  
  **LLVM** の __計測用passモジュール__ を指定します。  
  省略時は [llvm-BLXMLPerf.so](#LLVMBLXMLPerf) です。


### 使用例

	blxml-perf.sh -v -o output_blxml.xml -shim=SHIM.xml \
		-C -Ipath_to_model_code \
		-C -O \
		input_blxml.xml

- _output\_blxml.xml_  
  出力する **性能情報付きBLXML** を指定します
- _SHIM.xml_  
  性能の算出に用いる **SHIM** のファイルを指定します。  
- _path\_to\_model\_code_  
  Embedded Coderが生成したコードがあるパスを指定します。  
  ここでは **clang** に与えるインクルードパスのオプションとして使用します。
- _input\_blxml.xml_  
  入力となる **コード情報付きBLXML** を指定します。

### 補足

- **clang**のオプティマイズを行う場合とそうでない場合では、
  ループの構造に差異が出て、
  固定回数あるいは最大回数がわからないループになるケースがあります。  
  その場合、ループのイテレーションの処理量は、一回だけ回る分だけが
  性能情報に含まれることになります。  
  一般に **-C** _-O_ として最適化を実行した方が良好な結果が得られます。  
  また **opt** の**mem2reg**の最適化パスを実行した方が、
  ループの回数を適切に検出できるようです  
  \(その場合 **-O** _-O_ **-O** _-mem2reg_ を指定する\)
- **-O** _-MasterComponent=name_, **-O** _-CommonInstructionSet=name_
  を与えない場合、最初に見つける **CommonInstructionSet** を使用します。  
  詳細は[llvm-BLXMLPerf.so](#LLVMBLXMLPerf)の
  [注意](#LLVMBLXMLPerfNotice)を参照してください

## 環境変数

以下の環境変数を設定することで、本コマンドから使用する、各種コマンドを
変更・指定する事ができます。

- *MBP\_ROOT*  
  [blxml2code](#blxml2code)と[llvm-BLXMLPerf.so](#LLVMBLXMLPerf)
  の探索に使用するディレクトリを指定します。  
  なお、後述する *BLXML2CODE* および *LLVM\_BLXMLPERF* の環境変数は、
  これらの探索より優先されます。
	- この環境変数を設定した場合、以下のものを使用します。
		- *$\{MBP\_ROOT\}/bin/blxml2code*
		- *$\{MBP\_ROOT\}/libexec/llvm-BLXMLPerf.so*  
	- この環境変数を設定しない場合、以下のものを使用します。
		- *blxml-perf.shがあるディレクトリ/../bin/blxml2code*
		- *blxml-perf.shがあるディレクトリ/../libexec/llvm-BLXMLPerf*
- *BLXML2CODE*  
  使用する[blxml2code](#blxml2code)のコマンド名またはパス名を指定します。  
  \(パス要素がなく\)コマンド名を指定した場合、
  実行するコマンドの探索はPATH環境変数に従います。
- *CLANG*  
  使用する **clang** のコマンド名またはパス名を指定します。  
  \(パス要素がなく\)コマンド名を指定した場合、
  実行するコマンドの探索はPATH環境変数に従います。
- *LLVM\_OPT*  
  使用する **LLVM** の **opt** のコマンド名またはパス名を指定します。  
  \(パス要素がなく\)コマンド名を指定した場合、
  実行するコマンドの探索はPATH環境変数に従います。
- *LLVM\_BLXMLPERF*  
  使用する[llvm-BLXMLPerf.so](#LLVMBLXMLPerf)のパス名を指定します。

---

# <a id="Program">プログラムの構成</a>

本プログラムは、以下の三つのプログラムまたはモジュールから構成されます。
各プログラム・モジュールの個別の情報は以下の通りです。

1. [blxml-perf.sh](#BlxmlPerfSh)  
   **コード情報付きBLXML** のブロックのコード断片の性能を算出し、
   結果を反映した **性能情報付きBLXML** を出力する為の、
   一連の処理を実行するシェルスクリプトです。  
   詳細は[使用方法](#Synopsis)を参照してください。
2. [blxml2code](#blxml2code)  
   **コード情報付きBLXML** のブロックのコード断片から、
   コンパイル可能なCコードを生成するプログラムです。  
   詳細は[blxml2code](#blxml2code)を参照してください。  
   生成したコードは **clang** で **LLVM** の中間コードへコンパイルし、
   後述するLLVMのPass([llvm-BLXMLPerf.so](#LLVMBLXMLPerf))で処理することで、
   性能が算出されます。
3. [llvm-BLXMLPerf.so](#LLVMBLXMLPerf)  
   **blxml2code** が生成したコードから **clang** が生成した
   **LLVM** の中間コードと **SHIM** に記述された命令のレイテンシから、
   __コード情報付きBLXML__ のブロックのコードに対応する関数の
   性能を算出し、その結果を反映した**性能情報付きBLXML** を出力する
   **LLVM** の **opt** のPassです。  
   詳細は[llvm-BLXMLPerf.so](#LLVMBLXMLPerf)を参照してください。

---

## <a id="blxml2code">blxml2code</a>

**blxml2code** は __rtw\_test__ の出力した
**コード情報付きブロックレベル構造XML\(BLXML\)** を入力し、
性能算出用のCコードを生成するプログラムです。

このプログラムは、静的に性能を算出する
[blxml-perf.sh](#BlxmlPerfSh) から実行されますので、
性能算出の全体像は[blxml-perf.sh](#BlxmlPerfSh)を参照してください。

### 使用方法

コマンドの書式は以下の通りです。

    blxml2code [-n] [-v] [-o output] [-N] [-H user-header] input-BLXML

### オプション

- **-n**  
  このオプションを指定した場合、スキーマ\(_SimulinkModel.xsd_\)
  との整合性をチェックしません。  
  このため現在の作業ディレクトリに _SimulinkModel.xsd_ は不要ですが、  
  もしもスキーマと入力するBLXMLとが不整合であった場合、
  プログラムの異常動作や、不正な出力BLXMLの出力となる事があります。  
  一方、このオプションを省略した場合、スキーマと入力するBLXMLの整合性を
  チェックし、不整合の場合は、プログラムはエラー終了します。  
  なお、現在の作業ディレクトリに _SimulinkModel.xsd_ が必要です
- **-v**  
  **このオプションは現在は何もしません。**  
  verboseオプションです。
- **-o** _output_  
  性能算出用コードの出力ファイル名を指定します。  
  このオプションを指定しない場合、標準出力にコードを出力します
- **-N**  
  **このオプションは、現在は特殊用途向けのため使用しないでください。**  
  このオプションを指定した場合、ブロックで使用する変数を、
  生成する関数のパラメタとして出力しません。
  デフォルトでは、変数を生成する関数のパラメタとして出力します。
- **-H** _user-header_ 
  生成するCコードに、指定したファイルをプリプロセッサの
  **#include** 文として出力します。  
  通常、コードが必要とするヘッダーファイルは、入力するBLXMLにあるものを
  **#include** 文として出力しますが、それ以外に入力したい
  ヘッダーファイルがあれば、指定してください
- _input-BLXML_  
  __rtw\_test__ コマンドにより出力された、コード情報付きBLXMLを指定します

### 生成コードについて

生成したコードは、ブロックのコードを関数化し、
コンパイル可能にしたものに過ぎません。

概要は以下の通りです。

- Cソースファイルから抽出され、BLXMLに格納されているもの
	+ Cソースファイルに宣言されているマクロ定義
	+ Cソースファイルに宣言されている構造体
	+ Cソースファイルに宣言・定義されている外部変数、静的変数
	+ Cソースファイルに宣言・定義されている関数
- コンパイルに必要な、ブロックの状態変数、パラメタ等の宣言・定義
- 性能を算出する単位に出力する個別の関数
	+ ブロックの初期化コード
	+ ブロックの実行コード
	+ 遅延ブロックの場合のUpdateのコード

---

## <a id="LLVMBLXMLPerf">llvm-BLXMLPerf.so</a>

__BLXML__ のブロック毎のコード\(task,update,init\)単位の性能を計測し、
**性能付きBLXML** を出力するために利用する **LLVM** のPassです。

### 本Passの使用方法

本Passは以下のように **LLVM** の **opt** から実行します。

	opt -load=<path for llvm-BLXMLPerf.so> -S -blxml-perf \
	    [-shim=SHIM] [-shim-schema=SCHEMA] \
	    [-MasterComponent=name] [-CommonInstructionSet=name] \
	    [-no-ret] [-no-br] \
	    [-in-xml=INPUT.XML] [-out-xml=OUTPUT.XML] \
	    -o dummy_output.ll input.ll

### <a id="LLVMBLXMLPerfOption">オプション</a>

- **-shim**=_SHIM_  
  性能の算出に用いる **SHIM** のファイル名を指定します。  
  以下の場合は、命令のレイテンシにダミー値を使用します。
	+ オプションを省略した場合や "**-no**" を指定した場合
	+ **SHIM** に算出に用いる **CommonInstructionSet** が存在しない場合
	+ 命令に対するレイテンシが設定されていない場合
	+ ダミー値は以下の通りです
		- best: 1
		- typical: 1.5
		- worst: 2
- **-shim-schema**=_SCHEMA_  
  **SHIM** ファイルの整合性確認\(バリデーション\)を行う **SHIMのスキーマ**
  を指定します。  
  オプションを省略した場合、整合性確認を行いません
- **-MasterComponent**=_name_  
  性能の算出対象となる **SHIM** の **MasterComponent** を指定します。  
  オプションを省略した場合については
  [注意](#LLVMBLXMLPerfNotice)を参照してください。
- **-CommonInstructionSet**=_name_  
  性能の算出対象となる **SHIM** の **CommonInstructionSet** を指定します。  
  オプションを省略した場合については
  [注意](#LLVMBLXMLPerfNotice)を参照してください。
- **-no-ret**  
  **LLVM** のRET命令のサイクルを無視します。  
  ブロックのコードの断片は **blxml2code** が一つの関数として生成するため、
  RETのオーバヘッドを考慮したくない場合に使用します
- **-no-br**  
  基本ブロック間の分岐命令のサイクルを無視します。  
  **条件分岐などは算出対象とすべきなので、このオプションは非推奨です。**
- **-in-xml**=_INPUT.XML_  
  入力とする **コード情報付きBLXML** のファイル名を指定します。
- **-out-xml**=_OUTPUT.XML_  
  出力する **性能情報付きBLXML** のファイル名を指定します。
  このオプションを省略した場合 **性能情報付きBLXML**
  は標準出力に出力します。
- **-out-dot**=_basename_  
  **このオプションはデバッグ用なので、通常使用しません。**
  **詳細は**
  [dotファイル](#LLVMBLXMLDot)
  **を参照してください。**  
  このオプションを指定した場合、ブロックの性能の算出に用いたグラフ構造を
  ループおよび関数毎に順に **GraphViz** の **dot** 形式で出力します。  
  出力するファイル名は指定した **basename** を元に、
  _basename-&lt;番号&gt;.dot_ となります。

### <a id="LLVMBLXMLPerfNotice">注意</a>

1. 命令のレイテンシを取得する**SHIM**の**CommonInstructionSet**
   は次のように特定します。
	- 探索対象とする*MasterComponent*
		- *MasterComponent*が指定されている場合は、
		   当該*MasterComponent*を探索対象とする。
		- *MasterComponent*が指定されていない場合は、
		   すべての*MasterComponent*を探索対象とする
	- 対象とする*CommonInstructionSet*
		- *CommonInstructionSet*が指定されている場合、
		  探索対象の*MasterComponent*の当該
		  *CommonInstructionSet*を対象とする
		- *CommonInstructionSet*が指定されていない場合、
		  探索対象の*MasterComponent*で最初に見つけた
		  *CommonInstructionSet*を対象とする

### <a id="LLVMBLXMLDot">dotファイル</a>

**dot** ファイルは **GraphViz** のファイルで、
処理量を計算した際の経路探索結果です。
関数単位の処理としては最内ループから出力し、
最後にコード全体\(関数\)のグラフになります。

ノードは **LLVM** の基本ブロック、またはまとめられた固定回回るループです。

- 赤枠\(red\)かつ塗りつぶされているノード\(plum\)は、
  固定回回るループを一つにまとめたもの
- 青枠\(blue\)のノードは、最大回数がわかっているループのヘッド\(基本ブロック\)
- それ以外のノードは通常の基本ブロック
- boxのノードは、ループのヘッドへのbackedgeを示すためのダミーノード
- 内部の数字は上から順に、そのノードのbest/typical/worstの処理サイクル
- 最後の\[\]内の数字は、終端までの平均処理時間です
  \(successorの各処理時間と等確率分岐によって平均化した処理時間\)

エッジはsuccessorになります。

- 緑\(green\)は最短経路
- 青\(blue\)は最長経路。最長経路が最短経路と重なる場合は、最短経路の表示が優先
- 赤\(red\)の破線はループのバックエッジ
- 黒の破線はダミーノードへの接続を示す
- 数字はヘッドのノードの処理時間を経路探索のための距離\(distance\)で、
  上から順にbest/typical/worstとなる

### 処理概要

本Passはつぎのようになっています

1. FunctionPassを継承しており、関数単位に処理を行う
2. 事前に LoopInfo Passと ScalarEvolution Passを実行する
3. 初期化時にSHIMとブロックレベル構造XMLを読み込む
4. 終了時に性能付きブロックレベル構造XMLを出力する

関数単位の処理は以下のようになっています。

1. 基本ブロックのポインタをキーとした連想配列を作成\(付随情報の保持のため\)
2. 基本ブロックのサイクル数\(best/typical/worst\)を算出し、連想配列に保存
3. LoopInfoとScalarEvolutionを元にループの処理を実施
4. 基本ブロックのsuccessorおよび基本ブロックをまとめたループの情報
   \(LoopInfoによる連想配列\)から、有向グラフを作成する
   \(boostのグラフライブラリを使用する\)
5. 最短経路/best、最長経路/worst、分岐確率50%/typicalによる経路探索を実施し、
   関数のサイクル数\(best/typical/worst\)を算出し、関数の情報として保存

ループの処理はつぎのように行います

1. LoopInfo最内ループから処理するよう再帰処理する
2. ScalarEvolutionの結果から固定回回るループなら、
   ループに含まれる基本ブロックを一つのグラフとして下記それぞれを算出し
   best/worst/typicalともループ回数分加算する。
	+ best: グラフの最短経路のサイクル数
	+ typical: 分岐確率50%のサイクル数
	+ worst: グラフの最長経路のサイクル数
3. 最大回数がわかるループ\(途中で抜け出すことがある\)なら、
   ループに含まれる基本ブロックを一つのグラフとして、
   ループ内の基本ブロックのサイクル数をbest/typical/worst別に加算する。  
   ただし、worstだけループ回数分加算し、best/typicalは一回だけ回るものとする。
4. 上記のループの基本ブロックの情報をLoopInfo
   のポインタをキーとした連想配列に保存する
5. 上記いずれでもないループは、基本ブロックをループとしてまとめない
   \(つまり、一回だけ回るものとして取り扱うことになる\)

ループ内の基本ブロックも関数全体についても、
グラフ探索はつぎのように行います。

1. best値をエッジに距離として設定し
   dijkstraのアルゴリズムで最短経路探索を行う\(boostのアルゴリズムを使用\)
2. worst値の負数をエッジに距離として設定し、
   bellman-fordのアルゴリズムで最短経路探索を行う\(結果として最長経路になる\)。
   \(boostのアルゴリズムを使用\)
3. 分岐確率50%\(同確率分岐\)のtypicalの探索は、
   再帰的なdepth first探索で行う。  
	- ループを検出\(GRAY\)した場合は探索を打ちきる
	  \(つまりその分岐は当たらない\)
	- 到達済みノード\(BLACK\)であれば、
	  下位ノードの計算済みのサイクルを加算して行く
	- 最後に分岐数で割ってサイクル数とする
