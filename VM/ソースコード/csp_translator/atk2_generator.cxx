// -*- mode: c++; coding: utf-8-unix; -*-

/// \file atk2_generator.cxx
///
/// \brief TOPPERS/ATK2向けコード生成

#include <vector>
#include <deque>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

#include <errno.h>
#include <string.h>

/// \brief ATK2_WEAK を空にしてweakシンボルではない実体を宣言する
#define ATK2_WEAK		// none

#include "csp_translator.h"
#include "atk2_generator_i.h"
#include "smbp_generator_i.h"

/// \addtogroup CSP_TRANSLATOR
///
/// @{

/// \defgroup ATK2_GENERATOR TOPPERS/ATK2コード生成
///
/// \brief TOPPERS/ATK2用コード生成機能
///
/// @{

/// \defgroup ATK2_GENERATOR_C_CODE TOPPERS/ATK2向けCコード生成本体
///
/// \brief TOPPERS/ATK2向けコード生成本体
///
/// @{

/// \brief コード内で、生成する変数のプレフィクス。特に不要なので空
#define ATK2_VAR_PREFIX	""

/// \brief デフォルトのタイマ用ISR
#define ATK2_DEFAULT_TIMER_ISRREF	"SwCntTimerHdr"

/// \brief デフォルトのタイマ用カウンタ
#define ATK2_DEFAULT_TIMER_COUNTERREF	"SwCnt0"

/// \brief フェールセーフのタイマの周波数
#define ATK2_DEFAULT_TIMER_HZ		"1000"

/// @}

/// @}

/// @}

