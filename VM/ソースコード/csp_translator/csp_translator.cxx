/// \file csp_translator.cxx
///
/// \brief CSPトランスレータ本体
///

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <regex.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include <getopt.h>

#include <iostream>

#if defined(__MINGW32__) || defined(__MINGW64__)
/// \brief Windowsにはrealpathがないので_fullpathで代替
#define realpath(S,D)	_fullpath(D,S,PATH_MAX)
#endif

#include "csp_translator.h"

/// \addtogroup CSP_TRANSLATOR_MAIN
///
/// @{

/// \brief --create-t0 のオプション番号
#define OPTION_CREATE_T0		257

/// \brief --ignore-to-sink のオプション番号
#define OPTION_IGNORE_TO_SINK		258

/// \brief --iignore-isolated のオプション番号
#define OPTION_IGNORE_ISOLATED_BLOCK	259

/// \brief --ignore-leaves のオプション番号
#define OPTION_IGNORE_LEAF_BLOCK	260

/// \brief --xsd-validation のオプション番号
#define OPTION_XSD_VALIDATION		261

/// \brief --staticalize のオプション番号
#define OPTION_STATICALIZE		262

/// \brief --label-only-task-id のオプション番号
#define OPTION_LABEL_ONLY_TASK_ID	263

/// \brief --no-gcc のオプション番号
#define OPTION_NO_GCC			264

/// \brief --enable-multirate のオプション番号
#define OPTION_ENABLE_MULTIRATE		265

/// \brief --disable-multirate のオプション番号
#define OPTION_DISABLE_MULTIRATE	266

/// \brief --enable-csr のオプション番号
#define OPTION_ENABLE_CSR		267

/// \brief --disable-csr のオプション番号
#define OPTION_DISABLE_CSR		268

/// \brief --low-rate-pulsegenerator のオプション番号
#define OPTION_LOW_RATE_PULSEGENERATOR	269

/// \brief --schema-loc のオプション番号
#define OPTION_SCHEMA_LOCATION		270

/// \brief --atk2-appmode のオプション番号
#define OPTION_ATK2_APPMODE		271

/// \brief --atk2-timer-isrref のオプション番号
#define OPTION_ATK2_TIMER_ISRREF	272

/// \brief --atk2-no-timer-isrref のオプション番号
#define OPTION_ATK2_NO_TIMER_ISRREF	273

/// \brief --atk2-timer-counterref のオプション番号
#define OPTION_ATK2_TIMER_COUNTERREF	274

/// \brief --atk2-timer-hz のオプション番号
#define OPTION_ATK2_TIMER_HZ		275

/// \brief --atk2-gen-main のオプション番号
#define OPTION_ATK2_GEN_MAIN		276

/// \brief --config-file のオプション番号
#define OPTION_CONFIG_FILE		277

/// \brief --smbp-yaml のオプション番号
#define OPTION_SMBP_YAML		278

/// \brief --smbp-prefix のオプション番号
#define OPTION_SMBP_PREFIX		279

/// \brief --smbp-long64 のオプション番号
#define OPTION_SMBP_LONG64		280

/// \brief --smbp-real64 のオプション番号
#define OPTION_SMBP_REAL64		281

/// \brief --smbp-long64-align のオプション番号
#define OPTION_SMBP_LONG64_ALIGN	282

/// \brief --smbp-real64-align のオプション番号
#define OPTION_SMBP_REAL64_ALIGN	283

/// \brief --smbp-struct-align のオプション番号
#define OPTION_SMBP_STRUCT_ALIGN	284

/// \brief --smbp-mem-align のオプション番号
#define OPTION_SMBP_MEM_ALIGN		285

/// \brief --debug-flags のオプション番号
#define OPTION_DEBUG_FLAGS		513

/// \brief struct option の最初のメンバがに対するキャスト
///
/// Solarisでは struct option の最初のメンバが const char* では
/// なく char* であるので、強制的にconstを取り除く
///
/// それ以外では const char* だろうから空
#if defined(__sun) && defined (_GETOPT_H)
#define LONG_OPT_CAST	(char*)
#else
#define LONG_OPT_CAST
#endif

