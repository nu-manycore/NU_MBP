<!-- -*- mode: markdown; coding: utf-8-unix; -*- -->

<style type="text/css">
table { border-collapse: collapse; }
table, th, tr, td { border: 1px solid black; }
code { color: darkgreen; }
pre { background-color: gainsboro; }
</style>

# rtw\_testコマンドの使用方法

# はじめに

**rtw\_test**コマンドは、
__MathWorks社__の**Simulink**のモデルから構造を抽出した
__ブロックレベル構造XML\(BLXML\)__ と、
Embedded Coderが生成するCコードを入力し、  
**Simulink**のブロックに対応するCの実行コード、パラメタ、状態変数等を抽出し、
対応する **BLXML** のブロックに設定すると同時に、
変数のブロック間の参照関係を設定した **コード情報付きBLXML** を出力します。

---

# 使用方法

コマンドの書式は以下の通りです。

		rtw_test [-n] -c c_source -h c_header [-d c_data] [-P{x|a}] \
				input-BLXML [output-BLXML]

## オプション

- **-n**  
  このオプションを指定した場合、スキーマ\(_SimulinkModel.xsd_\)
  との整合性をチェックしません。  
  このため現在の作業ディレクトリに _SimulinkModel.xsd_ は不要ですが、  
  もしもスキーマと入力するBLXMLとが不整合であった場合、
  プログラムの異常動作や、不正な出力BLXMLの出力となる事があります。  
  一方、このオプションを省略した場合、スキーマと入力するBLXMLの整合性を
  チェックし、不整合の場合は、プログラムはエラー終了します。  
  なお、現在の作業ディレクトリに _SimulinkModel.xsd_ が必要です。
- **-c** _c\_source_  
  Embedded Coderが生成したモデルのCソースファイルを指定します
- **-h** _c\_header_  
  Embedded Coderが生成したモデルのヘッダーファイルを指定します
- **-d** _c\_data_  
  Embedded Coderが生成したモデルのパラメタ等のデーターファイルを指定します。  
  このオプションは、データファイルがない場合、省略してください
- **-P** _\{x|a\}_  
  **通常このオプションは指定しないでください。**  
  このオプションは特別な目的のために使用するもので、
  Cソースファイルのパースオプションの指定です。  
  \(通常は、Triggered/Enabled SubSystemやAtomic SubSystemは、
  それぞれ単一のブロックとしてCコードを抽出します\)
	+ **x**  
	  Triggered/Enabled等のSubSystem内のブロックのレベルでCコードを抽出します
	+ **a**  
	  Atomic SubSystemのSubSystem内のブロックのレベルでCコードを抽出します
- *input-XML*  
  Simulinkモデルから抽出された、入力**BLXML**ファイルを指定します。
- *output-XML*  
  出力する**コード情報付きBLXML**ファイルを指定します。
  省略した場合、標準出力へ出力します