using namespace SimulinkModel::XSD;
using namespace Simulink::XML;

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup ATK2_GENERATOR
    ///
    /// @{

    /// \defgroup ATK2_GENERATOR_OPTIONS ATK2コード生成用オプション変数
    ///
    /// \brief TOPPERS/ATK2用コード生成コマンドラインオプション変数
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    ///
    /// @{

    /// \brief このモデルのAPPMODEの識別子
    std::string atk2_appmode;

    /// \brief このモデル用の出力コードファイル名
    const char* atk2_output_code = NULL;

    /// \brief このモデル用のYAMLファイル名
    const char* atk2_abrex_yaml = NULL;

    /// \brief このモデル用のヘッダファイル名
    const char* atk2_app_header = NULL;

    /// \brief このモデルで使用するタイマのISR
    const char* atk2_timer_isrref = ATK2_DEFAULT_TIMER_ISRREF;

    /// \brief このモデルではタイマのISRを使用しない
    bool atk2_no_timer_isrref = false;

    /// \brief このモデルで使用するタイマのカウンタ
    const char* atk2_timer_counterref = ATK2_DEFAULT_TIMER_COUNTERREF;

    /// \brief このモデルで使用するタイマの周波数
    const char* atk2_timer_hz = ATK2_DEFAULT_TIMER_HZ;

    /// \brief 使用するタイマの周波数の定義
    std::string atk2_timer_clock_hz;

    /// \brief mainとStartupHookを生成するかどうかのフラグ
    bool atk2_gen_main = false;

    /// @}

    /// \brief TOPPERS/ATK2用コード生成の名前空間
    namespace ATK2
    {
      /// \addtogroup ATK2_GENERATOR_C_CODE
      ///
      /// @{

      /// \brief 状態変数のオブジェクトが未定義の場合の代替名
      const std::string unknown_state ("unknown_DW");

      /// \brief DataStoreMemory用状態変数のオブジェクトのプレフィクス
      const std::string dsm_prefix ("shared__");

      /// \brief モデル名
      std::string model_name ("unknown_model");

      /// \brief 処理済み関数リスト(マーク用ベクタ)
      std::vector< bool > output_function_mark;

      /// \brief 処理済み共通関数リスト
      std::vector< bool > output_common_function_mark;

      /// \brief アラーム設定を出力する TaskSet のポインタ
      ///
      /// アラーム設定は、開始オフセットが0で、最も早いレートの
      /// TaskSet のコード中に出力する
      static const TaskSet* alarm_taskset = NULL;

      /// \brief 物理コアのタスク割り当て
      std::vector < baseTaskSetList > pcore_tslist;

      /// \brief ATK2のコア数
      size_t number_of_cores;

      /// \brief TOPPERS/ATK2用タスク関連情報のセットアップ
      ///
      /// 次の事を行う
      /// -# コア割り当て指定に従い、同一のレートの CspNode
      ///    を同じ TaskSet に割り当てる
      int
      correct_task (void)
      {
	// CspNodeのレートと同じTaskSetを見つける叙述関数
	struct TaskSetEqRate
	{
	  const CspNode* sts_node;

	  TaskSetEqRate (const CspNode* node) : sts_node (node) {}

	  bool operator () (const TaskSet* ts) const
	  {
	    return (sts_node->coretype () == ts->coretype ()
		    && sts_node->core () == ts->core ()
		    && sts_node->rate () == ts->rate ());
	  }
	};

	// TaskSet をレートで昇順に並べる叙述関数
	struct TaskSetRateLess
	{
	  bool operator () (const TaskSet* a, const TaskSet* b) const
	  {
	    if (a->coretype () != b->coretype ())
	      return a->coretype () == SOFTWARE_CORE;
	    if (a->rate ().rate () < b->rate ().rate ())
	      return true;
	    else if (a->rate ().rate () == b->rate ().rate ())
	      return a->core () < b->core ();
	    return false;
	  }
	};

	struct CoreNum
	{
	  CoreType coretype;
	  int core;

	  CoreNum (CoreType t, int c) : coretype (t), core (c) {}

	  bool operator< (const CoreNum x) const
	  {
	    if (coretype != x.coretype)
	      return coretype == SOFTWARE_CORE;
	    return core < x.core;
	  }

	  bool operator== (const CoreNum x) const
	  {
	    return coretype == x.coretype && core == x.core;
	  }
	};

	struct CoreNumLess
	{
	  bool operator () (const CoreNum& a, const CoreNum& b) const
	  {
	    return a < b;
	  }
	};

	std::vector < CoreNum > logical_core_list;

	TaskSetList& tslist = c_codewriter.core_assigned_taskset_list ();

	// クリア
	tslist.set_size (0);

	// 最大/最小のコア番号を取得
	int min_coreid = -1;
	int max_coreid = -1;
	for (CspNodeList::const_iterator
	       cni (CspNode::csp_nodelist ().begin ());
	     cni != CspNode::csp_nodelist ().end (); cni++)
	  {
	    CspNode* cn = *cni;
	    int coreid = cn->core ();

	    if (cn->coretype () == SOFTWARE_CORE)
	      {
		if (coreid >= 0)
		  {
		    if (min_coreid == -1 || coreid < min_coreid)
		      min_coreid = coreid;
		    if (max_coreid == -1 || coreid > max_coreid)
		      max_coreid = coreid;
		  }
	      }
	  }

	// コア未割り当ての場合は、最小のコア番号を0とする
	if (min_coreid == -1)
	  min_coreid = 0;
	if (max_coreid == -1)
	  max_coreid = 0;

	for (CspNodeList::const_iterator
	       cni (CspNode::csp_nodelist ().begin ());
	     cni != CspNode::csp_nodelist ().end (); cni++)
	  {
	    CspNode* cn = *cni;

	    // コア割り当てされていない場合、最小のコア番号とする
	    if (cn->core () == -1)
	      {
		std::string tmpstr;
		std::cout << "Warning: task" << cn->id ()
			  << ": " << cn->get_block_name (tmpstr)
			  << ": not assigned core" << std::endl;
		cn->core () = min_coreid;
	      }

	    // 指定コア番号を論理コア番号に登録
	    CoreNum corenum (cn->coretype (), cn->core ());
	    if (std::find (logical_core_list.begin (),
			   logical_core_list.end (),
			   corenum) == logical_core_list.end ())
	      logical_core_list.push_back (corenum);

	    // RateTransitionの場合、Updateがあり、後段のレートが高レー
	    // トであれば、単位遅延のため、エッジにダブルバッファリング
	    // を設定する
	    enum {
	      rt_undef, rt_highlow, rt_lowhigh, rt_error
	    } rt = rt_undef;
	    if (cn->info () & CSP_NODE_INFO_DLY_RATETRANSITION)
	      {
		csp_block_list_T rtblocks;
		for (CspEdgeList::iterator cei (cn->forw_edges ().begin ());
		     cei != cn->forw_edges ().end (); cei++)
		  {
		    CspEdge& ce = *cei;
		    CspNode* cn_head = ce.node ();
		    if (cn->rate ().rate () > cn_head->rate ().rate ())
		      {
			if (rt == rt_highlow)
			  rt = rt_error; // invalid rate conversions
			else if (rt == rt_undef)
			  rt = rt_lowhigh; // low -> high
			ce.info () |= CSP_EDGE_INFO_DOUBLE_BUFFER;
			assert (ce.pair ());
			ce.pair ()->info () |= CSP_EDGE_INFO_DOUBLE_BUFFER;
			for (csp_const_block_connect_list_T::const_iterator
			       cbci (ce.block_connect_list ().begin ());
			     cbci != ce.block_connect_list ().end (); cbci++)
			  {
			    const CspBlockConnect* cbc = *cbci;
			    CspBlockInfo* cbinfo = cbc->tail ();
			    if (cbinfo->task_info ()
				& TASK_INFO_DLY_RATETRANSITION)
			      {
				if (std::find (rtblocks.begin (),
					       rtblocks.end (),
					       cbinfo) == rtblocks.end ())
				  rtblocks.push_back (cbinfo);
			      }
			  }
		      }
		    else
		      {
			if (rt == rt_lowhigh)
			  rt = rt_error; // invalid rate conversions
			else if (rt == rt_undef)
			  rt = rt_highlow; // high -> low
		      }

		    if (rt == rt_error)
		      {
			std::string tmpstr;
			std::cerr << "Error: task_" << cn->id ()
				  << ": " << cn->get_block_name (tmpstr)
				  << ": Different Rate Conversions"
				  << std::endl;
			return -1;
		      }
		  }

		if (rt == rt_lowhigh)
		  {
		    // 単位遅延のブロックへのエッジがupdateになっている
		    // 場合、それを解除する
		    for (CspEdgeList::iterator cei (cn->back_edges ().begin ());
			 cei != cn->back_edges ().end (); cei++)
		      {
			CspEdge& ce = *cei;
			if ((ce.info () & CSP_EDGE_INFO_HAS_UPDATE) == 0)
			  continue;

			for (csp_const_block_connect_list_T::const_iterator
			       cbci (ce.block_connect_list ().begin ());
			     cbci != ce.block_connect_list ().end ();
			     cbci++)
			  {
			    const CspBlockConnect* cbc = *cbci;
			    CspBlockInfo* cbinfo = cbc->head ();

			    if (std::find (rtblocks.begin (), rtblocks.end (),
					   cbinfo) != rtblocks.end ())
			      {
				ce.info () &= ~CSP_EDGE_INFO_HAS_UPDATE;
				assert (ce.pair ());
				ce.pair ()->info ()
				  &= ~CSP_EDGE_INFO_HAS_UPDATE;
				break;
			      }
			  }
		      }
		  }
	      }

	    // 同じコア、同じレートの TaskSet を探す
	    TaskSetList::iterator tsi = std::find_if (tslist.begin (),
						      tslist.end (),
						      TaskSetEqRate (cn));
	    TaskSet* ts;
	    if (tsi == tslist.end ())
	      {
		// 見つからなかったら TaskSetList へ追加し、
		tslist.add_node (cn);
		ts = cn->taskset ();
		// TaskSet のレートを設定
		ts->add_rate (cn->rate ()); // TODO: 不要
		ts->ratebyid ().push_back (cn->rate ()); // TODO: 不要
		ts->rate () = cn->rate ();

		// 最も早く起床するタスクコードにてアラームを相対的に生
		// 成するので、その TaskSet を保存
		// - オフセットが小さい
		// - オフセットが同じでレートが早い
		// - オフセット・レートが同じでコア番号が小さい
		if ((!smbp_mode || cn->coretype () == SOFTWARE_CORE)
		    && (alarm_taskset == NULL
			|| (ts->rate ().offset ()
			    < alarm_taskset->rate ().offset ())
			|| ((ts->rate ().offset ()
			     == alarm_taskset->rate ().offset ())
			    && (ts->rate ().rate ()
				< alarm_taskset->rate ().rate ()))
			|| (ts->rate () == alarm_taskset->rate ()
			    && cn->core () < alarm_taskset->core ())))
		  alarm_taskset = ts;
	      }
	    else
	      {
		ts = *tsi;
		ts->add_node (cn);	// 見つかった TaskSet へ追加
	      }
	  }

	// 論理コアを昇順にソートし、コア番号の論理物理マップを作成
	std::sort (logical_core_list.begin (),
		   logical_core_list.end (), CoreNumLess());
	std::map < CoreNum, int > core_map;
	for (size_t i = 0; i < logical_core_list.size (); i++)
	  core_map.insert (std::make_pair (logical_core_list[i], i));

	// TaskSet の論理コアを物理コアに割り当て、TaskSet内のCspNodeを
	// コードの出現順に並び替え、その順にタスクの情報をTaskSetに反
	// 映する
	pcore_tslist.resize (logical_core_list.size ());
	for (TaskSetList::const_iterator tsi (tslist.begin ());
	     tsi != tslist.end (); tsi++)
	  {
	    TaskSet *ts = *tsi;

	    std::map < CoreNum, int >::const_iterator
	      m = core_map.find (CoreNum (ts->coretype (), ts->core ()));
	    assert (m != core_map.end ());

	    pcore_tslist[m->second].push_back (ts);

	    if (ts->cspnodelist ().size () > 1)
	      std::sort (ts->cspnodelist ().begin (),
			 ts->cspnodelist ().end (), CspNodePosLess());
	    ts->setup_taskset ();
	  }

	// 論理コア毎にレート毎に昇順に並べる
	number_of_cores = 0;
	for (size_t i = 0; i < pcore_tslist.size (); i++)
	  {
	    baseTaskSetList& btsl = pcore_tslist[i];

	    if (!smbp_mode
		|| btsl[0]->coretype () == SOFTWARE_CORE)
	      number_of_cores++;

	    std::sort (btsl.begin (), btsl.end (), TaskSetRateLess ());
	  }

	// 全体をレート毎に昇順に並べる(タスクの優先度を与えるため)
	baseTaskSetList& basetslist = tslist;
	std::sort (basetslist.begin (), basetslist.end (), TaskSetRateLess ());

	// ダブルバッファとオフセットによる初期値送信が必要かどうかを判定
	for (TaskSetList::const_iterator tsi (tslist.begin ());
	     tsi != tslist.end (); tsi++)
	  {
	    TaskSet *ts = *tsi;

	    bool need_db = false;
	    bool need_send_init = false;

	    for (CspNodeList::const_iterator
		   cni (ts->cspnodelist ().begin ());
		 cni != ts->cspnodelist ().end (); cni++)
	      {
		CspNode* cn = *cni;

		const Rate_rate_T rate_x = cn->rate ().rate ();
		const Rate_offset_T offset_x = cn->rate ().offset ();

		for (CspEdgeList::iterator i (cn->forw_edges ().begin ());
		     i != cn->forw_edges ().end (); i++)
		  {
		    CspEdge& ce = *i;
		    CspNode* cn_head = ce.node ();

		    bool db = (ce.info () & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0;
		    const Rate_offset_T offset_y = cn_head->rate ().offset ();

		    // 同じ TaskSet なら不要
		    if (cn_head->taskset () == ts)
		      {
			if (db)
			  ce.info () &= ~CSP_EDGE_INFO_DOUBLE_BUFFER;
			continue;
		      }

		    // ダブルバッファが必要か
		    if (db && !need_db)
		      need_db = true;

		    // オフセットによる初期値送信が必要か
		    bool send_init = (db
				      ? (offset_y < offset_x + rate_x)
				      : (offset_y < offset_x));

		    if (send_init && !need_send_init)
		      need_send_init = true;
		  }
	      }

	    if (need_db)
	      ts->double_buffer (true);
	    if (need_send_init)
	      ts->send_init (true);
	  }

	return 0;
      }


      /// \brief 共通関数の出力処理
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// 共通の関数をファイルに出力し、出力した情報を
      /// output_common_function_mark に設定する。
      ///
      /// 共通の関数は、ブロックが所有しない関数である。
      ///
      void
      generate_common_functions (std::ostream& os)
      {
	const function_map_T& fmap = c_codewriter.functions ();

	// ブロックの関数をマーク
	reset_mark (output_common_function_mark, fmap.size ());
	set_mark_shared_block_function (output_common_function_mark, true);

	bool comout = true;

	// 先に関数定義を出力
	output_function_sub (os, fmap.defn_list (),
			     output_common_function_mark, true, 0, comout,
			     "", "", true, NULL);

	// 次に定義されない関数宣言を出力
	output_function_sub (os, fmap.decl_list (),
			     output_common_function_mark, false, 0, comout,
			     "", "", true, NULL);

	// ブロックの関数のマークをクリア
	set_mark_shared_block_function (output_common_function_mark, false);
      }


      /// \brief TOPPERS/ATK2用コードの先頭にタスクのコード以外を出力する
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      ///
      /// この処理で出力するのは以下の通り
      ///
      /// -# BLXMLにあるヘッダファイルに対するinclude文
      /// -# タスクのリソース用マクロ
      /// -# 共通のマクロ
      /// -# Real-Time Model Objectのマクロ
      /// -# Real-Time Model Objectの型
      /// -# 定義されている型
      /// -# 利用者が指定したヘッダファイルに対するinclude文
      /// -# コンパイラ向け依存マクロ
      /// -# チャネル操作用マクロおよび関数
      /// -# ATK2向けマクロ
      /// -# イベントが必要なSubSystemが送信するイベントの型とコピー用マクロ
      /// -# CspNode 間の通信チャネルの型
      /// -# 共有するDataStore
      int
      generate_preamble (std::ostream& os)
      {
	static const char* os_includes[] = {
#if 1
	  "aos.h",
	  "counter_handler.h"
#else
	  "Os.h",
	  "t_syslog.h",
	  "t_stdlib.h",
	  "sysmod/serial.h",
	  "sysmod/syslog.h",
	  "sysmod/banner.h",
	  "target_sysmod.h",
	  "target_serial.h",
	  "target_hw_counter.h",
#endif
	};

	int mark_index = -1;

	os << "/*\n"
	   << " * generate \""
	   << model_name
	   << "\" code for TOPPERS/ATK2";
	if (smbp_mode)
	  os << "(S-MBP)";
	os << "\n */\n\n";
	if (smbp_mode)
	  os << "#include \"systembuilder.h\"\n";

	// memcpy()/memset()
	os << "#include <string.h>\n\n";

	// TOPPERS/ATK2用ヘッダファイル出力
	for (size_t i = 0;
	     i < sizeof os_includes / sizeof os_includes[0]; i++)
	  os << "#include \"" << os_includes[i] << "\"\n";

	// Simulinkモデル用のヘッダファイルの前までinclude文を出力。モ
	// デル用ヘッダファイルはBLXMLに取り込まれているので、それは出
	// 力しない
	generate_includes (os, c_codewriter.include_file (), mark_index);

	// TODO: タスクのリソースマクロ

	// ヘッダファイルの二重インクルード抑止の記述があればそれを出力
	if (c_codewriter.header_preamble ().size ())
	  os << '\n' << c_codewriter.header_preamble ();

	// 共通のマクロ, Real-Time Model Objectのマクロ, Real-Time Model
	// Objectの型を出力
	if (c_codewriter.c_macro ().size ())
	  os << '\n' << c_codewriter.c_macro ();
	if (c_codewriter.rt_macro ().size ())
	  os << '\n' << c_codewriter.rt_macro ();
	if (c_codewriter.rt_type ().size ())
	  os << '\n' << c_codewriter.rt_type ();

	// 定義されている型の出力
	generate_deftype (os);

	// ヘッダファイルの二重インクルード抑止の記述があれば、endifを
	// 出力
	if (c_codewriter.header_preamble ().size ())
	  os << "\n#endif\n";

	// モデル用ヘッダファイルより後のヘッダファイルのinclude文を出力
	if (mark_index != -1)
	  {
	    mark_index++;
	    generate_includes (os, c_codewriter.include_file (), mark_index);
	  }

	// 利用者指定のinclude文
	generate_user_includes (os);

	// コンパイラ向け依存マクロ

	// gcc の inline
	os << '\n'
	   << "#ifndef __GNUC__\n"
	   << "#define __inline__\n"
	   << "#endif /* __GNUC__ */\n";

	// コンパイラ固有の pagma や attribute を設定可能にするためのマクロ
	os << '\n'
	   << "#ifndef CORE_TASK_DATA_PREPARE\n"
	   << "#define CORE_TASK_DATA_PREPARE(N)\t/* none */\n"
	   << "#endif /* CORE_TASK_DATA_PREPARE */\n"
	   << '\n'
	   << "#ifndef CORE_TASK_DATA_ATTRIBUTE\n"
	   << "#define CORE_TASK_DATA_ATTRIBUTE(N)\t/* none */\n"
	   << "#endif /* CORE_TASK_DATA_ATTRIBUTE */\n"
	   << '\n'
	   << "#ifndef CORE_TASK_RODATA_PREPARE\n"
	   << "#define CORE_TASK_RODATA_PREPARE(N)\t/* none */\n"
	   << "#endif /* CORE_TASK_DATA_PREPARE */\n"
	   << '\n'
	   << "#ifndef CORE_TASK_RODATA_ATTRIBUTE\n"
	   << "#define CORE_TASK_RODATA_ATTRIBUTE(N)\t/* none */\n"
	   << "#endif /* CORE_TASK_RODATA_ATTRIBUTE */\n"
	   << '\n'
	   << "#ifndef CORE_TASK_CODE_PREPARE\n"
	   << "#define CORE_TASK_CODE_PREPARE(N)\t/* none */\n"
	   << "#endif /* CORE_TASK_CODE_PREPARE */\n"
	   << '\n'
	   << "#ifndef CORE_TASK_CODE_ATTRIBUTE\n"
	   << "#define CORE_TASK_CODE_ATTRIBUTE(N)\t/* none */\n"
	   << "#endif /* CORE_TASK_CODE_ATTRIBUTE */\n";

#if 0
	// 今は使っていない
	os << '\n'
	   << "#ifndef TASK_DATA_PREPARE\n"
	   << "#define TASK_DATA_PREPARE(N)\t/* none */\n"
	   << "#endif /* TASK_DATA_PREPARE */\n"
	   << '\n'
	   << "#ifndef TASK_DATA_ATTRIBUTE\n"
	   << "#define TASK_DATA_ATTRIBUTE(N)\t/* none */\n"
	   << "#endif /* TASK_DATA_ATTRIBUTE */\n"
	   << '\n'
	   << "#ifndef TASK_RODATA_PREPARE\n"
	   << "#define TASK_RODATA_PREPARE(N)\t/* none */\n"
	   << "#endif /* TASK_RODATA_PREPARE */\n"
	   << '\n'
	   << "#ifndef TASK_RODATA_ATTRIBUTE\n"
	   << "#define TASK_RODATA_ATTRIBUTE(N)\t/* none */\n"
	   << "#endif /* TASK_RODATA_ATTRIBUTE */\n"
	   << '\n'
	   << "#ifndef TASK_CODE_PREPARE\n"
	   << "#define TASK_CODE_PREPARE(N)\t/* none */\n"
	   << "#endif /* TASK_CODE_PREPARE */\n"
	   << '\n'
	   << "#ifndef TASK_CODE_ATTRIBUTE\n"
	   << "#define TASK_CODE_ATTRIBUTE(N)\t/* none */\n"
	   << "#endif /* TASK_CODE_ATTRIBUTE */\n";
#endif

	// チャネル操作用マクロ生成
	generate_channel_macros (os, ATK2_VAR_PREFIX);

	//
	// 機種依存マクロ
	//

	// ATK2向けタスク用マクロ
	os << '\n'
	   << "/*\n"
	   << " * Task Macros for TOPPERS/ATK2\n"
	   << " */\n";

	os << '\n'
	   << "#ifndef " << atk2_timer_clock_hz << '\n'
	   << "#define " << atk2_timer_clock_hz << '\t'
	   << atk2_timer_hz << '\n'
	   << "#endif /* " << atk2_timer_clock_hz << " */\n";

	if (alarm_taskset)
	  {
	    const std::string tname (mk_task_name (alarm_taskset));

	    os << '\n'
	       << "#define ATK2_TASK_WAIT_TASKS\t" << tname << '\n'
	       << "#define ATK2_WE_WAIT_TASKS\t"
	       << block_name_prefix << "CompleteEvent" << '\n'
	       << "#define ATK2_SE_WAIT_TASKS\t"
	       << block_name_prefix << "CompleteEvent" << '\n'
	       << "#define ATK2_TNAME_WAIT_TASKS\t"
	       << "\"" << tname << "\"\n"
	       << "#define ATK2_TNAME_TASK_WAIT_TASKS\t"
	       << "\"" << tname << "\"\n";
	  }

	TaskSetList& tslist = c_codewriter.core_assigned_taskset_list ();
	for (TaskSetList::const_iterator
	       tsi (tslist.begin ()); tsi != tslist.end (); tsi++)
	  {
	    const TaskSet* ts = *tsi;

	    if (ts->coretype () == HARDWARE_CORE)
	      continue;

	    const std::string talarm (mk_alarm_name (ts));
	    const std::string tname (mk_task_name (ts));
	    std::string tmp;

	    os << "\n/* Task: " << tname << " */\n"
	       << "#define ATK2_TASK_" << tname << '\t' << tname << '\n'
	       << "#define ATK2_WE_" << tname << '\t'
	       << block_name_prefix << "WakeupEvent\n"
	       << "#define ATK2_SE_" << tname << '\t'
	       << block_name_prefix << "WakeupEvent\n"
	       << "#define ATK2_TNAME_TASK_" << tname << '\t'
	       << "\"" << tname << "\"" << '\n'
	       << "#define ATK2_ALARM_" << tname << '\t'
	       << talarm << '\n'
	       << "#define ATK2_TNAME_ALARM_" << tname << '\t'
	       << "\"" << talarm << "\"\n";

	    for (CspNodeList::const_iterator cni (ts->cspnodelist ().begin ());
		 cni != ts->cspnodelist ().end (); cni++)
	      {
		CspNode* cn = *cni;
		const std::string tid (mk_task_id (cn->id ()));

		os << "\n/* Task_" << tid << ": " << cn->get_block_name (tmp)
		   << " */\n"
		   << "#define ATK2_TASK_" << tid << "\t\t" << tname << '\n'
		   << "#define ATK2_TASK_IN_" << tid << "\t"
		   << "ATK2_TASK_" << tid << '\n'
		   << "#define ATK2_TASK_OUT_" << tid << "\t"
		   << "ATK2_TASK_" << tid << '\n'
		   << "#define ATK2_SE_IN_" << tid << "\t\t"
		   << "ATK2_TASK_" << tid << ", "
		   << block_name_prefix << "InputEvent"<< '\n'
		   << "#define ATK2_SE_OUT_" << tid << '\t'
		   << "ATK2_TASK_" << tid << ", "
		   << block_name_prefix << "OutputEvent"<< '\n'
		   << "#define ATK2_WE_IN_" << tid << "\t\t"
		   << block_name_prefix << "InputEvent"<< '\n'
		   << "#define ATK2_WE_OUT_" << tid << '\t'
		   << block_name_prefix << "OutputEvent"<< '\n'
		   << "#define ATK2_TNAME_TASK_" << tid
		   << "\t\"" << tname << "\"\n"
		   << "#define ATK2_TNAME_IN_" << tid << '\t'
		   << "ATK2_TNAME_TASK_" << tid << '\n'
		   << "#define ATK2_TNAME_OUT_" << tid << '\t'
		   << "ATK2_TNAME_TASK_" << tid << '\n'
		   << "#define ATK2_TID_TASK_" << tid
		   << "\t\"" << tid << "\"\n"
		   << "#define ATK2_TID_IN_" << tid << '\t'
		   << "ATK2_TID_TASK_" << tid << '\n'
		   << "#define ATK2_TID_OUT_" << tid << '\t'
		   << "ATK2_TID_TASK_" << tid << '\n'
		   << "#define ATK2_WE_TASK_" << tid << '\t'
		   << block_name_prefix << "WakeupEvent" << '\n'
		   << "#define ATK2_SE_TASK_" << tid << '\t'
		   << block_name_prefix << "WakeupEvent" << '\n';
	      }
	  }

	// メモリ同期
	os << "\n/* Synchronus Memory */\n"
	   << "#ifndef CH__SYNCM\n"
	   << "#if defined(__v850__) && defined(__GNUC__)\n"
	   << "#define CH__SYNCM()\t"
	   << "__asm__ __volatile__ (\"syncm\" ::: \"memory\")\n"
	   << "#elif defined(__nios2__) && defined(__GNUC__)\n"
	   << "#define CH__SYNCM()\t"
	   << "__asm__ __volatile__ (\"sync\" ::: \"memory\")\n"
	   << "#else\n"
	   << "#define CH__SYNCM()\tdo { ; } while (0)\n"
	   << "#endif /* !__v850__ || !__nios2__ || !__GNUC__ */\n"
	   << "#endif /* !CH__SYNCM */\n";

	// メモリバリア
	os << "\n/* Memory Barrier for Compiler */\n"
	   << "#ifndef CH__MEM_BARRIER\n"
	   << "#ifdef __GNUC__\n"
	   << "#define CH__MEM_BARRIER()\t"
	   << "__asm__ __volatile__ (\"\" ::: \"memory\")\n"
	   << "#else /* !__GNUC__ */\n"
	   << "#define CH__MEM_BARRIER()\tdo { ; } while (0)\n"
	   << "#endif /* !__GNUC__ */\n"
	   << "#endif /* !CH__MEM_BARRIER */\n";

	os << "\n/* TOPPERS/ATK2 OS API Wrapper Macros */\n"
	   << "#ifndef OS__SENDEVENT\n"
	   << "#define OS__SENDEVENT(T,R,E)\t\\\n"
	   << "  SetEvent(CH__VEC_CONCAT(ATK2_,R),"
	   << "CH__VEC_CONCAT(ATK2_,E))\n"
	   << "#endif\n"
	   << "#ifndef OS__WAITEVENT\n"
	   << "#define OS__WAITEVENT(T,E)\t\\\n"
	   << "  do { WaitEvent(CH__VEC_CONCAT(ATK2_,E));\t\\\n"
	   << "       ClearEvent(CH__VEC_CONCAT(ATK2_,E)); } while (0)\n"
	   << "#endif\n"
	   << "#ifndef OS__SETALARM\n"
	   << "#define OS__SETALARM(A,O,R)\t\\\n"
	   << "  SetRelAlarm(CH__VEC_CONCAT(ATK2_,A),O,R)\n"
	   << "#endif\n";

	// スケジューラ、イベント関連
	os << "\n/* TOPPERS/ATK2 Channel Macros */\n"
	   << "#ifndef CH__TASK_SCHED\n"
	   << "#define CH__TASK_SCHED(C,V,S)\t\\\n"
	   << "  do { WaitEvent(CH__VEC_CONCAT(ATK2_WE_,C));\t\\\n"
	   << "       ClearEvent(CH__VEC_CONCAT(ATK2_WE_,C)); } while (0)\n"
	   << "#endif\n"
	   << "#ifndef CH__CORE_THREAD_SCHED\n"
	   << "#define CH__CORE_THREAD_SCHED(C,V,S)\tCH__TASK_SCHED(C,V,S)\n"
	   << "#endif\n"
	   << "/* one argument wrapper macro */\n"
	   << "#ifndef CH__SetEvent\n"
	   << "#define CH__SetEvent(C)\tSetEvent(C)\n"
	   << "#endif\n"
	   << "#ifndef CH__EVENT\n"
	   << "#define CH__EVENT(T,C)\t\\\n"
	   << "  do { CH__SYNCM(); \\\n"
	   << "       CH__SetEvent(CH__VEC_CONCAT(ATK2_SE_,C)); } while (0)\n"
	   << "#endif\n"
	   << "#ifndef CH__END_LOG\n"
	   << "#define CH__END_LOG(C)\tdo { ; } while (0)\n"
	   << "#endif\n";

	// チャネル操作用inline関数生成
	generate_channel_functions (os);

	// イベントが必要なSubSystemが送信するイベントの型とコピー用マクロ
	generate_sybsystem_state_and_macros (os, unknown_state);

	// CspNode 間の通信チャネルの型
	generate_channel_types (os, unknown_state);

#if 0
	// S-MBPのMemoryChannelの型
	if (smbp_mode)
	  SMBP::generate_memchan_types (os, true);
#endif

	// 共有するDataStore。状態変数とは同居できないので独立に構造体
	// 名を変えて出力する
	generate_datastore (os, dsm_prefix);

	// 共通の関数を出力する
	generate_common_functions (os);

	return 0;
      }

      /// \brief タスクの定義・宣言の出力
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] num コア番号
      /// \param[in] taskset コア割り当てされたタスクの TaskSet のポインタ
      /// \param[in] outer タスク関数外かどうかを示す真理値
      /// \param[in] postfix プリプロセッサマクロで定義するものにつけるpostfix
      /// \param[in,out] define_list 出力マクロ名を登録するリスト
      ///
      /// \retval 0 成功
      /// \retval -1 失敗
      int
      generate_definitions (std::ostream& os, int num,
			    const TaskSet* taskset,
			    bool outer,
			    const std::string& postfix,
			    def_list_T* define_list)
      {
	int ind = 1;
	std::string stdef;

	std::string code_attr;	// コードに付加するコンパイラ用属性
	std::string data_attr;	// データに付加するコンパイラ用属性
	std::string rodata_attr; // 読込専用データに付加するコンパイラ用属性

	// タスク関数外の場合は code_attr, data_attr を設定
	if (outer && !smbp_mode)
	  {
	    ind = 0;
	    os << "\nCORE_TASK_DATA_PREPARE(" << num << ")\n";

	    std::ostringstream oss_ca;
	    oss_ca << "CORE_TASK_CODE_ATTRIBUTE(" << num << ")";
	    code_attr = oss_ca.str ();

	    std::ostringstream oss_da;
	    oss_da << "CORE_TASK_DATA_ATTRIBUTE(" << num << ")";
	    data_attr = oss_da.str ();

	    std::ostringstream oss_ra;
	    oss_ra << "CORE_TASK_RODATA_ATTRIBUTE(" << num << ")";
	    rodata_attr = oss_ra.str ();
	  }

	const indent_writer iw0 (ind);

	// タスクの定義・宣言を出力
	generate_task_definitions (os, taskset, outer, postfix,
				   code_attr, data_attr, rodata_attr,
				   define_list);

	// タスク関数内部であれば、イベントが必要な SubSystem の実行状態
	// 用変数を出力する
	if (!outer)
	  {
	    CspNode_map_T cnmap;
	    for (CspNodeList::const_iterator
		 j (taskset->cspnodelist ().begin ());
	       j != taskset->cspnodelist ().end (); j++)
	      {
		CspNode* node = *j;
		generate_task_subsystem_variables (os, node, 1, &cnmap);
	      }
	  }

	if (outer && !smbp_mode)
	  os << "\nCORE_TASK_CODE_PREPARE(" << num << ")\n";

	// タスクの関数外かつgccでない場合、またはタスク関数内でgccの場合
	// で、ブロックの関数を持つ場合は、関数を出力する
	if (((outer && no_gcc) || (!outer && no_gcc == 0))
	    && taskset->has_block_functions ())
	  {
	    os << '\n'
	       << iw0 << "/* functions */\n";

	    output_function_mark = output_common_function_mark;
	    for (CspNodeList::const_iterator
		   j (taskset->cspnodelist ().begin ());
		 j != taskset->cspnodelist ().end (); j++)
	      {
		CspNode* node = *j;
		bool comout = false;
		const function_ptr_list_T& flist = node->functions ();
		if (flist.size ())
		  {
		    std::string tmp;
		    const std::string& n = node->get_block_name (tmp);
		    os << '\n'
		       << iw0<< "/* Block: " << n << " */\n";
		    output_function_sub (os, node->functions (),
					 output_function_mark, true,
					 ind, comout,
					 code_attr, postfix, outer,
					 define_list);
		  }
	      }
	  }

	return 0;
      }


      /// \brief タスクループ内のタスクのコード生成
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] taskset コア割り当てされたタスクの TaskSet のポインタ
      /// \param[in] gentype 出力するコード種別
      void
      generate_task_code_in_loop (std::ostream& os, const TaskSet* taskset,
				  GenCodeType gentype)
      {
	// CspNode 毎に、実行タイミングになっているかどうかを条件にもつ
	// if 文および、入力・実行・出力のコードを出力する
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;

	    // 単位遅延のRateTransitionの出力制御
	    if (gentype == GENCODE_RATETRANSITION)
	      {
		if ((node->info () & CSP_NODE_INFO_DLY_RATETRANSITION) == 0)
		  continue;
	      }
	    else
	      {
		if (node->info () & CSP_NODE_INFO_DLY_RATETRANSITION)
		  continue;

		// Updateで、コードがない場合は出力の必要がない
		if (gentype == GENCODE_UPDATE)
		  {
		    CodeList::const_iterator
		      ci = node->code ().find_type_code ("update");
		    if (ci == node->code ().end ())
		      continue;
		  }
	      }

	    std::string tmp;
	    const std::string& block = node->get_block_name (tmp);

	    std::string my_task_id (mk_task_id (node->id ()));

	    int ind2 = 2;

	    // CspNode の情報を出力

	    const indent_writer iwCode (ind2);

	    os << iwCode << "/*\n"
	       << iwCode << " * Block: " << block << '\n'
	       << iwCode;
	    if (gentype == GENCODE_UPDATE)
	      os << " * Update: ";
	    else
	      os << " * Task: ";
	    os << my_task_id << '\n';

	    if (enable_multirate)
	      {
		os << iwCode << " * rate: " << node->rate ();
		if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		  os << '*';
		os << '\n';
	      }
	    os << iwCode << " */\n";

	    // 外部入力が必要な場合はその旨コメント表示
	    // ただし、ここには外部入力すべきUpdateはないかもしれない
	    if (node->info () & CSP_NODE_INFO_EXTIN)
	      generate_extvar_user_codes (os, VOUT_EXTIN, ind2);

	    // CspNode のチャネル入力コード生成
	    enum channel_io_mode io_mode = ((gentype == GENCODE_UPDATE)
					    ? CH_IO_MODE_UPDATE
					    : CH_IO_MODE_TASK);

	    // S-MBP/A-MBPのチャネル入力コード生成
	    if (smbp_mode)
	      SMBP::generate_task_smbp_input_channel (os, node, ind2,
						      io_mode);
	    else
	      generate_task_wait_input_channel (os, node, true, ind2,
						io_mode, unknown_state);

	    // CspNode のタスクコード生成
	    if (gentype == GENCODE_TASK)
	      generate_task_exec_code (os, node, NULL, ind2, dsm_prefix);
	    else if (gentype == GENCODE_UPDATE)
	      generate_task_update_code (os, node, NULL, ind2, false);
	    else
	      {
		generate_task_update_code (os, node, NULL, ind2, false);
		generate_task_exec_code (os, node, NULL, ind2, dsm_prefix);
	      }

	    if (gentype != GENCODE_UPDATE)
	      {
		// 外部出力が必要な場合はその旨コメント表示
		if (node->info () & CSP_NODE_INFO_EXTOUT)
		  generate_extvar_user_codes (os, VOUT_EXTOUT, ind2);

		// S-MBP/A-MBPのチャネル出力コード生成
		if (smbp_mode)
		  SMBP::generate_task_smbp_output_channel (os, node, ind2);
		else
		  generate_task_wait_output_channel (os, node, true, ind2,
						     unknown_state);
	      }
	  }
      }

      /// \brief 同一周期・オフセットを持つブロックのタスクを生成する
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] taskset
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      ///
      int
      generate_task (std::ostream& os, const TaskSet* taskset)
      {
	// 可変レートは扱えない
	assert (!taskset->has_variable_rate ());

	bool first = true;

	def_list_T define_list;

	const rate_T& task_rate = taskset->rate ();

	CoreType type = taskset->coretype ();
	int num = taskset->core ();

	const std::string task_name (mk_task_name (type, num, task_rate));

	// gcc用のコード生成ではない場合等、タスク関数外に定義が必要な関
	// 数や変数を出力する
	if (no_gcc || taskset->need_out_definitions ())
	  {
	    const char* postfix = task_name.c_str ();
	    if (generate_definitions (os, num, taskset, true, postfix,
				      &define_list))
	      return -1;
	  }
	else
	  os << "\nCORE_TASK_CODE_PREPARE(" << num << ")\n";

	// タスク関数の出力

	if (enable_multirate)
	  os << "/* rate: " << task_rate << " */\n"; // レートの表示

	os << "/* Core#" << num << ": Rate: " << task_rate << " */\n"
	   << "CORE_TASK_CODE_ATTRIBUTE(" << num << ")\n"
	   << "TASK(" << task_name << ")\n"
	   << "{";

	// マルチレートの場合は、自分の実行時刻、相手への送信タイミング
	// を知る必要があるのでその対応を行う
	if (enable_multirate)
	  os << '\n'		// 自分の時刻の変数出力
	     << indent_writer::iw1 << "/* task time */\n"
	     << indent_writer::iw1 << "unsigned long task__time = "
	     << task_rate.offset ()
	     << ";\n";

	// S-MBP用のタスクの定義を出力
	if (smbp_mode)
	  SMBP::generate_task_definitions (os, taskset);

	// タスク関数に、元コード(step関数)の先頭部分を出力
	if (c_codewriter.preamble ().size ())
	  os << '\n'
	     << indent_writer::iw1 << "/* definitions of step function */\n"
	     << c_codewriter.preamble ()
	     << indent_writer::iw1 << "/* end of definitions */\n";

	// CspNode 毎にチャネル用待ち合わせ用のベクタを出力
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;
	    std::string my_task_id (mk_task_id (node->id ()));
	    generate_task_channel_vector (os, node, my_task_id, first);
	  }

	// タスク関数内に変数や関数の定義を行う
	if (generate_definitions (os, num, taskset, false, ""))
	  return -1;

	// ユーザーが、コア割り当てされたタスク単位の変数等の定義を追加
	// できるポイントを出力
	os << '\n'
	   << "#ifdef TASK_" << task_name << "_DEFINITIONS\n"
	   << indent_writer::iw1
	   << "TASK_" << task_name << "_DEFINITIONS\n"
	   << "#endif\n";

	// ユーザーが変数等の定義を追加できるポイントを CspNode 単位に出
	// 力
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;
	    std::string my_task_id (mk_task_id (node->id ()));
	    os << '\n'
	       << "#ifdef TASK_" << my_task_id << "_DEFINITIONS\n"
	       << indent_writer::iw1
	       << "TASK_" << my_task_id << "_DEFINITIONS\n"
	       << "#endif\n";
	  }

	// 共通の初期化コードを出力
	if (c_codewriter.common_init ().size ())
	  {
	    os << '\n'
	       << indent_writer::iw1 << "/* Common Initialization */\n"
	       << c_codewriter.common_init ();
	  }

	// Real-Time Model Objectが必要な場合は、初期化コードを出力
	if (taskset->need_rtm ())
	  generate_task_rtw_initializer (os, 1);

	// 状態変数を持つ場合は、状態変数をゼロクリア
	if (taskset->has_state ())
	  {
	    const optional_refstring&
	      state_storage = c_codewriter.state_storage ();
	    os << '\n'
	       << indent_writer::iw1 << "/* initialize state structure */\n"
	       << indent_writer::iw1 << "memset(&" << state_storage
	       << ", 0, sizeof " << state_storage << ");\n";
	  }

	// CspNode 固有の初期化コード
	CspNode_map_T cnmap;
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;
	    std::string tmp;
	    const std::string& n = node->get_block_name (tmp);

	    os << '\n'
	       << indent_writer::iw1 << "/* Block: " << n << " */\n";

	    generate_task_context_initializer (os, node, 1, &cnmap);
	  }

	// 初期値の受信処理
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;

	    generate_task_wait_input_channel (os, node, true, 1,
					      CH_IO_MODE_INIT, unknown_state);
	  }

	// 初期化完了のポインタを出力
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;
	    std::string my_task_id (mk_task_id (node->id ()));
	    os << '\n'
	       << "#ifdef TASK_INITIALIZE_" << my_task_id << "_COMPLETED\n"
	       << indent_writer::iw1
	       << "TASK_INITIALIZE_" << my_task_id << "_COMPLETED();\n"
	       << "#endif\n";
	  }

	// 出力チャネルのオフセット設定
	if (enable_multirate)
	  {
	    bool first = true;
	    for (CspNodeList::const_iterator
		   j (taskset->cspnodelist ().begin ());
		 j != taskset->cspnodelist ().end (); j++)
	      {
		CspNode* node = *j;

		generate_task_set_offset (os, node, 1, first);
	      }

	    // 初期値送信を行ったので、アラームを設定するタスクに完了を
	    // 通知
	    if (taskset->send_init ())
	      os << '\n'
		 << indent_writer::iw1
		 << "/* send Completed-Preload Event to the fastest task */\n"
		 << indent_writer::iw1
		 << "OS__SENDEVENT(TASK_" << task_name << ",\n"
		 << indent_writer (8)
		 << "TASK_WAIT_TASKS, SE_WAIT_TASKS);\n";
	  }

	// アラームを設定するタスク以外はここで起床待ちにする
	if (taskset != alarm_taskset)
	  {
	    os << '\n'
	       << indent_writer::iw1
	       << "/* wait start event */\n"
	       << indent_writer::iw1
	       << "OS__WAITEVENT(TASK_" << task_name << ",\n"
	       << indent_writer (8)
	       << "WE_" << task_name << ");\n";
	  }

	// タスクのアラーム設定および起床
	if (taskset == alarm_taskset)
	  {
	    unsigned long start_offset = alarm_taskset->rate ().offset ();
	    const TaskSetList&
	      tslist = c_codewriter.core_assigned_taskset_list ();

	    size_t wait_tasks = 0;

	    // 初期値送信を行うタスクをカウント
	    for (TaskSetList::const_iterator tsi (tslist.begin ());
		 tsi != tslist.end (); tsi++)
	      {
		const TaskSet* ts = *tsi;

		if (smbp_mode && ts->coretype () == HARDWARE_CORE)
		  continue;

		if (ts->send_init ())
		  wait_tasks++;
	      }

	    // 初期値送信を行うタスクからの完了通知を待つ
	    if (wait_tasks)
	      {
		const Rate_rate_T rate_x = taskset->rate ().rate ();
		const Rate_offset_T offset_x = taskset->rate ().offset ();
		os << indent_writer::iw1 << '\n'
		   << indent_writer::iw1 << "/* wait Preload Events */\n"
		   << indent_writer::iw1 << "{\n";
		if (wait_tasks > 1)
		  os << indent_writer::iw2 << "CH__VEC(WAIT_TASKS,"
		     << wait_tasks << ");\n"
		     << indent_writer::iw2 << "CH__VEC_INIT(WAIT_TASKS);\n";
		os << indent_writer::iw2 << "while (1) {\n";

		size_t n = 0;
		for (TaskSetList::const_iterator tsi (tslist.begin ());
		     tsi != tslist.end (); tsi++)
		  {
		    const TaskSet* ts = *tsi;

		    if (smbp_mode && ts->coretype () == HARDWARE_CORE)
		      continue;

		    if (ts == alarm_taskset || !ts->send_init ())
		      continue;

		    const std::string tsname (mk_task_name (ts));

		    for (CspNodeList::const_iterator
			   cni (ts->cspnodelist ().begin ());
			 cni != ts->cspnodelist ().end (); cni++)
		      {
			CspNode* cn = *cni;

			const std::string my_task_id (mk_task_id (cn->id ()));

			for (CspEdgeList::iterator i
			       (cn->forw_edges ().begin ());
			     i != cn->forw_edges ().end (); i++)
			  {
			    CspEdge& ce = *i;
			    CspNode* cn_head = ce.node ();

			    const Rate_offset_T
			      offset_y = cn_head->rate ().offset ();

			    bool db = (ce.info ()
				       & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0;
			    bool send_init = (db
				      ? (offset_y < offset_x + rate_x)
				      : (offset_y < offset_x));

			    if (!send_init)
			      continue;

			    const std::string
			      xx_task_id (mk_task_id (cn_head->id ()));
			    const std::string
			      chan (my_task_id + "_" + xx_task_id);
			    const std::string
			      ch_prefix ("CH_" + block_name_prefix + chan);

			    if (n)
			      os << '\n';
			    os << indent_writer::iw3
			       << "if (";
			    if (wait_tasks > 1)
			      os << "CH__BITCHECK(WAIT_TASKS,"
				 << n << ") == 0\n"
				 << indent_writer (5)
				 << "&& ";
			    os  << ch_prefix << ".flag__" << chan;
			    if (db)
			      os << "[0]";
			    if (wait_tasks == 1)
			      os << ") {\n"
				 << indent_writer::iw4
				 << "CH__END_LOG(WAIT_TASKS);\n"
				 << indent_writer::iw4
				 << "break;\n"
				 << indent_writer::iw3
				 << "}\n";
			    else
			      {
				os << ")\n"
				   << indent_writer::iw4
				   << "CH__BITSET(WAIT_TASKS," << n << ");\n";
				n++;
			      }
			  }
		      }
		  }

		if (n > 1)
		  os << '\n'
		     << indent_writer::iw3
		     << "if (CH__COMPLETED(WAIT_TASKS," << n << ")) {\n"
		     << indent_writer::iw4
		     << "CH__END_LOG(WAIT_TASKS);\n"
		     << indent_writer::iw4
		     << "break;\n"
		     << indent_writer::iw3
		     << "}\n";

		os << indent_writer::iw3
		   << "CH__TASK_SCHED(WAIT_TASKS,";
		if (n > 1)
		  os << "CH__VEC_NAME(WAIT_TASKS)";
		else
		  os << "NULL";
		os << "," << n << ");\n"
		   << indent_writer::iw2 << "}\n"
		   << indent_writer::iw1 << "}\n";
	      }

	    // タスクのアラーム設定と一斉起動
	    os << '\n'
	       << indent_writer::iw1 << "/*\n"
	       << indent_writer::iw1 << " * Set Alarm and Send start event\n"
	       << indent_writer::iw1 << " */\n";

	    const indent_writer iw7 (7);
	    const indent_writer iw8 (8);

	    for (TaskSetList::const_iterator tsi (tslist.begin ());
		 tsi != tslist.end (); tsi++)
	      {
		const TaskSet* ts = *tsi;

		if (smbp_mode && ts->coretype () == HARDWARE_CORE)
		  continue;

		const std::string tsname (mk_task_name (ts));
		const rate_T& ts_rate = ts->rate ();
		Rate_offset_T offset = ts_rate.offset ();
		Rate_rate_T rate = ts_rate.rate ();
		unsigned long tick = round (1.0 / SampleTime::get_tick ());

		assert (start_offset <= offset);
		offset -= start_offset;

		os << '\n'
		   << indent_writer::iw1
		   << "/* " << tsname << " */\n"
		   << indent_writer::iw1
		   << "OS__SETALARM(ALARM_" << tsname << ",\n"
		   << iw7 << ' ';
		os << '(' << atk2_timer_clock_hz
		   << " / " << tick << ") * ";
		if (offset)
		  os << offset;
		else
		  os << rate;
		os << ",\n"
		   << iw7 << ' '
		   << '(' << atk2_timer_clock_hz
		   << " / " << tick << ") * " << rate
		   << ");\n";

		if (ts != alarm_taskset && offset == 0)
		  {
		    os << indent_writer::iw1
		       << "OS__SENDEVENT(TASK_" << task_name << ",\n"
		       << iw8
		       << "TASK_" << tsname << ",\n"
		       << iw8
		       << "SE_" << tsname << ");\n";
		  }
	      }
	  }

	// タスクのループ
	os << '\n'
	   << indent_writer::iw1 << "/* task loop */\n"
	   << indent_writer::iw1 << "while (1) {";

	// ループの先頭で何かするためのマクロ
	os << '\n'
	   << "#ifdef TASK_" << task_name << "_LOOP_TOP\n"
	   << indent_writer::iw2
	   << "TASK_" << task_name << "_LOOP_TOP\n"
	   << "#endif\n";

	// RateTransition の CspNode を除くタスクのコードを出力
	generate_task_code_in_loop (os, taskset, GENCODE_TASK);

	// RateTransition の CspNode のタスクのコードを出力
	generate_task_code_in_loop (os, taskset, GENCODE_RATETRANSITION);

#if 1
	generate_task_code_in_loop (os, taskset, GENCODE_UPDATE);
#else
	// Updateのコードを同様に出力する
	first = true;
	for (CspNodeList::const_iterator j (taskset->cspnodelist ().begin ());
	     j != taskset->cspnodelist ().end (); j++)
	  {
	    CspNode* node = *j;

	    // 単位遅延のRateTransitionは出力しない
	    if (node->info () & CSP_NODE_INFO_DLY_RATETRANSITION)
	      continue;

	    std::string tmp;
	    const std::string& block = node->get_block_name (tmp);
	    const indent_writer iw0 (2);
	    int ind2 = 2;

	    std::string my_task_id (mk_task_id (node->id ()));

	    CodeList::const_iterator
	      ci = node->code ().find_type_code ("update");
	    if (ci == node->code ().end ())
	      continue;

	    // CspNode の情報を出力

	    const indent_writer iwCode (ind2);

	    os << iwCode << "/*\n"
	       << iwCode << " * Block: " << block << '\n'
	       << iwCode << " * Update: " << my_task_id << '\n';
	    if (enable_multirate)
	      {
		os << iwCode << " * rate: " << node->rate ();
		if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		  os << '*';
		os << '\n';
	      }
	    os << iwCode << " */\n";

	    // 外部入力が必要な場合はその旨コメント表示。
	    // ただし、ここには外部入力すべきUpdateはないかもしれない
	    if (node->info () & CSP_NODE_INFO_EXTIN)
	      generate_extvar_user_codes (os, VOUT_EXTIN, ind2);

	    // CspNode のUpdateのチャネル入力コード生成
	    generate_task_wait_input_channel (os, node, true, ind2,
					      CH_IO_MODE_UPDATE,
					      unknown_state);

	    // S-MBPモードのUpdateのチャネル入力コード生成
	    if (smbp_mode)
	      SMBP::generate_task_smbp_input_channel (os, node, ind2,
						      CH_IO_MODE_UPDATE);

	    // CspNode のUpdateの処理コード生成
	    generate_task_update_code (os, node, NULL, ind2, false);
	  }
#endif

	// Real-Time Model Objectの更新が必要であれば出力
	if (taskset->need_rtm ())
	  generate_update_rtw (os, 2);

	// スリープ前に何かするためのマクロを出力
	os << '\n'
	   << "#ifdef TASK_" << task_name << "_BEFORE_SLEEP\n"
	   << indent_writer::iw2
	   << "TASK_" << task_name << "_BEFORE_SLEEP\n"
	   << "#endif\n";

	// 次の周期起動を待つ
	os << '\n'
	   << indent_writer::iw2
	   << "/* wait for next time */\n"
	   << indent_writer::iw2
	   << "OS__WAITEVENT(TASK_" << task_name << ",\n"
	   << indent_writer (9)
	   << "WE_" << task_name << ");\n";

	// タスクの時刻更新
	if (enable_multirate)
	  os << '\n'
	     << indent_writer::iw2
	     << "task__time += " << task_rate.rate () << ";\n";

	// スリープ後に何かするためのマクロを出力
	os << '\n'
	   << "#ifdef TASK_" << task_name << "_AFTER_SLEEP\n"
	   << indent_writer::iw2
	   << "TASK_" << task_name << "_AFTER_SLEEP\n"
	   << "#endif\n";

	// タスクのループ終了
	os << indent_writer::iw1 << "}\n";

	// タスク関数の終了
	os << "}" << std::endl;

	// タスク関数外に出力済みのマクロをundefineする
	if (no_gcc && !define_list.empty ())
	  {
	    os << '\n';
	    define_list.undefine (os);
	  }

	return 0;
      }

      /// \brief TOPPERS/ATK2用コードのmain関数生成
      int
      generate_main_code (std::ostream& os)
      {
	os << '\n'
	   << "#ifndef MULTIPLE_APPMODE\n"
	   << "/*\n"
	   << " * TOPPERS/ATK2 main function\n"
	   << " */\n"
	   << "sint32\n"
	   << "main (void)\n"
	   << "{\n"
	   << indent_writer::iw1 << "StatusType ercd;\n"
	   << indent_writer::iw1 << "CoreIdType i;\n"
	   << indent_writer::iw1 << "CoreIdType coreid = GetCoreID ();\n"
	   << '\n'
	   << indent_writer::iw1 << "if (coreid == OS_CORE_ID_MASTER) {\n"
	   << indent_writer::iw2 << "for (i = 0; i < TNUM_HWCORE; i++) {\n"
	   << indent_writer::iw3 << "if (i != OS_CORE_ID_MASTER)\n"
	   << indent_writer::iw4 << "StartCore (i, &ercd);\n"
	   << indent_writer::iw2 << "}\n"
	   << indent_writer::iw2
	   << "StartOS (" << atk2_appmode << ");\n"
	   << indent_writer::iw1 << "} else {\n"
	   << indent_writer::iw2
	   << "StartOS (" << atk2_appmode << ");\n"
	   << indent_writer::iw1 << "}\n"
	   << indent_writer::iw1 << "return 0;\n"
	   << "}\n"
	   << '\n'
	   << "/*\n"
	   << " * Startup Hook\n"
	   << " */\n"
	   << "void StartupHook(void)\n"
	   << "{\n"
	   << indent_writer::iw1 << "CoreIdType coreid = GetCoreID ();\n"
	   << '\n'
	   << indent_writer::iw1 << "if (coreid == OS_CORE_ID_MASTER) {\n"
	   << indent_writer::iw2 << "syslog_initialize ();\n"
	   << indent_writer::iw2 << "syslog_msk_log (LOG_UPTO(LOG_INFO));\n"
	   << indent_writer::iw2 << "InitSerial ();\n"
	   << indent_writer::iw2 << "print_banner ();\n"
	   << indent_writer::iw2 << "swcnt_timer_init ();\n"
	   << indent_writer::iw1 << "} else {\n"
	   << indent_writer::iw2 << "InitSerial ();\n"
	   << indent_writer::iw1 << "}\n"
	   << '\n'
	   << indent_writer::iw1
	   << "syslog(LOG_EMERG, \"StartupHook @ core%d\", coreid);\n"
	   << "}\n"
	   << "#endif /* MULTIPLE_APMODE */\n";

	return 0;
      }


      /// \brief TOPPERS/ATK2コード生成関数の本体
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      ///
      /// TOPPERS/ATK2のコードをストリームに出力する
      ///
      /// \sa atk2_generate_code(void)
      /// \sa correct_task()
      /// \sa generate_preamble()
      /// \sa generate_task()
      int
      generate_code (std::ostream& os)
      {
	int error = -1;

	TaskSetList& core_assign = c_codewriter.core_assigned_taskset_list ();

	if (generate_preamble (os) == -1)
	  goto out;

	for (TaskSetList::const_iterator tsi (core_assign.begin ());
	     tsi != core_assign.end (); tsi++)
	  {
	    const TaskSet* ts = *tsi;

	    if (smbp_mode && ts->coretype () == HARDWARE_CORE)
	      continue;

	    generate_task (os, ts);
	  }

	if (atk2_gen_main
	    && generate_main_code (os) == -1)
	  goto out;

	error = 0;

      out:
	return error;
      }

      /// \brief TOPPERS/ATK2コードの生成
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      ///
      /// \sa generate_code (std::ostream& os)
      int
      generate_code (void)
      {
	int error = -1;

	std::ofstream ofs (atk2_output_code);
	if (!ofs)
	  {
	    std::cerr << atk2_output_code
		      << ": Couldn't write: " << strerror (errno)
		      << std::endl;
	    return -1;
	  }

	ofs.exceptions ( std::ostream::failbit | std::ostream::badbit );

	try
	  {
	    error = generate_code (ofs);
	  }
	catch (std::ostream::failure e)
	  {
	    std::cerr << atk2_output_code
		      << ": " << e.what () << std::endl;
	    error = -1;
	  }

	ofs.close ();

	return error;
      }

      /// \brief TOPPERS/ATK2のABREX用YAMLファイルの生成の本体
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_yaml (std::ostream& os)
      {
	static const char *iw[] = {
	  "  ",			// 0
	  "    ",		// 1
	  "      ",		// 2
	  "        ",		// 3
	  "          ",		// 4
	  "            ",	// 5
	  "              ",	// 6
	  "                ",	// 7
	};

	const TaskSetList&
	  tslist = c_codewriter.core_assigned_taskset_list ();

	os << "Ecuc:\n"
	   << iw[0] << "Os:\n"
	   << iw[1] << "OsInclude:\n"
	   << iw[2] << "OsIncludeFileName: " << atk2_app_header << '\n'
	   << iw[1] << "OsOS:\n"
	   << iw[2] << "OsNumberOfCores: " << number_of_cores << '\n'

#if 0
	   << iw[2] << "OsStackMonitoring: true\n"
	   << iw[2] << "OsUseGetServiceId: true\n"
	   << iw[2] << "OsUseParameterAccess: true\n"
	   << iw[2] << "OsStatus: EXTENDED\n"
	   << iw[2] << "OsScalabilityClass: SC1\n"

	   << iw[2] << "OsHooks:\n"
	   << iw[3] << "OsErrorHook: false\n"
	   << iw[3] << "OsPostTaskHook: false\n"
	   << iw[3] << "OsPreTaskHook: false\n"
	   << iw[3] << "OsProtectionHook: false\n"
	   << iw[3] << "OsShutdownHook: false\n"
	   << iw[3] << "OsStartupHook: false\n"

	   << iw[2] << "OsHookStack:\n"
	   << iw[3] << "OsHookStackSize: 512\n"
	   << iw[3] << "OsHookStackCoreAssignment: 0\n"

	   << iw[2] << "OsOsStack:\n"
	   << iw[3] << "OsOsStackSize: 1536\n"
	   << iw[3] << "OsOsStackCoreAssignment: 0\n"
#endif

	   << iw[1] << atk2_appmode << ":\n"
	   << iw[2] << "DefinitionRef: OsAppMode\n"

	   << iw[1] << block_name_prefix << "WakeupEvent:\n"
	   << iw[2] << "DefinitionRef: OsEvent\n"
	   << iw[1] << block_name_prefix << "InputEvent:\n"
	   << iw[2] << "DefinitionRef: OsEvent\n"
	   << iw[1] << block_name_prefix << "OutputEvent:\n"
	   << iw[2] << "DefinitionRef: OsEvent\n"
	   << iw[1] << block_name_prefix << "CompleteEvent:\n"
	   << iw[2] << "DefinitionRef: OsEvent\n";

	for (TaskSetList::const_iterator tsi (tslist.begin ());
	     tsi != tslist.end (); tsi++)
	  {
	    const TaskSet* ts = *tsi;

	    if (smbp_mode && ts->coretype () == HARDWARE_CORE)
	      continue;

	    const std::string task_name (mk_task_name (ts));

	    os << iw[1] << task_name << ":\n"
	       << iw[2] << "DefinitionRef: OsTask\n"
	       << iw[2] << "OsTaskActivation: 1\n"
	       << iw[2] << "OsTaskPriority: "
	       << task_name << "_Priority\n"
	       << iw[2] << "OsTaskStackSize: "
	       << task_name << "_Stack\n"
	       << iw[2] << "OsTaskSchedule: FULL\n"
	       << iw[2] << "OsTaskAutostart:\n"
	       << iw[3] << "OsTaskAppModeRef:\n"
	       << iw[4] << "- /Ecuc/Os/" << atk2_appmode << "\n"
	       << iw[2] << "OsTaskEventRef:\n"
	       << iw[3] << "- /Ecuc/Os/"
	       << block_name_prefix << "WakeupEvent\n"
	       << iw[3] << "- /Ecuc/Os/"
	       << block_name_prefix << "InputEvent\n"
	       << iw[3] << "- /Ecuc/Os/"
	       << block_name_prefix << "OutputEvent\n"
	       << iw[3] << "- /Ecuc/Os/"
	       << block_name_prefix << "CompleteEvent\n";
	  }

	for (TaskSetList::const_iterator tsi (tslist.begin ());
	     tsi != tslist.end (); tsi++)
	  {
	    const TaskSet* ts = *tsi;

	    if (smbp_mode && ts->coretype () == HARDWARE_CORE)
	      continue;

	    const std::string task_name (mk_task_name (ts));
	    const std::string alarm_name (mk_alarm_name (ts));

	    os << iw[1] << alarm_name << ":\n"
	       << iw[2] << "DefinitionRef: OsAlarm\n"
	       << iw[2] << "OsAlarmCounterRef: ";
	    if (atk2_timer_counterref[0] != '/')
	      os << "/Ecuc/Os/";
	    os << atk2_timer_counterref << '\n'
	       << iw[2] << "OsAlarmAction:\n"
	       << iw[3] << "OsAlarmSetEvent:\n"
	       << iw[4] << "OsAlarmSetEventTaskRef: /Ecuc/Os/"
	       << task_name << '\n'
	       << iw[4] << "OsAlarmSetEventRef: /Ecuc/Os/"
	       << block_name_prefix << "WakeupEvent\n";
	  }

	for (size_t i = 0; i < pcore_tslist.size (); i++)
	  {
	    const baseTaskSetList& btsl = pcore_tslist[i];

	    if (smbp_mode
		&& btsl.size () && btsl[0]->coretype () == HARDWARE_CORE)
	      continue;

	    os << iw[1] << block_name_prefix << "OSAP_CORE" << i << ":\n"
	       << iw[2] << "DefinitionRef: OsApplication\n"
	       << iw[2] << "OsApplicationCoreAssignment: " << i << '\n'
	       << iw[2] << "OsAppTaskRef:\n";

	    for (baseTaskSetList::const_iterator tsi (btsl.begin ());
		 tsi != btsl.end (); tsi++)
	      os << iw[3] << "- /Ecuc/Os/" << mk_task_name (*tsi) << '\n';

	    if (i == 0)
	      {
		if (atk2_no_timer_isrref == 0)
		  {
		    os << iw[2] << "OsAppIsrRef: ";
		    if (atk2_timer_isrref[0] != '/')
		      os << "/Ecuc/Os/";
		    os << atk2_timer_isrref << '\n';
		  }
		os << iw[2] << "OsAppCounterRef: ";
		if (atk2_timer_counterref[0] != '/')
		  os << "/Ecuc/Os/";
		os << atk2_timer_counterref << '\n'
		   << iw[2] << "OsAppAlarmRef:\n";
		for (size_t j = 0; j < pcore_tslist.size (); j++)
		  {
		    const baseTaskSetList& btsl = pcore_tslist[j];

		    if (smbp_mode
			&& btsl.size ()
			&& btsl[0]->coretype () == HARDWARE_CORE)
		      continue;

		    for (baseTaskSetList::const_iterator tsi (btsl.begin ());
			 tsi != btsl.end (); tsi++)
		      os << iw[3]
			 << "- /Ecuc/Os/" << mk_alarm_name (*tsi)<< '\n';
		  }
	      }

	    os << iw[2] << "OsApplicationHooks:\n"
	       << iw[3] << "OsAppErrorHook: false\n"
	       << iw[3] << "OsAppShutdownHook: false\n"
	       << iw[3] << "OsAppStartupHook: false\n";
	  }

	return 0;
      }

      /// \brief TOPPERS/ATK2のABREX用YAMLファイルの生成
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_yaml (void)
      {
	std::ofstream ofs (atk2_abrex_yaml);
	if (!ofs)
	  {
	  std::cerr << atk2_abrex_yaml
		    << ": Couldn't write: " << strerror (errno) << std::endl;
	  return -1;
	}

	ofs.exceptions ( std::ostream::failbit | std::ostream::badbit );

	int error;
	try
	  {
	    error = generate_yaml (ofs);
	  }
	catch (std::ofstream::failure e)
	  {
	    std::cerr << atk2_abrex_yaml << ": " << e.what () << std::endl;
	    error = -1;
	  }

	if (ofs)
	  ofs.close ();

	return error;
      }

      /// \brief TOPPERS/ATK2のYAML用ヘッダファイルの生成の本体
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_app_header (std::ostream& os)
      {
	const TaskSetList&
	  tslist = c_codewriter.core_assigned_taskset_list ();

	int priority = 15;	// 0(low)..15(high)
	int last_rate = -1;

	std::string hdef (atk2_app_header);

	for (std::string::iterator i (hdef.begin ()); i != hdef.end (); i++)
	  {
	    if (isalpha ((unsigned char)*i))
	      {
		if (islower ((unsigned char)*i))
		  *i = toupper (*i);
	      }
	    else if (!isdigit ((unsigned char)*i))
	      *i = '_';
	  }

	os << "#ifndef " << hdef << '\n'
	   << "#define " << hdef << '\n';

	for (TaskSetList::const_iterator tsi (tslist.begin ());
	     tsi != tslist.end (); tsi++)
	  {
	    const TaskSet* ts = *tsi;

	    if (smbp_mode && ts->coretype () == HARDWARE_CORE)
	      continue;

	    const std::string task_name (mk_task_name (ts));

	    int ts_rate = ts->rate ().rate ();
	    if (last_rate == -1)
	      last_rate = ts_rate;
	    else if (last_rate != ts_rate)
	      {
		last_rate = ts_rate;
		if (priority)
		  --priority;	// 0 でサチる
	      }

	    os << '\n'
	       << "#define " << task_name << "_Priority\t" << priority << '\n'
	       << "#define " << task_name << "_Stack\t" << 0x200 << '\n';
	  }

	os << '\n'
	   << "#endif /* " << hdef << " */" << std::endl;

	return 0;
      }

      /// \brief TOPPERS/ATK2のYAML用ヘッダファイルの生成
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_app_header (void)
      {
	std::ofstream ofs (atk2_app_header);
	if (!ofs)
	  {
	    std::cerr << atk2_app_header
		      << ": Couldn't write: " << strerror (errno) << std::endl;
	    return -1;
	  }

	ofs.exceptions ( std::ostream::failbit | std::ostream::badbit );

	int error;

	try
	  {
	    error = generate_app_header (ofs);
	  }
	catch (std::ofstream::failure e)
	  {
	    std::cerr << atk2_app_header << ": " << e.what () << std::endl;
	    error = -1;
	  }

	if (ofs)
	  ofs.close ();

	return error;
      }

      /// \brief デフォルトの出力Cコードファイル名作成用
      static std::string output_code;

      /// \brief デフォルトのYAMLファイル名作成用
      static std::string output_yaml;

      /// \brief デフォルトのヘッダファイル名作成用
      static std::string output_header;

      /// \brief TOPPERS/ATK2コード生成のための準備用関数
      int
      prepare_generate_code (void)
      {
	if (!prepare_pulse_generator_regex ())
	  return -1;

	if (!make_dsmem_regpat ())
	  return -1;

	const blocks_T::name_optional& no =  csp_reader.xml_blocks ().name ();
	if (no)
	  model_name = *no;

	block_name_prefix = model_name + "_";

	if (atk2_output_code == NULL)
	  {
	    output_code = model_name + "-output.c";
	    atk2_output_code = output_code.c_str ();
	  }

	if (atk2_abrex_yaml == NULL)
	  {
	    output_yaml = model_name + "-app.yaml";
	    atk2_abrex_yaml = output_yaml.c_str ();
	  }

	if (atk2_app_header == NULL)
	  {
	    output_header = model_name + "-app.h";
	    atk2_app_header = output_header.c_str ();
	  }

	if (atk2_appmode.length () == 0)
	  atk2_appmode = block_name_prefix + "AppMode";

	const char* cntr = strrchr (atk2_timer_counterref, '/');
	if (cntr)
	  cntr++;
	else
	  cntr = atk2_timer_counterref;
	atk2_timer_clock_hz = std::string (cntr) + "_TIMER_CLOCK_HZ";

	// TOPPERS/ATK2用タスク関連情報のセットアップ
	if (correct_task ())
	  return -1;

	return 0;
      }

      /// @}
    }

    /// @}

    using namespace ATK2;

    /// \addtogroup ATK2_GENERATOR_C_CODE
    ///
    /// @{


    /// \brief TOPPERS/ATK2用コード生成機能の初期化
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    int
    atk2_generator_init (void)
    {
      SampleTime::set_tick (0.001); // 1ms
      assert_output_enable = 0;	    // assert()を出さない

      return 0;
    }

    /// \brief TOPPERS/ATK2用のコード生成を行う
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// 正規表現の準備とファイルオープン後、
    /// TOPPERS/ATK2コード生成関数の本体を呼び出す
    ///
    /// \sa atk2_generate_code(std::ostream& os)
    int
    atk2_generate_code (void)
    {
      int error;
      std::ofstream ofs;

      if (!smbp_mode)
	{
	  error = prepare_generate_code ();
	  if (error)
	    return error;
	}

      error = generate_code ();
      if (error)
	return error;

      error = generate_yaml ();
      if (error)
	return error;

      error = generate_app_header ();

      return error;
    }

    /// @}
  }
}
