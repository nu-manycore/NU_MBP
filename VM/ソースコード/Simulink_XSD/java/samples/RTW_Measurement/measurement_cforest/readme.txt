
			  CForest用実行環境


1. はじめに
===========

このディレクトリは、AddMeasurementプログラムにより、RTW-ECによって生成
されたCコードに性能計測マクロを埋め込んだものをCForest用にビルド・実行
する環境です。

2. 使い方
=========

このディレクトリに必要なファイルを配置して使用することもできますが、必
要なファイルを別ディレクトリに配置した状態で使用することを推奨します。

また、以降、ツールディレクトリとは、本ディレクトリのことを指します。

2.1. 事前準備
=============

    ツールディレクトリのMyEnv.shを、環境に合わせて変更してください。

    TOOL_PATH : V850のgccの実行ファイルがあるパスを設定します。

      [注意] このパスは、PATH環境変数にも合わせて設定してください。

    CFOREST_PATH : cforest_mpがあるパスを設定します

2.2. ビルドおよび計測
=====================

(1) ビルド・実行用ディレクトリをツールディレクトリとは別の階層に作成し
    ます

      $ mkdir sample

(2) AddMeasurementで作成した性能計測コード用を作成したディレクトリに配
    置します

      $ AddMeasurement.sh -o measure.c -m measure.h -i measure_cforest.h \
          -h ヘッダーファイル Cソースファイル

        measure.c: 計測コードを埋め込んだCコード
        measure.h: 各種生成マクロ
        measure_cforest.h: 計測マクロやmain関数等を定義したファイル
        (include/measure_cforest.hをインクルードすることになります)


(3) ツールディレクトリから、makefileを二つ、作成したディレクトリにコピー
    します

      $ cp .../rtw_measurement/Makefile Makefile
      $ cp .../rtw_measurement/Template-user.mk user.mk


(4) Makefileを編集します

      MEASUREMENT_TOOL_DIRに、本ディレクトリの相対または絶対パスを指定
      してください。

(5) user.mkを編集します

      AUX_SRC_DIR

        ビルドに必要なヘッダファイル、Cソースファイルがあるディレクトリ
        を指定してください。このディレクトリは、Makefileでのvpathとコン
        パイラの-Iオプションに設定されます。

      AUX_C_SRCS

        リンクする必要があるその他のCソースファイルがあれば指定してくだ
        さい。このソースは、AUX_SRC_DIRに設定したディレクトリから探索し
        ます。

      AUX_S_SRCS

         リンクする必要があるその他のasmソースがあれば指定してください。
         サフィックスは".s"が対象です。このソースは、AUX_SRC_DIRに設定
         したディレクトリから探索します。

      TARGET_SRC

         性能計測コードを埋め込んだCコードです。(1)で出力したCコードを
         指定します。

      TARGET_HDRS

         (1)で生成したヘッダファイルおよびCForest用の実行コードを指定し
         ます。


(6) ビルド

      $ make

      ビルドに問題がなければmeasureという名前のプログラムが出力されます


(7) 計測

      $ ./run.sh

      実行に問題がなければresult.csvというファイルが出力されます

      このファイルにはCSVファイル形式で計測結果が格納されており、以下の
      ようにSetMeasuringResultコマンドによって、XMLに反映することができ
      ます。

        $ SetMeasureingResult -i input.xml -o output.xml result.csv
