<!-- mode: text; coding: utf-8-unix; -*- -->
<!--
このファイルにはテーブルがあります。
markdown_pyを使用する場合は -x tables を指定してください。
-->
<style type="text/css">
table { border-collapse: collapse; }
table, th, tr, td { border: 1px solid black; }
code { color: darkgreen; }
pre { background-color: gainsboro; }
</style>


# CSPトランスレータの使い方

# はじめに

**CSPトランスレータ** は、 __コード情報付きBLXML__ を入力し、  
一般的なCSP、および以下のコード生成を行うトランスレータです。

- **TOPPERS/ATK2**\(以下ATK2\)用のCコード
- 名古屋大学の **SystemBuilder** 用Cコード\(ATK2用Cコード含む\)
- RH850向け **eSOL社eMCOS**\(以下eMCOS\)用のCコード

なお、以下ではATK2向けコード生成機能を **A-MBP**、  
SystemBuilder用コード生成機能を __S-MBP__ とします。

1. コマンドの使用方法については[使用方法](#Synopsis)を参照してください。
2. 生成コードで使用されるマクロについては、以下を参照してください。
	- [生成コードで使用されるマクロ](#CodeMacros)
		1. [(ATK2)実行環境・開発環境対応マクロ](#Atk2Macros)
		2. [(SystemBuilder)実行環境・開発環境対応マクロ](#SmbpMacros)
		3. [(eMCOS)実行環境・開発環境対応マクロ](#eMcosMacros)
3. デバッグ支援コマンドについては以下を参照してください
	 - [デバッグ支援コマンド](#DebugSupport)
		1. [(ATK2)デバッグ支援コマンド](#Atk2DebugSupport)
		2. [(eMCOS)デバッグ支援コマンド](#eMcosDebugSupport)

また、ATK2用のコードは、pthread環境で動作をエミュレーション可能です。  
詳細は **TOPPERS/ATK2用pthreadエミュレーション環境** のドキュメントを参照してください。

---

# <a id="Synopsis">使用方法</a>

コマンドの書式は以下の通りです。

1. **A-MBP**\(ATK2用\)
    <pre><code>csp_translator -Tatk2 -s model.xml \
    [-v] [-i filename] [-o output.dot] [-X output.c] \
    [-y output.yaml] [--atk2-appmode=APPMODE ] [-H output-app.h] \
    [--atk2-timer-isrref=IST] [--atk2-no-timer-isrref] \
    [--atk2-timer-counterref=COUNTER] [--atk2-timer-hz=HZ] [--atk2-gen-main]</code></pre>
2. **S-MBP**\(SystemBuilder用\)
    <pre><code>csp_translator -Tsmbp -s model.xml \
    [-v] [-i filename] [-o output.dot] [-X output.c] \
    [--config-file=config.yaml] \
    [--smbp-prefix=SMBPPREFIX] [--smbp-yaml=smbp-output.yaml] \
    [-y output.yaml] [--atk2-appmode=APPMODE ] [-H output-app.h] \
    [--atk2-timer-isrref=IST] [--atk2-no-timer-isrref] \
    [--atk2-timer-counterref=COUNTER] [--atk2-timer-hz=HZ] [--atk2-gen-main]</code></pre>
3. eMCOS用
    <pre><code>csp_translator -Tmcos -s model.xml \
    [-v] [-i filename] [-o output.dot] [-X output.c]</code></pre>

## <a id="Options">オプション</a>

- __-s__ *モデル.xml*  
  __--xml-csp__=*モデル.xml*  
  入力するモデルのBLXMLの指定
- __-v__  
  __--verbose__  
  verboseモードにする。
  色々な情報が出力されるので、デバッグや入力内容の確認に使用する
- __-T__ *ターゲット*  
  __--target__=*ターゲット*  
  コード生成のターゲットを指定する。  
  *ターゲット*は*atk2*と*smbp*
  と*mcos*
  が使用可能
- __-N__ *コア数*  
  __--number-of-core__=*コア数*  
  コア数を指定する。デフォルトは8、最大は256である。
- __-X__ *出力ファイル*  
  __--output-code__=*出力ファイル*  
  コード出力ファイルを指定する。  
  オプションを指定しない場合、
  出力ファイルは、"*ターゲット*\_default.c" となる。
- __-o__ _dotファイル_  
  __--output-csp__=*dotファイル*  
  出力する一般的CSPイメージファイルのファイル名指定する。
  本出力ファイル形式はdotである。  
  省略時は標準出力に出力する
- __--label-only-task-id__  
  CSPイメージファイル\(dot形式\)のラベルをCSPプロセスのタスク番号だけにする
- __-d__ *解析結果ファイル*  
  __--output-var__=*解析結果ファイル*  
  指定した解析結果ファイルに、各タスク毎に、
  入力変数、出力変数、タスクコードをCSV形式で出力する
- __-i__ _ファイル_  
  __--c-include__=*ファイル*  
  Cコード中に指定した*ファイル*のinclude文を出力する。
  このオプションは複数回指定でき、指定した順序でCコードに出力する
- __-P__ *param=val*  
  __--set-param__=*param=val*  
  ブロックのサンプル時間にシンボルが使われている場合、
  そのシンボルの値を指定する
- __--xsd-validation__  
  スキーマと入力BLXMLファイルのvalidationを行う。  
  デフォルトでは行わない
- __--schema-loc__=*スキーマ*  
  validationするスキーマの場所を指定する。  
  このオプションは自動的に **--xsd-validation** を有効にする  
  なお、デフォルトではカレントディレクトリの
  _SimulinkModel.xsd_ を使用する。  
  _スキーマ_  には以下が使用できる。
    1. URIとして指定する場合は、"file://"を付けた絶対パス
    2. 先頭が '/' で始まった絶対パス
    3. 環境変数 _MBP\_SCHEMAS_ が設定されていれば、
       そのディレクトリにあるファイル名が一致するファイル
    4. カレントディレクトリからの相対パス
- [__--config-file__=*設定ファイル*](#ConfigrationFile)  
  CSPトランスレータに追加で与える設定ファイルを指定する。  
  デフォルトでは設定ファイルは読み込まない  
  詳細は[設定ファイル](#ConfigrationFile)を参照
- __--create-t0__=\[*ny*\]  
  __*mcos*__
  __以外では使用しない事を推奨します。__
  全体同期を行うtask0\(t0\)を生成するかどうかを指定する。  
  *n* の場合はtask0を生成しない。デフォルトは *n*。  
  *y* の場合はtask0を生成する
- __--ignore-to-sink__=\[*yn*\]  
  *y* の時、Sinkブロック\(例えばToWorkSpace\)を削除する。デフォルトは *y*。  
  *n* の時は削除しない。
- __--ignore-isolated__=\[*yn*\]  
  *y* の時、孤立したブロックを削除する。デフォルトは *y*。  
  *n* の時は削除しない。
- __--ignore-leaves__=\[*ny*\]  
  *n* の時は削除しない。デフォルトは *n*。  
  *y* の時、出力のないブロックを削除する。
- __--enable-multirate__  
  マルチレートでのコード生成を有効にする。
  ブロックは異なるレートで動作する。  
  デフォルトで有効
- __--disable-multirate__  
  マルチレートでのコード生成を無効にする。
  ブロックはベースレートで動作する。
- __--low-rate-pulsegenerator__=\[*yn*\]  
  *y* の時、DiscretePulseGenerator のレートを、PeriodとPulseWidthから決定し、
  DiscretePulseGeneratorのコードを書き換える。デフォルトでは *y*。  
  *n* の時はベースレートでの動作となる。
- __--no-gcc__  
  GNU C拡張を使わないようにして、入れ子関数やそれに付随する変数等は
  タスク関数外に出力する。  
  デフォルトでは無効\(gcc拡張を使用する\)。
- __--staticalize__  
  タスク関数内に生成する入出力変数、状態変数、パラメタ等は通常自動変数だが、
  これをstatic宣言する。  
  デフォルトでは無効。  
  これは、自動変数ではスレッドのスタックサイズが大きくなりすぎる場合に
  使用するが、自動変数とは異なり、コンパイラの最適化が期待できないため、
  実行性能が低下する可能性がある。
- __--enable-csr__  
  eMCOSのCSR\(Cyclic Service Routine\)を用いて、
  異なるサンプル時間のブロックを駆動する。
  ただし、周期は最小で1msなので、実行時間が長くなることが予想される。
- __--disable-csr__  
  異なるサンプル時間のブロックの駆動に、eMCOSのCSR\(Cyclic
  Service Routine\)を用いない。ブロックは、ブロック間の周期比の
  入力で駆動される。\(デフォルトでは **--disable-csr**\)
- __--atk2-appmode__=*AppMode*  
  ATK2において、モデルのAppModeを指定する。  
  デフォルトでは、"*モデル名\_AppMode*"である
- __-y__ *出力YAMLファイル*  
  __--atk2-abrex-yaml__=*出力YAMLファイル*  
  ATK2において、出力するABREX用のYAMLファイルを指定する。  
  デフォルトでは、"*モデル名*-app.yaml"である
- __-H__ *出力ヘッダファイル*  
  __--atk2-app-header__=*出力ヘッダファイル*  
  ATK2において、出力するヘッダファイルを指定する。  
  デフォルトでは、"*モデル名*-app.h"である
- __--atk2-timer-isrref__=*ISR*  
  Core0に設定するタイマ用ISRを指定する。  
  デフォルトでは、"*SwCntTimerHdr*"であり、YAMLには、
  "*/Ecuc/Os/SwCntTimerHdr*" と出力する
- __--atk2-no-timer-isrref__  
  Core0にタイマ用のISRを出力しない。  
  デフォルトでは出力する
- __--atk2-timer-counterref__=*COUNTER*  
  アラーム用のカウンタを指定する。  
  デフォルトでは、"*SwCnt0*"である。  
  たとえは*MAIN\_HW\_COUNTER\_CORE0* といったハードウェアカウンタを
  使用する場合、 __--atk2-no-timer-isrref__ と共に使用する必要がある。  
  カウンタの周波数は*カウンタ名*に、"*\_TIMER\_CLOCK\_HZ*"
  をポストフィクスしたマクロによって定義される必要がある
- __--atk2-timer-hz__=*タイマの周波数*  
  タイマの周波数がマクロ定義されていない場合に使用する周波数を指定する。  
  Cコンパイラで評価されるので指定するものは数値に限らない。  
  デフォルトでは1000である。
- __--atk2-gen-main__  
  main関数およびStartupHookを出力する場合に指定する。  
  デフォルトでは出力しない
- __--smbp-prefix__=*プレフィクス*  
  SystemBuilderにおいては出力ファイルのプレフィクスを指定する。  
  デフォルトでは、"*モデル名*"である。
- __--smbp-yaml__=*出力YAMLファイル*  
  SystemBuilderにおいては出力YAMLファイル名を指定する。  
  デフォルトでは、"*プレフィクス*-smbp.yaml"である
- __--smbp-long64__  
  SystemBuilderにおいては例えばLP64のような環境で、
  longが64bitの場合に指定する。  
  デフォルトでは long は32bitとして扱う
- __--smbp-real64__  
  SystemBuilderにおいて、Embedded Coderの real\_T
  が double\(64bit\) の場合に指定する。  
  デフォルトでは float\(32bit\) である。
- __--smbp-long64-align__=*バイト数*  
  64ビット整数のアラインメントを指定する。  
  デフォルトでは4バイト
- __--smbp-real64-align__=*バイト数*  
  64ビット浮動小数点\(double\)のアラインメントを指定する。  
  デフォルトでは4バイト
- __--smbp-struct-align__=*バイト数*  
  構造体のアラインメントを指定する。  
  デフォルトでは4バイト
- __--smbp-mem-align__=*バイト数*  
  SystemBuilderのメモリチャネルのアラインメントを指定する。  
  デフォルトでは64バイト

### 補足

- 一般的CSPはgraphvizのdotファイル形式のため、
  画像として確認する為には[graphviz](http://www.graphviz.org)が必要です。
- ATK2用コードのコンパイル・実行にはATK2用開発環境が必要です。  
  生成したコードで使用するマクロについては
  [\(ATK2\)実行環境・開発環境対応マクロ](#Atk2Macros)を参照してください。
	+ 他にpthread環境で動作させるエミュレーションを提供します
- 出力されたS-MBP用コードのコンパイル・実行には
  SystemBuilder用開発環境が必要です。
- eMCOS用コードのコンパイルにはルネサスエレクトロニクス社の
  RH850用コンパイラとeSOL社のeMCOSが必要です。  
  また、実行にはルネサスエレクトロニクス社のCForestシミュレータ、
  またはマイダスラボ社製メニーコア評価ボードRHX-V1が必要です。  
  生成したコードで使用するマクロについては
  [\(eMCOS\)実行環境・開発環境対応マクロ](#eMcosMacros)を参照してください。

### 基本オプション

**A-MBP** や **S-MBP** 向けでは、
ほとんどのオプションはデフォルトで構いません。
従って、基本的に以下のオプションを使用すれば十分です。


ターゲット  |オプション                        |備考
------------|----------------------------------|----------------------------------------
\(全般\)    |-s,--xml-csp                      |
\(全般\)    |-T,--target                       |*atk2* または *smbp* を指定します
\(全般\)    |-X,--output-code                  |
\(全般\)    |-o,--output-csp                   |
\(全般\)    |-i,--c-include                    |実行時デバッグ用等のヘッダファイルの指定
\(全般\)    |--no-gcc                          |gcc以外のターゲットコンパイラの場合
\(全般\)    |--staticalize                     |スタックサイズが比較的小さい場合
smbp        |[--config-file](#ConfigrationFile)|S-MBPでは大抵の場合カスタマイズが必要です

## <a id="ConfigrationFile">設定ファイル</a>

YAML形式のファイルでCSPトランスレータの設定項目を
**--config-file** オプションで指定し、設定することができます。

なお、現状では
**SystemBuilder**
用の設定項目があります。

項目                                 |説明
-------------------------------------|--------------------------------------------------
[SystemBuilderParameters](#SBParams) |SystemBuilderのYAMLファイルに出力する各種パラメタ
[SystemBuilderTypes](#SBTypes)       |SystemBuilderが使用する方を定義する
[EmbeddedCoderTypes](#ECTypes)       |EmbeddedCoderが使用する方を定義する

### <a id="SBParams">SystemBuilderParameters</a>

*SystemBuilderParameters*
はSystemBuilderのYAMLファイルに出力する各種パラメタを設定します。

項目         |キー          |説明
-------------|--------------|--------------------------------------------------------
SW           |core          |SWのコア指定。省略時はNios
HW           |core          |HWのコア指定。省略時はFPGA
HW\_FREQ\_MHZ| -            |SystemBuilderに出力する値。省略時は出力しない
CyclicSync   |ini\_start    |trueまたはfalseを指定する。省略時はtrueを出力
\(同上\)     |sw\_base\_nsec|HWのベース周期\(ns\)。省略時は1000\(1us\)を出力
\(同上\)     |hw\_base\_nsec|SWのベース周期\(ns\)。省略時はタスク周期の最大公約数を出力

設定例は以下の通りです。

	SystemBuilderParameters:
	  SW: { core: Nios }
	  HW: { core: FPGA }
	  HW_FREQ_MHZ: 100
	  CyclicSync:
	    init_start: true
	    sw_base_nsec: 1000000
	    hw_base_nsec: 1000


### <a id="SBTypes">SystemBuilderTypes</a>

*SystemBuilderTypes*
はSystemBuilder用のコードのタスクとの通信に、
構造体になっているブロックの信号など、基本型以外の型が使われている場合、
CSP トランスレータが処理可能にするために設定します
\(ATK2 のコード内で通信が閉じている信号については不要です\)。  
最初のキーは型名を示します。
型の属性はハッシュで与え、設定内容は以下の通りです。

属性   |説明
-------|----
size   |要素のサイズ\(バイト数\)。省略不可
align  |要素のアラインメント\(バイト数\)。省略時はsizeと同じ
element|構造体のメンバのシーケンス
float  |要素が浮動小数点の場合指定する。省略時は整数型になる

設定例は以下の通りです。

	SystemBuilderTypes:
	  IntStruct: { size: 4, align: 4, element: [ a, b, c ] }
	  FloatStruct: { size: 4, align: 4, element: [ x, y, z ], float }

SystemBuilderて取り扱える型には以下の制約があります。

- 構造体のメンバは、全て同一の型でなければならない
- 構造体のメンバは、配列を指定できない

基本型である以下のものは定義する必要はありません。

- char, short, int, long  
  \(longはintと同じサイズとして扱います\)
- unsigned char, unsigned short, unsigned int, unsigned long
- float  


### <a id="ECTypes">EmbeddedCoderTypes</a>

*EmbeddedCoderTypes*
はystemBuilder用コードのタスクとの通信に、
構造体になっているブロックの信号など、基本型以外の型を
**CSPトランスレータ**
が処理可能にするために設定します
\(ATK2 のコード内で通信が閉じている信号については不要です\)。  
最初のキーは型名を示します。
型の属性はハッシュで与え、設定内容は以下の通りです。

属性   |説明
-------|----
ref    |SystemBuilderと型を一致させるための[SystemBuilderTypes](#SBTypes)の参照。この属性を指定した場合、size,align,element,floatを無視する
size   |要素のサイズ\(バイト数\)。省略不可
align  |要素のアラインメント\(バイト数\)。省略時はsizeと同じ
element|構造体のメンバのシーケンス
float  |要素が浮動小数点の場合指定する。省略時は整数型になる

設定例は以下の通りです。

	EmbeddedCoderTypes:
	  IntStruct: { ref: IntStruct }
	  FloatStruct: { ref: FloatStruct }

Embedded Coder の以下の型については定義不要です。

- int8\_T, uint8\_T, int16\_T, uint16\_T, int32\_T, uint32\_T
- real32\_T, real64\_T, real\_T  
- time\_T, boolean\_T, int\_T, uint\_T, ulong\_T, char\_T, uchar\_T, byte\_T
- creal32\_T, creal64\_T, creal\_T
- cint8\_T, cuint8\_T, cint16\_T, cuint16\_T, cint32\_T, cuint32\_T

なお、real\_T系は、32bit浮動小数点\(float\)である必要があります。

---

# <a id="CodeMacros">生成コードで使用されるマクロ</a>

生成コードには、以下のようなマクロを生成・使用します。

- 機種依存マクロ
- 実行環境用
- 開発環境用マクロ
- 実行状況確認用マクロ
- その他、コード生成ターゲット依存のマクロ

必要であれば、これらを置き換えることによって、
機種やプラットフォームの差違の吸収、
デバッグや実行トレースの利用といった事が可能になります。

---

## <a id="Atk2Macros">\(ATK2\)実行環境・開発環境対応マクロ</a>

ATK2コードでは、実行環境や開発環境に応じた対応が可能なように、
様々なマクロを定義する必要があります。
また、実行状況の確認のためのマクロを定義して、
動作を確認することが可能です。

### \(ATK2\)基本項目

#### <a id="Atk2Resources">ATK2のリソース</a>

ATK2で使用するリソースは、
CSPトランスレータが生成するコード中ではそのまま使わずに、
別に定義するマクロから参照されますので、
ATK2のリソースについて説明します。

##### ATK2のタスクID

ATK2のタスクIDは以下のようになります。

*&lt;モデル名&gt;\_Core&lt;コア割り当て番号&gt;\_&lt;レート&gt;\_&lt;開始オフセット&gt;*\_Task

ATK2のタスクIDを構成する要素項目は以下の通りです。

要素項目          |説明
------------------|-----------------------------------------------------------
_モデル名_        |BLXMLのトップレベルの&lt;blocks&gt;に指定されているモデル名
_コア割り当て番号_|&lt;block&gt;に指定されているコア割り当て番号
_レート_          |ブロックの周期\(1msを単位としたサンプル時間\)
_開始オフセット_  |ブロックのサンプル時間の開始オフセット

##### ATK2のアラーム

ATK2のアラームは、タスクと一対一に対応しており、
アラームIDは以下のようになります。

*&lt;モデル名&gt;\_Core&lt;コア割り当て番号&gt;\_&lt;レート&gt;\_&lt;開始オフセット&gt;*\_Alarm

##### ATK2のイベントID

ATK2のイベントIDの以下のものがあります。

イベントID                       |説明
---------------------------------|------------------
*&lt;モデル名&gt;*\_InputEvent   |入力イベント
*&lt;モデル名&gt;*\_OutputEvent  |出力イベント
*&lt;モデル名&gt;*\_WakeupEvent  |起床イベント
*&lt;モデル名&gt;*\_CompleteEvent|初期化完了イベント


#### <a id="Atk2CspProcessIdentMacros">\(ATK2\)CSPプロセス用識別子</a>

CSPプロセスでは、入力、出力の処理を行うため、
*タスク識別子* 、 *入力識別子* 、 *出力識別子* を定義します。

*ブロック識別子*
は、ブロックのブロック番号\(&lt;block&gt;のid属性\)を
以下のように整数を右詰4桁、”0”でパディングしたものです。
なお今後 *BID* と表記した場合、このブロック識別子を指します。

	0001

CSPプロセスの *タスク識別子* 、 *入力識別子* 、 *出力識別子* を、
CSPプロセスの代表となるブロックの *ブロック識別子* をポストフィクスに用いて、
以下のように定義します。

識別子                        |説明
------------------------------|-------------------------------------
TASK\_*&lt;ブロック識別子&gt;*|CSPプロセスを示す、 *タスク識別子*
IN\_*&lt;ブロック識別子&gt;*  |CSPプロセスの入力を示す、 *入力識別子*
OUT\_*&lt;ブロック識別子&gt;* |CSPプロセスの出力を示す、 *出力識別子*

上記の識別子は、様々なマクロの中で、
[\(ATK2\)CSPプロセス用マクロ](#Atk2CspProcessMacros)
で説明するマクロへ合成され、
[\(ATK2\)タスク用のマクロ](#Atk2TaskMacros)を経由して、
最終的に[\(ATK2\)ATK2のリソース](#Atk2Resources)へと置換されます。


### \(ATK2\)ATK2専用マクロ

ATK2環境では、タスクやイベントを関連付けるため、また、
デバッグやシステムコールとのインタフェースを適切に行うためのマクロを生成します。
これらのマクロは変更できません。

マクロは、システムコールやデバッグのため、
主に[\(ATK2\)CSPプロセス用マクロ](#Atk2CspProcessMacros)から、
目的別の
*ATK2\_*,
*ATK2\_WE\_*,
*ATK2\_SE\_*
といったプレフィクスを合成して使用するため冗長な定義を行いますが、
実行性能等には影響ありません。また、具体的にどのように合成するかについては、
生成コードを参考にしてください。

なお
*ATK2\_TNAME\_*,
*ATK2\_TID\_*
のプレフィクスを持つマクロは文字列になっており、
これらはデバッグ用に用意しています。

#### (ATK2)起動待ち合わせを行うタスク用のマクロ

最も早く起床し、レートが最も速いタスクが、
それ以外のタスクより実行を先に進めないように、
当該タスクを全てのタスクの起動待ち合わせを行うようにコード生成が行われます。
以下はCSPトランスレータが生成する、当該タスクに関連付けるためのマクロです。

マクロ                          |説明
--------------------------------|------------------------------------------------------------
*ATK2\_TASK\_WAIT\_TASKS*       |待ち合わせを行うタスクのタスクIDを示すマクロ
*ATK2\_WE\_WAIT\_TASKS*         |待ち合わせタスクが、WaitEventで待つイベントIDを示すマクロ
*ATK2\_SE\_WAIT\_TASKS*         |待ち合わせタスクに、SetEventで送信するイベントIDを示すマクロ
*ATK2\_TNAME\_WAIT\_TASKS*      |待ち合わせタスクを示す文字列
*ATK2\_TNAME\_TASK\_WAIT\_TASKS*|待ち合わせタスクを示す文字列

#### <a id="Atk2TaskMacros">\(ATK2\)タスク用のマクロ</a>

ATK2のタスクは、同じコアに割り当てられた、
同一レートかつ同一オフセットのCSPプロセスから構成されます。
以下は、CSPトランスレータが生成する、タスク識別子を用いた、
タスクとリソースや文字列と関連付けるためのマクロです。
CSPトランスレータでは、これらを直接使う個所では、
プレフィクスの
*ATK2\_*
を除く形で出力し、最終的に
[\(ATK2\)ATK2のリソース](#Atk2Resources)
に置換されます。

マクロ                                    |説明
------------------------------------------|-----------------------------
ATK2\_*&lt;タスク識別子&gt;*              |*タスクID* を示すマクロ
ATK2\_WE\_*&lt;タスク識別子&gt;*          |タスクがWaitEventで待つ *イベントID* を示すマクロ
ATK2\_SE\_*&lt;タスク識別子&gt;*          |タスクにSetEventで送信する *イベントID* を示すマクロ
ATK2\_TID\_*&lt;タスク識別子&gt;*         |タスクの *ブロック識別子* の文字列
ATK2\_TNAME\_*&lt;タスク識別子&gt;*       |タスクを示す文字列
ATK2\_ALARM\_*&lt;タスク識別子&gt;*       |タスクが起床される *アラームID* を示すマクロ
ATK2\_TNAME\_ALARM\_*&lt;タスク識別子&gt;*|アラームを示す文字列

これらのマクロは、
[\(ATK2\)CSPプロセス用マクロ](#Atk2CspProcessMacros)
で、CSPプロセスからATK2のタスクへマッピングするために使用します。


#### <a id="Atk2CspProcessMacros">\(ATK2\)CSPプロセス用マクロ</a>

ATK2のタスクは複数のCSPプロセスから構成されます。
従ってCSPプロセスとATK2のタスクを関連付けるマクロが必要になります。
以下はCSPトランスレータが生成するCSPプロセスと
ATK2のタスクを関連付けるためのマクロです。

マクロ                           |説明
---------------------------------|-----------------------------
ATK2\_TASK\_*&lt;入力識別子&gt;* |入力処理に対応する *タスクID* を示すマクロ
ATK2\_TASK\_*&lt;出力識別子&gt;* |出力処理に対応する *タスクID* を示すマクロ
ATK2\_SE\_*&lt;入力識別子&gt;*   |当該入力を行うタスクに入力可イベント送信する、SetEventの引数を示すマクロ
ATK2\_SE\_*&lt;出力識別子&gt;*   |当該出力を行うタスクに出力可イベント送信する、SetEventの引数を示すマクロ
ATK2\_WE\_*&lt;入力識別子&gt;*   |入力可イベントを待つための、WaitEventの引数を示すマクロ
ATK2\_WE\_*&lt;出力識別子&gt;*   |出力可イベントを待つための、WaitEventの引数を示すマクロ
ATK2\_TNAME\_*&lt;入力識別子&gt;*|入力するタスクを示す文字列
ATK2\_TNAME\_*&lt;出力識別子&gt;*|出力するタスクを示す文字列
ATK2\_TID\_*&lt;入力識別子&gt;*  |入力を行うタスクを示す *ブロック識別子* の文字列
ATK2\_TID\_*&lt;出力識別子&gt;*  |出力を行うタスクを示す *ブロック識別子* の文字列

CSPトランスレータは生成コード中に、これらのマクロを直接出力せず、
[\(ATK2\)CSPプロセス用マクロ](#Atk2CspProcessMacros)
を利用し、様々なマクロの中でこれらマクロへと合成され、最終的に
[\(ATK2\)ATK2のリソース](#Atk2Resources)
に置換されます。

#### \(ATK2\)機種依存マクロ

機種依存として定義する必要があるマクロは以下の通りです。

- *CH\_\_SYNCM\(\)*  
  複数コア間でのメモリ同期命令の定義。  
  未定義の場合、V850のgccでは、以下となる。  
  ```
  __asm__ __volatile__ (“syncm” ::: “memory”)
  ```  
  また、Niosのgccでは、以下となる。  
  ```
  __asm__ __volatile__ (“sync” ::: “memory”)
  ```
- *CH\_\_MEM\_BARRIER\(\)*  
  コンパイラに対し、それ以前のロードストアを完了させる記述。  
  gcc では以下となる。  
  ```
  __asm__ __volatile__ (“” ::: “memory”)
  ```

#### \(ATK2\)実行環境用マクロ

実行環境に対して定義可能なマクロは以下の通りです。

- *OS\_\_SENDEVENT\(T,R,E\)*  
  SetEventを呼ぶラッパー。  
  Tは *タスクID* を与えるもので、自身の *タスクの識別子*。  
  Rは *タスクID* を与えるもので、受信タスクの *タスク識別子*。  
  Eは *イベントID* を与えるもので、
  自身の ATK2\_*SE\_&lt;タスク識別子&gt;* から ATK2\_ を削除したもの
- *OS\_\_WAITEVENT\(T,E\)*  
  WaitEventとClearEventを呼ぶラッパー。  
  Tは *タスクID* を与えるもので、自身の *タスクの識別子*。  
  Eは *イベントID* を与えるもので、
  自身の ATK2\_*WE\_&lt;タスク識別子&gt;* から ATK2\_ を削除したもの
- *OS\_\_SETALARM\(A,O,R\)*  
  SetRelAlarmを呼ぶラッパー。  
  Aは *アラームID* を与えるもので、
  ATK2\_*ALARM\_&lt;タスク識別子&gt;* から ATK2\_ を削除したもの  
  Oは\(1ms単位の\)インターバル。  
  Rは\(1ms単位の\)レート
- *CH\_\_TASK\_\_SCHED\(CH,VEC,SIZE\)*  
  チャネル入力\(または出力\)が完了しない場合にイベント待ちを行うためのマクロ
- *CH\_\_CORE\_THREAD\_SCHED\(CH,VEC,SIZE\)*  
  未使用\(定義は上記と同じ\)
- *CH\_\_SETEVENT\(C\)*  
  マクロで置換したSetEventを
  *CH\_\_EVENT*
  から呼び出しても問題ないようにするための1引数マクロ
- *CH\_\_EVENT\(TID,CH\)*  
  上記の*CH\_\_TASK\_SCHED\(\)*
  で待っているタスクに対してイベントを送信するマクロ。
- *CH\_\_END\_LOG\(CH\)*  
  チャネルの入力\(または出力\)が完了したときに呼び出す定義。
  ログ\(デバッグ\)用であり、未定義の場合、なにもしない

#### \(ATK2\)開発環境用マクロ

開発環境に対して定義可能なマクロは以下の通りです。

- *CORE\_TASK\_DATA\_PREPARE\(CORENUM\)*  
  コアに配置されるタスク関数外のデータの配置開始時用のマクロ。
  例えば
  `#pragma`
  によるデータセクションを指定するために用いる。  
  未定義の場合、なにもしない。  
  なお、CORENUMはコア番号で、0以上の整数である。
- *CORE\_TASK\_CODE\_PREPARE\(CORENUM\)*  
  コアに配置されるタスク関数外のコード\(関数\)の配置開始時用のマクロ。
  例えば
  `#pragma`
  によるテキストセクションを指定するために用いる。  
  未定義の場合、なにもしない。
- *CORE\_TASK\_RODATA\_PREPARE\(CORENUM\)*  
  コアに配置されるタスク関数外の読み込み専用データの配置開始時用のマクロ。
  例えば
  `#pragma`
  によるテキストセクションを指定するために用いる。  
  未定義の場合、なにもしない。
- *CORE\_TASK\_DATA\_ATTRIBUTE\(CORENUM\)*  
  コアに配置されるタスク関数外のデータに対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたデータセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *CORE\_TASK\_CODE\_ATTRIBUTE\(CORENUM\)*  
  コアに配置されるタスク関数外のコード\(関数\)に対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたテキストセクションや
  アラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *CORE\_TASK\_RODATA\_ATTRIBUTE\(CORENUM\)*  
  コアに配置されるタスク関数外の読み込み専用データに対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたテキストセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。


#### <a id="Atk2ExecMacros">\(ATK2\)実行状況確認用マクロ</a>

実行状況確認用として定義可能なマクロは以下の通りです。

- *TASK\_&lt;タスクID&gt;\_DEFINITIONS*  
  ATK2のタスク毎に型や変数を定義するポイント。  
  型や変数が定義できるが、
  *TASK\_&lt;BID&gt;\_DEFINITIONS*
  と衝突しないようにする必要がある。  
  未定義の場合なにもしない。  
  なお、タスクIDはATK2のタスクIDである。
- *TASK\_&lt;タスクID&gt;\_LOOP\_TOP*  
  ATK2のタスクが周期起動を開始したポイント。  
  定義した処理を実行する。  
  未定義の場合なにもしない。
- *TASK\_&lt;タスクID&gt;\_BEFORE\_SLEEP*  
  ATK2のタスクが周期処理を終了し、スリープするポイント。  
  定義した処理を実行する。
  未定義の場合なにもしない。
- *TASK\_&lt;タスクID&gt;\_AFTER\_SLEEP*  
  ATK2のタスクが上記のスリープを終了したポイント。  
  定義した処理を実行する。
  \(このマクロは、周期起動中は、
  *TASK\_&lt;タスクID&gt;\_LOOP\_TOP*
  と同じタイミングとなる\)  
  未定義の場合なにもしない。
- *TASK\_&lt;BID&gt;\_DEFINITIONS*  
  CSPプロセス毎に型や変数を定義するポイント。  
  型や変数が定義できるが、
  *TASK\_&lt;タスクID&gt;\_DEFINITIONS*
  や他の
  *TASK\_&lt;BID&gt;\_DEFINITIONS*
  と衝突しないようにする必要がある。
- *TASK\_INITIALIZE\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスの変数等の初期化が終わり、
  タスクの処理開始前に呼び出されるマクロ。  
  CSPプロセス毎に定義する必要がある。  
  未定義の場合なにもしない。  
  なお、
  *&lt;BID&gt;*
  はCSPプロセスを代表するブロックのブロック識別子である。
- *TASK\_IN\_&lt;BID&gt;\_COMPLETED\(\)*  
  タスクの入力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_EXECUTE\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスのブロックの処理\(演算\)が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_OUT\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスの出力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_UPDATE\_IN\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスのUpdateの処理用の入力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_UPDATE\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスのUpdateの処理の実行\(演算\)が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。

---

## <a id="SmbpMacros">\(SystemBuilder\)実行環境・開発環境対応マクロ</a>

SystemBuider用コードでは、実行状況の確認のためのマクロを定義して、
動作を確認することが可能です。

### \(SystemBuilder\)基本項目

#### \(SystemBuilder\)タスクID

SystemBuilderのタスクIDは以下のようになります。

*&lt;モデル名&gt;\_HWCore&lt;コア割り当て番号&gt;\_&lt;レート&gt;\_&lt;開始オフセット&gt;*\_Task

SystemBuilderのタスクIDを構成する項目は以下の通りです。

要素項目          |説明
------------------|-----------------------------------------------------------
*モデル名*        |BLXMLのトップレベルの&lt;blocks&gt;に指定されているモデル名
*コア割り当て番号*|&lt;block&gt;に指定されているコア割り当て番号
*レート*          |ブロックの周期\(ベースレートを単位としたサンプル時間\)
*開始オフセット*  |ブロックのサンプル時間の開始オフセット

#### \(SystemBuilder\)ブロック識別子

ブロック識別子は、ブロックのブロック番号\(&lt;block&gt;のid属性\)
を以下のように整数を右詰4桁、”0”でパディングしたものです。
なお今後 *BID* と表記した場合、このブロック識別子を指します。

	0001

CSPプロセスの *タスク識別子* を、CSPプロセスの代表となるブロックの
*ブロック識別子* をポストフィクスに用いて、以下のように定義します。

識別子                        |説明
------------------------------|---------------------------------------
TASK\_*&lt;ブロック識別子&gt;*|CSPプロセスを示す、 *タスク識別子*


### \(SystemBuilder\)実行状況確認用マクロ

実行状況確認用として定義可能なマクロは以下の通りです。

- *TASK\_&lt;タスク識別子&gt;\_DEFINITIONS*  
  SystemBuilderのタスク毎に型や変数を定義するポイント。  
  型や変数が定義できるが、
  *TASK\_&lt;BID&gt;\_DEFINITIONS*
  と衝突しないようにする必要がある。  
  未定義の場合なにもしない。  
  なお、タスクIDはSystemBuilderのタスクIDである。
- *TASK\_&lt;タスク識別子&gt;\_ITERATION\_TOP*  
  SystemBuilderのタスクが周期起動を開始したポイント。  
  定義した処理を実行する。  
  未定義の場合なにもしない。
- *TASK\_&lt;タスク識別子&gt;\_ITERATION\_END*  
  SystemBuilderのタスクが周期処理を終了したポイント。  
  定義した処理を実行する。  
  未定義の場合なにもしない。
- *TASK\_&lt;BID&gt;\_DEFINITIONS*  
  CSPプロセス毎に型や変数を定義するポイント。  
  型や変数が定義できるが、
  *TASK\_&lt;タスク識別子&gt;\_DEFINITIONS*
  や他の
  *TASK\_&lt;BID&gt;\_DEFINITIONS*
  と衝突しないようにする必要がある。
- *TASK\_INITIALIZE\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスの変数等の初期化が終わり、タスクの処理開始前に呼び出されるマクロ。  
  CSPプロセス毎に定義する必要がある。  
  未定義の場合なにもしない。  
  なお、 *BID* はCSPプロセスを代表するブロックの *ブロック識別子* である。
- *TASK\_IN\_&lt;BID&gt;\_COMPLETED\(\)*
  タスクの入力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_EXECUTE\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスのブロックの処理\(演算\)が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_OUT\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスの出力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_UPDATE\_IN\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスのUpdateの処理用の入力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_UPDATE\_&lt;BID&gt;\_COMPLETED\(\)*  
  CSPプロセスのUpdateの処理の実行\(演算\)が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。

---

## <a id="eMcosMacros">\(eMCOS\)実行環境・開発環境対応マクロ</a>

eMCOS コードでは、実行環境や開発環境に応じた対応が可能なように、
様々なマクロを定義する必要があります。
また、実行状況の確認のためのマクロを定義して、
動作を確認することが可能です。

### \(eMCOS\)機種依存マクロ

機種依存として定義する必要があるマクロは以下通りです。

- *CH\_\_SYNCM\(\)*  
  複数コア間でのメモリ同期命令の定義。  
  未定義の場合、V850のgccでは、以下となる。  
  ```
  __asm__ __volatile__ (“syncm” ::: “memory”)
  ```
- *CH\_\_MEM\_BARRIER\(\)*  
  コンパイラに対し、それ以前のロードストアを完了させる記述。  
  gcc では以下となる。  
  ```
  __asm__ __volatile__ (“” ::: “memory”)
  ```

### \(eMCOS\)実行環境用マクロ

実行環境に対して定義可能なマクロは以下の通りです。

- *CH\_\_TASK\_\_SCHED\(CH,VEC,SIZE\)*  
  マイグレーション可能スレッドのチャネル入力\(または出力\)が完了しない場合に
  OSにタスクスケジューラを呼び出す処理のマクロ。  
  未定義の場合eMCOSでは以下となる。  
  ```
  mcos_thread_delay(0)
  ```  
  上記のCHは入力または出力チャネル、  
  VECはチャンネル処理で使用するビットベクタを格納する配列、  
  SIZEは処理するチャネル数\(ベクタ数\)である。  
  なお、SIZEが1の場合、VECはNULLである。
- *CH\_\_CORE\_THREAD\_SCHED\(CH,VEC,SIZE\)*  
  コアアフィニティスレッドのチャネル入力\(または出力\)が完了しない場合に
  OSにタスクスケジューラを呼び出す処理のマクロ。  
  未定義の場合eMCOSではなにもしない
- *CH\_\_EVENT\(TID,CH\)*  
  上記の
  *CH\_\_TASK\_SCHED\(\)*
  で待っているタスクに対してイベントを送信するマクロ。  
  未定義の場合、eMCOSではなにもしない。  
  なお、TIDはイベント送信元のタスクIDである。
- *CH\_\_END\_LOG\(CH\)*  
  チャネルの入力\(または出力\)が完了したときに呼び出す定義。
  ログ\(デバッグ\)用である。  
  未定義の場合、なにもしない。

### \(eMCOS\)開発環境用マクロ

開発環境に対して定義可能なマクロは以下の通りです。

- *CORE\_TASK\_DATA\_PREPARE\(CORENUM\)*  
  コアアフィニティスレッドの関数外のデータの配置開始時用のマクロ。
  例えば
  `#pragma`
  によるデータセクションを指定するために用いる。  
  未定義の場合、なにもしない。  
  なお、CORENUMはコア番号で、0以上の整数である。
- *CORE\_TASK\_CODE\_PREPARE\(CORENUM\)*  
  コアアフィニティスレッドの関数外のコード\(関数\)の配置開始時用のマクロ。
  例えば
  `#pragma`
  によるテキストセクションを指定するために用いる。  
  未定義の場合、なにもしない。
- *CORE\_TASK\_RODATA\_PREPARE\(CORENUM\)*  
  コアアフィニティスレッドの関数外の読み込み専用データの配置開始時用のマクロ。
  例えば
  `#pragma`
  による読み込み専用セクションを指定するために用いる。  
  未定義の場合、なにもしない。
- *CORE\_TASK\_DATA\_ATTRIBUTE\(CORENUM\)*  
  コアアフィニティスレッドの関数外のデータに対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたデータセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *CORE\_TASK\_CODE\_ATTRIBUTE\(CORENUM\)*  
  コアアフィニティスレッドの関数外のコード\(関数\)に対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたテキストセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *CORE\_TASK\_RODATA\_ATTRIBUTE\(CORENUM\)*  
  コアアフィニティスレッドの関数外の読み込み専用データに対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたテキストセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *TASK\_DATA\_PREPARE\(TID\)*  
  マイグレーション可能スレッドの関数外のデータの配置開始時用のマクロ。
  例えば
  `#pragma`
  によるデータセクションを指定するために用いる。  
  未定義の場合、なにもしない。
  なお、TIDはタスクIDである。
- *TASK\_CODE\_PREPARE\(TID\)*  
  マイグレーション可能スレッドの関数外のコード\(関数\)の配置開始時用のマクロ。
  例えば
  `#pragma`
  によるテキストセクションを指定するために用いる。  
  未定義の場合、なにもしない。
- *TASK\_RODATA\_PREPARE\(TID\)*  
  マイグレーション可能スレッドの関数外の読み込み専用データの
  配置開始時用のマクロ。
  例えば
  `#pragma`
  によるテキストセクションを指定するために用いる。  
  未定義の場合、なにもしない。
- *TASK\_DATA\_ATTRIBUTE\(TID\)*  
  マイグレーション可能スレッドの関数外のデータに対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたデータセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *TASK\_CODE\_ATTRIBUTE\(TID\)*  
  マイグレーション可能スレッドの関数外のコード\(関数\)に対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたテキストセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。
- *TASK\_CODE\_ATTRIBUTE\(TID\)*  
  マイグレーション可能スレッドの関数外の読み込み専用データに対し属性を指定する。
  例えばgccの
  `__attribute__()`
  を用いたテキストセクションやアラインメントを指定するために用いる。  
  未定義の場合なにもしない。


### <a id="eMcosExecMacros">\(eMCOS\)実行状況確認用マクロ</a>

実行状況確認用として定義可能なマクロは以下の通りです。

- *TASK\_&lt;TID&gt;\_DEFINITIONS*  
  タスク毎に型や変数を定義できるポイント。
  タスクID毎に定義する必要がある。  
  未定義の場合なにもしない。  
  なお、TIDはタスクIDである。
- *TASK\_INITIALIZE\_&lt;TID&gt;\_COMPLETED\(\)*  
  タスクの変数等の初期化が終わり、タスクの処理開始前に呼び出されるマクロ。  
  未定義の場合なにもしない。
- *TASK\_IN\_&lt;TID&gt;\_COMPLETED\(\)*  
  タスクの入力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_EXECUTE\_&lt;TID&gt;\_COMPLETED\(\)*  
  タスクのブロックの処理\(演算\)が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_OUT\_&lt;TID&gt;\_COMPLETED\(\)*  
  タスクの出力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_UPDATE\_IN\_&lt;TID&gt;\_COMPLETED\(\)*  
  タスクのUpdateの処理用の入力が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。
- *TASK\_UPDATE\_&lt;TID&gt;\_COMPLETED\(\)*  
  タスクのUpdateの処理の実行\(演算\)が完了したときに呼び出されるマクロ。  
  未定義の場合、なにもしない。

### \(eMCOS\)非同期・イベント送信制御機能マクロ

非同期・イベント送信制御用として定義可能なマクロは以下の通りです。

- *TASK\_ENABLE\_ASYNC\_SEND\_&lt;TID1&gt;\_&lt;TID2&gt;\(T,F\)*  
  非同期送信を行うかどうか判定するマクロ。
  Tはブロックの時刻。Fは送信時には1、送信しない場合は0を設定する変数名である。  
  未定義の場合なにもしない。  
  なお、TID1は送信側タスクID、TID2は受信側タスクIDである。
- *TASK\_ENABLE\_EVENT\_SEND\_&lt;TID1&gt;\_&lt;TID2&gt;\(T,F\)*  
  イベント送信を行うかどうか判定するマクロ。
  Tはブロックの時刻。Fは送信時には1、送信しない場合は0を設定する変数名である。  
  未定義の場合なにもしない。  
  なお、TID1は送信側タスクID、TID2は受信側タスクIDである。

---

# <a id="DebugSupport">デバッグ支援コマンド</a>

## <a id="Atk2DebugSupport">\(ATK2\)デバッグ支援コマンド</a>

[\(ATK2\)実行状況確認用マクロ](#Atk2ExecMacros)の
*TASK\_EXECUTE\_&lt;BID&gt;\_COMPLETED\(\)*
を用いた出力変数のデバッグプリントを行うコード生成コマンドです。

このコマンドはBLXMLから、出力変数名、型、配列といった情報を取得し、
以下のような
*TASK\_EXECUTE\_&lt;BID&gt;\_COMPLETED\(\)*
のマクロを生成します。

	/_ FromWorkspace:multirate2_FromWorkspace _/
	#define TASK_EXECUTE_0001_COMPLETED()\
	  do {\
	  syslog(LOG_INFO, "[%lu]ATK2_TNAME_TASK_0001"(0001):"multirate2_FromWorkspace_1:%.7f",\
	         task__time, (double)multirate2_FromWorkspace_1);\
	  } while (0);

このマクロを使用し、出力変数を表示することで
実行中の状況確認やブロック間の実行順序等の確認を行うことができます。

ただし、ATK2 の syslog\(\)は、浮動小数点の表示や、多数の変数を出力することができません。

そのような場合は、例えば後述のオプションで、タスク名のみ表示する、といった対応や、
正しく表示できるように作成した syslog\(\)に置き換えるといった対応が必要です。

コマンドの実行方法は以下の通りで、標準出力に出力します。

	$ atk2_dump_dbg_print [-b ブロックID] [-c インデント量] [-s] [-t] BLXML

オプションは以下の通りです。

- **-b** _ブロックID_  
  対象とするブロックのBLXML内のブロックID\(複数指定可。省略時は全て\)
- **-c** _インデント量_  
  指定したインデント量で呼び出し側コードを生成\(逐次コードでの実行確認等\)
- **-s**  
  シングルレート時にタスクの時刻を表示しない\(省略時はタスク時刻を表示\)
- **-t**  
  タスク名のみ表示する\(省略時は全ての変数を表示する\)

このように作成したマクロを含んだヘッダーファイルを
あらかじめ[**CSPトランスレータ**のオプション](#Options)の **-i** で指定し、
コンパイル時に読み込むようにしておくと便利です。

---

## <a id="eMcosDebugSupport">\(eMCOS\)デバッグ支援コマンド</a>

[\(eMCOS\)実行状況確認用マクロ](#eMcosExecMacros)の
*TASK\_EXECUTE\_&lt;TID&gt;\_COMPLETED\(\)*
を用いた出力変数のデバッグプリントを行うコード生成コマンドです。

このコマンドはBLXMLから、出力変数名、型、配列といった情報を取得し、
以下のような
*TASK\_EXECUTE\_&lt;TID&gt;\_COMPLETED*
のマクロを生成します。

	/_ RealImagToComplex:synrmmodel_mt_RTW_Subsystem2_R2C _/
	#define TASK_EXECUTE_0336_COMPLETED()\
	  do {\
	    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_R2C_1:(%.7f,%.7f)\n",\
	                      mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_R2C_1.re,\
	                      (double)synrmmodel_mt_RTW_Subsystem2_R2C_1.im);\
	  } while (0)


このマクロを使用し、出力変数を表示することで
実行中の状況確認やブロック間の実行順序等の確認を行うことができます。

コマンドの実行方法は以下の通りで、標準出力に出力します。

	$ mcos_dump_dbg_print [-b ブロックID] [-c インデント量] BLXML

オプションは以下の通りです。

- **-b** _ブロックID_  
  対象とするブロックのBLXML内のブロックID\(複数指定可。省略時は全て\)
- **-c** _インデント量_  
  指定したインデント量で呼び出し側コードを生成\(逐次コードでの実行確認等\)


このように作成したマクロを含んだヘッダーファイルを
あらかじめ[**CSPトランスレータ**のオプション](#Options)の **-i** で指定し、
コンパイル時に読み込むようにしておくと便利です。
