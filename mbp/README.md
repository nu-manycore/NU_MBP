MBP評価環境

# はじめに

これはMBPの評価環境です。

この評価環境では、MBPコマンドを使用することで、
Simulinkのモデルから構造を抽出したBLXMLファイルと、
Embedded Coderが生成したCコードを入力することで、
TOPPERS/ATK2向けのコードを生成することできます。

---

# MBPの環境の場所

MBPの環境は /opt/MBP にあります。

アカウントmbpでログインした場合、.bash\_profileにより、
PATH環境変数の最初に/opt/MBP/binが設定されます。

## /opt/MBP/bin

MBPで使用する各プログラムが配置されます

## /opt/MBP/libexec

MBPで使用する動的ライブラリ等のファイルが配置されます

## /opt/MBP/share

MBPで使用する様々なファイルが配置されます

### /opt/MBP/share/schemas

MBPで使用するXMLスキーマが配置されます。

## /opt/MBP/atk2

これは、TOPPERS/ATK2向けのコードをpthread環境で実行するための、
簡易エミュレーション機能の配置場所です。  
概要は
[TOPPERS/ATK2のコードのpthreadエミューレーション簡易環境](#ATK2EMU)
を参照してください。

## /opt/MBP/FlowVisualizer

実行順序決定ツールおよびガントチャート生成ツールです。

---

# MBPの使い方

MBPコマンドを使用します。コマンド形式は以下の通りです。

	usage: MBP [-v] [-l logfile] \
		-c model.c -h model.h [-d model_data.c] \
		-S SHIM.xml \
		-N numberOfCore \
		-I modelSourceDirectory \
		[-F] \
		[-i include-header] \
		入力BLXML

- **-v**  
  verboseオプションです。  
  実行するコマンドの情報を "+" をポストフィクスとして表示します。
- **-l** \(小文字のエル\[L\]\)  
  各コマンドの標準出力を指定したファイルへ出力します
- **-c**  
  Embedded Coderが生成したモデルのCソースファイルを指定します。  
  後述の **-I** オプションで指定したパス部分はファイル名から省略可能です。
- **-h**  
  Embedded Coderが生成したモデルのヘッダーファイルを指定します。  
  後述の **-I** オプションで指定したパス部分はファイル名から省略可能です。
- **-d**  
  Embedded Coderが生成したモデルのパラメタ等のデーターファイルを指定します。
  このオプションは、データファイルがない場合、省略してください。  
  後述の **-I** オプションで指定したパス部分はファイル名から省略可能です。
- **-S**  
  性能計測に用いるSHIMを指定します。
- **-N**  
  コア数を指定します。
- **-I** \(大文字のアイ\[I\]\)  
  Embedded Coderが生成したモデルのソースディレクトリや、
  その他、必要なヘッダファイル等のディレクトリを指定します。
  \(複数指定可能\)  
  **-c**, **-h**, **-d**
  オプションで指定したファイルの検索するパスとしても使用します。
- **-F**  
  FlowVisualizerを実行します。
- **-i** \(小文字のアイ\[I\]\)  
  コード生成時に、インクルードさせるファイルがある場合に指定します。
  \(複数指定可能\)  
  これは例えば、生成したコードからビルドした実行ファイルの
  実行時の動作確認やデバッグのための定義を含んだヘッダファイルです。
- **入力BLXML**  
  Simulinkモデルから抽出された入力BLXMLファイルです。

MBPコマンドにより、TOPPERS/ATK2向けの以下のファイルが生成されます。

- **モデル名**-app.h  
  タスクの優先度やスタックサイズのデフォルトが定義されたファイルです。  
  必要な変更を行ってください。
- **モデル名**-app.yaml  
  TOPPERS/ATK2で使用するabrex用のYAMLファイルです。
  このファイルは、上記の、モデル名-app.h をインクルードします。
- **モデル名**-output.c  
  TOPPERS/ATK2用に生成されたタスクのコードです

これらはTOPPERS/ATK2の開発環境でビルドするために使用できます。

また、以下のファイルは、中間的に作成されたファイルです。
なお、ここでは入力BLXMLファイル名を "**入力BLXML**.xml" とします。

1. コード情報付きBLXML生成
	- **入力BLXML**\_code.xml
2. 性能情報付きBLXML生成
	- **入力BLXML**\_perf.xml
3. コア割り当て付きBLXML生成
	- tmp\_results.csv
	- _blgraph.dot
	- **入力BLXML**\_result.xml
	- tmp\_sch\_results.csv
4. 並列実行コード生成
	- **入力BLXML**\_result.dot
5. FlowVisualizer
	- setting.yaml
	- outputs/output\_rate\_\*

---

# <a id="ATK2EMU">TOPPERS/ATK2のコードのpthreadエミューレーション簡易環境</a>

生成されたTOPPERS/ATK2向けのタスクのコードを
pthread環境で実行するための、簡易エミュレーション機能があります。  
詳細は _/opt/MBP/atk2/README.md_ を参照してください。

## 使い方

1. _/opt/MBP/atk2/Makefile.tmpl_ を _Makefile_ にコピー
2. _Makefile_ の以下の部分を編集する
	- __ATK2\_MODEL\_DIR__  
	  Embedded Coderが生成したモデルのソースディレクトリを設定します。  
	  \(このパスは MBP コマンドの -I オプションで使ったものと
	  同じである必要があります\)
	- __ATK2\_ABREX\_YAML__  
	  MBP コマンドで生成した "**モデル名-app.yaml**" を設定します
	- __ATK2\_SRCS__  
	  MBP コマンドで生成した "**モデル名-output.c**" を設定します。  
	  また、Embedded Coder が生成した、
	  リンクする必要のあるファイルを指定してください。
	- __ATK2\_PROGRAM__  
	  実行プログラム名を設定します。
	  "**モデル名-output**" とするとよいでしょう。
3. その他必要な設定を行う
4. make

以上で実行形式のファイル\(__ATK2\_PROGRAM__ で指定\)が生成されますが、
元々生成されたコードはモデルの動作を行うだけですので、
実行確認のためには、別途、値や動作状況を出力するといった対応が必要です。

これについては、pthread向けの動作確認やデバッグの為に、
あらかじめヘッダファイルを作成しておき、
MBPコマンドの **-i** オプションでそれを指定するようにすると、
生成されたコードを修正する必要がありません。
