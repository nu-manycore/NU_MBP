// -*- mode: c++; coding: utf-8-unix; -*-

/// \file arch_memchan.cxx
///
/// \brief チャネルをメモリに配置するアーキテクチャ用コード生成


#include "csp_translator.h"

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup ARCH_MEMCHAN
    ///
    /// @{

    /// \brief ダブルバッファの面数
    ///
    /// ダブルバッファだから2面だが、将来の対応に備え、面数は念のためマ
    /// クロ化しておく
#define DOUBLEBUFFER_PLANE	2

    // assertを出してよいかどうか
    int assert_output_enable = 1;

    // S-MBPモード
    bool smbp_mode = false;

    // 変数または構造体のプレフィクスの const な string
    const std::string ch_member_prefix = ARCH_MEMCH_MEMBER_PREFIX;

    // チャネル操作用マクロ生成
    void
    generate_channel_macros (std::ostream& os, const std::string& var_prefix)
    {
      os << '\n'
	 << "/*\n"
	 << " * Channel Macros\n"
	 << " */\n";
      os << "#define CH__VEC__CONCAT(A,B)\tA##B\n";
      os << "#define CH__VEC_CONCAT(A,B)\tCH__VEC__CONCAT(A,B)\n";
      os << "#define CH__VEC_NAME(N)\t\t"
	 << "CH__VEC_CONCAT(" << var_prefix << "ch__vec_,N)\n";
      os << "#define CH__VEC_ELEMENT_TYPE\tunsigned long\n";
      os << "#define CH__VEC_ELEMENT_BITS\t"
	 << "(sizeof (CH__VEC_ELEMENT_TYPE) * 8)\n";
      os << "#define CH__VEC_ELEMENT_SIZE(S)\t\\\n"
	 << indent_writer::iw1
	 << "(((S)+CH__VEC_ELEMENT_BITS-1)/CH__VEC_ELEMENT_BITS)\n";
      os << "#define CH__VEC_ELEMENT_0\t((CH__VEC_ELEMENT_TYPE)0)\n";
      os << "#define CH__VEC_ELEMENT_1\t((CH__VEC_ELEMENT_TYPE)1)\n";
      os << "#define CH__VEC(N,S)\t\\\n"
	 << indent_writer::iw1
	 << "CH__VEC_ELEMENT_TYPE " << var_prefix
	 << "CH__VEC_NAME(N)[CH__VEC_ELEMENT_SIZE(S)] = {0,}\n";
      os << "#define CH__VEC_INIT(N)\t\tmemset("
	 << var_prefix << "CH__VEC_NAME(N), 0, sizeof "
	 << var_prefix << "CH__VEC_NAME(N))\n";
      os << "#define CH__BITCHECK(N,B)\t"
	 << "ch__bitcheck(" << var_prefix << "CH__VEC_NAME(N),B)\n";
      os << "#define CH__BITSET(N,B)\t\t"
	 << "ch__bitset(" << var_prefix << "CH__VEC_NAME(N),B)\n";
      os << "#define CH__COMPLETED(N,S)\t"
	 << "ch__completed(" << var_prefix << "CH__VEC_NAME(N),S)\n";

      if (enable_multirate)
	os << "\n#define CH__TIME_LT(L,R)\t((long)((L)-(R)) < (long)0)\n"
	   << "#define CH__TIME_LE(L,R)\t((long)((L)-(R)) <= (long)0)\n"
	   << "#define CH__SEND_COUNT(N,D,R)\t(((N)-(D)+((R)-1))/R)\n";
    }


    // チャネル操作用inline関数生成
    void
    generate_channel_functions (std::ostream& os)
    {
      os << "\n/* check channle bitvec */\n"
	 << "static __inline__ int\n"
	 << "ch__bitcheck (CH__VEC_ELEMENT_TYPE *v, size_t b)\n"
	 << "{\n"
	 << "  return (v[b/CH__VEC_ELEMENT_BITS]\n"
	 << "          & (CH__VEC_ELEMENT_1 << (b % CH__VEC_ELEMENT_BITS)));\n"
	 << "}\n";
      os << "\n/* set channle bitvec */\n"
	 << "static __inline__ void\n"
	 << "ch__bitset (CH__VEC_ELEMENT_TYPE *v, size_t b)\n"
	 << "{\n"
	 << "  v[b/CH__VEC_ELEMENT_BITS]\n"
	 << "    |= (CH__VEC_ELEMENT_1 << (b % CH__VEC_ELEMENT_BITS));\n"
	 << "}\n";
      os << "\n/* completed channel wait */\n"
	 << "static __inline__ int\n"
	 << "ch__completed (CH__VEC_ELEMENT_TYPE *v, size_t n)\n"
	 << "{\n"
	 << "  const size_t restbits = n % CH__VEC_ELEMENT_BITS;\n"
	 << "  size_t i;\n"
	 << "  size_t e = (n - 1) / CH__VEC_ELEMENT_BITS;\n"
	 << "  CH__VEC_ELEMENT_TYPE const\n"
	 << "    restmask =  ((restbits == 0)\n"
	 << "\t\t ? (~CH__VEC_ELEMENT_0)\n"
	 << "\t\t : ((CH__VEC_ELEMENT_1 << restbits) - 1));\n"
	 << "  for (i = 0; i < e; i++, v++)\n"
	 << "    {\n"
	 << "      if (*v != ~(CH__VEC_ELEMENT_0))\n"
	 << "\treturn 0;\n"
	 << "    }\n"
	 << "  if (*v != restmask)\n"
	 << "    return 0;\n"
	 << "  return 1;\n"
	 << "}\n";
    }


    // イベントが必要なSubSystemが送信するイベントの型とコピー用マクロ
    //
    // TODO: SystemBuilder対応
    //       現状、イベントを渡すことは無くなっているので、不要だろう。
    void
    generate_sybsystem_state_and_macros (std::ostream& os,
					 const std::string& unknown_state)
    {
      size_t t = 0;

      const optional_refstring&
	opt_state_storage = c_codewriter.state_storage ();

      const std::string&
	state_storage = (opt_state_storage
			 ? *opt_state_storage : unknown_state);

      for (CspNodeList::const_iterator
	     i (CspNode::csp_nodelist ().begin ());
	   i != CspNode::csp_nodelist ().end (); i++)
	{
	  CspNode* node = *i;

	  if (!node->need_subsystem_event ())
	    continue;

	  std::string my_task_id (mk_task_id (node->id ()));

	  if (t == 0)
	    {
	      os << '\n'
		 << "/*\n"
		 << " * Enabled/Triggered SubSystem states (Event)\n"
		 << " */\n";
	      t = 1;
	    }

	  std::string tmp;
	  const std::string& name = node->get_block_name (tmp);

	  os << '\n'
	     << "/* Subsystem: " << name << " */\n"
	     << "typedef struct {\n"
	     << indent_writer::iw1
	     << "int subsystem__" << my_task_id << "_state;\n";

	  for (ParamList::const_iterator i (node->state ().begin ());
	       i != node->state ().end (); i++)
	    {
	      const var_defn_writer& vdw = *i;
	      os << vdw.set_indent (1) << ";\n";
	    }

	  os << "} task_" << my_task_id << "_" << state_storage << ";\n";

	  // copyin/copyout macros

	  os << "#define COPY_SUBSYSTEM_STATE_" << my_task_id
	     << "(TARGET,LOCAL)\t\\\n"
	     << indent_writer::iw1 << "do {\\\n";

	  for (ParamList::const_iterator i (node->state ().begin ());
	       i != node->state ().end (); i++)
	    {
	      const var_defn_writer& vdw = *i;
	      const std::string& vname = vdw.name ();

	      if (vdw.size ())
		os << indent_writer::iw2 << "memcpy((TARGET)." << vname << ", "
		   << "(LOCAL)." << vname << ", "
		   << "sizeof ((LOCAL)." << vname << "));\\\n";
	      else
		os << indent_writer::iw2 << "(TARGET)." << vname
		   << " = (LOCAL)." << vname << ";\\\n";
	    }

	  os << indent_writer::iw1 << "} while (0)\n";
	}
    }


    // CspNode 間の通信チャネルの型
    void
    generate_channel_types (std::ostream& os, const std::string& unknown_state)
    {
      os << '\n'
	 << "/*\n"
	 << " * Channels\n"
	 << " */\n";

      const optional_refstring&
	opt_state_storage = c_codewriter.state_storage ();

      const std::string&
	state_storage = (opt_state_storage
			 ? *opt_state_storage : unknown_state);

      /* i/o channel */
      for (CspNodeList::const_iterator i (CspNode::csp_nodelist ().begin ());
	   i != CspNode::csp_nodelist ().end (); i++)
	{
	  CspNode *cn_tail = *i;

	  std::string my_task_id (mk_task_id (cn_tail->id ()));

	  std::string tmp;
	  const std::string& t_name = cn_tail->get_block_name (tmp);

	  // CspNode の出力チャネルの内容を出力する。対抗する入力チャネ
	  // ルも同じものになる
	  for (CspEdgeList::const_iterator
		 ei (cn_tail->forw_edges ().begin ());
	       ei != cn_tail->forw_edges ().end (); ei++)
	    {
	      const CspEdge& fedge = *ei;
	      CspNode* cn_head = fedge.node ();

	      // 同じ TaskSet に属す場合、チャネルは不要
	      if (cn_tail->taskset () == cn_head->taskset ())
		continue;

	      // SWコア同士以外であれば、チャネルを生成しない
	      if (smbp_mode
		  && (cn_tail->coretype () == HARDWARE_CORE
		      || cn_head->coretype () == HARDWARE_CORE))
		continue;

	      std::string tmp2;
	      const std::string& h_name = cn_head->get_block_name (tmp2);

	      std::string xx_task_id (mk_task_id (cn_head->id ()));
	      std::string chan (my_task_id + "_" + xx_task_id);

	      Rate_rate_T rate_x = cn_tail->rate ().rate ();
	      Rate_rate_T rate_y = cn_head->rate ().rate ();
	      Rate_offset_T offset_x = cn_tail->rate ().offset ();
	      Rate_offset_T offset_y = cn_head->rate ().offset ();

	      bool multirate_code;

	      if ((fedge.info () & CSP_EDGE_INFO_INITIAL_VALUE) == 0)
		multirate_code = enable_multirate;
	      else
		{
		  multirate_code = false;
		  rate_y = rate_x; // same rate
		}

	      // チャネルに時刻が必要なケースは、
	      // ・異なるレートの場合
	      // ・開始オフセットが異なる場合
	      // ・レートが変わる場合
	      bool
		change_rate = (multirate_code
			       && ((cn_tail->info ()
				    & CSP_NODE_INFO_CHANGE_RATE)
				   || (cn_head->info ()
				       & CSP_NODE_INFO_CHANGE_RATE)
				   || rate_x != rate_y
				   || offset_x != offset_y));

	      os << '\n'
		 << "/* From: " << t_name << '\n'
		 << "   To: " << h_name;
	      if (change_rate)
		{
		  os << "\n"
		     << "    [" << rate_x << "] -> [" << rate_y << "]";
		}
	      os << " */\n"
		 << "typedef struct {\n";

	      // ダブルバッファ対応
	      bool double_buffer = false;
	      if (multirate_code
		  && (fedge.info () & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0)
		double_buffer = true;

	      // チャネルデータの有効回数として使用する
	      // ダブルバッファの場合は面ごとに必要である
	      if (double_buffer)
		os << indent_writer::iw1
		   << "int flag__" << chan
		   << "[" << DOUBLEBUFFER_PLANE << "];\n";
	      else
		os << indent_writer::iw1
		   << "int flag__" << chan << ";\n";

	      // CspNode の実行時間
	      if (change_rate)
		os << indent_writer::iw1
		   << "unsigned long time__" << chan << ";\n";

	      // 相手側のレート
	      if (multirate_code
		  && (cn_tail->info () & CSP_NODE_INFO_CHANGE_RATE))
		os << indent_writer::iw1
		   << "unsigned long rate__" << xx_task_id << ";\n";

	      // 同期イベントか非同期入力の場合のタイムスタンプ
	      if (multirate_code
		  && (fedge.info ()
		      & (CSP_EDGE_INFO_SYNC_EVENT|CSP_EDGE_INFO_ASYNC_INPUT)))
		{
		  // ダブルバッファはRateTransitionだけのはず
		  assert (!double_buffer);
		  os << indent_writer::iw1
		     << "unsigned long timestamp__" << my_task_id << ";\n";
		}

	      // イベントが必要な SubSystem 用の状態変数
	      if (cn_tail->need_subsystem_event ()
		  && (fedge.info ()
		      & CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT) != 0)
		{
		  // ダブルバッファはRateTransitionだけのはず
		  assert (!double_buffer);
		  os << indent_writer::iw1
		     << "task_" << my_task_id << '_' << state_storage
		     << " state_" << my_task_id << '_' << state_storage
		     << ";\n";
		}

	      // ダブルバッファリングの入出力サイド
	      if (double_buffer)
		os << indent_writer::iw1
		   << "int input_buffer_side;\n"
		   << indent_writer::iw1
		   << "int output_buffer_side;\n";

	      // 伝播するブロックシグナル
	      const optional_refstring&
		opt_signal_storage = c_codewriter.signal_storage ();
	      const PropagateVarList& povl = fedge.var ().order ();

	      if (opt_signal_storage)
		{
		  bool first = true;
		  for (PropagateVarList::const_iterator i (povl.begin ());
		       i != povl.end (); i++)
		    {
		      const PropagateVar* pv = *i;
		      const CspVar* cv = *pv;

		      if (cv->mode () & VAR_MODE_NODEF_VAR)
			continue;

		      if ((cv->mode () & VAR_MODE_BIOSIG) == 0)
			continue;

		      if (first)
			{
			  os << indent_writer::iw1
			     << "struct {\n";
			  first = false;
			}

		      const var_defn_writer& vdw = *cv;
		      if (double_buffer)
			os << make_doublebuffer (vdw, DOUBLEBUFFER_PLANE)
			  .set_indent (2)
			   << ";\n";
		      else
			os << vdw.set_indent (2) << ";\n";
		    }

		  if (!first)
		    os << indent_writer::iw1
		       << "} " ARCH_MEMCH_MEMBER_PREFIX
		       << *opt_signal_storage << ";\n";
		}

	      // 伝播する外部入力変数
	      const optional_refstring&
		opt_extin_storage = c_codewriter.extin_storage ();
	      if (opt_extin_storage)
		{
		  bool first = true;
		  for (PropagateVarList::const_iterator i (povl.begin ());
		       i != povl.end (); i++)
		    {
		      const PropagateVar* pv = *i;
		      const CspVar* cv = *pv;

		      if (cv->mode () & VAR_MODE_NODEF_VAR)
			continue;

		      const optional_refstring&
			storage = cv->storage ();

		      if ((cv->mode () & VAR_MODE_EXTIN) == 0
			  || !storage
			  || *storage != *opt_extin_storage)
			continue;

		      if (first)
			{
			  os << indent_writer::iw1
			     << "struct {\n";
			  first = false;
			}

		      const var_defn_writer& vdw = *cv;
		      if (double_buffer)
			os << make_doublebuffer (vdw, DOUBLEBUFFER_PLANE)
			  .set_indent (2)
			   << ";\n";
		      else
			os << vdw.set_indent (2) << ";\n";
		    }

		  if (!first)
		    os << indent_writer::iw1
		       << "} " ARCH_MEMCH_MEMBER_PREFIX
		       << *opt_extin_storage << ";\n";
		}

	      // 伝播する外部出力変数
	      const optional_refstring&
		opt_extout_storage = c_codewriter.extout_storage ();
	      if (opt_extout_storage)
		{
		  bool first = true;
		  for (PropagateVarList::const_iterator i (povl.begin ());
		       i != povl.end (); i++)
		    {
		      const PropagateVar* pv = *i;
		      const CspVar* cv = *pv;

		      if (cv->mode () & VAR_MODE_NODEF_VAR)
			continue;

		      const optional_refstring&
			storage = cv->storage ();

		      if ((cv->mode () & VAR_MODE_EXTOUT) == 0
			  || !storage
			  || *storage != *opt_extout_storage)
			continue;

		      if (first)
			{
			  os << indent_writer::iw1
			     << "struct {\n";
			  first = false;
			}

		      const var_defn_writer& vdw = *cv;
		      if (double_buffer)
			os << make_doublebuffer (vdw, DOUBLEBUFFER_PLANE)
			  .set_indent (2)
			   << ";\n";
		      else
			os << vdw.set_indent (2) << ";\n";
		    }

		  if (!first)
		    os << indent_writer::iw1
		       << "} " ARCH_MEMCH_MEMBER_PREFIX
		       << *opt_extin_storage << ";\n";
		}

	      // 伝播する変数
	      for (PropagateVarList::const_iterator i (povl.begin ());
		   i != povl.end (); i++)
		{
		  const PropagateVar* pv = *i;
		  const CspVar* cv = *pv;

		  if (cv->mode () & VAR_MODE_NODEF_VAR)
		    continue;

		  const optional_refstring&
		    storage = cv->storage ();

		  if ((cv->mode () & VAR_MODE_BIOSIG)
		      || ((cv->mode () & VAR_MODE_EXTOUT)
			  && (opt_extout_storage
			      && storage
			      && *opt_extout_storage == *storage))
		      || ((cv->mode () & VAR_MODE_EXTIN)
			  && (opt_extin_storage
			      && storage
			      && *opt_extin_storage == *storage)))
		    continue;

		  const var_name_wrapper < CspVar >
		    vnw (*cv, ch_member_prefix);
		  const var_defn_writer& vnw_vdw = vnw;

		  if (double_buffer)
		    os << make_doublebuffer (vnw_vdw, DOUBLEBUFFER_PLANE)
		      .set_indent (1)
		       << ";\n";
		  else
		    os << vnw_vdw.set_indent (1) << ";\n";
		}

	      os << "} TYPE_" << block_name_prefix << chan << ";\n";

	      os << "TYPE_" << block_name_prefix << chan
		 << " CH_" << block_name_prefix << chan << ";\n";
	    }
	}
    }


    // 出力チャネルに変数を設定するコードの出力
    void
    generate_task_output_values (std::ostream& os, const CspEdge& ce,
				 const std::string& chan, int ind)
    {
      const indent_writer iw0 (ind);
      const indent_writer iw1 (ind + 1);

      bool double_buffer = (ce.info () & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0;

      const std::string ch_prefix ("CH_" + block_name_prefix + chan);
      const std::string plane (ch_prefix + ".output_buffer_side");

      // 出力変数をチャネルへコピーするコードを生成
      const PropagateVarList& pvlist = ce.var ().order ();
      for (PropagateVarList::const_iterator i (pvlist.begin ());
	   i != pvlist.end (); i++)
	{
	  const PropagateVar* pv = *i;
	  const CspVar* cv = *pv;
	  const var_rhs_writer& vrw = *cv;

	  if (cv->mode () & VAR_MODE_NODEF_VAR)
	    continue;

	  const var_name_wrapper < CspVar > vnw (*cv, ch_member_prefix);
	  const var_rhs_writer& vnw_vrw = vnw;

	  var_doublebuffer_writer < var_rhs_writer, std::string >
	    vdbw = make_doublebuffer (vnw_vrw, plane);

	  if (!bug_partial_array && pv->is_partial_array ())
	    {
	      for (PartialArrayList::const_iterator
		     pai (pv->partial_array ().begin ());
		   pai != pv->partial_array ().end (); pai++)
		{
		  const PartialArray& pa = *pai;
		  if (pa.size () > 1)
		    {
		      os << iw0 << "memcpy("
			 << '&' << ch_prefix << '.';
		      if (double_buffer)
			os << vdbw.set_index (pa.start ());
		      else
			os << vnw_vrw.set_index (pa.start ());
		      os << ",\n"
			 << vrw.set_index (pa.start ()).pointer() << ",\n"
			 << iw1
			 << iw1
			 << pa.size () << " * "
			 << "sizeof " << vrw.set_index (pa.start ())
			 << ");\n";
		    }
		  else
		    {
		      os << iw0
			 << ch_prefix << '.';
		      if (double_buffer)
			os << vdbw.set_index (pa.start ());
		      else
			os << vnw_vrw.set_index (pa.start ());
		      os << " = "
			 << vrw.set_index (pa.start ())
			 << ";\n";
		    }
		}
	    }
	  else if (cv->size ())
	    {
	      os << iw0 << "memcpy(" << ch_prefix << '.';
	      if (double_buffer)
		os << vdbw;
	      else
		os << vnw_vrw;
	      os << ",\n"
		 << iw1 << vrw << ",\n"
		 << iw1 << "sizeof " << vrw << ");\n";
	    }
	  else
	    {
	      os << iw0
		 << ch_prefix << '.';
	      if (double_buffer)
		os << vdbw;
	      else
		os << vnw_vrw;
	      os << " = "
		 << vrw << ";\n";
	    }
	}

      os << iw0 << "CH__SYNCM();\n";
    }


    // 出力チャネル毎のサンプル時間のオフセット設定
    void
    generate_task_set_offset (std::ostream& os, CspNode* node, int ind,
			      bool& first)
    {
      const indent_writer iw0 (ind);
      const indent_writer iw1 (ind + 1);

      std::string my_task_id (mk_task_id (node->id ()));

      const Rate_rate_T rate_x = node->rate ().rate ();
      const Rate_offset_T offset_x = node->rate ().offset ();

      // CspNode の後段へのエッジに対し、出力を行うコードを生成する
      for (CspEdgeList::iterator i (node->forw_edges ().begin ());
	   i != node->forw_edges ().end (); i++)
	{
	  CspEdge& ce = *i;
	  CspNode* xx_node = ce.node ();

	  // SystemBuilderの対応では初期値送信やオフセットの対応をここ
	  // では行わない。
	  //
	  // 必要な時刻、ダブルバッファの面情報は、
	  // SMBP::generate_task_definitions() で行う。
	  if (smbp_mode
	      && (node->coretype () == HARDWARE_CORE
		  || xx_node->coretype () == HARDWARE_CORE))
	    continue;

	  const Rate_offset_T offset_y = xx_node->rate ().offset ();
	  const Rate_rate_T rate_y = xx_node->rate ().rate ();

	  // 同じ TaskSet のものなら不要
	  if (node->taskset () == xx_node->taskset ())
	    continue;

	  // ダブルバッファを必要する場合。
	  // これはRateTransitionの単位遅延の場合である。
	  bool double_buffer = (ce.info () & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0;

	  // 0次ホールドで、ともにオフセットが0(開始時刻は0)ならば不要
	  if (!double_buffer && offset_x == 0 && offset_y == 0)
	    continue;

	  // 0次ホールドで同じ時刻同士なら不要
	  if (!double_buffer
	      && (node->info () & CSP_NODE_INFO_CHANGE_RATE) == 0
	      && (xx_node->info () & CSP_NODE_INFO_CHANGE_RATE) == 0
	      && rate_x == rate_y && offset_x == offset_y)
	    continue;

	  const std::string xx_task_id (mk_task_id (xx_node->id ()));
	  const std::string chan (my_task_id + "_" + xx_task_id);
	  const std::string ch_prefix ("CH_" + block_name_prefix + chan);

	  if (first)
	    {
	      os << '\n'
		 << iw0 << "/*\n"
		 << iw0 << " * setup SampleTime Offset and Double buffer\n"
		 << iw0 << " */\n";
	      first = false;
	    }

	  // 初期値送信が必要な場合は、
	  //   1. ダブルバッファかつ2回めの起床前に後段が動き出すとき
	  //   2. シングルバッファで後段が自分より前に動き出すとき
	  bool need_initial_value = (double_buffer
				     ? (offset_y < offset_x + rate_x)
				     : (offset_y < offset_x));

	  // 初期値を書き込むサイドを最初の面(これは初期値書き込みの関
	  // 数を使用するため)、読み込む面も最初の面にする
	  if (double_buffer)
	    {
	      os << '\n'
		 << iw0 << "/* setup double buffer planes */\n"
		 << iw0 << ch_prefix << ".output_buffer_side = 0;\n"
		 << iw0 << ch_prefix << ".input_buffer_side = 0;\n";
	    }

	  // チャネルの初期値設定
	  if (need_initial_value)
	    {
	      os << '\n'
		 << iw0 << "/* preload initial values */\n";
	      generate_task_output_values (os, ce, chan, ind);
	    }

	  // 後段の開始時刻が自分より早い場合
	  if (need_initial_value)
	    {
	      os << iw0 << '{' << '\n'
		 << iw1 << "unsigned long ch__sendcount;\n";

	      // 初期値の有効な時刻は、開始オフセットまで。ただし、ダブ
	      // ルバッファの場合は、さらに単位遅延分を加算する
	      unsigned long valid_time = offset_x;
	      if (double_buffer)
		valid_time += rate_x;

	      // チャネルの送信回数設定
	      os << iw1 << "ch__sendcount = CH__SEND_COUNT("
		 << valid_time << ", " << offset_y << ", " << rate_y << ");\n"
		 << iw1 << ch_prefix << ".flag__" << chan;
	      if (double_buffer)
		os << "[0]";
	      os << " = ch__sendcount;\n";

	      // 次回送信時刻設定は、開始オフセットと有効回数分の時間を
	      // 加算した後である。
	      os << iw1 << ch_prefix << ".time__" << chan << " = ";
	      if (offset_y)
		os << offset_y << " + ";
	      os << "ch__sendcount * " << rate_y << ";\n";
	    }
	  else if (offset_y)
	    {
	      // 初期値送信しない場合は、次回送信時刻は相手の開始オフセッ
	      // トである
	      os << iw0 << ch_prefix
		 << ".time__" << chan << " = " << offset_y << ";\n";
	    }

	  if (need_initial_value)
	    {
	      // ダブルバッファの場合、初期値送信を行った情報を設定し、
	      // 書き込み面を変更
	      if (double_buffer)
		{
		  ce.info () |= CSP_EDGE_INFO_DOUBLE_BUFFER_INIT_SEND;
		  os << iw1 << ch_prefix << ".output_buffer_side = 1;\n";
		}

	      // 初期値送信を完了する
	      os << iw0 << "}\n"
		 << iw0 << "CH__EVENT(TASK_" << my_task_id
		 << ",IN_" << xx_task_id << ");\n";
	    }
	}
    }


    // 共有するDataStoreの出力
    void
    generate_datastore (std::ostream& os, const std::string& prefix)
    {
      const AutoStorage*
	dsm_as = c_codewriter.dsmem ()
	.get_storage (c_codewriter.state_storage ());

      if (dsm_as)
	{
	  os << '\n'
	     << "/*\n"
	     << " * DataStoreMemories\n"
	     << " */\n";

	  std::string dsm_storage (prefix + dsm_as->storage ());
	  os << dsm_as->set_indent (std::string (""), dsm_storage, 0);
	}
    }


    // 入出力の待ち合わせを行うためのチェック用ベクタを出力する
    void
    generate_task_channel_vector (std::ostream& os, CspNode* node,
				  const std::string& task_id, bool& first)
    {
      size_t inch_cnt;
      size_t outch_cnt;

      if (smbp_mode)
	{
	  // チェックを要するチャネルはSW-SWの場合のみ
	  outch_cnt = 0;
	  for (CspEdgeList::const_iterator ei (node->forw_edges ().begin ());
	       ei != node->forw_edges ().end (); ei++)
	    {
	      const CspEdge& fe = *ei;
	      CspNode* cn_head = fe.node ();
	      if (cn_head->coretype () == SOFTWARE_CORE
		  && node->coretype () == SOFTWARE_CORE)
		outch_cnt++;
	    }

	  inch_cnt = 0;
	  for (CspEdgeList::const_iterator ei (node->back_edges ().begin ());
	       ei != node->back_edges ().end (); ei++)
	    {
	      const CspEdge& be = *ei;
	      CspNode* cn_tail = be.node ();
	      if (cn_tail->coretype () == SOFTWARE_CORE
		  && node->coretype () == SOFTWARE_CORE)
		inch_cnt++;
	    }
	}
      else
	{
	  inch_cnt = node->back_edges ().size ();
	  outch_cnt = node->forw_edges ().size ();
	}

      if (inch_cnt)
	{
	  if (first)
	    first = false;
	  else
	    os << '\n';
	}

      if (inch_cnt > 1)
	os << indent_writer (1) << "/* input channel bit vector */\n"
	   << indent_writer (1) << "CH__VEC(IN_" << task_id
	   << "," << inch_cnt << ");\n";
      else if (inch_cnt == 1)
	os << indent_writer (1) << "/* omit input channel bit vector */\n"
	   << indent_writer (1) << "/* CH__VEC(IN_" << task_id << ",1); */\n";

      if (outch_cnt)
	{
	  if (first)
	    first = false;
	  else
	    os << '\n';
	}

      if (outch_cnt > 1)
	os << indent_writer (1) << "/* output channel bit vector */\n"
	   << indent_writer (1) << "CH__VEC(OUT_" << task_id
	   << "," << outch_cnt << ");\n";
      else if (outch_cnt == 1)
	os << indent_writer (1) << "/* omit output channel bit vector */\n"
	   << indent_writer (1) << "/* CH__VEC(OUT_" << task_id
	   << ",1); */\n";
    }


    // CspNode のチャネルの入力処理を出力する
    void
    generate_task_wait_input_channel (std::ostream& os, CspNode* node,
				      bool core, int ind,
				      channel_io_mode input_mode,
				      const std::string& unknown_state)
    {
      int ch_cnt = 0;
      const indent_writer iw0 (ind);
      const indent_writer iw1 (ind + 1);
      const indent_writer iw2 (ind + 2);
      const indent_writer iw3 (ind + 3);
      const indent_writer iw4 (ind + 4);

      std::string my_task_id (mk_task_id (node->id ()));

      // CspNode の前段からのエッジに対し、入力が必要か調べる
      for (CspEdgeList::const_iterator i (node->back_edges ().begin ());
	   i != node->back_edges ().end (); i++)
	{
	  const CspEdge& ce = *i;

	  // エッジに、Update部への入力がある場合でUpdate部の入力を生成
	  // する指定(input_modeがCH_IO_MODE_UPDATA)でないならここで処
	  // 理しない。
	  //
	  // エッジに、初期値の送受信がある場合で、初期値の入力を生成す
	  // る指定(input_modeがCH_IO_MODE_INIT)でないならここで処理し
	  // ない
	  //
	  // エッジがUpdateでも初期値でもない場合で、タスクの入力を生成
	  // する指定(input_modeがCH_IO_MODE_TASK)でないならここで処理
	  // しない
	  if ((ce.info ()
	       & CSP_EDGE_INFO_HAS_UPDATE) == CSP_EDGE_INFO_HAS_UPDATE)
	    {
	      if (input_mode != CH_IO_MODE_UPDATE)
		continue;
	    }
	  else if ((ce.info () & CSP_EDGE_INFO_INITIAL_VALUE)
		   == CSP_EDGE_INFO_INITIAL_VALUE)
	    {
	      if (input_mode != CH_IO_MODE_INIT)
		continue;
	    }
	  else if (input_mode != CH_IO_MODE_TASK)
	    continue;

	  // SystemBuilderの対応で、SW-SW以外はカウントしない
	  if (smbp_mode
	      && (node->coretype () == HARDWARE_CORE
		  || ce.node ()->coretype () == HARDWARE_CORE))
	    continue;

	  // 異なる TaskSet 間のエッジであれば、チャネルが必要なのでカ
	  // ウントする
	  if (node->taskset () != ce.node ()->taskset ())
	    ch_cnt++;
	}

      // チャネルが不要ならば、入力を行うコードを出力しない
      if (ch_cnt == 0)
	return;

      // チャネルが1を越えていれば入力用ビットベクタがあるので、初期化
      // するコードを出力する
      if (ch_cnt > 1)
	os << '\n'
	   << iw0 << "/* clear input channel bit vector */\n"
	   << iw0 << "CH__VEC_INIT(IN_" << my_task_id << ");\n";

      // チャネルの入力ループ生成
      os << '\n' << iw0;

      if (input_mode == CH_IO_MODE_INIT)
	os << "/* wait input channel (initial values) */\n";
      else if (input_mode == CH_IO_MODE_UPDATE)
	os << "/* wait input channel (update) */\n";
      else
	os << "/* wait input channel */\n";

      os << iw0 << "while (1) {\n";

      int ch_index = 0;
      bool first = true;

      // このCspNodeに、イベントを送信するSubSystemのCspNodeを取得
      // イベントがなければNULL
      CspNode* cn_ev = node->event_subsystem ();

      // CspNode の前段からのエッジに対し、入力を行うコードを生成する
      for (CspEdgeList::const_iterator i (node->back_edges ().begin ());
	   i != node->back_edges ().end (); i++)
	{
	  const CspEdge& ce = *i;

	  // エッジに、Update部への入力がある場合、それ以外の変数の入力
	  // がなく Update部の入力を生成する指定(input_modeが
	  // CH_IO_MODE_UPDATE)でないらば、ここでは出力しない。
	  // 通常の入力の場合は、Update部の入力を生成する指定
	  // (input_modeがCH_IO_MODE_UPDATE)であれば出力しない。
	  if ((ce.info ()
	       & CSP_EDGE_INFO_HAS_UPDATE) == CSP_EDGE_INFO_HAS_UPDATE)
	    {
	      if (input_mode != CH_IO_MODE_UPDATE)
		continue;
	    }
	  else if ((ce.info () & CSP_EDGE_INFO_INITIAL_VALUE)
		   == CSP_EDGE_INFO_INITIAL_VALUE)
	    {
	      if (input_mode != CH_IO_MODE_INIT)
		continue;
	    }
	  else if (input_mode != CH_IO_MODE_TASK)
	    continue;

	  // SystemBuilderの対応で、SW-SW以外は出力しない
	  if (smbp_mode
	      && (node->coretype () == HARDWARE_CORE
		  || ce.node ()->coretype () == HARDWARE_CORE))
	    continue;

	  CspNode* xx_node = ce.node ();
	  std::string xx_task_id (mk_task_id (xx_node->id ()));
	  std::string chan (xx_task_id + "_" + my_task_id);

	  std::string tmp;
	  const std::string& t_name = xx_node->get_block_name (tmp);

	  if (first)
	    first = false;
	  else
	    os << '\n';

	  // TaskSet が同じであればチャネルは不要
	  if (node->taskset () == xx_node->taskset ())
	    {
	      os << iw1 << "/* skip local input CH_" << chan
		 << ": " << t_name << " */\n";
	      continue;
	    }

	  bool multirate_code = false;
	  bool async_input = false;
	  bool sync_event = false;
	  if (input_mode != CH_IO_MODE_INIT)
	    {
	      multirate_code = (enable_multirate != 0);
	      if (ce.info () & CSP_EDGE_INFO_ASYNC_INPUT)
		async_input = true;
	      if (ce.info () & CSP_EDGE_INFO_SYNC_EVENT)
		sync_event = true;
	    }

	  bool double_buffer = (ce.info () & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0;
	  const std::string ch_prefix ("CH_" + block_name_prefix + chan);
	  const std::string plane (ch_prefix + ".input_buffer_side");
	  const std::string input_plane ("ch__side");

	  if (input_mode == CH_IO_MODE_UPDATE)
	    os << iw1 << "/* update";
	  else if (input_mode == CH_IO_MODE_INIT)
	    os << iw1 << "/* initial value";
	  else
	    os << iw1 << "/* input";

	  os << ": " << t_name << " */\n";

	  os << iw1 << "if (";

	  // 入力チャネルが1を越えている場合は入力用ビットベクタの当該
	  // チャネルをチェックするコードを出力
	  if (ch_cnt != 1)
	    os << "CH__BITCHECK(IN_" << my_task_id
	       << "," << ch_index << ") == 0 && ";

	  // 非同期入力では、入力がなければ待たずに当該チャネルの処理を
	  // しないコードを出力する
	  if (multirate_code && async_input)
	    {
	      os << ch_prefix << ".flag__" << chan;
	      if (double_buffer)
		os << '[' << plane << ']';
	      os << " == 0) {\n";
	      os << iw2 << "/* async none */\n";
	      if (ch_cnt == 1)
		os << iw2 << "break;\n";
	      else
		os << iw2 << "goto out_" << chan << ";\n";
	      os << iw1 << "} else if (";

	      // 入力チャネルが1を越えている場合は入力用ビットベクタの
	      // 当該チャネルをチェックするコードを出力
	      if (ch_cnt != 1)
		os << "CH__BITCHECK(IN_" << my_task_id
		   << "," << ch_index << ") == 0 && ";
	    }

	  // フラグの有効性を確認する条件を出力。
	  // マルチレートの場合は1以上、マルチレートでない場合は1である。
	  os << ch_prefix << ".flag__" << chan;
	  if (double_buffer)
	    os << '[' << plane << ']';
	  if (!async_input && !sync_event && multirate_code)
	    {
	      // マルチレートの入力の場合、チャネルのフラグをアトミック
	      // の変更を期待して、自動変数を介してアクセスする
	      os << " > 0) {\n"
		 << iw2 << "int ch__flag;\n";
	    }
	  else
	    os << " == 1) {\n";

	  if (double_buffer)
	    os << iw2 << "int " << input_plane << " = " << plane << ";\n";

	  // 非同期入力の場合は、送信側の時刻が有効か判定する条件を出力
	  // する
	  if (multirate_code && async_input)
	    {
	      os << iw2 << "/* check valid data */\n"
		 << iw2 << "if (CH__TIME_LT(task__time,"
		 << ch_prefix << ".timestamp__" << xx_task_id << ")) {\n"
		 << iw3 << "/* ignore this time */\n";
	      if (ch_cnt == 1)
		os << iw3 << "break;\n";
	      else
		os << iw3 << "goto out_" << chan << ";\n";
	      os << iw2 << "}\n";
	    }

	  const optional_refstring&
	    opt_state_storage = c_codewriter.state_storage ();

	  const std::string&
	    state_storage = (opt_state_storage
			     ? *opt_state_storage : unknown_state);

	  // SubSystemのイベントがある場合、SubSystemの状態をコピーする
	  // コードを生成する
	  if ((ce.info () & CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT) != 0
	      && cn_ev == xx_node)
	    {
	      os << iw2
		 << "subsystem__" << xx_task_id << "_state"
		 << " = "
		 << ch_prefix << ".state_" << xx_task_id << '_'
		 << state_storage << ".subsystem__" << xx_task_id << "_state"
		 << ";\n";
	      os << iw2
		 << "COPY_SUBSYSTEM_STATE_" << xx_task_id << '('
		 << state_storage
		 << ","
		 << ch_prefix << ".state_" << xx_task_id << '_'
		 << state_storage << ");\n";
	    }

	  /// 同期入力の場合、次に入力を行うための実行時刻を、相手側の
	  /// 時刻とレートから算出する
	  if (multirate_code && sync_event)
	    {
	      os << iw2 << "{\n"
		 << iw3 << "unsigned long send__time = "
		 << ch_prefix << ".timestamp__" << xx_task_id << ";\n"
		 << iw3
		 << "unsigned long time__delta = send__time - task__time;\n"
		 << iw3 << "time__delta = (time__delta + ";
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		os << "task__rate_t" << my_task_id;
	      else
		os << node->rate ().rate ();
	      os << " - 1) / ";
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		os << "task__rate_t" << my_task_id << ";\n";
	      else
		os << node->rate ().rate () << ";\n";
	      os << iw3 << "task__time = time__delta * ";
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		os << "task__rate_t" << my_task_id << ";\n";
	      else
		os << node->rate ().rate () << ";\n";
	      os << iw2 << "}\n";
	    }

	  // 各入力変数のコピー
	  const PropagateVarList& pvlist = ce.var ().order ();
	  for (PropagateVarList::const_iterator i (pvlist.begin ());
	       i != pvlist.end (); i++)
	    {
	      const PropagateVar* pv = *i;
	      const CspVar* cv = *pv;
	      const var_rhs_writer& vrw = *cv;

	      if (cv->mode () & VAR_MODE_NODEF_VAR)
		continue;

	      const var_name_wrapper < CspVar > vnw (*cv, ch_member_prefix);
	      const var_rhs_writer& vnw_vrw = vnw;

	      var_doublebuffer_writer < var_rhs_writer, std::string >
		vdbw = make_doublebuffer (vnw_vrw, input_plane);

	      if (!bug_partial_array && pv->is_partial_array ())
		{
		  for (PartialArrayList::const_iterator
			 pai (pv->partial_array ().begin ());
		       pai != pv->partial_array ().end (); pai++)
		    {
		      const PartialArray& pa = *pai;
		      if (pa.size () > 1)
			{
			  os << iw2 << "memcpy("
			     << vrw.set_index (pa.start ()).pointer() << ",\n"
			     << iw3
			     << '&' << ch_prefix << ".";
			  if (double_buffer)
			    os << vdbw.set_index (pa.start ());
			  else
			    os << vnw_vrw.set_index (pa.start ());
			  os << ",\n"
			     << iw3
			     << pa.size () << " * "
			     << "sizeof " << vrw.set_index (pa.start ())
			     << ");\n";
			}
		      else
			{
			  os << iw2 << vrw.set_index (pa.start ())
			     << " = "
			     << ch_prefix << ".";
			  if (double_buffer)
			    os << vdbw.set_index (pa.start ());
			  else
			    os << vnw_vrw.set_index (pa.start ());
			  os << ";\n";
			}
		    }
		}
	      else if (cv->size ())
		{
		  os << iw2 << "memcpy(" << vrw << ",\n"
		     << iw3 << ch_prefix << ".";
		  if (double_buffer)
		    os << vdbw;
		  else
		    os << vnw_vrw;
		  os << ",\n"
		     << iw3 << "sizeof " << vrw << ");\n";
		}
	      else
		{
		  os << iw2 << vrw << " = " << ch_prefix << ".";
		  if (double_buffer)
		    os << vdbw;
		  else
		    os << vnw_vrw;
		  os << ";\n";
		}
	    }

	  if (!async_input && !sync_event && multirate_code)
	    {
	      bool flag_checked = false;

	      os << iw2 << "ch__flag = " << ch_prefix << ".flag__" << chan;
	      if (double_buffer)
		os << '[' << input_plane << ']';
	      os << " - 1;\n";

	      // 自分が可変レートであれば、データがなくなった時、こちら
	      // のレートをチャネルに反映する
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		{
		  os << iw2 << "if (ch__flag == 0)\n"
		   << iw3 << ch_prefix << ".rate__" << my_task_id << " = "
		   << "task__rate_t" << my_task_id << ";\n";
		  flag_checked = true;
		}

	      // 変更後のフラグをチャネルに書く
	      os << iw2 << ch_prefix
		 << ".flag__" << chan;
	      if (double_buffer)
		os << '[' << input_plane << ']';
	      os << " = ch__flag;\n";

	      // ダブルバッファの場合、読みきったら入力面を切り替える。
	      // 入力面の変更は、出力側には影響が無いので、flagとの
	      // write orderは無関係であり、メモリ同期は不要
	      if (double_buffer)
		{
		  if (!flag_checked)
		    {
		      os << iw2 << "if (ch__flag == 0) {\n";
		      flag_checked = true;
		    }
		  os << iw3 << "/* change side */\n"
		     << iw3 << "if (++" << input_plane << " == "
		     << DOUBLEBUFFER_PLANE << ")\n"
		     << iw4 << input_plane << " = 0;\n"
		     << iw3 << plane << " = " << input_plane << ";\n";
		}

	      // 入力が完了したことを相手側に通知するためのマクロを出力
	      if (!flag_checked)
		{
		  os << iw2 << "if (ch__flag == 0) {\n";
		  flag_checked = true;
		}
	      os << iw3 << "CH__EVENT(TASK_" << my_task_id
		 << ", OUT_" << xx_task_id << ");\n";

	      if (flag_checked)
		os << iw2 << "}\n";

	      os
		 << iw2 << "else\n"
		 << iw3 << "CH__SYNCM();\n";
	    }
	  else
	    {
	      // 非同期入力やマルチレートでない場合はフラグを直接クリア
	      // し、イベント送信
	      os << iw2 << ch_prefix << ".flag__" << chan << " = 0;\n"
		 << iw2 << "CH__EVENT(TASK_" << my_task_id
		 << ", OUT_" << xx_task_id << ");\n";
	    }

	  if (ch_cnt == 1)
	    {
	      // 入力チャネルが一つしかなければこのタイミングで入力処理
	      // は終了
	      os << iw2 << "CH__END_LOG(IN_" << my_task_id << ");\n"
		 << iw2 << "break;\n";
	    }
	  else
	    {
	      // 非同期入力時の終了時の goto ラベルを出力
	      if (async_input)
		os << iw1 << "out_" << chan << ":\n";

	      // ビットベクタの当該チャネルのビットに1をセット
	      os << iw2 << "CH__BITSET(IN_" << my_task_id
		 << "," << ch_index << ");\n";
	      ch_index++;
	    }

	  os << iw1 << "}\n";
	}

      // 複数のチャネルの場合は、全チャネルの入力が完了したか判定するコー
      // ドを出力
      if (ch_cnt != 1)
	os << '\n'
	   << iw1 << "if (CH__COMPLETED(IN_" << my_task_id
	   << ","<< ch_cnt << ")) {\n"
	   << iw2 << "CH__END_LOG(IN_" << my_task_id << ");\n"
	   << iw2 << "break;\n"
	   << iw1 << "}\n";

      // 入力が継続しているので、このタスクをスケジューラに任せるための
      // マクロを出力する
      os << '\n' << iw1;
      if (core)
	os << "CH__CORE_THREAD_SCHED(IN_";
      else
	os << "CH__TASK_SCHED(IN_";
      os << my_task_id << ",";
      if (ch_cnt != 1)
	os << "CH__VEC_NAME(IN_" << my_task_id << ")";
      else
	os << "NULL";
      os << "," << ch_cnt << ");\n";

      // チャネルのフラグが変わる事をコンパイラに示すためのメモリバリア
      // を出力
      os << '\n' << iw1 << "CH__MEM_BARRIER();\n";

      os << iw0 << "}\n";

      if (!smbp_mode)
	{
	  // 入力が完了した事を通知するマクロを出力
	  if (input_mode == CH_IO_MODE_UPDATE)
	    os << '\n'
	       << "#ifdef TASK_UPDATE_IN_" << my_task_id << "_COMPLETED\n"
	       << iw0 << "TASK_UPDATE_IN_" << my_task_id << "_COMPLETED();\n"
	       << "#endif\n";
	  else if (input_mode == CH_IO_MODE_INIT)
	    os << '\n'
	       << "#ifdef TASK_INITIAL_IN_" << my_task_id << "_COMPLETED\n"
	       << iw0 << "TASK_INITIAL_IN_" << my_task_id << "_COMPLETED();\n"
	       << "#endif\n";
	  else
	    os << '\n'
	       << "#ifdef TASK_IN_" << my_task_id << "_COMPLETED\n"
	       << iw0 << "TASK_IN_" << my_task_id << "_COMPLETED();\n"
	       << "#endif\n";
	}
    }


    // CspNode のチャネルの出力処理を出力する
    void
    generate_task_wait_output_channel (std::ostream& os, CspNode* node,
				       bool core, int ind,
				       const std::string& unknown_state)
    {
      const indent_writer iw0 (ind);
      const indent_writer iw1 (ind + 1);
      const indent_writer iw2 (ind + 2);
      const indent_writer iw3 (ind + 3);
      int ch_cnt = 0;
      int ch_index = 0;
      bool enable_rate_x = false;
      bool first = true;

      std::string my_task_id (mk_task_id (node->id ()));

      // CspNode の後段へのエッジに対し、出力が必要か調べる
      for (CspEdgeList::const_iterator i (node->forw_edges ().begin ());
	   i != node->forw_edges ().end (); i++)
	{
	  const CspEdge& ce = *i;
	  CspNode* xx_node = ce.node ();

	  // SystemBuilderの対応で、SW-SW以外はカウントしない
	  if (smbp_mode
	      && (node->coretype () == HARDWARE_CORE
		  || ce.node ()->coretype () == HARDWARE_CORE))
	    continue;

	  if (node->taskset () != xx_node->taskset ())
	    {
	      ch_cnt++;

	      // 初期値出力でなく、同期イベントか非同期入力の場合、出力
	      // をenableするためのマクロを出力する。このマクロはユーザー
	      // が用意しなければならない
	      if (enable_multirate
		  && (ce.info () & CSP_EDGE_INFO_INITIAL_VALUE) == 0
		  && (ce.info ()
		      & (CSP_EDGE_INFO_ASYNC_INPUT|CSP_EDGE_INFO_SYNC_EVENT)))
		{
		  if (first)
		    {
		      first = false;
		      os << '\n'
			 << iw0 << "/* event or async enable */\n";
		    }

		  std::string xx_task_id (mk_task_id (xx_node->id ()));

		  const char *macro = "TASK_ENABLE_EVENT_SEND_";
		  if (ce.info () & CSP_EDGE_INFO_ASYNC_INPUT)
		    macro = "TASK_ENABLE_ASYNC_SEND_";

		  os << "#ifdef "
		     << macro << my_task_id << '_' << xx_task_id << "\n"
		     << iw0
		     << macro << my_task_id << '_' << xx_task_id
		     << "(task__time,enable__send_t" << xx_task_id << ");\n"
		     << "#endif\n";
		}
	    }
	}

      // チャネルが不要ならば、出力を行うコードを出力しない
      if (ch_cnt == 0)
	return;

      // チャネルが1を越えていれば出力用ビットベクタがあるので、初期化
      // するコードを出力する
      if (ch_cnt > 1)
	os << '\n'
	   << iw0 << "/* clear output channel bit vector */\n"
	   << iw0 << "CH__VEC_INIT(OUT_" << my_task_id << ");\n";

      // チャネルの出力ループ生成
      os << '\n'
	 << iw0 << "/* wait output channel */\n"
	 << iw0 << "while (1) {\n";

      // 可変レートの場合
      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
	enable_rate_x = true;

      // CspNode の後段へのエッジに対し、出力を行うコードを生成する
      first = true;
      for (CspEdgeList::const_iterator i (node->forw_edges ().begin ());
	   i != node->forw_edges ().end (); i++)
	{
	  const CspEdge& ce = *i;
	  CspNode* xx_node = ce.node ();

	  // SystemBuilderの対応で、SW-SW以外は出力しない
	  if (smbp_mode
	      && (node->coretype () == HARDWARE_CORE
		  || ce.node ()->coretype () == HARDWARE_CORE))
	    continue;

	  std::string xx_task_id (mk_task_id (xx_node->id ()));
	  std::string chan (my_task_id + "_" + xx_task_id);

	  std::string tmp;
	  const std::string& h_name = xx_node->get_block_name (tmp);

	  if (first)
	    first = false;
	  else
	    os << '\n';

	  // TaskSet が同じであれば、チャネルは不要
	  if (node->taskset () == xx_node->taskset ())
	    {
	      os << iw1 << "/* skip local output CH_" << chan
		 << ": " << h_name <<" */\n";
	      continue;
	    }

	  os << iw1 << "/* output: " << h_name << " */\n";

	  Rate_rate_T rate_x = node->rate ().rate ();
	  Rate_rate_T rate_y = xx_node->rate ().rate ();
	  Rate_offset_T offset_x = node->rate ().offset ();
	  Rate_offset_T offset_y = xx_node->rate ().offset ();
	  bool enable_rate_y = false;
	  bool event_or_async = false;

	  bool multirate_code;
	  if ((ce.info () & CSP_EDGE_INFO_INITIAL_VALUE) == 0)
	    multirate_code = enable_multirate;
	  else
	    {
	      rate_y = rate_x;
	      multirate_code = false;
	    }

	  bool check_time = false;

	  bool double_buffer = (ce.info () & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0;
	  const std::string ch_prefix ("CH_" + block_name_prefix + chan);
	  const std::string plane (ch_prefix + ".output_buffer_side");

	  // 各出力条件設定と判定
	  if (multirate_code)
	    {
	      if (xx_node->info () & CSP_NODE_INFO_CHANGE_RATE)
		enable_rate_y = true;

	      if (ce.info ()
		  & (CSP_EDGE_INFO_SYNC_EVENT|CSP_EDGE_INFO_ASYNC_INPUT))
		event_or_async = true;

	      // 時刻チェックを要するケース
	      //
	      // ・ダブルバッファの場合で、後段が2周期以降に開始する
	      // ・ダブルバッファでなく(0次ホールド)
	      // 　・早いレートから遅いレートへの送信
	      // 　・または、後段のサンプル時間のオフセットが大きい
	      //   　-> 実行時間に後段が開始前
	      // 　・または、可変レートである
	      if ((double_buffer && offset_x + rate_x <= offset_y)
		  || (!double_buffer
		      && (rate_x < rate_y
			  || offset_x < offset_y
			  || enable_rate_x || enable_rate_y)))
		check_time = true;

	      // 出力側の時刻を入力側が追い越していないかを確認するコード出力
	      if (!event_or_async && check_time && assert_output_enable)
		{
		  os << iw1 << "assert(!CH__TIME_LT(CH_"
		     << block_name_prefix << chan
		     << ".time__" << chan << ", task__time));\n";
		}
	    }

	  // 出力可能か調べるif文の出力開始
	  os << iw1 << "if (";

	  if (event_or_async)
	    {
	      // 非同期入力か同期イベントは出力がenableかだけをチェック
	      os << "enable__send_t" << xx_task_id << ") {\n";
	    }
	  else
	    {
	      if (check_time)
		{
		  // 可変レートか、マルチレートで高速レートから低速レー
		  // トへの出力、またはサンプル時間のオフセットにより後
		  // 段が実行前である場合

		  // チャネル数が1を越えていたら出力用のビットベクタを
		  // チェックして未出力か調べるコードを生成
		  if (ch_cnt != 1)
		    os << "CH__BITCHECK(OUT_" << my_task_id
		       << "," << ch_index << ") == 0 && ";

		  // 出力時刻になったか判定する条件を出力
		  os << "CH__TIME_LE(task__time+";
		  if (enable_rate_x)
		    os << "task__rate_t" << my_task_id;
		  else
		    os << rate_x;

		  // ダブルバッファの場合は、面数に対応した周期が対象
		  if (double_buffer)
		    os << "*" << DOUBLEBUFFER_PLANE;

		  os << ", " << ch_prefix << ".time__" << chan << ")) {\n";

		  // 出力時刻ではない場合は、このタイミングでは出力はし
		  // ない
		  os << iw2 << "/* no send this period */\n";
		  if (ch_cnt == 1)
		    {
		      os << iw2 << "CH__END_LOG(OUT_" << my_task_id << ");\n"
			 << iw2 << "break;\n";
		    }
		  else
		    {
		      os << iw2 << "CH__BITSET(OUT_" << my_task_id
			 << "," << ch_index << ");\n";
		    }

		  os << iw1 << "} else if (";
		}


	      // チャネル数が1を越えていたら出力用のビットベクタを
	      // チェックして未出力か調べるコードを生成
	      if (ch_cnt != 1)
		os << "CH__BITCHECK(OUT_" << my_task_id
		   << "," << ch_index << ") == 0 && ";

	      // チャネルが空いているか調べるコードを生成
	      os << ch_prefix << ".flag__" << chan;
	      if (double_buffer)
		os << '[' << plane << ']';
	      os << " == 0) {\n";
	    }

	  //　出力コード生成

	  // 遅いレートから速いレートの出力は、1以上の出力回数になるの
	  // で、その出力回数の変数を出力
	  if (!event_or_async
	      && multirate_code
	      && (rate_x > rate_y || enable_rate_x || enable_rate_y))
	    os << iw2 << "unsigned long ch__sendcount;\n";

	  // 非同期入力や同期イベントではなく、相手が可変レートならば、
	  // 相手側レートを取得するコードを出力
	  if (!event_or_async && enable_rate_y)
	    os << iw2 << "unsigned long rate__y = CH_"
	       << block_name_prefix << chan << ".rate__"
	       << xx_task_id << ";\n";

	  const optional_refstring&
	    opt_state_storage = c_codewriter.state_storage ();
	  const std::string&
	    state_storage = (opt_state_storage
			     ? *opt_state_storage : unknown_state);

	  // イベントが必要なSubSystem(Triggered/Enabled SubSystem)の場
	  // 合は、SubSystemの状態変数をチャネルへコピーするコードを生
	  // 成
	  if (node->need_subsystem_event ()
	      && (ce.info () & CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT) != 0
	      && (xx_node->event_subsystem () == node))
	    {
	      os << iw2
		 << "COPY_SUBSYSTEM_STATE_" << my_task_id << "(" << ch_prefix
		 << ".state_" << my_task_id << '_' << state_storage
		 << ", "
		 << state_storage
		 << ");\n";
	      os << iw2
		 << ch_prefix << ".state_" << my_task_id << '_'
		 << state_storage << ".subsystem__" << my_task_id
		 << "_state = " << "subsystem__" << my_task_id << "_state;\n";
	    }

	  // 非同期入力または同期イベントの場合はタイムスタンプを更新
	  if (event_or_async)
	    os << iw2 << ch_prefix << ".timestamp__" << my_task_id
	       << " = task__time;\n";

	  // 出力変数をチャネルへコピーするコードを生成
	  generate_task_output_values (os, ce, chan, ind + 2);

	  // フラグを設定するコードを生成

	  if (!event_or_async
	      && multirate_code
	      && (enable_rate_x || enable_rate_y || rate_x > rate_y))
	    {
	      // 遅いレートから速いレートへの出力の場合、算出した出力回
	      // 数(データの有効回数)をフラグに設定することと、相手側の
	      // 次の実行時刻を設定するコードを出力
	      os << iw2 << "ch__sendcount = CH__SEND_COUNT(task__time+";

	      if (enable_rate_x)
		os << "task__rate_t" << my_task_id;
	      else
		os << rate_x;

	      // ダブルバッファ(単位遅延)の場合は、面数に対応した周期が
	      // 対象
	      if (double_buffer)
		os << "*" << DOUBLEBUFFER_PLANE;

	      os << ", CH_" << block_name_prefix << chan
		 << ".time__" << chan << ", ";

	      if (enable_rate_y)
		os << "rate__y";
	      else
		os << rate_y;

	      os <<");\n"
		 << iw2 << ch_prefix << ".flag__" << chan;
	      if (double_buffer)
		os << '[' << plane << ']';
	      os << " = ch__sendcount;\n"
		 << iw2 << ch_prefix
		 << ".time__" << chan << " += ch__sendcount";
	      if (enable_rate_y)
		os << " * rate__y";
	      else if (rate_y > 1)
		os << " * " << rate_y;
	      os << ";\n";

	      // 出力面を切り替える
	      if (double_buffer)
		{
		  os << iw2 << "/* change side */\n"
		     << iw2 << "int ch__side = " << plane << ";\n"
		     << iw2 << "if (++ch__side == "
		     << DOUBLEBUFFER_PLANE << ")\n"
		     << iw3 << "ch__side = 0;\n"
		     << iw2 << plane << " = ch__side;\n";
		}
	    }
	  else
	    {
	      // 一回出力する場合のフラグの設定と次の実行時刻を設定する
	      // コードを出力
	      os << iw2 << ch_prefix << ".flag__" << chan << " = 1;\n";
	      if (!event_or_async && check_time)
		os << iw2 << ch_prefix << ".time__" << chan
		   << " += " << rate_y << ";\n";
	    }

	  // 出力が完了したので入力側へ通知するコードを出力
	  os << iw2 << "CH__EVENT(TASK_" << my_task_id
	     << ",IN_" << xx_task_id << ");\n";

	  if (ch_cnt == 1)
	    {
	      // チャネルが一つの場合、ここでループを終了するコードを出
	      // 力
	      os << iw2 << "CH__END_LOG(OUT_" << my_task_id << ");\n"
		 << iw2 << "break;\n";
	    }
	  else
	    {
	      // 複数のチャネルの場合、ビットベクタの当該チャネルのビッ
	      // トをセットするコードを出力
	      os << iw2 << "CH__BITSET(OUT_" << my_task_id
		 << "," << ch_index << ");\n";
	      ch_index++;
	    }

	  os << iw1 << "}\n";
	}

      // チャネルが複数の場合、全出力が完了したかチェックし、終了するコー
      // ドを出力
      if (ch_cnt != 1)
	os << '\n'
	   << iw1 << "if (CH__COMPLETED(OUT_" << my_task_id
	   << "," << ch_cnt << ")) {\n"
	   << iw2 << "CH__END_LOG(OUT_" << my_task_id << ");\n"
	   << iw2 << "break;\n"
	   << iw1 << "}\n";

      // 出力が継続しているので、このタスクをスケジューラに任せるための
      // マクロを出力する
      os << '\n' << iw1;
      if (core)
	os << "CH__CORE_THREAD_SCHED(OUT_";
      else
	os << "CH__TASK_SCHED(OUT_";
      os << my_task_id << ",";
      if (ch_cnt != 1)
	os << "CH__VEC_NAME(OUT_" << my_task_id << ")";
      else
	os << "NULL";
      os << "," << ch_cnt << ");\n";

      // チャネルのフラグが変わる事をコンパイラに示すためのメモリバリア
      // を出力
      os << '\n' << iw1 << "CH__MEM_BARRIER();\n";

      os << iw0 << "}\n";

      if (!smbp_mode)
	{
	  // 出力が完了した事を通知するマクロを出力
	  os << '\n'
	     << "#ifdef TASK_OUT_" << my_task_id << "_COMPLETED\n"
	     << iw0 << "TASK_OUT_" << my_task_id << "_COMPLETED();\n"
	     << "#endif\n";
	}
    }


    /// @}
  }
}