/// @}

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup CSP_TRANSLATOR
    ///
    /// @{

    //
    // CSPトランスレータのサポート機能のグループの宣言
    //
    // サポート機能の実体はどこにもないので、とりあえずここで、サポート
    // 機能の namespace に対する doxygen のグループを作成しておく
    //

    /// \defgroup CSP_TRANSLATOR_SUPPORT CSPトランスレータのサポート機能
    ///
    /// \brief CSPトランスレータのサポート機能
    ///
    /// @{

    /// \brief サポート機能の名前空間
    ///
    /// CSPトランスレータのサポート機能
    namespace Support { }

    /// @}

    /// \defgroup COMMON_VARIABLES CSPトランスレータの共通変数
    ///
    /// \brief CSPトランスレータの共通変数
    ///
    /// @{

    /// \brief ブロック名のプレフィクス(任意)
    ///
    /// 現在はこれは空文字列である
    std::string block_name_prefix;

    /// \brief 最大のタスク番号
    long max_task_id = -1;

    /* options */

    /// \brief verboseオプション(-v)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int verbose = 0;

    /// \brief コア数(-N)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int core_num = DEFAULT_CORE_NUM;

    /// \brief テストコードを生成する(-y)[ターゲット依存]
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int gen_c_test_code = 0;

    /// \brief ターゲット指定(-T)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int code_target = 0;

    /// \brief 大域変数をstatic宣言する(--staticalize)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int do_staticalize = 0;

    /// \brief gcc以外のコンパイラ用のコード生成にする(--no-gcc)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int no_gcc = 0;

    /// \brief CSPトランスレータのコンフィグレーションの読み込み
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    const char* config_file = NULL;

    /// @}

    /// \defgroup CSP_TRANSLATOR_MAIN CSPトランスレータのmain関数部
    ///
    /// \brief CSPトランスレータのmain関数部
    ///
    /// CSPトランスレータの main() である。動作はそちらを参照。
    ///
    /// コマンドラインオプションは CSP_TRANSLATOR_MAIN_OPTIONS を参照
    ///
    /// \sa main()
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    ///
    /// @{

    /// \defgroup CSP_TRANSLATOR_MAIN_OPTIONS CSPトランスレータのオプション
    ///
    /// \brief CSPトランスレータのコマンドラインオプション
    ///
    /// CSPトランスレータのオプションは以下の通り
    /// - -s <em>モデル.xml</em>,
    ///   --xml-csp=<em>モデル.xml</em>\n
    ///   入力するBLXMLファイルを指定する
    /// - -v, --verbose\n
    ///   verboseモードにする。色々な情報が出力されるので、
    ///   デバッグや入力内容の確認に利用する
    /// - -T <em>ターゲット</em>,
    ///   --target=<em>ターゲット</em>\n
    ///   コード生成のターゲットを指定する
    ///   <em>ターゲット</em> はmcosとatk2とsmbpが使用可能
    /// - -N <em>コア数</em>,
    ///   --number-of-core=<em>コア数</em>\n
    ///   コア数を指定する。デフォルトは8、最大は256である。
    /// - -X <em>出力ファイル</em>,
    ///   --output-code=<em>出力ファイル</em>\n
    ///   コード出力ファイルを指定する。オプションを指定しない場合、
    ///   出力ファイルは、<em>ターゲット</em>_default.c となる。
    /// - -o <em>dotファイル</em>,
    ///   --output-csp=<em>dotファイル</em>\n
    ///   出力するCSPイメージを指定する。本出力ファイル形式はdotである。
    ///   省略時は標準出力に出力する
    /// - --label-only-task-id\n
    ///   CSPイメージファイル(dot形式)のラベルを
    ///   CSPプロセスのタスク番号だけにする
    /// - -d <em>解析結果ファイル</em>,
    ///   --output-var=<em>解析結果ファイル</em>\n
    ///   指定した解析結果ファイルに、各タスク毎に、入力変数、出力変数、
    ///   タスクコードをCSV形式で出力する
    /// - -i <em>ファイル</em>,
    ///   --c-include=<em>ファイル</em>\n
    ///   Cコード中に指定した <em>ファイル</em>
    ///   のinclude文を出力する。
    ///   このオプションは複数回指定でき、指定した順序でCコードに出力する
    /// - -P <em>param=val</em>,
    ///   --set-param=<em>param=val</em>\n
    ///   ブロックのサンプル時間にシンボルが使われている場合、
    ///   そのシンボルの値を指定する
    /// - --xsd-validation\n
    ///   スキーマと入力BLXMLファイルのvalidationを行う\n
    ///   デフォルトでは行わない
    /// - --schema-loc=<em>スキーマ</em>\n
    ///   validationするスキーマの場所を指定する。以下が使用できる
    ///   -# URIとして指定する場合は、"file://"を付けた絶対パス
    ///   -# 先頭が '/' で始まった絶対パス
    ///   -# 環境変数 MBP_SCHEMAS が設定されていれば、そのディレクトリ
    ///      の一致するファイル
    ///   -# カレントディレクトリからの相対パス
    ///   .
    ///   このオプションは自動的に --xsd-validation を有効にする\n
    ///   なお、デフォルトではカレントディレクトリの SimulinkModel.xsd
    ///   を使用する
    /// - --config-file=<em>設定ファイル</em>\n
    ///   CSPトランスレータに追加で与える設定ファイルを指定する。\n
    ///   デフォルトでは設定ファイルは読み込まない\n
    ///   \sa SMBP::yaml_cb_SystemBuilderParametes(YAMLObj* obj)
    ///   \sa SMBP::yaml_cb_EmbeddedCoderTypes(YAMLObj* obj)
    ///   \sa SMBP::yaml_cb_SystemBuilderTypes(YAMLObj* obj)
    /// - --create-t0=[<em>ny</em>]\n
    ///   全体同期を行うtask0(t0)を生成するかどうかを指定する。\n
    ///   \a n の場合はtask0を生成しない。
    ///   デフォルトは \a n \n
    ///   \a y の場合はtask0を生成する
    /// - --ignore-to-sink=[<em>yn</em>]\n
    ///   \a y の時、Sinkブロック(例えばToWorkSpace)を削除する。
    ///   デフォルトは \a y \n
    ///   \a n の時は削除しない。
    /// - --ignore-isolated=[<em>yn</em>]\n
    ///   \a y の時、孤立したブロックを削除する。
    ///   デフォルトは \a y \n
    ///   \a n の時は削除しない。
    ///   デフォルトは \a y
    /// - --ignore-leaves=[<em>ny</em>]\n
    ///   \a y の時、出力のないブロックを削除する。\n
    ///   \a n の時は削除しない。
    ///   デフォルトは \a n
    /// - --enable-multirate\n
    ///   マルチレートでのコード生成を有効にする。
    ///   ブロックは異なるレートで動作する。\n
    ///   デフォルトで有効
    /// - --disable-multirate\n
    ///   マルチレートでのコード生成を無効にする。
    ///   ブロックはベースレートで動作する。
    /// - --low-rate-pulsegenerator=[yn]\n
    ///   \a y の時、DiscretePulseGenerator のレートを、Periodと
    ///   PulseWidthから決定し、DiscretePulseGenerator
    ///   のコードを書き換える。
    ///   デフォルトでは \a y \n
    ///   \a n の時はベースレートでの動作となる。
    /// - --no-gcc\n
    ///   GNU C拡張を使わないようにする。\n
    ///   デフォルトでは無効(gcc拡張を使用する)\n
    ///   入れ子関数やそれに付随する変数等はタスク関数外に出力する。
    /// - --staticalize\n
    ///   タスク関数内に生成する入出力変数、状態変数、パラメタ等は通常
    ///   自動変数だが、これをstatic宣言する。\n
    ///   デフォルトでは無効。\n
    ///   これは、自動変数ではスレッドのスタックサイズが大きくなりすぎ
    ///   る場合に使用するが、自動変数とは異なり、コンパイラの最適化が
    ///   期待できないため、実行性能が低下する可能性がある。
    /// - --enable-csr\n
    ///   eMCOSのCSR(Cyclic Service Routine)を用いて、異なるサンプル時
    ///   間のブロックを駆動する。ただし、周期は最小で1msなので、実行時
    ///   間が長くなることが予想される。
    /// - --disable-csr\n
    ///   異なるサンプル時間のブロックの駆動に、eMCOSのCSR(Cyclic
    ///   Service Routine)を用いない。ブロックは、ブロック間の周期比の
    ///   入力で駆動される。\n
    ///   デフォルトで無効。
    /// - --atk2-appmode=<em>AppMode</em>\n
    ///   TOPPERS/ATK2において、モデルのAppModeを指定する。\n
    ///   デフォルトでは　<em>モデル名_AppMode</em>である
    /// - -y <em>出力YAMLファイル</em>,\n
    ///   --atk2-abrex-yaml=<em>出力YAMLファイル</em>\n
    ///   TOPPERS/ATK2において、出力するABREX用のYAMLファイルを指定する。\n
    ///   デフォルトでは <em>モデル名</em>-app.yaml である
    /// - -H <em>出力ヘッダファイル</em>,\n
    ///   --atk2-app-header=<em>出力ヘッダファイル</em>\n
    ///   TOPPERS/ATK2において、出力するヘッダファイルを指定する。\n
    ///   デフォルトでは <em>モデル名</em>-app.h である
    /// - --atk2-timer-isrref=<em>ISR</em>\n
    ///   Core0に設定するタイマ用ISRを指定する。\n
    ///   デフォルトでは <em>SwCntTimerHdr</em> であり、YAMLには、
    ///   <em>/Ecuc/Os/SwCntTimerHdr</em> と出力する
    /// - --atk2-no-timer-isrref\n
    ///   Core0にタイマ用のISRを出力しない。\n
    ///   デフォルトでは出力する
    /// - --atk2-timer-counterref=<em>COUNTER</em>\n
    ///   アラーム用のカウンタを指定する。\n
    ///   デフォルトでは <em>SwCnt0</em> である。\n
    ///   たとえは MAIN_HW_COUNTER_CORE0 といったハードウェアカウンタを
    ///   使用する場合、 <em>--atk2-no-timer-isrref</em> と共に使用する
    ///   必要がある。\n
    ///   カウンタの周波数はカウンタ名に <em>_TIMER_CLOCK_HZ</em>
    ///   をポストフィクスしたマクロによって定義される必要がある
    /// - --atk2-timer-hz=<em>タイマの周波数</em>\n
    ///   タイマの周波数がマクロ定義されていない場合に使用する周波数を
    ///   指定する。\n
    ///   Cコンパイラで評価されるので指定するものは数値に限らない。\n
    ///   デフォルトでは1000である。
    /// - --atk2-gen-main\n
    ///   main関数およびStartupHookを出力する場合に指定する。\n
    ///   デフォルトでは出力しない
    /// - --smbp-prefix=<em>プレフィクス</em>\n
    ///   SystemBuilderにおいては出力ファイルのプレフィクスを指定する。\n
    ///   デフォルトでは <em>モデル名</em> である。
    /// - --smbp-yaml=<em>出力YAMLファイル</em>\n
    ///   SystemBuilderにおいては出力YAMLファイル名を指定する。\n
    ///   デフォルトでは <em>プレフィクス</em>-smbp.yaml である
    /// - --smbp-long64\n
    ///   SystemBuilderにおいては例えばLP64のような環境で、
    ///   longが64bitの場合に指定する。\n
    ///   デフォルトでは long は32bitとして扱う
    /// - --smbp-real64
    ///   SystemBuilderにおいて、Embedded Coderの real_T
    ///   が double(64bit) の場合に指定する。\n
    ///   デフォルトでは float(32bit) である。
    /// - --smbp-long64-align=<em>バイト数</em>\n
    ///   64ビット整数のアラインメントを指定する。\n
    ///   デフォルトでは4バイト
    /// - --smbp-real64-align=<em>バイト数</em>\n
    ///   64ビット浮動小数点(double)のアラインメントを指定する。\n
    ///   デフォルトでは4バイト
    /// - --smbp-struct-align=<em>バイト数</em>\n
    ///   構造体のアラインメントを指定する。\n
    ///   デフォルトでは4バイト
    /// - --smbp-mem-align=<em>バイト数</em>\n
    ///   SystemBuilderのメモリチャネルのアラインメントを指定する。\n
    ///   デフォルトでは64バイト
    ///
    /// @{

    /// \brief longオプションの設定
    static const struct option long_options[] = {
      { LONG_OPT_CAST"verbose", no_argument, NULL, 'v', },
      { LONG_OPT_CAST"target", required_argument, NULL, 'T', },
      { LONG_OPT_CAST"number-of-core", required_argument, NULL, 'N', },
      { LONG_OPT_CAST"xml-csp", required_argument, NULL, 's', },
      { LONG_OPT_CAST"output-code", required_argument, NULL, 'X', },
      { LONG_OPT_CAST"output-csp", required_argument, NULL, 'o', },
      { LONG_OPT_CAST"label-only-task-id",
	no_argument, NULL, OPTION_LABEL_ONLY_TASK_ID, },
      { LONG_OPT_CAST"output-var", required_argument, NULL, 'd', },
      { LONG_OPT_CAST"c-include", required_argument, NULL, 'i', },
      { LONG_OPT_CAST"set-param", required_argument, NULL, 'P', },

      // XMLオプション
      { LONG_OPT_CAST"schema-loc",
	required_argument, NULL, OPTION_SCHEMA_LOCATION, },
      { LONG_OPT_CAST"xsd-validation",
	no_argument, NULL, OPTION_XSD_VALIDATION, },

      // CSPオプション
      { LONG_OPT_CAST"enable-multirate",
	no_argument, NULL, OPTION_ENABLE_MULTIRATE, },
      { LONG_OPT_CAST"disable-multirate",
	no_argument, NULL, OPTION_DISABLE_MULTIRATE, },
      { LONG_OPT_CAST"create-t0", required_argument, NULL, OPTION_CREATE_T0, },
      { LONG_OPT_CAST"ignore-to-sink",
	required_argument, NULL, OPTION_IGNORE_TO_SINK, },
      { LONG_OPT_CAST"ignore-isolated",
	required_argument, NULL, OPTION_IGNORE_ISOLATED_BLOCK, },
      { LONG_OPT_CAST"ignore-leaves",
	required_argument, NULL, OPTION_IGNORE_LEAF_BLOCK, },
      { LONG_OPT_CAST"low-rate-pulsegenerator",
	required_argument, NULL, OPTION_LOW_RATE_PULSEGENERATOR, },

      { LONG_OPT_CAST"config-file",
	required_argument, NULL, OPTION_CONFIG_FILE, },

      // eMCOS用オプション
      { LONG_OPT_CAST"no-gcc", no_argument, NULL, OPTION_NO_GCC, },
      { LONG_OPT_CAST"staticalize", no_argument, NULL, OPTION_STATICALIZE, },
      { LONG_OPT_CAST"enable-csr", no_argument, NULL, OPTION_ENABLE_CSR, },
      { LONG_OPT_CAST"disable-csr", no_argument, NULL, OPTION_DISABLE_CSR, },

      // TOPPERS/ATK2
      { LONG_OPT_CAST"atk2-appmode",
	required_argument, NULL, OPTION_ATK2_APPMODE, },
      { LONG_OPT_CAST"atk2-abrex-yaml", required_argument, NULL, 'y', },
      { LONG_OPT_CAST"atk2-app-header", required_argument, NULL, 'H', },
      { LONG_OPT_CAST"atk2-timer-isrref",
	required_argument, NULL, OPTION_ATK2_TIMER_ISRREF, },
      { LONG_OPT_CAST"atk2-no-timer-isrref",
	no_argument, NULL, OPTION_ATK2_NO_TIMER_ISRREF, },
      { LONG_OPT_CAST"atk2-timer-counterref",
	required_argument, NULL, OPTION_ATK2_TIMER_COUNTERREF, },
      { LONG_OPT_CAST"atk2-timer-hz",
	required_argument, NULL, OPTION_ATK2_TIMER_HZ, },
      { LONG_OPT_CAST"atk2-gen-main",
	no_argument, NULL, OPTION_ATK2_GEN_MAIN, },

      // SystemBuilder
      { LONG_OPT_CAST"smbp-yaml",
	required_argument, NULL, OPTION_SMBP_YAML, },
      { LONG_OPT_CAST"smbp-prefix",
	required_argument, NULL, OPTION_SMBP_PREFIX, },
      { LONG_OPT_CAST"smbp-long64", no_argument, NULL, OPTION_SMBP_LONG64, },
      { LONG_OPT_CAST"smbp-real64", no_argument, NULL, OPTION_SMBP_REAL64, },
      { LONG_OPT_CAST"smbp-long64-align",
	required_argument, NULL, OPTION_SMBP_LONG64_ALIGN, },
      { LONG_OPT_CAST"smbp-real64-align",
	required_argument, NULL, OPTION_SMBP_REAL64_ALIGN, },
      { LONG_OPT_CAST"smbp-struct-align",
	required_argument, NULL, OPTION_SMBP_STRUCT_ALIGN, },
      { LONG_OPT_CAST"smbp-mem-align",
	required_argument, NULL, OPTION_SMBP_MEM_ALIGN, },

      /* private options */
      { LONG_OPT_CAST"debug-flags",
	required_argument, NULL, OPTION_DEBUG_FLAGS, },

      { LONG_OPT_CAST NULL, 0, (int*)NULL, 0, },
    };

    /// @}

    /// \brief スキーマのロケーションのURIを生成する
    ///
    /// \param[out] pathurl スキーマのパスのURL
    /// \param[in] dir スキーマがあるディレクトリ(またはNULL)
    /// \param[in] path スキーマのパス名またはファイル名
    ///
    /// \retval true 成功
    /// \retval false エラー
    ///
    /// -# \a dir が指定されている場合は、\a dir と \a path を連結する
    /// -# realpath() で \a path を実体パスにする
    /// -# 正しく処理できたら true を返す。それ以外であれば false を返す
    static bool
    absuri (std::string& pathurl, const char *dir, const char *path)
    {
      char file[PATH_MAX];
      char rfile[PATH_MAX + 7]; /* with "file://" */

      if (dir)
	{
	  size_t dlen = strlen (dir);
	  size_t plen = strlen (path);
	  if (dlen + 1 + plen >= PATH_MAX)
	    {
	      std::cerr << "schema path " << dir << '/' << path << ": too long"
			<< std::endl;
	      return false;
	    }

	  strcpy (file, dir);
	  file[dlen] = '/';
	  strcpy (file + dlen + 1, path);
	  path = file;
	}

      strcpy (rfile, "file://");
      if (realpath (path, rfile + 7) == NULL)
	{
	  std::cerr << file << ": " << strerror (errno) << std::endl;
	  return false;
	}

      pathurl = rfile;
      return true;
    }


    /// \brief タスクの変数をCSVファイル形式で出力する
    ///
    /// \param[in] taskvar タスクの変数を出力するファイル
    ///
    /// \retval 0 成功
    /// \retval -1 失敗
    ///
    /// トランスレート結果の確認用に、タスクの入力、
    /// 出力変数や実行コードをCSV形式で出力する。
    ///
    /// なお入出力変数は1変数/1セルである。そのため、
    /// 複数の変数がある場合は、複数行で出力を行う
    static int
    csv_output_task_var (char const *taskvar)
    {
      std::ofstream ofs (taskvar);
      if (!ofs)
	{
	  std::cerr << "Couldn't open file: " << taskvar
		    << ": " << strerror (errno) << std::endl;
	  return -1;
	}

      ofs.exceptions (std::ostream::failbit | std::ostream::badbit);

      try
	{
	  ofs << "\"TASK\",\"INPUT\",\"OUTPUT\",\"POINTER\",\"EXPRESSION\"\r\n";

	  for (CspNodeList::const_iterator
		 ci (CspNode::csp_nodelist ().begin ());
	       ci != CspNode::csp_nodelist ().end (); ci++)
	    {
	      CspNode* cn = *ci;

	      if (cn->info () & (CSP_NODE_INFO_TASK_0|CSP_NODE_INFO_TASK_EXTOUT))
		continue;

	      ofs << "\"task" << cn->id () << "\",,,,\r\n";

	      PropagateVarList::const_iterator
		pvii (cn->input_var ().order ().begin ());
	      PropagateVarList::const_iterator
		pvoi (cn->output_var ().order ().begin ());

	      while (pvii != cn->input_var ().order ().end ()
		     || pvoi != cn->output_var ().order ().end ())
		{
		  ofs << ',';

		  if (pvii != cn->input_var ().order ().end ())
		    {
		      const PropagateVar* pv = *pvii;
		      ofs << "\"" << (*pv)->name () << "\"";
		      pvii++;
		    }

		  ofs << ',';

		  if (pvoi != cn->output_var ().order ().end ())
		    {
		      const PropagateVar* pv = *pvoi;
		      ofs << "\"" << (*pv)->name () << "\"";
		      pvoi++;
		    }

		  ofs << ",,\r\n";	// no <POINTER>
		}

	      CodeList::const_iterator cdi = cn->code ().find_type_code ("task");
	      if (cdi != cn->code ().end ())
		{
		  for (; cdi != cn->code().end (); cdi++)
		    {
		      const code_ent_T& code = *cdi;
		      if (code->type () != "task")
			break;

		      for (code_frag_list_T::const_iterator
			     cfi (code.code_frag_list ().begin ());
			   cfi != code.code_frag_list ().end (); cfi++)
			{
			  const code_frag_T& cf = *cfi;
			  if (cf)
			    {
			      ofs << ",,,,\"";

			      const std::string& c = *cf;
			      std::string::size_type p = 0;
			      std::string::size_type n;
			      while (1)
				{
				  n = c.find ('"', p);
				  if (n == std::string::npos)
				    break;
				  n++;
				  ofs << c.substr (p, n - p);
				  ofs << '"';
				  p = n;
				}

			      ofs << c.substr (p)
				  << "\"\r\n";
			    }
			}
		    }
		}
	    }
	  return 0;
	}

      catch (std::ostream::failure e)
	{
	  std::cerr << taskvar << ": " << e.what () << std::endl;
	}

      return -1;
    }

    /// \brief サンプル時間のシンボルと値を設定する
    ///
    /// \param[in] param 設定するパラメタ
    ///
    /// \retval 0 正常
    /// \retval -1 エラー
    ///
    /// パラメタは、"シンボル=値"の形式である
    static int
    set_param (const char* param)
    {
      const std::string strparam (param);
      std::string::size_type x = strparam.find ('=');
      if (x != std::string::npos)
	{
	  std::istringstream iss (strparam.substr (x + 1));
	  double pv;
	  iss >> pv;
	  if (iss.eof ())
	    {
	      SampleTime::set_param (strparam.substr (0, x), pv);
	      return 0;
	    }
	}
      return 0;
    }
  }

  /// @}

  /// @}
}

