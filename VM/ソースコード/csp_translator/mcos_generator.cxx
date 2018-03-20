// -*- mode: c++; coding: utf-8-unix; -*-

/// \file mcos_generator.cxx
///
/// \brief eMCOS向けコード生成

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
#include <stdarg.h>

#include <vector>
#include <deque>
#include <list>
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

/// \brief MCOS_WEAK を空にしてweakシンボルではない実体を宣言する
#define MCOS_WEAK		/* none */

#include "csp_translator.h"

/// \addtogroup MCOS_GENERATOR_C_CODE
///
/// @{

/// \brief 変数名の前のプレフィクス設定マクロ
///
/// 現状、空でよい
#define MCOS_VAR_PREFIX	""

/// @}

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup CSP_TRANSLATOR
    ///
    /// @{

    /// \defgroup MCOS_GENERATOR eMCOSコード生成
    ///
    /// \brief eMCOS用コード生成機能
    ///
    /// @{

    /// \defgroup MCOS_GENERATOR_OPTIONS eMCOSコード生成用オプション変数
    ///
    /// \brief eMCOS用コード生成コマンドラインオプション変数
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    ///
    /// @{

    /// \brief 周期比による同期実行ではなく、最小1ms周期としたCSRにする
    int enable_csr = 0;		// --enable-csr

    /// @}

    /// \defgroup MCOS_GENERATOR_C_CODE eMCOS用Cコード生成本体
    ///
    /// \brief eMCOS用Cコード生成モジュール
    ///
    /// このモジュールはeMCOS向けのコード生成を行うクラスである。
    ///
    /// Cのコード生成は Simulink::CspTrans::CspReader が提供するCSP内部
    /// 情報を元に、eMCOS用に Simulink::CspTrans::C_CodeWriter に設定し、
    /// コード生成の処理順に Simulink::CspTrans::C_CodeWriter のライタ
    /// 機能を呼び出す、というようになっている。
    ///
    /// このモジュールでのトップレベルの関数は
    /// mcos_generate_code(const char *mcos_out) である
    ///
    /// \sa mcos_generate_code(const char *mcos_out)
    /// @{

    using namespace SimulinkModel::XSD;
    using namespace Simulink::XML;

    /// \brief 状態変数のオブジェクトが未定義の場合の代替名
    static const std::string unknown_state ("unknown__DW");

    /// \brief DataStoreMemory用状態変数のオブジェクトのプレフィクス
    static const std::string dsm_prefix ("shared__");

    /// \brief レートが切り替わる先頭のタスクのリストの連想配列
    ///
    /// キーはレートで、値は CspNodeList
    class csr_task_T
      : public std::map < rate_T, CspNodeList >
    {
    public:
      /// \brief csr_task_T のデフォルトコンストラクタ
      csr_task_T(){}

      /// \brief レート毎の CspNodeList に CspNode を追加
      ///
      /// \param[in] rate レート
      /// \param[in] node レートが切り替わる先頭の CspNode
      ///
      /// レートが切り替わる先頭の CspNode をレート毎の
      /// CspNodeList に追加する
      ///
      /// この先頭タスクがCSRタスクになる
      void
      add_task(const rate_T& rate, CspNode* node)
      {
	iterator i = find (rate);
	if (i == end ())
	  {
	    std::pair < iterator, bool >
	      res = insert (std::make_pair (rate, CspNodeList ()));
	    i = res.first;
	  }

	i->second.push_back (node);
      }
    };

    /// \brief CSRタスクの情報
    static csr_task_T csr_task;

    /// \brief 処理済み関数リスト(マーク用ベクタ)
    static std::vector< bool > output_function_mark;

    /// \brief 処理済み共通関数リスト
    static std::vector< bool > output_common_function_mark;

    /// \brief
    /// 開始点から後段に同じレートを探索し、開始点をマークとして設定する
    ///
    /// \param[in] head 開始点の CspNode のポインタ
    /// \param[in] node 探索中の CspNode のポインタ
    /// \param[out] mark 探索した CspNode をインデクスとした CspNodeList
    ///
    /// mark の CspNode のインデクスに、head を設定し、
    /// 開始点から後段を探索し同じレートで、かつ未マークであれば、
    /// 探索を継続する。
    ///
    /// これはCSRタスクの先頭ブロックと、
    /// 先頭ブロックにより駆動されるブロックを識別するために行う
    static void
    search_same_rate (CspNode* head, const CspNode* node, CspNodeList& mark)
    {
      mark[node->index ()] = head;
      for (CspEdgeList::const_iterator fei (node->forw_edges ().begin ());
	   fei != node->forw_edges ().end (); fei++)
	{
	  const CspNode* n_node = fei->node ();
	  if (head->rate () == n_node->rate ()
	      && mark[n_node->index ()] == NULL)
	    search_same_rate (head, n_node, mark);
	}
    }


    /// \brief eMCOS用タスク関連情報のセットアップ
    ///
    /// 次の事をおこなう
    /// -# コア割り当て用 TaskSetList を作成
    /// -# マルチレートの場合、コアごとのレートの設定を行う
    /// -# コア割り当てされているものをそうでないものをそれぞれの
    ///    TaskSetList へ振り分ける
    /// -# CSRを利用する場合、レートが切り替わる先頭ブロックと、
    ///    それから駆動されるブロックを識別する
    /// -# それぞれの TaskSetList の TaskSet を設定する
    static void
    mcos_correct_task (void)
    {
      // コア割り当て用 TaskSetList を作成
      c_codewriter.core_assigned_taskset_list().set_size (core_num);

      if (enable_multirate)
	{
	  // マルチレートの場合

	  // コア割り当てされている CspNode について、コアに対応する
	  // TaskSet にrateを設定する
	  for (CspNodeList::const_iterator
		 ci (CspNode::csp_nodelist ().begin ());
	       ci != CspNode::csp_nodelist ().end (); ci++)
	    {
	      CspNode* cn = *ci;
	      if (cn->core () == -1 || core_num <= cn->core ())
		continue;

	      TaskSet*
		ts = c_codewriter.core_assigned_taskset_list ()[cn->core ()];

	      if ((cn->info () & CSP_NODE_INFO_CHANGE_RATE) == 0)
		ts->add_rate (cn->rate ());

	      if (verbose)
		{
		  std::string tmpstr;
		  std::cout << "task" << cn->id ()
			    << ": rate=" << cn->rate ();
		  if (cn->info () & CSP_NODE_INFO_CHANGE_RATE)
		    std::cout << '*';
		  std::cout << ": " << cn->get_block_name (tmpstr)
			    << std::endl;
		}
	    }

	  // コアごとに、含まれるレートと、基本レート(最大公約数のレー
	  // ト)を決定する
	  for (int i = 0; i < core_num; i++)
	    {
	      TaskSet* ts = c_codewriter.core_assigned_taskset_list ()[i];
	      ts->select_rate ();
	    }
	}

      size_t node_index_size = 0;	// タスクIDの最大値
      for (CspNodeList::const_iterator
	     ci (CspNode::csp_nodelist ().begin ());
	   ci != CspNode::csp_nodelist ().end (); ci++)
	{
	  CspNode* cn = *ci;

	  // コア割り当てされている CspNode とコア割り当てされていない
	  // CspNode をそれぞれの TaskSetList に登録
	  if (cn->core () != -1 && cn->core () < core_num)
	    c_codewriter.
	      core_assigned_taskset_list()[cn->core()]->add_node (cn);
	  else
	    c_codewriter.taskset_list ().add_node (cn);

	  // 最大のタスク番号を設定
	  if ((size_t)cn->id () > node_index_size)
	    node_index_size = cn->id ();
	}

      // タスク番号をインデクスとした場合のベクタの大きさ
      node_index_size++;

      if (enable_csr)
	{
	  //
	  // CSRの先頭ブロックとそこから駆動されるブロックを識別する
	  //

	  CspNodeList exec_task_list = CspNode::csp_nodelist ();
	  CspNodeList csr_mark_list (node_index_size, NULL);
	  CspNodeList csr_core_task (static_cast <size_t> (core_num), NULL);

	  // CspNode をコードの出現順にソート
	  std::sort (exec_task_list.begin (), exec_task_list.end (),
		     CspNodePosLess());

	  // 出現順に、先頭ブロックを探す
	  for (CspNodeList::iterator i (exec_task_list.begin ());
	       i != exec_task_list.end (); i++)
	    {
	      CspNode* head_node = *i;
	      CspNode*& csr_mark = csr_mark_list[head_node->id ()];

	      // 既にチェックされている
	      if (csr_mark)
		continue;

	      if (head_node->core () != -1)
		{
		  // コア割り当てされている場合、コア割り当てされている
		  // 先頭ブロックについては、NULLでなければならない。な
		  // ぜなら同じコアに割り当てられている場合は既にマーク
		  // されていなければならない
		  CspNode*& core_head = csr_core_task[head_node->core ()];
		  assert (core_head == NULL);
		  core_head = head_node;
		}

	      // レートの先頭ブロックとマーク
	      head_node->info () |= CSP_NODE_INFO_MULTIRATE_HEAD;

	      // 後段のブロックについて、同じレートの範囲を、この先頭ブ
	      // ロックでマークする
	      search_same_rate (head_node, head_node, csr_mark_list);

	      if (head_node->core () != -1)
		{
		  // コア割り当てされているブロックであれば、同じコアに
		  // 割り当てされている CspNode について、マークする
		  TaskSetList&
		    tsl = c_codewriter.core_assigned_taskset_list ();
		  TaskSet* ts = tsl[head_node->core ()];
		  for (CspNodeList::iterator i (ts->cspnodelist ().begin ());
		       i != ts->cspnodelist ().end (); i++)
		    {
		      CspNode* n_head_node = *i;
		      if (csr_mark_list[n_head_node->id ()] == NULL)
			search_same_rate (head_node,
					  n_head_node, csr_mark_list);
		    }
		}
	    }
	}

      // コア割り当てされている TaskSet 内の CspNodeを、コードの出現順
      // に並び替え、その順にタスクの情報を TaskSet に設定する
      for (TaskSetList::iterator
	     i (c_codewriter.core_assigned_taskset_list ().begin ());
	   i != c_codewriter.core_assigned_taskset_list ().end (); i++)
	{
	  TaskSet* t = *i;
	  if (t->cspnodelist ().size () > 1)
	    std::sort (t->cspnodelist ().begin (),
		       t->cspnodelist ().end (), CspNodePosLess());
	  t->setup_taskset ();
	}

      // コア割り当てされていない TaskSet 内の CspNodeを、コードの出現
      // 順に並び替え、その順にタスクの情報を TaskSet に設定する
      for (TaskSetList::iterator i (c_codewriter.taskset_list ().begin ());
	   i != c_codewriter.taskset_list ().end (); i++)
	{
	  TaskSet* t = *i;
	  if (t->cspnodelist ().size () > 1)
	    std::sort (t->cspnodelist ().begin (),
		       t->cspnodelist ().end (), CspNodePosLess());
	  t->setup_taskset ();
	}
    }


    /// \brief 生成ファイルの先頭にタスクのコード以外を出力する
    ///
    /// \param[in] os 出力ストリーム
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// この処理で出力するのは以下の通り
    ///
    /// -# BLXMLにあるヘッダファイルに対するinclude文
    /// -# スレッドのリソース用マクロ
    /// -# 共通のマクロ
    /// -# Real-Time Model Objectのマクロ
    /// -# Real-Time Model Objectの型
    /// -# 定義されている型
    /// -# 利用者が指定したヘッダファイルに対するinclude文
    /// -# コンパイラ向け依存マクロ
    /// -# チャネル操作用マクロおよび関数
    /// -# イベントが必要なSubSystemが送信するイベントの型とコピー用マクロ
    /// -# CspNode 間の通信チャネルの型
    /// -# CSRを利用する場合のCSRのタスクのスレッドID
    /// -# 共有するDataStore
    static int
    mcos_generate_preamble (std::ostream& os)
    {
      os << "#include <mcos/mcos.h>\n"
	 << "#include <mcos/mcos_ds.h>\n";

      os << "#include <string.h>\n"; // for memcpy/memset

      os << "#include <assert.h>\n";

      // Simulinkモデル用のヘッダファイルの前までinclude文を出力。モデ
      // ル用ヘッダファイルはBLXMLに取り込まれているので、それは出力し
      // ない
      int mark_index = -1;
      generate_includes (os, c_codewriter.include_file (), mark_index);

      /// スレッドのリソース用マクロ
      os << '\n'
	 << "/*\n"
	 << " * Resource by Core\n"
	 << " */\n";

      for (int i = 0; i < core_num; i++)
	{
	  TaskSet* ts = c_codewriter.core_assigned_taskset_list ()[i];
	  if (ts->cspnodelist ().size ())
	    os << '\n'
	       << "/* Core " << i << " affinity task */\n"
	       << "#ifndef CORE_" << i << "_STACK_SIZE\n"
	       << "#define CORE_" << i << "_STACK_SIZE\t0 /* default */\n"
	       << "#endif /* CORE_" << i << "_STACK_SIZE */\n"
	       << '\n'
	       << "#ifndef CORE_" << i << "_PRIORITY\n"
	       << "#define CORE_" << i << "_PRIORITY\t128\n"
	       << "#endif /* CORE_" << i << "_PRIVATE */\n";
	}

      os << '\n'
	 << "/* Logical Core task */\n"
	 << "#ifndef LCORE_STACK_SIZE\n"
	 << "#define LCORE_STACK_SIZE\t0 /* default */\n"
	 << "#endif /* LCORE_STACK_SIZE */\n"
	 << '\n'
	 << "#ifndef LCORE_PRIORITY\n"
	 << "#define LCORE_PRIORITY\t128\n"
	 << "#endif /* LCORE_PRIVATE */\n";

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

      // ヘッダファイルの二重インクルード抑止の記述があれば、endifを出
      // 力
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
	 << "#endif /* TASK_DATA_PREPARE */\n"
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

      // チャネル操作用マクロ生成
      generate_channel_macros (os, MCOS_VAR_PREFIX);

      // 機種依存マクロ生成
      os << "\n#ifndef CH__SYNCM\n"
	 << "#if defined(__v850__) && defined(__GNUC__)\n"
	 << "#define CH__SYNCM()\t"
	 << "__asm__ __volatile__ (\"syncm\" ::: \"memory\")\n"
	 << "#else /* !__v850__ || !__GNUC__ */\n"
	 << "#define CH__SYNCM()\tdo { ; } while (0)\n"
	 << "#endif /* !__v850__ || __GNUC__ */\n"
	 << "#endif /* !CH__SYNCM */\n";
      os << "\n#ifndef CH__MEM_BARRIER\n"
	 << "#ifdef __GNUC__\n"
	 << "#define CH__MEM_BARRIER()\t"
	 << "__asm__ __volatile__ (\"\" ::: \"memory\")\n"
	 << "#else /* !__GNUC__ */\n"
	 << "#define CH__MEM_BARRIER()\tdo { ; } while (0)\n"
	 << "#endif /* !__GNUC__ */\n"
	 << "#endif /* !CH__MEM_BARRIER */\n";
      os << "\n#ifndef CH__TASK_SCHED\n"
	 << "#ifdef MCOS_H\n"
	 << "#define CH__TASK_SCHED(C,V,S)\tmcos_thread_delay(0)\n"
	 << "#else /* !MCOS_H */\n"
	 << "#define CH__TASK_SCHED(C,V,S)\tdo { ; } while (0)\n"
	 << "#endif /* !MCOS_H */\n"
	 << "#endif /* !CH__TASK_SCHED */\n";
      os << "\n#ifndef CH__EVENT\n"
	 << "#define CH__EVENT(T,C)\tCH__MEM_BARRIER()\n"
	 << "#endif /* CH__EVENT */\n";
      os << "\n#ifndef CH__END_LOG\n"
	 << "#define CH__END_LOG(C)\tdo { ; } while (0)\n"
	 << "#endif /* CH__END_LOG */\n";
      os << "\n#ifndef CH__CORE_THREAD_SCHED\n"
	 << "#define CH__CORE_THREAD_SCHED(C,V,S)\tdo { ; } while (0)\n"
	 << "#endif /* CH__CORE_THREAD_SCHED */\n";

      // チャネル操作用inline関数生成
      generate_channel_functions (os);

      // イベントが必要なSubSystemが送信するイベントの型とコピー用マクロ
      generate_sybsystem_state_and_macros (os, unknown_state);

      // CspNode 間の通信チャネルの型
      generate_channel_types (os, unknown_state);

      // CSRを利用する場合のCSRのタスクのスレッドID
      if (enable_multirate && enable_csr)
	{
	  os << '\n'
	     << "/*\n"
	     << " * CSR controlled thread IDs\n"
	     << " */\n";

	  TaskSetList& tsl = c_codewriter.core_assigned_taskset_list ();
	  for (int i = 0; i < core_num; i++)
	    {
	      const TaskSet* ts = tsl[i];
	      if (ts->cspnodelist ().size () && ts->multirate_head ())
		{
		  os << '\n'
		     << "/* Core#" << i << " [" << ts->rate () << "] */\n"
		     << "mcos_id_t tid__core_" << i << ";\n";

		  csr_task.add_task (ts->rate (),
				     *ts->cspnodelist ().begin ());
		}
	    }

	  for (TaskSetList::const_iterator
		 i (c_codewriter.taskset_list ().begin ());
	       i != c_codewriter.taskset_list ().end (); i++)
	    {
	      const TaskSet* ts = *i;
	      CspNode* node = *ts->cspnodelist ().begin ();
	      if (node->info () & CSP_NODE_INFO_MULTIRATE_HEAD)
		{
		  std::string tmp;
		  const std::string& bname = node->get_block_name (tmp);
		  std::string my_task_id (mk_task_id (node->id ()));
		  os << '\n'
		     << "/* " << bname << " [" << node->rate () << "] */\n"
		     << "mcos_id_t tid__task_" << my_task_id << ";\n";
		  csr_task.add_task (node->rate (), node);
		}
	    }

	  for (csr_task_T::iterator i (csr_task.begin ());
	       i != csr_task.end (); i++)
	    {
	      os << '\n'
		 << "/* csr thread [" << i->first << "] */\n"
		 << "void mcos_csr_thread_rate_" << i->first
		 << "(mcos_id_t, uintptr_t);\n";
	    }
	}

      // 共有するDataStore。状態変数とは同居できないので独立に構造体名
      // を変えて出力する
      generate_datastore (os, dsm_prefix);

      //
      // 共通の関数を出力する
      //

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

      return 0;
    }


    /// \brief マイグレーション可能スレッドのタスクの定義・宣言の出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] taskset タスク情報 TaskSet のポインタ
    /// \param[in] node 対象となる CspNode のポインタ
    /// \param[in] task_attr タスク属性
    /// \param[in] outer タスク関数外かどうかを示す真理値
    /// \param[in] postfix プリプロセッサマクロで定義するものにつけるpostfix
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// outerがfalseの場合は、タスク関数内での定義を行う。\n
    /// outer がtrueの場合は、タスクの固有の変数等と関数が、タスク関数
    /// 以外の関数(SubSystemが関数として外出しされているような場合)から
    /// 参照できる必要がある場合には、それらはタスク関数外で定義される
    /// 必要がある。具体的には、他と衝突しないように参照する名前に
    /// \a postfix を付けたものをdefineし、後でundefする。
    ///
    /// \sa task_attr_T
    /// \sa generate_task_subsystem_variables()
    /// \sa generate_task_rtw_context()
    /// \sa generate_task_definitions()
    int
    mcos_generate_lcore_task_definitions (std::ostream& os, TaskSet* taskset,
					  CspNode* node,
					  const task_attr_T& task_attr,
					  bool outer,
					  const std::string& postfix,
					  def_list_T* define_list = NULL)
    {
      std::string stdef;		// static か空
      int ind = 1;

      std::string code_attr;	// コードに付加するコンパイラ用属性
      std::string data_attr;	// データに付加するコンパイラ用属性
      std::string rodata_attr;	// 読込専用データに付加するコンパイラ用属性

      // タスク関数外の場合は code_attr, data_attr, rodata_attr を設定
      if (outer)
	{
	  ind = 0;
	  os << "\nTASK_DATA_PREPARE(" << postfix << ")\n";

	  code_attr = "TASK_CODE_ATTRIBUTE(" + postfix + ")";
	  data_attr = "TASK_DATA_ATTRIBUTE(" + postfix + ")";
	  rodata_attr = "TASK_RODATA_ATTRIBUTE(" + postfix + ")";
	}

      // タスクの定義・宣言を出力
      generate_task_definitions (os, taskset, outer, postfix,
				 code_attr, data_attr, rodata_attr,
				 define_list);

      // タスク関数内では、イベントが必要な SubSystem 用の変数を出力する
      if (!outer)
	generate_task_subsystem_variables (os, node, ind);

      if (outer)
	os << "\nTASK_CODE_PREPARE(" << postfix << ")\n";

      // タスク関数外でgccでない、またはタスク関数内でgcc用コード出力の
      // 場合で、ブロックの関数を持つ場合は、関数を出力する
      if (((outer && no_gcc) || (!outer && no_gcc == 0))
	  && task_attr.has_block_functions ())
	{
	  bool comout = true;

	  output_function_mark = output_common_function_mark;
	  output_function_sub (os, node->functions (), output_function_mark,
			       true, ind, comout, code_attr, postfix, outer,
			       define_list);
	}

      return 0;
    }

    /// \brief マイグレーション可能スレッドのタスクコードの生成
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] taskset タスク情報 TaskSet のポインタ
    ///
    /// \retval 0 成功
    /// \retval -1 失敗
    ///
    /// タスク関数と、コード生成がgcc用ではない場合は、
    /// タスク関数外に関数や変数、状態変数、パラメタ等を出力する
    ///
    /// \sa TaskSet
    /// \sa mcos_generate_lcore_task_definitions()
    /// \sa generate_task_channel_vector()
    /// \sa generate_task_wait_input_channel()
    /// \sa generate_task_exec_code()
    /// \sa generate_task_wait_output_channel()
    /// \sa generate_task_set_offset()
    /// \sa generate_extvar_user_codes()
    /// \sa generate_task_rtw_initializer()
    /// \sa generate_task_context_initializer()
    /// \sa def_list_T::undefine()
    int
    mcos_generate_lcore_task (std::ostream& os, TaskSet *taskset)
    {
      // タスク属性を取得
      const task_attr_T& task_attr = *taskset;

      // 代表の CspNode を取得
      CspNode* node = *taskset->cspnodelist ().begin ();

      bool first = true;
      def_list_T define_list;	// 定義されたマクロ

      std::string my_task_id (mk_task_id (node->id ()));

      std::string tmp;
      const std::string& block = node->get_block_name (tmp);

      std::string my_func_base ("mcos_lcore_task_" + my_task_id);

      // gcc用のコード生成ではない場合等、タスク関数外に定義が必要な関
      // 数や変数を出力する
      if (no_gcc || task_attr.need_out_definitions ())
	{
	  if (mcos_generate_lcore_task_definitions (os, taskset,
						    node, task_attr,
						    true, // outer
						    my_task_id,
						    &define_list))
	    return -1;
	}
      else
	os << "\nTASK_CODE_PREPARE(" << my_task_id << ")\n";

      // タスク関数の出力
      os << "\n"
	 << "/* Block: " << block << " */\n";
      if (enable_multirate)
	{
	  os << "/* rate: " << node->rate ();
	  if (task_attr.has_variable_rate ())
	    os << '*';
	  os << " */\n";
	  if (enable_csr && task_attr.multirate_head ())
	    os << "/* CSR controlled task */\n";
	}
      os << "TASK_CODE_ATTRIBUTE(" << my_task_id << ")\n"
	 << "void " << my_func_base
	 << "(uint32_t stacd, uintptr_t extinfo)\n"
	 << "{";

      // マルチレートの場合は、自分の実行時刻、相手への送信タイミングを
      // 知る必要があるのでその対応を行う
      if (enable_multirate)
	{
	  // 自分の時刻の変数出力
	  os << '\n'
	     << indent_writer::iw1 << "/* task time */\n"
	     << indent_writer::iw1 << "unsigned long task__time = "
	     << node->rate().offset () << ";\n";

	  // 可変レートの場合の、レートは固定値ではなく変数化する
	  if (task_attr.has_variable_rate ())
	    os << indent_writer::iw1 << "/* task rate(variable) */\n"
	       << indent_writer::iw1
	       << "unsigned long task__rate_t" << my_task_id
	       << " = " << node->rate ().rate () << ";\n";

	  // 同期イベントや非同期入力の場合に、制御に必要な変数を出力す
	  // る
	  bool first = true;
	  for (CspEdgeList::const_iterator i (node->forw_edges ().begin ());
	       i != node->forw_edges ().end (); i++)
	    {
	      const CspEdge& ce = *i;

	      if (ce.info ()
		  & (CSP_EDGE_INFO_SYNC_EVENT|CSP_EDGE_INFO_ASYNC_INPUT))
		{
		  if (first)
		    {
		      os << '\n'
			 << indent_writer::iw1
			 << "/* event or async enable */\n";
		      first = false;
		    }
		  std::string xx_task_id (mk_task_id (ce.node ()->id ()));
		  os << indent_writer::iw1
		     << "int enable__send_t" << xx_task_id << " = 0;\n";
		}
	    }

	  // CSRを使う場合には、メッセージ用の変数を出力する
	  if (enable_csr && task_attr.multirate_head ())
	    os << '\n'
	       << indent_writer::iw1 << "/* message */\n"
	       << indent_writer::iw1 << "uint32_t task__msg_data;\n"
	       << indent_writer::iw1 << "size_t task__msg_size;\n";
	}

      // タスク関数に、元コード(step関数)の先頭部分を出力
      if (c_codewriter.preamble ().size ())
	{
	  os << '\n'
	     << indent_writer::iw1 << "/* definitions of step function */\n"
	     << c_codewriter.preamble ()
	     << indent_writer::iw1 << "/* end of definitions */\n";
	}

      // チャネル用待ち合わせ用のベクタを出力
      generate_task_channel_vector (os, node, my_task_id, first);

      // タスク関数内に変数や関数の定義を行う
      if (mcos_generate_lcore_task_definitions (os, taskset, node, task_attr,
						false, ""))
	return -1;

      // ユーザーが変数等の定義を追加できるポイントを出力
      os << '\n'
	 << "#ifdef TASK_" << my_task_id << "_DEFINITIONS\n"
	 << indent_writer::iw1
	 << "TASK_" << my_task_id << "_DEFINITIONS\n"
	 << "#endif\n";

      // 共通の初期化コードを出力
      if (c_codewriter.common_init ().size ())
	{
	  os << '\n'
	     << indent_writer::iw1 << "/* Common Initialization */\n"
	     << c_codewriter.common_init ();
	}

      // Real-Time Model Objectが必要な場合は、初期化コードを出力
      if (node->info () & CSP_NODE_INFO_NEED_RTM)
	generate_task_rtw_initializer (os, 1);

      // 状態変数を持つ場合は、状態変数をゼロクリア
      if (task_attr.has_state ())
	{
	  const optional_refstring&
	    state_storage = c_codewriter.state_storage ();
	  assert (state_storage);
	  os << '\n'
	     << indent_writer::iw1 << "/* initialize state structure */\n"
	     << indent_writer::iw1 << "memset(&"
	     << state_storage << ", 0, sizeof "
	     << state_storage << ");\n";
	}

      // タスク固有の初期化コードを出力
      if (generate_task_context_initializer (os, node, 1))
	return -1;

      // 初期値受信の処理
      if (node->info () & CSP_NODE_INFO_RECV_INITVAL)
	generate_task_wait_input_channel (os, node, false, 1,
					  CH_IO_MODE_INIT, unknown_state);

      // 初期化完了のポイントを出力
      os << '\n'
	 << "#ifdef TASK_INITIALIZE_" << my_task_id << "_COMPLETED\n"
	 << indent_writer::iw1
	 << "TASK_INITIALIZE_" << my_task_id << "_COMPLETED();\n"
	 << "#endif\n";

      // 出力チャネルの時刻にサンプル時間のオフセットを設定
      if (enable_multirate)
	{
	  bool first = true;
	  generate_task_set_offset (os, node, 1, first);
	}

      // タスクのループ
      os << '\n'
	 << indent_writer::iw1 << "/* task loop */\n"
	 << indent_writer::iw1 << "while (1) {\n";

      // CSRの場合は、起動待ち
      if (enable_multirate && enable_csr && task_attr.multirate_head ())
	{
	  os << '\n'
	     << indent_writer::iw2
	     << "(void)"
		"mcos_message_recieve(&task__msg_data, &task__msg_size, "
	        "MCOS_MSG_RECV_SYNC);\n";
	}

      // 外部入力が必要な場合はその旨コメント表示
      if (node->info () & CSP_NODE_INFO_EXTIN)
	generate_extvar_user_codes (os, VOUT_EXTIN, 2);

      // チャネル入力コード生成
      generate_task_wait_input_channel (os, node, false, 2,
					CH_IO_MODE_TASK, unknown_state);

      // タスクコード生成
      generate_task_exec_code (os, node, NULL, 2, dsm_prefix);

      // 外部出力が必要な場合はその旨コメント表示
      if (node->info () & CSP_NODE_INFO_EXTOUT)
	generate_extvar_user_codes (os, VOUT_EXTOUT, 2);

      // チャネル出力コード生成
      generate_task_wait_output_channel (os, node, false, 2, unknown_state);

      // Updateのチャネル入力コード生成
      generate_task_wait_input_channel (os, node, false, 2,
					CH_IO_MODE_UPDATE, unknown_state);

      // Updateの処理コード生成
      generate_task_update_code (os, node, NULL, 2);

      // 初回送信のみの場合
      if (node->info () & CSP_NODE_INFO_SEND_INITVAL)
	{
	  os << '\n'
	     << indent_writer::iw2 << "/* Once, send for initial values */\n"
	     << indent_writer::iw2 << "break;\n";
	}

      // タスクのループ終了
      os << indent_writer::iw1 << "}\n";

      // タスク関数の終了
      os << "}\n";

      // タスク関数外に出力済みのマクロをundefineする
      if (no_gcc && !define_list.empty ())
	{
	  os << '\n';
	  define_list.undefine (os);
	}

      return 0;
    }

    /// \brief
    /// コア割り当てされた(コアアフィニティスレッド)
    /// のタスクの定義・宣言の出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] num コア番号
    /// \param[in] coretask コア割り当てされたタスクの TaskSet のポインタ
    /// \param[in] outer タスク関数外かどうかを示す真理値
    /// \param[in] postfix プリプロセッサマクロで定義するものにつけるpostfix
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// \retval 0 成功
    /// \retval -1 失敗
    ///
    /// \sa generate_task_rtw_context()
    /// \sa generate_task_subsystem_variables()
    /// \sa generate_task_definitions();
    int
    mcos_generate_core_task_definitions (std::ostream& os,
					 int num, const TaskSet* coretask,
					 bool outer,
					 const std::string& postfix,
					 def_list_T* define_list = NULL)
    {
      int ind = 1;
      std::string stdef;

      std::string code_attr;	// コードに付加するコンパイラ用属性
      std::string data_attr;   	// データに付加するコンパイラ用属性
      std::string rodata_attr;	// 読込専用データに付加するコンパイラ用属性

      // タスク関数外の場合は code_attr, data_attr を設定
      if (outer)
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
      generate_task_definitions (os, coretask, outer, postfix,
				 code_attr, data_attr, rodata_attr,
				 define_list);

      // タスク関数内部であれば、イベントが必要な SubSystem の実行状態
      // 用変数を出力する
      if (!outer)
	{
	  CspNode_map_T cnmap;
	  for (CspNodeList::const_iterator
		 j (coretask->cspnodelist ().begin ());
	       j != coretask->cspnodelist ().end (); j++)
	    {
	      CspNode* node = *j;
	      generate_task_subsystem_variables (os, node, 1, &cnmap);
	    }
	}

      if (outer)
	os << "\nCORE_TASK_CODE_PREPARE(" << num << ")\n";

      // タスクの関数外かつgccでない場合、またはタスク関数内でgccの場合
      // で、ブロックの関数を持つ場合は、関数を出力する
      if (((outer && no_gcc) || (!outer && no_gcc == 0))
	  && coretask->has_block_functions ())
	{
	  os << '\n'
	     << iw0 << "/* functions */\n";

	  output_function_mark = output_common_function_mark;
	  for (CspNodeList::const_iterator
		 j (coretask->cspnodelist ().begin ());
	       j != coretask->cspnodelist ().end (); j++)
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

    /// \brief コア割り当てされたブロックのタスクを生成する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] num コア番号
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// \sa generate_task_rtw_initializer()
    /// \sa generate_update_rtw()
    int
    mcos_generate_core_task (std::ostream& os, int num )
    {
      // コア割り当ての TaskSetList を取得
      const TaskSetList&
	core_assign = c_codewriter.core_assigned_taskset_list();

      // 指定コアに割り当てされた TaskSet を取得
      const TaskSet* coretask = core_assign[num];

      bool first = true;

      def_list_T define_list;

      // このタスクで、レート毎の時刻として用意する必要がある、レートの
      // リストの開始インデクス
      // タスクの最小レートが0番目と同じ場合は、そのレートの時刻はタス
      // クの時刻と同じなので省略できる(つまり1から始まる)
      size_t task_rate_index = ~0U;

      // 最後に使用したレートのインデクス
      size_t last_rate_index = ~0U;

      // gcc用のコード生成ではない場合等、タスク関数外に定義が必要な関
      // 数や変数を出力する
      if (no_gcc || coretask->need_out_definitions ())
	{
	  std::ostringstream oss;
	  oss << "CORE" << num;
	  std::string spfix = oss.str ();
	  const char* postfix = spfix.c_str ();

	  if (mcos_generate_core_task_definitions (os, num, coretask,
						   true, postfix,
						   &define_list))
	    return -1;
	}
      else
	os << "\nCORE_TASK_CODE_PREPARE(" << num << ")\n";

      // コア割り当てされたタスクのタスク関数の出力
      os << "\n/* Core#" << num << " */\n";
      if (enable_multirate)
	{
	  bool min_is_task = coretask->minrate_is_taskrate ();
	  size_t rates = coretask->ratebyid ().size ();

	  // 基本レートがタスクの最小レートの場合かつ可変レートをもつ場
	  // 合、または、最小レートがタスクの基本レートと異なる場合は、
	  // 開始インデクスを0にする。
	  // そうでない場合は、最小レートはタスクの基本レートと同じで、
	  // 最小レートの時刻省略できるので、開始インデクスを1にする。
	  if (min_is_task)
	    {
	      if (coretask->has_variable_rate ())
		task_rate_index = 0;
	      else
		task_rate_index = 1;
	    }
	  else if (rates > 0)
	    task_rate_index = 0;

	  // レートの表示
	  if (task_rate_index != ~0U)
	    {
	      os << "/* rate: " << coretask->rate ();
	      if (coretask->has_variable_rate ())
		os << '*';
	      os << " */\n";
	    }
	  else
	    os << "/* rate: * */\n";

	  // CSRでコントロールされるタスク
	  if (enable_csr && coretask->multirate_head ())
	    os << "/* CSR controlled task */\n";
	}

      os << "CORE_TASK_CODE_ATTRIBUTE(" << num << ")\n"
	 << "void mcos_core" << num
	 << "_thread(uint32_t stacd, uintptr_t exinf)\n"
	 << "{";

      // マルチレートの場合は、自分の実行時刻、相手への送信タイミングを
      // 知る必要があるのでその対応を行う
      if (enable_multirate)
	{
	  // 自分の時刻の変数出力
	  os << '\n'
	     << indent_writer::iw1 << "/* task time */\n"
	     << indent_writer::iw1 << "unsigned long task__time = 0;\n";

	  // 複数のレートがある場合は、レート分の時刻の変数を出力する。
	  // ただし、タスクの最小レートと基本レートが同じならそれは省略
	  // する
	  if (task_rate_index != ~0U)
	    {
	      // タスクの基本レート
	      os << indent_writer::iw1
		 << "const unsigned long task__rate = "
		 << coretask->rate ().rate () << ";\n";
	      os << '\n'
		 << indent_writer::iw1 << "/* multirate */\n";

	      // タスク内のレートと時刻を出力(開始インデクスから)
	      for (size_t i = task_rate_index;
		   i < coretask->ratebyid ().size (); i++)
		{
		  const rate_T& r = coretask->ratebyid ()[i];
		  os << indent_writer::iw1
		     << "const unsigned long task__rate_" << i << " = "
		     << r.rate () << ";\n"
		     << indent_writer::iw1
		     << "unsigned long task__time_" << i << " = "
		     << r.offset () << ";\n";
		}
	    }

	  // 可変レートの CspNode は、それぞれのレートを、固定値ではな
	  // く変数化する
	  if (coretask->has_variable_rate ())
	    {
	      for (CspNodeList::const_iterator
		     j (coretask->cspnodelist ().begin ());
		   j != coretask->cspnodelist ().end (); j++)
		{
		  CspNode* node = *j;
		  if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		    {
		      std::string my_task_id (mk_task_id (node->id ()));
		      os << '\n'
			 << indent_writer::iw1
			 << "/* time and rate for task_" << my_task_id
			 << " */\n"
			 << indent_writer::iw1
			 << "unsigned long task__rate_t" << my_task_id
			 << " = " << node->rate ().rate () << ";\n"
			 << indent_writer::iw1
			 << "unsigned long task__time_t" << my_task_id
			 << " = " << node->rate().offset () << ";\n";
		    }
		}
	    }

	  // CSRを使う場合には、メッセージ用の変数を出力する。
	  // ただのイベントなので、中身は何でもよく、個別には用意しない
	  if (enable_csr && coretask->multirate_head ())
	    os << '\n'
	       << indent_writer::iw1 << "/* message */\n"
	       << indent_writer::iw1 << "uint32_t task__msg_data;\n"
	       << indent_writer::iw1 << "size_t task__msg_size;\n";
	}

      // タスク関数に、元コード(step関数)の先頭部分を出力
      if (c_codewriter.preamble ().size ())
	{
	  os << '\n'
	     << indent_writer::iw1 << "/* definitions of step function */\n"
	     << c_codewriter.preamble ()
	     << indent_writer::iw1 << "/* end of definitions */\n";
	}

      // CspNode 毎にチャネル用待ち合わせ用のベクタを出力
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
	{
	  CspNode* node = *j;
	  std::string my_task_id (mk_task_id (node->id ()));
	  generate_task_channel_vector (os, node, my_task_id, first);
	}

      // タスク関数内に変数や関数の定義を行う
      if (mcos_generate_core_task_definitions (os, num, coretask, false, ""))
	return -1;

      // ユーザーが、コア割り当てされたタスク単位の変数等の定義を追加で
      // きるポイントを出力
      os << '\n'
	 << "#ifdef CORE_TASK_" << num << "_DEFINITIONS\n"
	 << indent_writer::iw1
	 << "CORE_TASK_" << num << "_DEFINITIONS\n"
	 << "#endif\n";

      // ユーザーが変数等の定義を追加できるポイントを CspNode 単位に出
      // 力
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
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
      if (coretask->need_rtm ())
	generate_task_rtw_initializer (os, 1);

      // 状態変数を持つ場合は、状態変数をゼロクリア
      if (coretask->has_state ())
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
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
	{
	  CspNode* node = *j;
	  std::string tmp;
	  const std::string& n = node->get_block_name (tmp);

	  os << '\n'
	     << indent_writer::iw1 << "/* Block: " << n << " */\n";

	  generate_task_context_initializer (os, node, 1, &cnmap);
	}

      // 初期値の受信処理
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
	{
	  CspNode* node = *j;

	  generate_task_wait_input_channel (os, node, true, 1,
					    CH_IO_MODE_INIT, unknown_state);
	}

      // 初期化完了のポインタを出力
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
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
		 j (coretask->cspnodelist ().begin ());
	       j != coretask->cspnodelist ().end (); j++)
	    {
	      CspNode* node = *j;

	      generate_task_set_offset (os, node, 1, first);
	    }
	}

      // タスクのループ
      os << '\n'
	 << indent_writer::iw1 << "/* task loop */\n"
	 << indent_writer::iw1 << "while (1) {";

      // CSRの場合は、起動待ち
      if (enable_multirate && enable_csr && coretask->multirate_head ())
	os << '\n'
	   << indent_writer::iw2
	   << "task__msg_size = 0;\n"
	   << "(void)mcos_message_recieve(&task__msg_data, &task__msg_size, "
	   << "MCOS_MSG_RECV_SYNC);\n";

      // if 文が、タスクのレートのリストか、可変レートかの識別
      enum {none, in_ridx, in_vrate} if_mode = none;

      // CspNode 毎に、実行タイミングになっているかどうかを条件にもつ
      // if 文および、入力・実行・出力のコードを出力する
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
	{
	  CspNode* node = *j;
	  std::string tmp;
	  const std::string& block = node->get_block_name (tmp);

	  std::string my_task_id (mk_task_id (node->id ()));

	  int ind2 = 2;

	  if (enable_multirate)
	    {
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		{
		  // 可変レートの場合

		  // if 文を出力している場合はコードブロックを閉じる
		  if (if_mode != none)
		    os << indent_writer::iw2 << "}\n";

		  // 可変レートの実行時刻かを判定するif文を出力
		  os << '\n'
		     << indent_writer::iw2
		     << "if (task__time == task__time_t"
		     << my_task_id << ") {\n";

		  // i文は可変レートの時刻なので、最後のレートのインデ
		  // クスはクリア
		  last_rate_index = ~0U;
		  if_mode = in_vrate;
		  ind2 = 3;
		}
	      else if (task_rate_index != ~0U)
		{
		  // タスクのレートの場合

		  // CspNode のレートに対するインデクスを取得
		  size_t idx = coretask->rate2idx (node->rate ());
		  if (idx >= task_rate_index )
		    {
		      // 最後のレートのインデクスと異なる場合は、新たな
		      // if文を開始する必要があり、同じ場合は不要
		      if (last_rate_index != idx)
			{
			  // if 文を出力している場合はコードブロックを
			  // 閉じる
			  if (if_mode == in_vrate
			      || (if_mode == in_ridx
				  && last_rate_index != ~0U))
			    os << indent_writer::iw2 << "}\n";

			  // レートのリストのif文を開始する
			  os << '\n'
			     << indent_writer::iw2
			     << "if (task__time == task__time_"
			     << idx << ") {\n";

			  // if文はレートのリストで、そのインデクスを保
			  // 存
			  last_rate_index = idx;
			  if_mode = in_ridx;
			}

		      ind2 = 3;
		    }
		  else
		    {
		      // タスクの基本レートの場合

		      // if 文を出力している場合はコードブロックを閉じ
		      // る
		      if (if_mode == in_vrate
			  || (if_mode == in_ridx && last_rate_index != ~0U))
			{
			  os << indent_writer::iw2 << "}\n";

			  // if 文の状態をクリア
			  last_rate_index = ~0U;
			  if_mode = none;
			}
		    }
		}
	    }

	  // CspNode の情報を出力

	  const indent_writer iwCode (ind2);

	  os << iwCode << "/*\n"
	     << iwCode << " * Block: " << block << '\n'
	     << iwCode << " * Task: " << my_task_id << '\n';

	  if (enable_multirate)
	    {
	      os << iwCode << " * rate: " << node->rate ();
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		os << '*';
	      os << '\n';
	    }
	  os << iwCode << " */\n";

	  // 外部入力が必要な場合はその旨コメント表示
	  if (node->info () & CSP_NODE_INFO_EXTIN)
	    generate_extvar_user_codes (os, VOUT_EXTIN, ind2);

	  // CspNode のチャネル入力コード生成
	  generate_task_wait_input_channel (os, node, true, ind2,
					    CH_IO_MODE_TASK, unknown_state);

	  // CspNode のタスクコード生成
	  generate_task_exec_code (os, node, NULL, ind2, dsm_prefix);

	  // 外部出力が必要な場合はその旨コメント表示
	  if (node->info () & CSP_NODE_INFO_EXTOUT)
	    generate_extvar_user_codes (os, VOUT_EXTOUT, ind2);

	  // CspNode のチャネル出力コード生成
	  generate_task_wait_output_channel (os, node, true, ind2,
					     unknown_state);
	}

      // if 文を出力している場合はコードブロックを閉じる
      if (enable_multirate
	  && (if_mode == in_vrate
	      || (if_mode == in_ridx && last_rate_index != ~0U)))
	{
	  os << indent_writer::iw2 << "}\n";
	  last_rate_index = ~0U;
	  if_mode = none;
	}

      // Updateのコードを同様に出力する
      first = true;
      for (CspNodeList::const_iterator j (coretask->cspnodelist ().begin ());
	   j != coretask->cspnodelist ().end (); j++)
	{
	  CspNode* node = *j;
	  std::string tmp;
	  const std::string& block = node->get_block_name (tmp);
	  const indent_writer iw0 (2);
	  int ind2 = 2;

	  std::string my_task_id (mk_task_id (node->id ()));

	  CodeList::const_iterator
	    ci = node->code ().find_type_code ("update");
	  if (ci == node->code ().end ())
	    continue;

	  if (enable_multirate)
	    {
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		{
		  // 可変レートの場合

		  // if 文を出力している場合はコードブロックを閉じる
		  if (if_mode != none)
		    os << indent_writer::iw2 << "}\n";

		  // 可変レートの実行時刻かを判定するif文を出力
		  os << '\n'
		     << indent_writer::iw2
		     << "if (task__time == task__time_t"
		     << my_task_id << ") {\n";

		  // i文は可変レートの時刻なので、最後のレートのインデ
		  // クスはクリア
		  last_rate_index = ~0U;
		  if_mode = in_vrate;
		  ind2 = 3;
		}
	      else if (task_rate_index != ~0U)
		{
		  // タスクのレートの場合

		  // CspNode のレートに対するインデクスを取得
		  size_t idx = coretask->rate2idx (node->rate ());
		  if (idx >= task_rate_index )
		    {
		      // 最後のレートのインデクスと異なる場合は、新たな
		      // if文を開始する必要があり、同じ場合は不要
		      if (last_rate_index != idx)
			{
			  // if 文を出力している場合はコードブロックを
			  // 閉じる
			  if (if_mode == in_vrate
			      || (if_mode == in_ridx
				  && last_rate_index != ~0U))
			    os << indent_writer::iw2 << "}\n";

			  // レートのリストのif文を開始する
			  os << '\n'
			     << indent_writer::iw2
			     << "if (task__time == task__time_"
			     << idx << ") {\n";

			  // if文はレートのリストで、そのインデクスを保
			  // 存
			  last_rate_index = idx;
			  if_mode = in_ridx;
			}
		      ind2 = 3;
		    }
		  else
		    {
		      // タスクの基本レートの場合

		      // if 文を出力している場合はコードブロックを閉じ
		      // る
		      if (if_mode == in_vrate
			  || (if_mode == in_ridx && last_rate_index != ~0U))
			{
			  os << indent_writer::iw2 << "}\n";

			  // if 文の状態をクリア
			  last_rate_index = ~0U;
			  if_mode = none;
			}
		    }
		}
	    }

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
					    CH_IO_MODE_UPDATE, unknown_state);

	  // CspNode のUpdateの処理コード生成
	  generate_task_update_code (os, node, NULL, ind2, false);
	}

      // if 文を出力している場合はコードブロックを閉じる
      if (enable_multirate
	  && (if_mode == in_vrate
	      || (if_mode == in_ridx && last_rate_index != ~0U)))
	{
	  os << indent_writer::iw2 << "}\n";
	  last_rate_index = ~0U;
	  if_mode = none;
	}

      // タスクの各時刻更新
      if (enable_multirate)
	{
	  bool first_assign = false;

	  const indent_writer* iw2 = &indent_writer::iw2;
	  const indent_writer* iw3 = &indent_writer::iw3;
	  const indent_writer* iw4 = &indent_writer::iw4;

	  // 可変レート、またはタスクの基本レートと最小レートが異なる場
	  // 合は、タスクの次の時刻は、いずれかのレートの次の時刻のうち、
	  // 最も近い時刻にする必要がある。
	  // task__next_timeは、それを決める時刻である
	  os << '\n';
	  if (coretask->has_variable_rate () || task_rate_index == 0)
	    {
	      os << indent_writer::iw2 << "/* schedule next running time*/\n"
		 << indent_writer::iw2 << "{\n";

	      // 複数のレートがある場合はまず基本レートの次の時刻が最も
	      // 小さいのでそれで初期化。そうでない場合は、出現時に代入
	      // する
	      if (task_rate_index != ~0U)
		os << indent_writer::iw3
		   << "unsigned long task__next_time"
		      " = task__time + task__rate;\n";
	      else
		{
		  os << indent_writer::iw3
		     << "unsigned long task__next_time;\n";
		  first_assign = true;
		}
	      iw2 = &indent_writer::iw3;
	      iw3 = &indent_writer::iw4;
	    }

	  // タスクのレートの時刻を更新する
	  if (task_rate_index != ~0U)
	    {
	      const task_rates_T& rates = coretask->ratebyid ();
	      for (size_t i = task_rate_index; i < rates.size (); i++)
		{
		  // そのレートの更新時刻であれば、レート毎の時刻を次の
		  // 時刻に更新する
		  os << iw2 << "if (task__time_" << i << " == task__time)\n"
		     << iw3 << "task__time_" << i << " += "
		     << "task__rate_" << i << ";\n";

		  // 最小の時刻を、次の実行時刻にする
		  if (coretask->has_variable_rate () || task_rate_index == 0)
		    {
		      os << iw3
			 << "if (CH__TIME_LT(task__time_" << i
			 << ", task__next_time))\n"
			 << iw4
			 << "task__next_time = task__time_" << i
			 << ";\n";
		    }
		}
	    }

	  // 可変レートの CspNode は時刻の処理
	  if (coretask->has_variable_rate ())
	    {
	      for (CspNodeList::const_iterator
		     j (coretask->cspnodelist ().begin ());
		   j != coretask->cspnodelist ().end (); j++)
		{
		  CspNode* node = *j;
		  if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		    {
		      std::string my_task_id (mk_task_id (node->id ()));

		      // 更新時刻になっていたら、レートを変更するマクロ
		      // を実行するのと、時刻を更新するコードを出力する
		      os << iw2 << "if (task__time_t" << my_task_id
			 << " == task__time) {\n";

		      os << "#ifdef TASK_CHANGE_RATE_" << my_task_id << '\n'
			 << iw3
			 << "TASK_CHANGE_RATE_" << my_task_id
			 << "(task__time_t" << my_task_id
			 << ", task__rate_t" << my_task_id << ");\n"
			 << "#endif\n"
			 << iw3 << "task__time_t" << my_task_id
			 << " += task__rate_t" << my_task_id << ";\n"
			 << iw2 << "}\n";

		      // 出現時の代入か、もしくは、最小時刻の選択
		      if (first_assign)
			{
			  os << iw2
			     << "task__next_time = task__time_t"
			     << my_task_id << ";\n";
			  first_assign = false;
			}
		      else
			os << iw2 << "if (CH__TIME_LT(task__time_t"
			   << my_task_id << ", task__next_time))\n"
			   << iw3 << "task__next_time = task__time_t"
			   << my_task_id << ";\n";
		    }
		}
	    }


	  // 可変レート、またはタスクの基本レートと最小レートが異なる場
	  // 合は、選択した最小時刻を次の時刻に設定。
	  // そうでなければ、基本レートで次の時刻を算出
	  if (coretask->has_variable_rate () || task_rate_index == 0)
	    os << indent_writer::iw3 << "task__time = task__next_time;\n"
	       << indent_writer::iw2 << "}\n";
	  else
	    os << indent_writer::iw2 << "task__time += task__rate;\n";
	}

      // Real-Time Model Objectの更新が必要であれば出力
      if (coretask->need_rtm ())
	generate_update_rtw (os, 2);

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

    /// \brief eMCOSの create_init_threads() を生成する
    ///
    /// \param[in] os 出力ストリーム
    void
    mcos_generate_threads (std::ostream& os)
    {
      size_t a_task = 0;	// コアアフィニティスレッドの個数
      size_t l_task = 0;	// マイグレーション可能スレッドの個数
      size_t num = 0;		// 全体のスレッドの数

      // コアアフィニティスレッドの数を算出
      const TaskSetList&
	core_assign = c_codewriter.core_assigned_taskset_list ();
      for (size_t i = 0; i < core_assign.size (); i++)
	{
	  if (core_assign[i]->cspnodelist ().size () != 0)
	    a_task++;
	}

      // マイグレーション可能スレッドの数
      l_task = c_codewriter.taskset_list ().size ();

      // 全スレッド数
      num = a_task + l_task;

      if (num == 0)
	return;

      // create_init_threads() の生成
      os << '\n'
	 << "/*\n"
	 << " * MCOS thread creation\n"
	 << " *\n"
	 << " *   create_init_threads\n"
	 << " */\n"
	 << "void\n"
	 << "create_init_threads (void)\n"
	 << "{\n"
	 << indent_writer::iw1 << "mcos_erid_t tid;\n"
	 << indent_writer::iw1 << "mcos_id_t lcid;\n"
	 << indent_writer::iw1 << "mcos_thr_attr_t attr;\n"
	 << indent_writer::iw1 << "static mcos_id_t tids[" << num << "];\n"
	 << indent_writer::iw1 << "size_t num = 0;\n"
	 << indent_writer::iw1 << "size_t i;\n";

      // CSRを利用する場合、設定用構造体を出力
      if (enable_multirate && enable_csr && csr_task.size ())
	os << indent_writer::iw1 << "mcos_cyc_cre_t csr_ent;\n";

      // DataStoreMemoryのパラメタを出力
      const AutoStorageMap& dsmem_map = c_codewriter.dsmem ();
      if (dsmem_map.size ())
	{
	  const AutoStorage*
	    dsmem_param = dsmem_map.get_storage (c_codewriter.param_storage ());
	  if (dsmem_param)
	    {
	      os << '\n'
		 << indent_writer::iw1
		 << "/*\n"
		 << indent_writer::iw1
		 << " * Parameters for DataStorememories\n"
		 << indent_writer::iw1
		 << " */\n";
	      os << dsmem_param->set_indent (1);
	    }
	}

      // 可変レートのタスクは、初期レートを出力チャネルに設定
      if (enable_multirate)
	{
	  bool first = true;
	  for (CspNodeList::const_iterator
		 i (CspNode::csp_nodelist ().begin ());
	       i != CspNode::csp_nodelist ().end (); i++)
	    {
	      CspNode* t_node = *i;
	      std::string my_task_id (mk_task_id (t_node->id ()));

	      /* output channel */
	      for (CspEdgeList::const_iterator
		     i (t_node->forw_edges ().begin ());
		   i != t_node->forw_edges ().end (); i++)
		{
		  const CspEdge& edge = *i;
		  CspNode* h_node = edge.node ();

		  if ((h_node->info () & CSP_NODE_INFO_CHANGE_RATE) == 0)
		    continue;

		  std::string xx_task_id (mk_task_id (h_node->id ()));

		  if (first)
		    {
		      os << '\n'
			 << indent_writer::iw1
			 << "/*\n"
			 << indent_writer::iw1
			 << " * initial rate for variable rate tasks\n"
			 << indent_writer::iw1
			 << " */\n";
		      first = false;
		    }

		  os << indent_writer::iw1
		     << "CH_" << my_task_id << '_' << xx_task_id
		     << ".rate__" << xx_task_id
		     << " = " << h_node->rate ().rate () << ";\n";
		}
	    }
	}

      // DataStoreMemoryの初期値設定
      if (c_codewriter.dsmem_init ().size ())
	{
	  os << '\n'
	     << indent_writer::iw1
	     << "/*\n"
	     << indent_writer::iw1
	     << " * Initializers for DataStoreMemories\n"
	     << indent_writer::iw1
	     << " */\n";

	  for (text_list_T::const_iterator
		 i (c_codewriter.dsmem_init ().begin ());
	       i != c_codewriter.dsmem_init ().end (); i++)
	    {
	      const std::string& code = *i;

	      os << '\n';

	      // DataStoreMemoryの初期化コード中にある状態変数の構造体
	      // 名に dsm_prefix を付ける
	      range_T vlist;
	      const char* s = code.c_str ();
	      if (!datastore_memory_regex.empty ()
		  && check_datastore_code (s, vlist))
		{
		  const char* p = s;
		  for (range_T::const_iterator di (vlist.begin ());
		       di != vlist.end (); di++)
		    {
		      size_t o = s + di->first - p;
		      os << std::string (p, 0, o)
			 << dsm_prefix;
		      p += o;
		    }
		  os << p; // rest
		}
	      else
		os << code;
	    }
	}

      // スレッド属性の初期化
      os << "\n"
	 << indent_writer::iw1
	 << "mcos_threadattr_init (&attr);\n";

      // コア割り当てされたタスクのスレッド生成
      for (size_t i = 0; i < core_assign.size (); i++)
	{
	  if (core_assign[i]->cspnodelist ().size () == 0)
	    continue;

	  os << '\n'
	     << indent_writer::iw1
	     << "mcos_threadattr_setlcid (&attr, " << i + 1 << ");\n"
	     << indent_writer::iw1
	     << "mcos_threadattr_setpriority (&attr, CORE_" << i
	     << "_PRIORITY);\n"
	     << indent_writer::iw1
	     << "mcos_threadattr_setstacksize (&attr, CORE_" << i
	     << "_STACK_SIZE);\n"
	     << indent_writer::iw1
	     << "tid = mcos_thread_create (&attr, mcos_core" << i
	     << "_thread, 0);\n"
	     << indent_writer::iw1
	     << "if (tid == MCOS_EPAR || tid == MCOS_ENOMEM)\n"
	     << indent_writer::iw2
	     << "return;\n"
	     << indent_writer::iw1
	     << "tids[num++] = tid;\n";

	  // CSRの場合はヘッドのスレッドIDをコア単位のCSR管理用変数に設
	  // 定
	  if (enable_multirate
	      && enable_csr
	      && core_assign[i]->multirate_head ())
	    os << indent_writer::iw1
	       << "tid__core_" << i << " = tid;\n";
	}

      // マイグレーション可能スレッドの生成
      if (l_task)
	{
	  os << '\n'
	     << indent_writer::iw1
	     << "mcos_threadattr_setlcid (&attr, MCOS_LCID_ANY);\n"
	     << indent_writer::iw1
	     << "mcos_threadattr_setpriority (&attr, LCORE_PRIORITY);\n";

	  for (TaskSetList::const_iterator
		 i (c_codewriter.taskset_list ().begin ());
	       i != c_codewriter.taskset_list ().end (); i++)
	    {
	      const TaskSet* ts = *i;

	      for (CspNodeList::const_iterator
		     ci (ts->cspnodelist ().begin ());
		   ci != ts->cspnodelist ().end (); ci++)
		{
		  CspNode* t_node = *ci;

		  std::string my_task_id (mk_task_id (t_node->id ()));

		  std::string my_func_base ("mcos_lcore_task_" + my_task_id);
		  std::string my_stack_size ("MCOS_LCORE_TASK_"
					     + my_task_id
					     + "_STATCK_SIZE");

		  os << '\n'
		     << "#ifdef " << my_stack_size << '\n'
		     << indent_writer::iw1
		     << "mcos_threadattr_setstacksize (&attr, "
		     << my_stack_size
		     <<");\n"
		     << "#else\n"
		     << indent_writer::iw1
		     << "mcos_threadattr_setstacksize "
		        "(&attr, LCORE_STACK_SIZE);\n"
		     << "#endif\n"
		     << indent_writer::iw1
		     << "tid = mcos_thread_create (&attr, "
		     << my_func_base << ", 0);\n"
		     << indent_writer::iw1
		     << "if (tid == MCOS_EPAR || tid == MCOS_ENOMEM)\n"
		     << indent_writer::iw2
		     << "return;\n"
		     << indent_writer::iw1
		     << "tids[num++] = tid;\n";

		  // CSRの場合はスレッドIDをCSR管理用の変数に設定
		  if (enable_multirate && enable_csr
		      && (t_node->info () & CSP_NODE_INFO_MULTIRATE_HEAD))
		    os << indent_writer::iw1
		       << "tid__task_" << my_task_id << " = tid;\n";
		}
	    }
	}

      // CSRの場合、同じレートの先頭タスクにメッセージを送信するスレッ
      // ドを登録
      if (enable_multirate && enable_csr && csr_task.size ())
	{
	  for (csr_task_T::iterator i (csr_task.begin ());
	       i != csr_task.end (); i++)
	    {
	      os << '\n'
		 << indent_writer::iw1
		 << "csr_ent.entry = mcos_csr_thread_rate_"
		 << i->first <<";\n"
		 << indent_writer::iw1
		 << "csr_ent.cycle = " << i->first << ";\n"
		 << indent_writer::iw1
		 << "csr_ent.lcid = MCOS_LCID_ANY;\n"
		 << indent_writer::iw1
		 << "csr_ent.stacksize = 0;\n"
		 << indent_writer::iw1
		 << "csr_ent.arg = 0;\n"
		 << indent_writer::iw1
		 << "mcos_cyclic_create (&csr_ent);\n";
	    }
	}

      // スレッドの起動
      os << '\n'
	 << indent_writer::iw1
	 << "for (i = 0; i < num; i++) {\n"
	 << indent_writer::iw2
	 << "mcos_thread_start (tids[i], 0);\n"
	 << indent_writer::iw1
	 << "}\n";

      os << "}\n";

      // CSRでコントロールされる、同じレートの先頭タスクにメッセージを
      // 送信するスレッドの関数を生成する
      // 生成される関数はメッセージを、非同期送信するだけである
      if (enable_multirate && enable_csr && csr_task.size ())
	{
	  for (csr_task_T::const_iterator i (csr_task.begin ());
	       i != csr_task.end (); i++)
	    {
	      os << '\n'
		 << "void mcos_csr_thread_rate_" << i->first
		 << "(mcos_id_t tid, uintptr_t arg)\n"
		 << "{\n"
		 << indent_writer::iw1
		 << "uint32_t task__msg_data = 0;\n"
		 << '\n';

	      const CspNodeList& v = i->second;
	      for (CspNodeList::const_iterator i (v.begin ());
		   i != v.end (); i++)
		{
		  CspNode* node = *i;
		  std::string tmp;
		  if (node->core () == -1)
		    {
		      std::string my_task_id (mk_task_id (node->id ()));
		      tmp = "tid__task_";
		      tmp += my_task_id;
		    }
		  else
		    {
		      std::ostringstream oss;
		      oss << "tid__core_" << node->core ();
		      tmp = oss.str ();
		    }

		  os
		    << indent_writer::iw1
		    << "(void)mcos_message_send(" << tmp
		    << ", &task__msg_data, sizeof task__msg_data, "
		    << "MCOS_MSG_SEND_ASYNC);\n";
		}

	      os << "}\n";
	    }
	}
    }

    /// \brief eMCOSコード生成関数の本体
    ///
    /// \param[in] os 出力ストリーム
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// eMCOSのコードをストリームに出力する
    ///
    /// \sa mcos_generate_code(const char *mcos_out)
    /// \sa mcos_correct_task()
    /// \sa mcos_generate_preamble()
    /// \sa mcos_generate_core_task()
    /// \sa mcos_generate_lcore_task()
    /// \sa mcos_generate_threads()
    int
    mcos_generate_code (std::ostream& os)
    {
      int error = -1;

      // eMCOS用タスク関連情報のセットアップ
      mcos_correct_task ();

      // コア割り当てされたタスクのリスト取得
      const TaskSetList&
	core_assign = c_codewriter.core_assigned_taskset_list ();

      // 生成ファイルの先頭にタスクのコード以外を出力
      if (mcos_generate_preamble (os) == -1)
	goto out;

      // タスク関数の出力
      os << '\n'
	 << "/*\n"
	 << " * Tasks\n"
	 << " */\n";

      // 最初にコア割り当てされたタスク関数を出力
      for (size_t i = 0; i < core_assign.size (); i++)
	{
	  if (core_assign[i]->cspnodelist ().size () == 0)
	    continue;

	  if (mcos_generate_core_task (os, i))
	    goto out;
	}

      // コア割り当てされていないタスク関数を出力
      for (TaskSetList::const_iterator
	     tsi = c_codewriter.taskset_list ().begin ();
	   tsi != c_codewriter.taskset_list ().end (); tsi++)
	{
	  if (mcos_generate_lcore_task (os, *tsi))
	    goto out;
	}

      // eMCOSの create_init_threads() を生成
      mcos_generate_threads (os);

      error = 0;

    out:
      return error;
    }

    /// \brief eMCOS用のコード生成を行う
    ///
    /// \param[in] mcos_out eMCOS用コードの出力ファイル
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// 正規表現の準備とファイルオープン後、
    /// eMCOSコード生成関数の本体を呼び出す
    ///
    /// \sa mcos_generate_code(std::ostream& os)
    int
    mcos_generate_code (const char *mcos_out)
    {
      int error = -1;
      std::ofstream ofs;
      std::ostream* os;

      if (!prepare_pulse_generator_regex ())
	return -1;

      if (!make_dsmem_regpat ())
	return -1;

      if (mcos_out)
	{
	  ofs.open (mcos_out);
	  if (!ofs)
	    {
	      std::cerr << mcos_out << ": Couldn't write: " << strerror (errno)
			<< std::endl;
	      return -1;
	    }
	  os = &ofs;
	}
      else
	{
	  mcos_out = "<stdout>";
	  os = &std::cout;
	}

      os->exceptions ( std::ostream::failbit | std::ostream::badbit );

      try
	{
	  error = mcos_generate_code (*os);
	}
      catch (std::ostream::failure e)
	{
	  std::cerr << mcos_out << ": " << e.what () << std::endl;
	  return -1;
	}

      return error;
    }

    /// @}

    /// @}

    /// @}
  }
}