using namespace Simulink::CspTrans;
using namespace Simulink::CspTrans::Support::YAML;

/// \addtogroup CSP_TRANSLATOR_MAIN
///
/// @{

/// \brief CSPトランスレータのmain関数
///
/// \param[in] argc コマンド引数の個数
/// \param[in] argv コマンド引数
///
/// \retval 0 成功
/// \retval それ以外 エラー
///
/// - BLXMLの読み込み\n
///   Simulink::CspTrans::CspReader::input_xml (const char* file, const char* ns = NULL, const char* sc = NULL, bool validation = false)
/// - Dotファイルの出力\n
///   Simulink::CspTrans::CspReader::output_csp(const char* file)
/// - eMCOSコード生成\n
///   mcos_generate_code(const char* mcos_out)
///
/// \sa Simulink::CspTrans::CspReader::input_xml (const char* file, const char* ns, const char* sc, bool validation)
/// \sa Simulink::CspTrans::CspReader::output_csp(const char* file)
/// \sa mcos_generate_code(const char* mcos_out)
int
main (int argc, char *argv[])
{
  int c;
  char const *output = NULL;
  char const *taskvar = NULL;
  char const *code_out = NULL;
  char *p;
  char const *xml_csp = NULL;
  int gen_type = 'm';	/* mcos */
  int option_index = 0;
  int xsd_validation = 0;
  int ret;
  const char* default_target = "mcos";
  bool linked_mcos = true;

  std::string scloc_ns;
  std::string scloc_loc;

  // eMCOSのコード生成がない場合はATK2をデフォルト
  if (mcos_generate_code == NULL)
    {
      gen_type = 'a';
      default_target = "atk2";
      linked_mcos = false;
    }

  while ((c = getopt_long (argc, argv,
			   "vT:s:X:o:d:i:P:N:"
			   "y:",
			   long_options, &option_index)) != -1)
    {
      switch (c)
	{
	case 'v':
	  verbose = 1;
	  break;

	case 'T':
	  if (strcmp (optarg, "mcos") == 0 && linked_mcos == true)
	    gen_type = 'm';
	  else if (strcmp (optarg, "atk2") == 0
		   && atk2_generator_init != NULL
		   && atk2_generate_code != NULL)
	    gen_type = 'a';
	  else if (strcmp (optarg, "smbp") == 0
		   && smbp_generator_init != NULL
		   && smbp_generate_code != NULL)
	    gen_type = 'S';
	  else
	    {
	      std::cerr << "Bad Target: " << optarg << std::endl;
	      goto usage;
	    }

	  code_target = gen_type;
	  break;

	case 's':
	  xml_csp = optarg;
	  break;

	case 'X':
	  code_out = optarg;
	  break;

	case 'o':
	  output = optarg;
	  break;

	case 'd':
	  taskvar = optarg;
	  break;

	case 'i':
	  c_codewriter.c_include_headers ().add_header_file (optarg);
	  break;

	case 'N':
	  core_num = (int)strtol (optarg, &p, 0);
	  if (*p || core_num <= 0 || MAX_CORE_NUM < core_num)
	    {
	      std::cerr << "Invalid Core Num: " << optarg << std::endl;
	      goto usage;
	    }
	  break;

	case 'P':
	  if (set_param (optarg) == -1)
	    {
	      std::cerr << "invalid parameter: " << optarg << std::endl;
	      goto usage;
	    }
	  break;

	case OPTION_DEBUG_FLAGS:
	  for (; *optarg; optarg++)
	    {
	      switch (*optarg)
		{
		case 'c':
		  gen_c_test_code = 1;
		  break;

		default:
		  std::cerr << "Invalid DebgOption: " << optarg << std::endl;
		  goto usage;
		}
	    }
	  break;

	case OPTION_CREATE_T0:
	  if (optarg[0] == 'y' || optarg[0] == 'Y')
	    do_create_t0 = 1;
	  else if (optarg[0] == 'n' || optarg[0] == 'N')
	    do_create_t0 = 0;
	  else
	    {
	      std::cerr << "Wrong Option: --create-t0="
			<< optarg << std::endl;
	      goto usage;
	    }

	  break;

	case OPTION_IGNORE_TO_SINK:
	  if (optarg[0] == 'y' || optarg[0] == 'Y')
	    ignore_to_sink = 1;
	  else if (optarg[0] == 'n' || optarg[0] == 'N')
	    ignore_to_sink = 0;
	  else
	    {
	      std::cerr << "Wrong Option: --ignore-to-sink="
			<< optarg << std::endl;
	      goto usage;
	    }

	  break;

	case OPTION_IGNORE_ISOLATED_BLOCK:
	  if (optarg[0] == 'y' || optarg[0] == 'Y')
	    ignore_isolated_block = 1;
	  else if (optarg[0] == 'n' || optarg[0] == 'N')
	    ignore_isolated_block = 0;
	  else
	    {
	      std::cerr << "Wrong Option: --ignore-isolated="
			<< optarg << std::endl;
	      goto usage;
	    }

	  break;

	case OPTION_IGNORE_LEAF_BLOCK:
	  if (optarg[0] == 'y' || optarg[0] == 'Y')
	    ignore_leaf_block = 1;
	  else if (optarg[0] == 'n' || optarg[0] == 'N')
	    ignore_leaf_block = 0;
	  else
	    {
	      std::cerr << "Wrong Option: --ignore-leaves="
			<< optarg << std::endl;
	      goto usage;
	    }

	  break;

	case OPTION_XSD_VALIDATION:
	  xsd_validation = 1;
	  break;

	case OPTION_STATICALIZE:
	  do_staticalize = 1;
	  break;

	case OPTION_LABEL_ONLY_TASK_ID:
	  label_only_task_id = 1;
	  break;

	case OPTION_ENABLE_MULTIRATE:
	  enable_multirate = 1;
	  break;

	case OPTION_DISABLE_MULTIRATE:
	  enable_multirate = 0;
	  break;

	case OPTION_LOW_RATE_PULSEGENERATOR:
	  if (optarg[0] == 'y' || optarg[0] == 'Y')
	    low_rate_pulsegenerator = 1;
	  else if (optarg[0] == 'n' || optarg[0] == 'N')
	    low_rate_pulsegenerator = 0;
	  else
	    {
	      std::cerr << "Wrong Option: --low-rate-pulsegenerator="
			<< optarg << std::endl;
	      goto usage;
	    }
	  break;

	case OPTION_NO_GCC:
	  no_gcc = 1;
	  break;

	case OPTION_ENABLE_CSR:
	  if (&enable_csr == NULL)
	    {
	      std::cerr << "Couldn't use : --enable-csr" << std::endl;
	      goto usage;
	    }
	  enable_csr = 1;
	  break;

	case OPTION_DISABLE_CSR:
	  if (&enable_csr == NULL)
	    {
	      std::cerr << "Couldn't use : --disable-csr" << std::endl;
	      goto usage;
	    }
	  enable_csr = 0;
	  break;

	case OPTION_SCHEMA_LOCATION:
	  {
	    p = strchr (optarg, '=');
	    if (p == NULL)
	      goto usage;
	    size_t len = p - optarg;

	    scloc_ns = std::string (optarg, len);

	    ++p;
	    if (strncmp (p, "file:///", 8))
	      {
		/* already ok */
		scloc_loc = p;
	      }
	    else if (p[0] == '/')
	      {
		/* path check */
		absuri (scloc_loc, (const char*)NULL, p);
	      }
	    else
	      {
		const char* mbploc = getenv ("MBP_SCHEMAS");
		if (mbploc)
		  absuri (scloc_loc, mbploc, p);

		if (scloc_loc.size () == 0)
		  {
		    char cwd[PATH_MAX];
		    if (getcwd (cwd, sizeof cwd) == NULL)
		      {
			std::cerr << "getcwd: " << strerror (errno)
				  << std::endl;
			return 1;
		      }
		    absuri (scloc_loc, cwd, p);
		  }
	      }

	    if (scloc_loc.size () == 0)
	      {
		std::cerr << "Invalid Schema Location: "
			  << optarg << std::endl;
		goto usage;
	      }

	  }
	  break;

	case OPTION_ATK2_APPMODE:
	  if (&atk2_appmode == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-appmode" << std::endl;
	      goto usage;
	    }

	  atk2_appmode = optarg;
	  break;

	case 'y':
	  if (&atk2_abrex_yaml == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-abrex-yaml" << std::endl;
	      goto usage;
	    }
	  atk2_abrex_yaml = optarg;
	  break;

	case 'H':
	  if (&atk2_app_header == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-app-header" << std::endl;
	      goto usage;
	    }
	  atk2_app_header = optarg;
	  break;

	case OPTION_ATK2_TIMER_ISRREF:
	  if (&atk2_timer_isrref == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-timer-isrref" << std::endl;
	      goto usage;
	    }

	  if (optarg[0] == 0)
	    atk2_no_timer_isrref = true; // no output isrref
	  else
	    {
	      if (isspace (((unsigned char*)optarg)[0]))
		{
		  std::cerr << "Invalid CounterRef: " << optarg << std::endl;
		  goto usage;
		}
	      atk2_timer_isrref = optarg;
	    }

	  break;

	case OPTION_ATK2_NO_TIMER_ISRREF:
	  if (&atk2_no_timer_isrref == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-no-timer-isrref"
			<< std::endl;
	      goto usage;
	    }

	  atk2_no_timer_isrref = true; // no output isrref
	  break;

	case OPTION_ATK2_TIMER_COUNTERREF:
	  if (&atk2_timer_counterref == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-timer-counterref"
			<< std::endl;
	      goto usage;
	    }

	  if (optarg[0]
	      && !isspace (((unsigned char *)optarg)[0]))
	    atk2_timer_counterref = optarg;
	  else
	    {
	      std::cerr << "Invalid CounterRef: " << optarg << std::endl;
	      goto usage;
	    }

	  break;

	case OPTION_ATK2_TIMER_HZ:
	  if (&atk2_timer_hz == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-timer-hz" << std::endl;
	      goto usage;
	    }

	  atk2_timer_hz = optarg;
	  break;

	case OPTION_ATK2_GEN_MAIN:
	  if (&atk2_gen_main == NULL)
	    {
	      std::cerr << "Couldn't use : --atk2-gen-main" << std::endl;
	      goto usage;
	    }

	  atk2_gen_main = true;
	  break;

	case OPTION_CONFIG_FILE:
	  config_file = optarg;
	  break;

	case OPTION_SMBP_YAML:
	  if (&smbp_yaml == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-yaml" << std::endl;
	      goto usage;
	    }

	  smbp_yaml = optarg;
	  break;

	case OPTION_SMBP_PREFIX:
	  if (&smbp_prefix == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-prefix" << std::endl;
	      goto usage;
	    }

	  smbp_prefix = optarg;
	  break;

	case OPTION_SMBP_LONG64:
	  if (&smbp_long_is_64bit == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-long64" << std::endl;
	      goto usage;
	    }

	  smbp_long_is_64bit = true;
	  break;

	case OPTION_SMBP_REAL64:
	  if (&smbp_real_is_64bit == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-real64" << std::endl;
	      goto usage;
	    }

	  smbp_real_is_64bit = true;
	  break;

	case OPTION_SMBP_LONG64_ALIGN:
	  if (&smbp_long64_align == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-long64-align" << std::endl;
	      goto usage;
	    }
	  smbp_long64_align = strtoul (optarg, &p, 0);
	  if (*p
	      || (smbp_long64_align != 1
		  && smbp_long64_align != 4
		  && smbp_long64_align != 8))
	    {
	      std::cerr << "Invalid alignment: " << optarg << std::endl;
	      goto usage;
	    }
	  break;

	case OPTION_SMBP_REAL64_ALIGN:
	  if (&smbp_real64_align == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-real64-align" << std::endl;
	      goto usage;
	    }
	  smbp_real64_align = strtoul (optarg, &p, 0);
	  if (*p
	      || (smbp_real64_align != 1
		  && smbp_real64_align != 4
		  && smbp_real64_align != 8))
	    {
	      std::cerr << "Invalid alignment: " << optarg << std::endl;
	      goto usage;
	    }
	  break;

	case OPTION_SMBP_STRUCT_ALIGN:
	  if (&smbp_struct_align == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-struct-align" << std::endl;
	      goto usage;
	    }
	  smbp_struct_align = strtoul (optarg, &p, 0);
	  if (*p
	      || (smbp_struct_align != 1
		  && smbp_struct_align != 4
		  && smbp_struct_align != 8))
	    {
	      std::cerr << "Invalid alignment: " << optarg << std::endl;
	      goto usage;
	    }
	  break;

	case OPTION_SMBP_MEM_ALIGN:
	  if (&smbp_mem_align == NULL)
	    {
	      std::cerr << "Couldn't use : --smbp-mem-align" << std::endl;
	      goto usage;
	    }
	  smbp_mem_align = strtoul (optarg, &p, 0);
	  if (*p
	      || (smbp_mem_align != 1
		  && smbp_mem_align != 4
		  && smbp_mem_align != 8))
	    {
	      std::cerr << "Invalid alignment: " << optarg << std::endl;
	      goto usage;
	    }
	  break;

	default:
	  {
	    std::cerr << "Bad Option@" << optind << std::endl;
	  usage:
	    std::cerr << "usage: " << argv[0]
		      << " -s csp.xml"
	      " [-v] "
	      " [-T " << default_target << "] "
	      " [-N core_num] "
	      " [-X out.c] "
	      " [-o out.dot]"
	      " [-d out.csv] "
	      " [-i c_inc_hdr]"
	      " [-P param=val]\n"
	      "\n"
	      "-s, --xml-csp                  "
	      "input XML CSP\n"
	      "-v, --verbose                  "
	      "verbose\n"
	      "-T, --target                   "
	      "target ("
		      << (linked_mcos
			  ? "atk2, smbp or mcos"
			  : "atk2 or smbp")
		      << ")\n"
	      "-N, --number-of-core           "
	      "number of core (default 8, Max 256)\n"
	      "-X, --output-code              "
	      "output target code\n"
	      "-o, --output-csp               "
	      "output CSP(dot) file\n"
	      "    --label-only-task-id       "
	      "only TaskID in dot file\n"
	      "-d, --output-var               "
	      "output variable list\n"
	      "-i, --c-include                "
	      "set C include file\n"
	      "-P, --set-param=PARAM=VALUE    "
	      "set VALUE to PARAM for block rate\n"
	      "    --config-file=config.yaml  "
	      "load configuration file\n"
	      "(XML Options)\n"
	      "    --xsd-validation           "
	      "enable XML Schema validation\n"
	      "    --schema-loc=<ns>=<loc>    "
	      "Set Schema Location(auto enable validation)\n"

	      "(CSP Options)\n"
	      "    --create-t0=[yn]           "
	      "Create/NotCreate t0 task (default n)\n"
	      "    --ignore-to-sink=[yn]      "
	      "ignore Sink Block (e.g. ToWorkSpace) (default y)\n"
	      "    --ignore-isolated=[yn]     "
	      "ignore isolated blocks (default y)\n"
	      "    --ignore-leaves=[ny]       "
	      "ignore leaf blocks (default n)\n"
	      "    --enable-multirate         "
	      "generate multirate C code (default)\n"
	      "    --disable-multirate        "
	      "disable generate mutirate C code\n"
	      "    --low-rate-pulsegenerator=[yn]\n"
	      "                               "
	      "low rate DiscretePulseGenerator (default y)\n"


	      "(Code Generation Options)\n"
	      "    --no-gcc                   "
	      "generate code for non-gcc\n"
	      "    --staticalize              "
	      "staticalize any ontexts\n"

		      << (linked_mcos ? "(eMCOS Options)\n"
			  "    --enable-csr               "
			  "enable CSR controlled task\n"
			  "    --disable-csr              "
			  "disable CSR controled task (default)\n"
			  : "")

		      << "(TOPPERS/ATK2 Options)\n"
	      "-y, --atk2-abrex-yaml          "
	      "output ABREX YAML file(default model.yaml)\n"
	      "    --atk2-appmode=<APPMODE>\n"
	      "                               "
	      "APPMODE identifier(default model_APPMODE)\n"
	      "-H, --atk2-app-header          "
	      "output App heder file, definitions for YAML\n"
	      "    --atk2-timer-isrref=<ISR>  "
	      "set OsAppIsrRef to /Ecuc/Os/<ISR>\n"
	      "                               "
	      "(default SwCntTimerHdr)\n"
	      "    --atk2-no-timer-isrref     "
	      "no output OsAppIsrRef\n"
	      "    --atk2-timer-counterref=<COUNTER>\n"
	      "                               "
	      "set OsAppCounterRef to /Ecuc/Os/<COUNTER>\n"
	      "                               "
	      "(default SwCnt0)\n"
	      "    --atk2-timer-hz=<Hz>       "
	      "use Timer frequency unless defined Macro as:\n"
	      "                               "
	      "  <COUNTER>_TIMER_CLOCK_HZ (default 1000)\n"
	      "    --atk2-gen-main            "
	      "generate main() and StarupHook\n"

	      "    --smbp-prefix              "
	      "prefix of SMBP files (default `model')\n"
	      "    --smbp-yaml                "
	      "output SMBP YAML(default `prefix'-smbp.yaml)\n";
	  }
	  return 1;
	}
    }

  if (xml_csp == NULL)
    goto usage;

  // デフォルト出力ファイル
  if (code_out == NULL)
    {
      if (gen_type == 'm')
	code_out = "mcos_default.c";
      else if (gen_type == 'a')
	;
      else if (gen_type == 'S')
	;
      else
	{
	  std::cerr << "Unknown generation type: " << gen_type << std::endl;
	  goto error;
	}
    }

  if (gen_type == 'a' && atk2_generator_init () != 0)
    {
      std::cerr << "Error: TOPPERS/ATK2 generator initializing failed"
		<< std::endl;
      goto error;
    }

  if (gen_type == 'S' && smbp_generator_init () != 0)
    {
      std::cerr << "Error: S-MBP generator initializing failed"
		<< std::endl;
      goto error;
    }

  // コンフィグレーションが指定されている場合はそれを読み込む
  if (config_file)
    {
      YAMLParser config (config_file);
      if (config)
	{
	  if (YAMLCallBack::run_callbacks (&config))
	    {
	      std::cerr << "Error: " << config_file << ": configuration fail"
			<< std::endl;
	      goto error;
	    }
	}
      else
	{
	  std::cerr << "Error: " << config_file << ": invalid"
		    << std::endl;
	  goto error;
	}
    }

  // スキーマのロケーションが指定されている場合は、validationをする
  if (scloc_ns.size ())
    xsd_validation = 1;

  // BLXMLを入力
  ret = csp_reader.input_xml (xml_csp,
			      scloc_ns.c_str (), scloc_loc.c_str (),
			      !!xsd_validation);
  if (ret == -1)
    return 1;

  // DOTファイルを出力
  if (csp_reader.output_csp (output))
    {
      std::cerr << "Couldn't output file: " << output
		<< ": " << strerror (errno);
      goto error;
    }

  // タスクの変数を出力
  if (taskvar)
    csv_output_task_var (taskvar);

  // コード生成
  if (gen_type == 'm')
    {
      if (mcos_generate_code (code_out) == -1)
	goto error;
    }
  else if (gen_type == 'a')
    {
      atk2_output_code = code_out;
      if (atk2_generate_code () == -1)
	goto error;
    }
  else if (gen_type == 'S')
    {
      atk2_output_code = code_out;
      if (smbp_generate_code () == -1)
	goto error;
    }
  else
    {
      std::cerr << "Unknown generation type: " << gen_type << std::endl;
      goto error;
    }

  return 0;

error:
  return 1;
}

/// @}
