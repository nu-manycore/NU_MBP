#include <iostream>
#include <sstream>
#include <stack>

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>

#include "csp_translator.h"

using namespace SimulinkModel;
using namespace SimulinkModel::XSD;
using namespace Simulink::XML;

#if (defined(__MINGW32__) || defined(__MINGW64__)) && !defined(strncasecmp)
#define strncasecmp(X,Y,Z) _strnicmp(X,Y,Z)
#endif

#ifndef INPUT_MULTIRATE
#define INPUT_MULTIRATE		0
#endif

#ifndef LOW_RATE_PULSEGENERATOR
#define LOW_RATE_PULSEGENERATOR	1
#endif

namespace Simulink
{
  namespace CspTrans
  {
    //
    // namespace or class static members
    //

    /// \addtogroup COMMON_VARIABLES
    ///
    /// @{

    /// \brief task0を生成する(--create-t0=[ny])
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int do_create_t0 = 0;

    /// \brief PulseGeneratorを低レート化(--low-rate-pulsegenerator=[yn])
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int low_rate_pulsegenerator = LOW_RATE_PULSEGENERATOR;

    /// \brief Sinkブロックを無視(--ignore-to-sink=[yn])
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int ignore_to_sink = 1;

    /// \brief 孤立したブロックを無視(--ignore-isolated=[yn])
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int ignore_isolated_block = 1;

    /// \brief 末端のブロックを無視(--ignore-leaves=[ny])
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int ignore_leaf_block = 0;

    /// \brief ラベル出力をタスクIDだけにする(--label-only-task-id)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int label_only_task_id = 0;

    /// \brief マルチレートを有効(--enable-multirate)
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    int enable_multirate = 1;

    /// @}

    /// \addtogroup XMLCSP_MAIN
    ///
    /// @{

    /// \brief 入力をマルチレートとする
    int input_multirate = INPUT_MULTIRATE;

    /// \brief 遅延ブロックが結合可能ならば結合する
    int do_concat_delay_block = 1;

    /// \brief 新規に作成するタスク番号
    unsigned int new_task_id;

    // SampleTime
    double SampleTime::st_tick = 0.000001; // 1us
    std::map < const std::string, double > SampleTime::param_map;

    // CspNode
    cspnode_list_T CspNode::n_csp_node;
    int CspNode::n_csp_node_count = 0;
    CspNodeList CspNode::n_csp_nodelist;

    // CspReader
    CspReader csp_reader;

    //
    // base_CspBlockInfoのメンバ関数
    //

    const var_T*
    base_CspBlockInfo::find_var (const std::string& name) const
    {
      const CspBlockInfo* binfo = static_cast< const CspBlockInfo* >(this);
      const block_T* b = binfo->block ();
      for (block_T::var_const_iterator vi (b->var ().begin ());
	   vi != b->var ().end (); vi++)
	{
	  if (vi->name () == name)
	    return &*vi;
	}
      return NULL;
    }

    //
    // Sample Times
    //

    bool
    SampleTime::check_param (const std::string& param)
    {
      return param_map.find (param) != param_map.end ();
    }

    void
    SampleTime::set_param (const std::string& param, double val)
    {
      param_map.insert (std::make_pair (param, val));
    }

    double
    SampleTime::get_param (const std::string& param)
    {
      std::map < const std::string, double >::const_iterator
	i = param_map.find (param);
      if (i == param_map.end ())
	return 0;
      return i->second;
    }

    unsigned long
    SampleTime::gcd (unsigned long x, unsigned long y)
    {
      unsigned long t;
      while (y != 0)
	{
	  t = x % y;
	  x = y;
	  y = t;
	}
      return x;
    }

    SampleTime::SampleTime (const std::string& str)
      : st_type(ST_TYPE_NONE), st_inherited (false), st_baserate (false)
    {
      set (str);
    }

    void
    SampleTime::set (const std::string& str)
    {
      bool has_bracket = false;
      int n = 0;

      type () = ST_TYPE_NONE;

      const char* s = str.c_str();
      const char* x = tok (s, n);

      if (n == 0 && *s == '[')
	{
	  has_bracket = true;
	  s++;
	  x = tok (s, n);
	}

      if (type () == ST_TYPE_INVALID)
	{
	  svalue () = str;
	  return;
	}

      if (n)
	{
	  if (n == 3 && strncasecmp (x, "inf", 3) == 0)
	    type () = ST_TYPE_CONSTANT;
	  else if (n == 1 && x[0] == '0')
	    type () = ST_TYPE_CONTINUOUS; // check FIXED
	  else if (n == 2 && x[0] == '-' && x[1] == '1')
	    type () = ST_TYPE_INHERITED; // check ASYNC
	  else if (n == 2 && x[0] == '-' && x[1] == '2')
	    type () = ST_TYPE_VARIABLE;
	  else
	    {
	      char *p;
	      double v = strtod (x, &p);
	      if (static_cast<const char*> (p) - x == n)
		{
		  type () = ST_TYPE_DISCRETE;
		  dvalue () = v;

		  if (v != 0.0 && v < get_tick ())
		    {
		      // 最小単位を1usとして、最大公約数の周期を取る
		      unsigned long rx = round (get_tick () / 1e-6);
		      unsigned long ry = round (v / 1e-6);
		      set_tick (gcd (rx, ry) * 1e-6);
		    }
		  if (v == 0.0)
		    std::cout << "Warning: Couldn't convert: "
				<< x << std::endl;
		}
	      else
		{
		  type () = ST_TYPE_DISCRETE;
		  svalue () = std::string (x, n);
		}
	    }
	}

      if (has_bracket)
	{
	  n = 0;
	  x = tok (s, n);

	  if (n == 0 && *s == ',') // comma?
	    {
	      s++;
	      x = tok (s, n);
	    }

	  if (type () == ST_TYPE_INVALID)
	    {
	      svalue () = str;
	      return;
	    }

	  if (n == 1 && x[0] == '1' && type () == ST_TYPE_CONTINUOUS)
	    type () = ST_TYPE_FIXED;
	  else if (n == 2 && x[0] == '-' && x[1] == 'n'
		   && type () == ST_TYPE_INHERITED)
	    type () = ST_TYPE_ASYNCHRONOUS;
	  else if (n > 0 && (n != 1 || x[0] != '0'))
	    {
	      char *p;
	      double v = strtod (x, &p);
	      if (p - x == n)
		doffset () = v;
	      else
		soffset () = std::string (x, n);
	    }
	}
    }

    const char*
    SampleTime::tok (const char *& s, int& n)
    {
      // skip space
      while (*s != 0 && isspace (static_cast<unsigned char> (*s)))
	s++;

      const char* p = s;

      if (*s == '-')
	s++;
      while (*s != 0 && *s != '[' && *s != ']' && *s != ',')
	{
	  if ((*s == 'e' || *s == 'E') && s[1] == '-')
	    s += 2;
	  else if (isalnum (static_cast<unsigned char> (*s))
		   || *s == '_' || *s == '.')
	    s++;
	  else
	    {
	      // invalid char
	      type() = ST_TYPE_INVALID;
	      n = 0;
	      return NULL;
	    }
	}

      n = s - p;
      return p;
    }

    bool SampleTime::operator== (const SampleTime& x) const
    {
      if (type () != x.type ())
	return false;

      if (type () != ST_TYPE_DISCRETE)
	return true;

      if (svalue ().length ())
	{
	  if (svalue () != x.svalue ())
	    return false;
	}
      else if (dvalue () != x.dvalue ())
	return false;

      if (soffset ().length ())
	{
	  if (soffset () != x.soffset ())
	    return false;
	}
      else if (soffset () != x.soffset ())
	return false;

      return true;
    }

    SampleTime::operator std::string () const
    {
      std::ostringstream out;

      switch (type ())
	{
	case ST_TYPE_NONE:
	  out << "<none>";
	  break;
	case ST_TYPE_DISCRETE:
	  if (svalue ().length ())
	    {
	      if (soffset ().length ())
		out << "[\"" << svalue () << "\" \"" << soffset () << "\"]";
	      else if (doffset ())
		out << "[\"" << svalue () << "\" " << doffset () << ']';
	      else
		out << svalue ();
	    }
	  else
	    {
	      if (soffset ().length ())
		out << '[' << dvalue () << " \"" << soffset () << "\"]";
	      else if (doffset ())
		out << '[' << dvalue () << ' ' << doffset () << ']';
	      else
		out << dvalue ();
	    }
	  break;
	case ST_TYPE_CONTINUOUS:
	  out << '0';
	  break;
	case ST_TYPE_FIXED:
	  out << "[0 1]";
	  break;
	case ST_TYPE_INHERITED:
	  out << "-1";
	  break;
	case ST_TYPE_CONSTANT:
	  out << "inf";
	  break;
	case ST_TYPE_VARIABLE:
	  out << "-2";
	  break;
	case ST_TYPE_HYBRID:
	  out << "N/A";
	  break;
	case ST_TYPE_TRIGGERED:
	  out << "TRIGGERED";
	  break;
	case ST_TYPE_ASYNCHRONOUS:
	  out << "[-1 -n]";
	  break;
	case ST_TYPE_INVALID:
	  out << "INVALID<" << svalue() << '>';
	}

      if (!inherited ())
	out << '*';
      if (baserate ())
	out << '!';

      return out.str ();
    }

    std::ostream&
    operator<< (std::ostream& s, const SampleTime& x)
    {
      return s << static_cast< const std::string > (x);
    }

    //
    // CspReader
    //

    bool
    CspReader::is_inport_type (const CspBlockInfo* binfo) // static
    {
      return is_inport_type (binfo->block ());
    }

    bool
    CspReader::is_inport_type (const SimulinkModel::XSD::block_T* b) // static
    {
      return (b->blocktype () == "Inport"
	      || b->blocktype () == "TriggerPort"
	      || b->blocktype () == "EnablePort"
	      || b->blocktype () == "ActionPort");
    }

    bool
    CspReader::is_pass_block (const CspBlockInfo* binfo) // static
    {
      return is_pass_block (binfo->block ());
    }

    bool
    CspReader::is_pass_block (const SimulinkModel::XSD::block_T* b) // static
    {
      return (b->blocktype () == "Demux"
	      || b->blocktype () == "Mux"
	      || b->blocktype () == "From"
	      || b->blocktype () == "Goto"
	      || b->blocktype () == "Merge"
	      || b->blocktype () == "BusCreator"
	      || b->blocktype () == "BusSelector");
    }


    bool
    CspReader::cmp_line (const CspBlockInfo* a, const CspBlockInfo* b)
    {
      if (a == NULL)
	return false;		// don't care

      if (b == NULL)
	return true;		// `a' first

      int al = a->line_position ();
      int bl = b->line_position ();
      if (al != -1 && bl != -1)
	return al < bl;

      // 外部入力に関する比較条件。
      // 以下の場合はaを先にする
      //   1. aにコードがあり、(bはコードがなく)extinではない
      //   2. aは単独のextinの場合、bはコードがあるかまたはextinではない
      //
      bool ain = (a->task_info () & TASK_INFO_EXTIN) != 0;
      bool bin = (b->task_info () & TASK_INFO_EXTIN) != 0;
      return ((al != -1 && !bin)
	      || (al == -1 && ain && (bl != -1 || !bin)));
    }

    void
    CspReader::import_code (CspNode* cn,
			    const std::string& name,
			    code_ent_T& ce)
    {
      const code_T& c = *ce.code ();
      const std::string& type = c.type ();
      const std::string& c_code = c;

      // コードを断片化するのはtaskおよびupdateの実行コード
      // これはSubSystemのイベントやTIDのコードを分割するため
      if (type == "task" || type == "update")
	{
	  // コード中の書式に従った断片化情報を作成
	  codelist x (c_code);

	  // コードは自動変数等の宣言・定義を局所化するためCのブロック
	  // とする
	  ce.code_frag_list ().push_back (code_frag_T ("{\n"));

	  int need_rtm = 0;

	  // コードを書式に従い断片化する際、レートを保持するためのスタック
	  std::stack < std::pair < bool, unsigned int > > tid_stack;

	  // 現在の状態(初期値)
	  std::pair < bool, unsigned int > tid_state (false, 0);

	  // codeタグ中のテキストの開始と終了を取得し、
	  const char* p = c_code.c_str ();
	  const char* e = c_code.c_str () + c_code.length();

	  // 書式によってコードを断片化する
	  for (marklist_T::iterator mi (x.mark_list().begin ());
	       mi != x.mark_list().end (); mi++)
	    {
	      c_mark& m = *mi;
	      if (p < m.beg())
		{
		  // 書式の前までの部分の処理
		  std::string t (p, m.beg() - p);

		  // Real-Time Model Objectが必要かどうかの判定。
		  // ただし、multirate時のTID参照を
		  if (need_rtm == 0
		      && check_rtm (t) == 1
		      && (enable_multirate == 0 || check_tid (t) == 0))
		    need_rtm = 1;

		  // 書式より前までを断片化
		  ce.code_frag_list ().push_back (code_frag_T (t));
		  p = m.beg();
		}

	      // 共通か対象のブロックのコード
	      if (m.name () == "none" || m.name () == name)
		{
		  std::string t = m;

		  if (enable_multirate)
		    {
		      // マルチレートのコード生成の場合は、コード中の
		      // TIDのコード等を削除する必要がある

		      bool do_add = !tid_state.first;
		      if (m.type () == 'S')
			{
			  // 条件付きコードの開始なので、状態をスタック
			  // に保存
			  tid_stack.push (tid_state);
			  tid_state.second = m.num ();

			  // TIDのコード(subrateのtimer counterを参照)
			  // があれば、それは追加しない
			  if (check_tid (t) == 1)
			    {
			      // delete subrate timer counter,
			      // eg `if (...->TimerCounters.TID[N] == 0) {'
			      tid_state.first = true;
			      do_add = false;
			    }
			  else
			    {
			      tid_state.first = false; // none
			      do_add = true;
			    }
			}
		      else if (m.type () == 'E')
			{
			  // ブロックのコードの終了なので、状態をスタッ
			  // クから回復
			  if (tid_state.second != m.num ())
			    {
			      std::cerr << "Error: " << name
					  << "C mark nesting incorrect"
					  << std::endl;
			      has_error = true;
			    }
			  if (tid_stack.size ())
			    {
			      tid_state = tid_stack.top ();
			      tid_stack.pop ();
			    }
			}

		      if (do_add)
			{
			  // TIDを除くReal-Time Model Objectへの参照コー
			  // ドがあればReal-Time Model Objectが必要
			  if (need_rtm == 0
			      && check_rtm (t) == 1
			      && check_tid (t) == 0)
			    need_rtm = 1;

			  // 断片化したコードを追加
			  ce.code_frag_list ().push_back (code_frag_T (t));
			}
		    }
		  else
		    {
		      // Real-Time Model Objectを参照するパターンがあれ
		      // ばReal-Time Model Objectが必要
		      if (need_rtm == 0 && check_rtm (t) == 1)
			need_rtm = 1;

		      // 断片化したコードを追加
		      ce.code_frag_list ().push_back (code_frag_T (t));
		    }

		  if (m.name() == name)
		    {
		      // Enabled/Triggered SubSystemの条件付きコードの
		      // 開始であれば、区切りを挿入する
		      if (m.type() == 'S'
			  && (enable_multirate == 0
			      || tid_state.first == false)
			  && (cn->info ()
			      & (CSP_NODE_INFO_ENABLED_SUBSYSTEM
				 | CSP_NODE_INFO_TRIGGERED_SUBSYSTEM)) != 0
			  && (cn->info () & CSP_NODE_INFO_ONE_SUBSYSTEM) == 0)
			ce.code_frag_list ().push_back (code_frag_T ());
		    }
		}

	      p = m.end();
	    }

	  if (p < e)
	    {
	      // 最後の書式より後の部分の処理
	      std::string t (p, e - p);

	      // Real-Time Model Objectが必要かどうかの判定。
	      // ただし、multirate時のTID参照を
	      if (need_rtm == 0
		  && check_rtm (t) == 1
		  && (enable_multirate == 0 || check_tid (t) == 0))
		need_rtm = 1;

	      // 最後の断片を追加
	      ce.code_frag_list ().push_back (code_frag_T (t));
	    }

	  // Real-Time Model Objectが必要なので属性を設定
	  if (need_rtm)
	    cn->info () |= CSP_NODE_INFO_NEED_RTM;

	  // Cのブロックを閉じる
	  ce.code_frag_list ().push_back (code_frag_T ("}\n"));
	}
      else
	{
	  // initまたはstateのコードはCのブロックとする
	  bool need_brace = (type == "init" || type == "state");
	  if (need_brace)
	    ce.code_frag_list ().push_back (code_frag_T ("{\n"));
	  ce.code_frag_list ().push_back (code_frag_T (c_code));
	  if (need_brace)
	    ce.code_frag_list ().push_back (code_frag_T ("}\n"));
	}
    }


    bool
    CspReader::check_delay_as_other_input (const CspNode* cn_tail,
				     const CspNode* cn_head) const
    {
      bool prev_is_delay_and_other_input = false;

      // エッジのhead(終点)に含まれるdelayブロックについて調べる
      for (csp_block_list_const_iterator
	     hbi (cn_head->contain_blocks ().begin ());
	   prev_is_delay_and_other_input == false
	     && hbi != cn_head->contain_blocks ().end (); hbi++)
	{
	  const CspBlockInfo* h_binfo = *hbi;

	  // delayでない(updateを持たない)かS-Function(つまりother)なら
	  // 調査しない
	  if (!h_binfo->has_update () || h_binfo->is_sfunction ())
	    continue;

	  // delayブロックの前段のブロックについて調べる
	  for (csp_block_connect_list_T::const_iterator
		 bci (h_binfo->backward ().begin ());
	       prev_is_delay_and_other_input == false
		 && bci != h_binfo->backward ().end (); bci++)
	    {
	      const CspBlockInfo* p_binfo = bci->tail ();

	      // このエッジのtail(始点)に含まれないブロックは調べない
	      if (!cn_tail->is_contained (p_binfo))
		continue;

	      // このエッジのheadのブロックで、着目しているdelayブロッ
	      // ク以外(other)のものを調べる
	      for (csp_block_list_const_iterator
		     xbi (cn_head->contain_blocks ().begin ());
		   prev_is_delay_and_other_input == false
		     && xbi != cn_head->contain_blocks ().end ();
		   xbi++)
		{
		  const CspBlockInfo* x_binfo = *xbi;

		  // 着目しているdelayブロックである
		  if (x_binfo == h_binfo)
		    continue;

		  // otherでない
		  if (x_binfo->has_update ()
		      && !x_binfo->is_sfunction ()
		      && !x_binfo->is_subsystem ())
		    continue;

		  // otherの前段のブロックをチェック
		  for (csp_block_connect_list_T::const_iterator
			 ybci (x_binfo->backward ().begin ());
		       prev_is_delay_and_other_input == false
			 && ybci != x_binfo->backward ().end ();
		       ybci++)
		    {
		      const CspBlockInfo* y_binfo = ybci->tail ();

		      // delayブロックとotherとの前段が同じなのでこのエッ
		      // ジはUPDATEとはせず、普通の入力とする
		      if (p_binfo == y_binfo)
			{
			  prev_is_delay_and_other_input = true;
#if 0
			  std::cout << '!' << __LINE__
				    << ": "
				    << t_tail->task_id
				    << '('
				    << p_binfo->id ()
				    << ')'
				    << " -> "
				    << t_head->task_id
				    << '('
				    << h_binfo->id ()
				    << ','
				    << x_binfo->id ()
				    << ')'
				    << std::endl;
#endif
			}
		    }
		}
	    }
	}
      return prev_is_delay_and_other_input;
    }

    int
    CspReader::assign_variables (CspBlockConnect& bc,
				 CspNode* cn_tail, CspBlockInfo* cbi_tail,
				 CspNode* cn_head, CspBlockInfo* cbi_head,
				 CspEdge& edge, bool multirate)
    {
      var_mode_T vm_tail = 0;
      var_mode_T vm_head = 0;

      if (cn_head->info () & CSP_NODE_INFO_INVAR_UPDATE)
	{
	  vm_tail = VAR_MODE_UPDATE;
	  vm_head = VAR_MODE_UPDATE;
	}

      if (multirate)
	vm_head |= VAR_MODE_MULTIRATE;

      for (csp_var_list_T::iterator vi (bc.var_list ().begin());
	   vi != bc.var_list ().end (); vi++)
	{
	  CspVar& cv = *vi;

	  // 出力側の自動変数に含まれたら無視
	  PropagateVarMap::const_iterator
	    pvma = cn_tail->auto_var ().find (cv->name ());
	  if (pvma != cn_tail->auto_var ().end ())
	    continue;

	  // 出力側では出力変数に含まれねばならない
	  PropagateVarMap::const_iterator
	    pvmo = cn_tail->output_var ().find (cv->name ());
	  assert (pvmo != cn_tail->output_var ().end ());

	  // 構造体の変数かどうか
	  var_mode_T
	    vm_struct = (pvmo->second.var ()->mode ()
			 & (VAR_MODE_EXTOUT|VAR_MODE_EXTIN|VAR_MODE_BIOSIG));

	  // CspEdgeに出力する変数を設定
	  edge.var ().add_varent (&cv);
	  cv.mode () |= (vm_tail | vm_struct);

	  if (cv.mode () & VAR_MODE_UPDATE)
	    edge.info () |= CSP_EDGE_INFO_HAS_UPDATE;

	  if ((edge.info () & CSP_EDGE_INFO_INITIAL_VALUE))
	    {
	      // 初期値送受信の場合、入力側では原則として出力変数である
	      PropagateVarMap::const_iterator
		pvmi = cn_head->output_var ().find (cv->name ());
	      if (pvmi == cn_head->output_var ().end ())
		{
		  // マージしている場合、入力変数にもなる
		  PropagateVarMap::const_iterator
		    pvmi = cn_head->input_var ().find (cv->name ());
		  assert (pvmi != cn_head->input_var ().end ());
		}
	    }
	  else
	    {
	      // 初期値送受信でない場合は、入力側では入力変数に含まれね
	      // ばならない
	      PropagateVarMap::const_iterator
		pvmi = cn_head->input_var ().find (cv->name ());
	      assert (pvmi != cn_head->input_var ().end ());
	    }

	  // 入力側BlockConnect
	  CspBlockConnect* bbc = bc.pair ();
	  assert (bbc);

	  // 入力側edge
	  CspEdge* bedge = edge.pair ();
	  assert (bedge);

	  // 入力側BlockConnectの変数
	  CspVar* bcvp = NULL;
	  for (csp_var_list_T::iterator bvi (bbc->var_list ().begin ());
	       bvi != bbc->var_list ().end (); bvi++)
	    {
	      CspVar& bcv = *bvi;
	      if (bcv->name () == cv->name ())
		{
		  bcvp = &bcv;
		  break;
		}
	    }

	  assert (bcvp);

	  // 入力側のCspEdge(backedge)に変数を設定
	  bedge->var ().add_varent (bcvp);
	  bcvp->mode () |= (vm_head | vm_struct);
	}

      // エッジにラベルを設定
      if (label_only_task_id == 0)
	add_var_list_label_to_edge (edge);

      return 0;
    }

    /// \brief includeのエントリ
    typedef std::pair < int, const include_T* > include_ent_T;

    /// \brief includeのリスト
    typedef std::vector < include_ent_T > include_list_T;

    /// \brief includeのソートの為の比較関数
    ///
    /// \param[in] a インクルード1
    /// \param[in] b インクルード2
    ///
    /// \retval true インクルード1が前
    /// \retval false インクルード1はインクルード2と同じか後
    bool
    cmp_include (const include_ent_T& a, const include_ent_T& b)
    {
      return a.first < b.first;
    }

    int
    CspReader::EC_code (void)
    {
      const blocks_T& bs = xml_blocks ();

      // Real-Time Model Objectに関するマクロ/型/コード/初期化コードと、
      // 共通のコード/初期化コード、自動変数、マクロをC_CodeWriteへ設定
      for (blocks_T::code_const_iterator ci (bs.code ().begin ());
	   ci != bs.code ().end (); ci++)
	{
	  const code_T& c = *ci;
	  if (c.type () == "rt-macro")
	    c_codewriter.rt_macro ().push_back (c);
	  else if (c.type () == "rt-type")
	    c_codewriter.rt_type ().push_back (c);
	  else if (c.type () == "rt-code")
	    {
	      // rate_scheduler()の呼び出しがあれば、それより前を分離。
	      // (コード生成時に "rate_scheduler" を簡易に識別するため)
	      // また、その場合はマルチレートである
	      try {
		std::tr1::regex rs_def ("^[[:blank:]]+"
					  "rate_scheduler"
					  "[[:blank:]]*"
					  "\\("
					  "[[:blank:]]*"
					  "\\)"
					  "[[:blank:]]*"
					  ";.*$");
		std::tr1::cmatch mr;
		if (std::tr1::regex_search (c.c_str(), mr, rs_def))
		  {
		    std::string c1 (c.c_str (), mr[0].first - c.c_str ());
		    std::string c2 (mr[0].first);
		    is_single_rate() = false;
		    c_codewriter.rt_code ().push_back (c1);
		    c_codewriter.rt_code ().push_back (c2);
		  }
		else
		  c_codewriter.rt_code ().push_back (c);
	      }
	      catch (const std::tr1::regex_error& e)
		{
		  std::cerr << "Error in " << __FUNCTION__ << ":"
			      << e.what() << std::endl;
		  return -1;
		}
	      catch (const std::exception& e)
		{
		  std::cerr << "Error in " << __FUNCTION__ << ":"
			      << e.what() << std::endl;
		  return -1;
		}
	      catch (...)
		{
		  std::cerr << "Error in " << __FUNCTION__ << std::endl;
		  return -1;
		}
	    }
	  else if (c.type () == "rt-init")
	    c_codewriter.rt_init ().push_back (c);
	  else if (c.type () == "common")
	    c_codewriter.common ().push_back (c);
	  else if (c.type () == "common-init")
	    c_codewriter.common_init ().push_back (c);
	  else if (c.type () == "autovars")
	    c_codewriter.preamble ().push_back (c);
	  else if (c.type () == "c_macro")
	    c_codewriter.c_macro ().push_back (c);
	  else if (c.type () == "preamble")
	    c_codewriter.header_preamble ().push_back (c);
	}

      // ブロックシグナルやパラメタ、状態変数の構造体をC_CodeWriteへ設
      // 定
      for (blocks_T::deftype_const_iterator di (bs.deftype ().begin ());
	   di != bs.deftype ().end (); di++)
	{
	  const object_T& d = *di;
	  const object_T::has_struct_optional& has_struct_o = d.has_struct ();
	  const object_T::value_optional& value_o = d.value ();
	  const object_T::kind_optional& kind_o = d.kind ();

	  if (*kind_o == "param"
	      || *kind_o == "bsignal"
	      || *kind_o == "bstate")
	    {
	      int obj_attr = 0;

	      if (has_struct_o)
		obj_attr |= C_OBJECT_EC_ATTR_WITH_STRUCT;
	      obj_attr |= C_OBJECT_EC_ATTR_VALUE_IS_CODE;

	      const std::string& v = d;
	      c_codewriter.c_deftype ()
		.push_back (c_object_ent_T (&d, &v, obj_attr));
	    }
	  else
	    c_codewriter.c_deftype ()
	      .push_back (c_object_ent_T (&d, &*value_o, 0));
	}

      // Real-Time Model Object、またはそれ以外の構造体をC_CodeWriterへ
      // 設定
      for (blocks_T::object_const_iterator oi (bs.object ().begin ());
	   oi != bs.object ().end (); oi++)
	{
	  const object_T& o = *oi;
	  const object_T::value_optional& value_o = o.value ();
	  const object_T::kind_optional& kind_o = o.kind ();
	  int obj_attr = 0;

	  if (kind_o)
	    {
	      if (*kind_o == "rtm-extern")
		obj_attr |= C_OBJECT_EC_ATTR_RTM_EXTERN;
	      else if (*kind_o == "rtm-pointer")
		{
		  obj_attr |= C_OBJECT_EC_ATTR_RTM_POINTER;
		  c_rtm_variable = o.name ();
		}
	      else if (*kind_o == "rtm-object")
		obj_attr |= C_OBJECT_EC_ATTR_RTM_OBJECT;
	    }

	  c_codewriter.c_object ()
	    .push_back (c_object_ent_T (&o, &*value_o, obj_attr));
	}

      // includeの処理
      // 一旦入力後、ソートした結果をC_CodeWriteへ設定
      include_list_T il;
      for (blocks_T::includes_const_iterator ii (bs.includes ().begin ());
	   ii != bs.includes ().end (); ii++)
	{
	  const include_T& x = *ii;
	  il.push_back (std::make_pair (x.index (), &x));
	}

      std::sort (il.begin (), il.end (), cmp_include);

      for (include_list_T::iterator i (il.begin ()); i != il.end (); i++)
	{
	  const include_T* x = i->second;
	  c_codewriter.include_file ().push_back (x);
	}

      // シングルレートであればマルチレートの処理をdisable
      if (enable_multirate && is_single_rate ())
	enable_multirate = 0;

      return 0;
    }


    int
    CspReader::function_code (void)
    {
      const blocks_T& bs = xml_blocks ();

      // functionのタグをC_CodeWriteの連想配列へ設定
      for (blocks_T::function_const_iterator fi (bs.function ().begin ());
	   fi != bs.function ().end (); fi++)
	{
	  const function_T& f = *fi;
	  c_codewriter.functions ().add_function_map (f.name (), f);
	}

      // 全てのfunctionに対し、呼び出している関数の情報
      // (function_ent_T)のポインタを設定
      c_codewriter.functions ().correct ();

      return 0;
    }


    int
    CspReader::check_rtm_types (const char* beg, const char* end,
				rtm_type t, int *tid) const
    {
      try
	{
	  std::tr1::cmatch mr;

	  if (t == CHECK_RTM_VAR)
	    {
	      if (c_rtm_variable.length () == 0)
		return 0;

	      // Real-Time Model Objectを参照しているかどうか
	      std::tr1::regex rtmdef ("\\b"
					+ c_rtm_variable
					+ "[[:space:]]*"
					+ "->");

	      if (std::tr1::regex_search (beg, end, mr, rtmdef))
		return 1;
	    }
	  else if (t == CHECK_RTM_TID)
	    {
	      // TIDのコードがあるかどうか
	      std::tr1::regex tiddef ("->"
					"Timing\\."
					"TaskCounters\\."
					"TID\\[([[:digit:]]+)\\]");

	      if (std::tr1::regex_search (beg, end, mr, tiddef))
		{
		  if (tid)
		    {
		      std::istringstream iss (mr[1]);
		      iss >> *tid;
		    }
		  return 1;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		      << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		      << e.what() << std::endl;
	  return -1;
	}
      catch (...) {
	std::cerr << "Error in " << __FUNCTION__ << std::endl;
	return -1;
      }

      return 0;
    }

    void
    CspReader::depth_search_and_mark (CspBlockInfo* tail, int index,
				      const CspBlockInfo* subsys, int *color)
    {
      CspBlockInfo* binfo = block_list ()[index];
      const block_T* b = binfo->block ();

      color[index] = -1;	// gray

      // 対象が探索開始位置(SwitchCaseとかIf)ではない場合は、tailへマー
      // ジする
      if (binfo != tail)
	{
	  // 既に他にマージされていたらおかしい
	  if (binfo->merge_to ().size ()
	      && binfo->merge_to ().checkinfo (tail) == false
	      && b->blocktype () != "Constant")
	    {
	      std::cout << "Warning: " << b->name ()
			  << " merged to " << tail->block ()->name ()
			  << ", but already merged to other."
			  << std::endl;
	    }
	  binfo->merge_to () += tail;
	}

      if (b->blocktype () == "SubSystem")
	{
	  // SwitchCase/If Action SubSystemなので中のブロックを探索
	  for (csp_block_list_const_iterator
		 s (binfo->lower_block ().begin ());
	       s != binfo->lower_block ().end (); s++)
	    {
	      const CspBlockInfo* s_binfo = *s;
	      int s_index = s_binfo->id ();
	      if (color[s_index] == 0)
		depth_search_and_mark (tail, s_index, subsys, color);
	    }
	}

      csp_block_connect_list_T::iterator beg = binfo->forward ().begin ();
      csp_block_connect_list_T::iterator end = binfo->forward ().end ();

      for (csp_block_connect_list_T::iterator i (beg); i != end; i++)
	{
	  CspBlockInfo* n_binfo = i->head ();
	  int n_index = n_binfo->id ();
	  const block_T* nb = n_binfo->block ();

	  if (nb->blocktype () == "Merge"
	      && n_binfo->upper_subsystem () == subsys)
	    {
	      // 探索開始ブロックと同階層のMergeは終端なので探索終了

	      assert (n_binfo != tail);

	      // 未訪問なら探索開始ブロックへマージ
	      if (color[n_index] != 1)
		{
		  // Mergeが他のブロックにマージされていたら異常
		  if (n_binfo->merge_to ().size ()
		      && n_binfo->merge_to ().checkinfo (tail) == false)
		    {
		      std::cout << "Warning: " << nb->name ()
				<< " merged to "
				<< tail->block ()->name ()
				<< ", but already merged to other."
				<< std::endl;
		    }
		  n_binfo->merge_to () += tail;
		  color[n_index] = 1;
		}
	    }
	  else if (color[n_index] == 0)
	    {
	      // 未訪問のブロックなら探索
	      depth_search_and_mark (tail, n_index, subsys, color);
	    }
	}

      color[index] = 1;		// visited
    }

    /// \brief 間数名の長さ比較関数
    ///
    /// \param[in] x 比較1
    /// \param[in] y 比較2
    ///
    /// \retval true xの関数名が長い
    /// \retval false そうではない
    ///
    /// 正規表現の生成に用いるソートの比較関数なので長い順にする
    bool fnlen_cmp (function_ent_T* x, function_ent_T* y)
    {
      size_t xlen = 0;
      size_t ylen = 0;
      const function_T* xdefn = x->get_defn ();
      const function_T* ydefn = y->get_defn ();

      if (xdefn)
	xlen = xdefn->name ().length ();
      if (ydefn)
	ylen = ydefn->name ().length ();

      return xlen > ylen;
    }

    /// \brief string中のコードに、関数呼び出しがあれば、その関数のリス
    /// トを取得する
    ///
    /// \param[in] scode コード(string)
    /// \param[in] fpat 関数呼び出しを見つけるためのコンパイル済み正規表現
    /// \param[in] all_flist 全関数のリスト
    /// \param[in,out] code_flist コード中に見つかった関数のリスト
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    int
    check_function (const std::string& scode, std::tr1::regex& fpat,
		    const function_ptr_list_T& all_flist,
		    function_ptr_list_T& code_flist)
    {
      try
	{
	  std::tr1::cmatch mr;
	  size_t n = all_flist.size ();

	  const char* code = scode.c_str ();

	  while (code)
	    {
	      if (std::tr1::regex_search (code, mr, fpat) == false)
		break;

	      for (size_t i = 0; i < n; i++)
		{
		  if (mr[i + 2].matched == false)
		    continue;

		  function_ent_T* fe = all_flist[i];

		  if (code_flist.end () == std::find (code_flist.begin (),
						      code_flist.end (),
						      fe))
		    code_flist.push_back (fe);
		  break;
		}

	      code = mr[0].second;
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		      << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		      << e.what() << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    /// \brief テキストのリストに、関数呼び出しがあれば、その関数のリス
    /// トを取得する
    ///
    /// \param[in] texts テキストのリスト(text_list_T)
    /// \param[in] fpat 関数呼び出しを見つけるためのコンパイル済み正規表現
    /// \param[in] all_flist 全関数のリスト
    /// \param[in,out] code_flist コード中に見つかった関数のリスト
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// text_list_Tの要素毎にcheck_function()を実行する
    int
    check_function (const text_list_T& texts, std::tr1::regex& fpat,
		    const function_ptr_list_T& all_flist,
		    function_ptr_list_T& code_flist)
    {
      for (text_list_T::const_iterator ti (texts.begin ());
	   ti != texts.end (); ti++)
	{
	  const std::string& scode = *ti;
	  if (scode.length () > 0
	      && check_function (scode, fpat, all_flist, code_flist) == -1)
	    return -1;
	}
      return 0;
    }

    /// \brief 断片化したコードのリストに、関数呼び出しがあれば、その関
    /// 数のリストを取得する
    ///
    /// \param[in] frags 断片化したコードリスト(code_frag_list_T)
    /// \param[in] fpat 関数呼び出しを見つけるためのコンパイル済み正規表現
    /// \param[in] all_flist 全関数のリスト
    /// \param[in,out] code_flist コード中に見つかった関数のリスト
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// code_frag_list_Tの要素毎にcheck_function()を実行する
    int
    check_function (const code_frag_list_T& frags, std::tr1::regex& fpat,
		    const function_ptr_list_T& all_flist,
		    function_ptr_list_T& code_flist)
    {
      for (code_frag_list_T::const_iterator fi (frags.begin ());
	   fi != frags.end (); fi++)
	{
	  const code_frag_T& cf = *fi;
	  if (!cf)
	    continue;		// デリミタは無視
	  const std::string& scode = cf;
	  if (scode.length () > 0
	      && check_function (scode, fpat, all_flist, code_flist) == -1)
	    return -1;
	}
      return 0;
    }

    int
    CspReader::set_function_info (void)
    {
      std::string sfpat;

      function_map_T& fmap = c_codewriter.functions ();
      const function_ptr_list_T& flist = fmap.defn_list ();
      if (flist.size () == 0)
	return 0;

      // ソースコードに定義されている関数について、間数名の長い順にソー
      // トし、関数呼び出しを行っている正規表現を作成する。
      // ただし、ブロックの関数は除外
      function_ptr_list_T nflist;

      for (function_ptr_list_T::const_iterator i (flist.begin ());
	   i != flist.end (); i++)
	{
	  function_ent_T* fe = *i;
	  const function_T* defn = fe->get_defn ();
	  if (defn != NULL && defn->block ().size () > 0)
	    continue;
	  nflist.push_back (fe);
	}

      if (nflist.size ())
	{
	  std::sort (nflist.begin (), nflist.end (), fnlen_cmp);

	  for (function_ptr_list_T::const_iterator i (nflist.begin ());
	       i != nflist.end (); i++)
	    {
	      if (sfpat.length () == 0)
		sfpat += "\\b(";
	      else
		sfpat += '|';

	      sfpat += '(' + (*i)->get_defn ()->name () + ')';
	    }

	  sfpat += ")[[:space:]]*\\(";
	}

      try
	{
	  std::tr1::regex fpat;

	  if (sfpat.length ())
	    {
	      fpat = sfpat;

	      // Real-Time Modelに関する関数呼び出しを調べ、設定する

	      // Real-Time Model Objectの共通コード(rt_code)から呼び出
	      // す関数をC_CodeWriterのrt_code_funcに設定
	      if (check_function (c_codewriter.rt_code (), fpat, nflist,
				  c_codewriter.rt_code_func ()) == -1)
		return -1;

	      // Real-Time Model Objectの初期化コード(rt_init)から呼び
	      // 出す関数をC_CodeWriterのrt_code_funcに設定
	      if (check_function (c_codewriter.rt_init (), fpat, nflist,
				  c_codewriter.rt_code_func ()) == -1)
		return -1;

	      // 共通コード(common)から呼び出す関数をC_CodeWriterの
	      // comon_funcに設定
	      if (check_function (c_codewriter.common (), fpat, nflist,
				  c_codewriter.common_func ()) == -1)
		return -1;

	      // 共通初期化コード(common_init)から呼び出す関数を
	      // C_CodeWriterのcomon_funcに設定
	      if (check_function (c_codewriter.common_init (), fpat, nflist,
				  c_codewriter.common_func ()) == -1)
		return -1;
	    }

	  // CspNodeが呼び出す関数を調べ、設定する
	  for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	       ci != CspNode::csp_node ().end (); ci++)
	    {
	      CspNode& cn = *ci;

	      // task0とextoutについては無視
	      if (cn.info ()
		  & (CSP_NODE_INFO_TASK_0|CSP_NODE_INFO_TASK_EXTOUT))
		continue;

	      // CspNodeの呼び出し関数リスト
	      function_ptr_list_T& task_flist = cn.functions ();

	      // Real-Time Model Objectを使用する場合は、まず、
	      // C_CodeWriterのrt_code_funcをコピーする
	      if ((cn.info () & CSP_NODE_INFO_NEED_RTM)
		  && c_codewriter.rt_code_func ().size () > 1)
		task_flist = c_codewriter.rt_code_func ();

	      // ブロックが所有する関数(関数化サブシステム)を追加
	      for (csp_block_list_const_iterator
		     bi (cn.contain_blocks ().begin ());
		   bi != cn.contain_blocks ().end (); bi++)
		{
		  const CspBlockInfo* binfo = *bi;
		  const block_T* b = binfo->block ();

		  for (block_T::function_const_iterator
			 fi (b->function ().begin ());
		       fi != b->function ().end (); fi++)
		    {
		      const function_T& f = *fi;
		      function_map_T::iterator fmi = fmap.find (f.name ());
		      if (fmi == fmap.end ())
			{
			  std::cout << "Warning: Couldn't find function: "
				    << f.name () << ": in " << b->name ()
				    << std::endl;
			  continue;
			}

		      function_ent_T& fe = fmi->second;
		      if (std::find (task_flist.begin (),
				     task_flist.end (),
				     &fe) == task_flist.end ())
			{
			  task_flist.push_back (&fe);

			  if (std::find (fe.csp_nodes ().begin (),
					 fe.csp_nodes ().end (),
					 &cn) == fe.csp_nodes ().end ())
			    fe.csp_nodes ().push_back (&cn);
			}

		      if (cn.info () & CSP_NODE_INFO_NEED_RTM)
			continue;

		      const function_basic_writer&
			defn = fe.defn_basic_writer ();
		      if (defn)
			{
			  const function_T::code_optional&
			    co = defn.code ();
			  if (co)
			    {
			      const std::string& code = *co;
			      if (check_rtm (code) == 1
				  && (enable_multirate == 0
				      || check_tid (code) == 0))
				cn.info () |= CSP_NODE_INFO_NEED_RTM;
			    }
			}
		    }
		}

	      if (sfpat.length ())
		{
		  // 最後にCspNodeのコードから呼び出す関数を調べ、呼び
		  // 出す関数リストに設定する
		  for (CodeList::const_iterator cdi (cn.code ().begin ());
		       cdi != cn.code ().end (); cdi++)
		    {
		      const code_ent_T& ce = *cdi;

		      const std::string& t = ce.code ()->type ();
		      if (t != "init" && t != "task" && t != "update")
			continue;

		      const code_frag_list_T& cfl = ce.code_frag_list ();

		      if (check_function (cfl, fpat, nflist, task_flist) == -1)
			return -1;
		    }
		}
	    }

	  // ブロックの関数が二つ以上参照されていた場合、それは前提と異
	  // なるので警告する
	  for (function_map_T::const_iterator fmi (fmap.begin ());
	       fmi != fmap.end (); fmi++)
	    {
	      const function_ent_T& fe = fmi->second;
	      if (fe.csp_nodes ().size () > 1)
		std::cout << "Warning: block function " << fmi->first
			  << ": too many called" << std::endl;
	    }
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		      << e.what() << std::endl;
	  return -1;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		      << e.what() << std::endl;
	  return -1;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	  return -1;
	}

      return 0;
    }

    int
    CspReader::input_xml (const char* file,
			  const char* ns, const char* sc,
			  bool validation)
    {
      // BLXMLを入力する
      base_CspReader::load_xml (file, ns, sc, validation);

      if (!*this)
	{
	  std::cerr << "Error: Couldn't parse " << file
		      << ": " << strerror (errno) << std::endl;
	  return -1;
	}

      // BLXMLの主にblock以外の情報の読み込み
      if (EC_code ())
	return -1;

      // BLXMLの関数宣言・定義の読み込み
      if (function_code ())
	return -1;

      // 処理の準備
      prepare_process ();

      // サンプル時間の継承の解決
      if (enable_multirate && Inherit_SampleTime () != 0)
	return -1;

      // CspNode の作成
      Make_Nodes ();

      // CspNode 間の CspEdge の生成
      Make_Edges ();

      // コード生成に必要な情報の生成
      Make_Context ();

      // 情報のdump
      if (verbose)
	Dump_Nodes ();

      // コードから呼び出す関数の対応
      if (set_function_info ())
	return -1;

      return 0;
    }

    int
    CspReader::input_xml (const char* file, bool validation)
    {
      return input_xml (file, (char*)NULL, (char*)NULL, validation);
    }


    int
    CspReader::output_csp (const char* file)
    {
      CspGraph& g = get_graph ();
      if (file)
	{
	  std::ofstream out (file);
	  if (!out)
	    {
	      std::cerr << file << ": " << strerror (errno) << std::endl;
	      return -1;
	    }
	  boost::write_graphviz(out, g,
				my_make_label_writer (g),
				my_make_edge_writer (g),
				my_make_graph_writer (g),
				boost::get (boost::vertex_BlockID, g));
	}
      else
	{
	  boost::write_graphviz(std::cout, g,
				my_make_label_writer (g),
				my_make_edge_writer (g),
				my_make_graph_writer (g),
				boost::get (boost::vertex_BlockID, g));
	}
      return 0;
    }

    /// \brief ブロックが低レートから高レートの接続かどうかを判定する
    ///
    /// \param[in] binfo CspBlockInfo のポインタ
    ///
    /// \retval true 低レートから高レートへの接続
    /// \retval false そうではない
    static inline bool
    is_low_high_rate (const CspBlockInfo* binfo)
    {
      for (csp_block_connect_list_T::const_iterator
	     fi (binfo->forward ().begin ());
	   fi != binfo->forward ().end (); fi++)
	{
	  if (binfo->get_SampleTime () > fi->head ()->get_SampleTime ())
	    return true;
	}
      return false;
    }

    void
    CspReader::Make_Nodes (void)
    {
      // ブロックのベクタのサイズ
      size_t size = block_list ().size ();

      // ブロック順序のベクタ(ソートする)
      csp_block_list_T block_sequence (size, (CspBlockInfo*)NULL);

      // 1. 外部入出力の判定とブロックのソート準備
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  // ソート用ベクタの設定
	  block_sequence[binfo->id ()] = binfo;
	  if ((long)binfo->id () > max_task_id)
	    max_task_id = binfo->id ();

	  // 外部入出力の有無のチェック
	  if (binfo->upper () == NULL)
	    {
	      if (b->blocktype () == "Outport")
		need_extout = true;
	      else if (b->blocktype () == "Inport")
		need_extin = true;
	    }

	  // コードの行番号取得
	  int line = -1;
	  for (block_T::code_const_iterator ci (b->code ().begin ());
	       ci != b->code ().end (); ci++)
	    {
	      if (ci->type () == "task")
		{
		  code_T::line_optional lo = ci->line ();
		  if (lo)
		    {
		      if (line == -1 || line < *lo)
			line = *lo;
		    }
		}
	    }

	  if (line != -1)
	    binfo->line_position (line);
	}

      // 新規作成タスクの開始番号
      new_task_id = (max_task_id / 100 + 1) * 100;

      // 2. コード上でマージされているブロックの処理
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  // 既にマージされている
	  if (b->merged_to ().size () || b->bound_to ().size ())
	    continue;

	  // 例えばDotProduct等
	  if (binfo->merge_blocks ().size ())
	    {
	      for (csp_block_list_const_iterator
		     j (binfo->merge_blocks ().begin ());
		   j != binfo->merge_blocks ().end (); j++)
		{
		  CspBlockInfo* m = *j;
		  if (m->merge_to ().size ()
		      && m->merge_to ().checkinfo (binfo) == false
		      && m->block ()->blocktype () != "Constant")
		    {
		      std::cout << "Warning: "
				  << m->block ()->name ()
				  << " merged to "
				  << binfo->block ()->name ()
				  << ", but already merged to other."
				  << std::endl;
		    }
		  m->merge_to () += binfo;
		}
	      continue;
	    }

	  // SwitchCase/Ifの場合、含まれるブロックをサーチし、マージさ
	  // れていると扱う
	  if (b->blocktype () == "SwitchCase" || b->blocktype () == "If")
	    {
	      std::vector < int > color (size, 0);
	      depth_search_and_mark (binfo, binfo->id (),
				     binfo->upper_subsystem (), color.data ());
	      continue;
	    }
	}

      // 3. ユーザーが結合と指示したブロックの処理
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  // このブロックにマージせよと指示されているブロックを対象
	  if (binfo->bind_blocks ().size ())
	    {
	      // 既に他にマージされているブロックは非対象
	      if (binfo->merge_to ().size ())
		{
		  std::cout << "Warning: "
			      << b->name ()
			      << '[' << binfo->id () << ']'
			      << " ignore bind Blocks, "
			      << "this is already merged by them:"
			      << std::endl;
		  for (csp_block_list_const_iterator
			 j (binfo->merge_to ().begin ());
		       j != binfo->merge_to ().end (); j++)
		    {
		      const CspBlockInfo* m = *j;
		      std::cout << "  " << m->block ()->name ()
				  << '[' << m->block ()->id () << ']'
				  << std::endl;
		    }
		  continue;
		}

	      // S-Function以外でUPDATEがあるものはデッドロックの可能性
	      // があるので、非対象とする
	      if (binfo->has_update () && !binfo->is_sfunction ())
		{
		  std::cout << "Warning: "
			      << b->name ()
			      << '[' << binfo->id () << ']'
			      << " it has Update, "
			      << " ded-lock may occur by this bind operation"
			      << std::endl;
		  continue;
		}

	      // マージするブロックを処理
	      for (csp_block_list_const_iterator
		     j (binfo->bind_blocks ().begin ());
		   j != binfo->bind_blocks ().end (); j++)
		{
		  CspBlockInfo* m = *j;
		  const block_T* mb = m->block ();

		  // レートが異なるものはマージできないので処理を打ちきる
		  if (binfo->get_SampleTime () != m->get_SampleTime ())
		    {
		      std::cout << "Warning: "
				  << b->name ()
				  << " bind ignore "
				  << mb->name ()
				  << " different rate"
				  << std::endl;
		      break;	// don't continue
		    }

		  // Enabled/Triggered SubSystemやStateFlow、
		  // If/SwitchCaseは、マージしないので処理を打ちきる
		  if ((m->is_subsystem ()
		       && (m->subsystem_type () == CspBlockInfo::ITERATOR
			   || m->subsystem_type () == CspBlockInfo::TRIGGERED
			   || m->subsystem_type () == CspBlockInfo::ENABLED
			   || (m->subsystem_type ()
			       == CspBlockInfo::ENABLED_TRIGGERED)
			   || m->subsystem_type () == CspBlockInfo::STATEFLOW))
		      || mb->blocktype () == "If"
		      || mb->blocktype () == "SwitchCase")
		    {
		      std::cout << "Warning: "
				  << "ignore merge "
				  << mb->blocktype () << ':' << mb->name ()
				  << " to "
				  << b->name ()
				  << std::endl;
		      break;	// don't continue
		    }

		  // 既に自分以外のブロックにマージされている場合で
		  // Constant以外であれば、マージできないので処理を打ち
		  // きる
		  if (m->merge_to ().size ()
		      && m->merge_to ().checkinfo (binfo) == false
		      && m->block ()->blocktype () != "Constant")
		    {
		      std::cout << "Warning: "
				  << mb->name ()
				  << " merged to "
				  << b->name ()
				  << ", but already merged to other."
				  << std::endl;
		      break;	// don't continue
		    }

		  // bind
		  m->merge_to () += binfo;
		}
	      continue;
	    }
	}

      // 4. ブロックのタスク属性設定
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  CspBlockInfo* m = NULL;

	  // マージされているブロックがあれば、自分にマージの先頭ブロッ
	  // ク(MARGE_HEAD)、マージしているブロックにMARGEDを設定
	  if (binfo->merge_to ().size ())
	    {
	      for (csp_block_list_const_iterator
		     j (binfo->merge_to ().begin ());
		   j != binfo->merge_to ().end (); j++)
		{
		  m = *j;
		  m->task_info () |= TASK_INFO_MERGE_HEAD;

		  // 状態を持つSubSystemをマージするなら、
		  // MERGE_STATE_SUBSYSを設定
		  if (binfo->is_subsystem ()
		      && (binfo->subsystem_type () == BlockInfo::TRIGGERED
			  || (binfo->subsystem_type ()
			      == BlockInfo::ENABLED_TRIGGERED)
			  || binfo->subsystem_type () == BlockInfo::ENABLED
			  || b->state ().size ()))
		    m->task_info () |= TASK_INFO_MERGE_STATE_SUBSYS;
		}
	      binfo->task_info () |= TASK_INFO_MERGED;
	    }
	  else
	    {
	      // ユーザ指定で結合したブロックがあり、そのブロックのマー
	      // ジ先が自分であれば、そのブロックに結合に先頭ブロック
	      // (BIND_HEAD)を設定
	      for (csp_block_list_const_iterator
		     j (binfo->bind_blocks ().begin ());
		   j != binfo->bind_blocks ().end (); j++)
		{
		  const CspBlockInfo* b = *j;
		  if (b->merge_to ().checkinfo (binfo))
		    {
		      binfo->task_info () |= TASK_INFO_BIND_HEAD;
		      break;
		    }
		}
	    }

	  unsigned int val = binfo->task_info ();

	  // Constantの場合
	  if (b->blocktype () == "Constant")
	    val |= TASK_INFO_CONSTANT;

	  if (ignore_to_sink != 0
	      && (b->blocktype () == "ToWorkspace"
		  || b->blocktype () == "Scope"
		  || b->blocktype () == "FloatingScope"
		  || b->blocktype () == "Terminator"
		  || b->blocktype () == "Display"
		  || b->blocktype () == "ToFile"
		  || b->blocktype () == "Ground"))
	    {
	      // sinkブロックで、オプションで無視が指定されている場
	      val |= TASK_INFO_IGNORE_BLOCK;
	    }
	  else if (b->blocktype () == "DataStoreMemory")
	    {
	      // DataStoreMemoryの場合の処理
	      // なおこのブロック自身は無視する
	      val |= (TASK_INFO_HAS_DATASTOREMEMORY|TASK_INFO_IGNORE_BLOCK);

	      // DataStoreMemoryの名前があれば連想配列に設定する
	      const block_T::DataStoreName_optional& nm = b->DataStoreName ();
	      if (nm)
		{
		  assert (dsm_map ().find (*nm) == dsm_map ().end ());
		  dsm_map ().insert (std::make_pair (*nm, dsm_entry (binfo)));
		}
	    }
	  else if (b->blocktype () == "RateTransition"
		   && binfo->has_update ()
		   && is_low_high_rate (binfo))
	    {
	      // 遅延を持ち、低レートから高レートへのRateTransition
	      val |= TASK_INFO_DLY_RATETRANSITION;
	    }
	  else
	    {
	      // 一般的なブロックの場合
	      int x = 0;
	      int xi = 0;
	      for (block_T::code_const_iterator c (b->code ().begin ());
		   c != b->code ().end (); c++)
		{
		  if (c->type () == "task")
		    x++;
		  else if (c->type () == "init")
		    {
		      x++;
		      xi++;
		    }
		  else if (c->type () == "update")
		    {
		      x++;
		      val |= TASK_INFO_HAS_UPDATE; // has update code
		    }
		}

	      if (x == 0)
		val |= TASK_INFO_NOCODE; // no code

	      if (binfo->is_sfunction ())
		val |= TASK_INFO_SFUNCTION;

	      // 条件付きサブシステムのOutportの場合
	      if (x != 0
		  && b->blocktype () == "Outport"
		  && binfo->upper_subsystem () != NULL)
		{
		  bool ok = false;
		  if (b->forward ().size ())
		    ok = true;
		  for (block_T::forward_const_iterator
			 fi (b->forward ().begin ());
		       fi != b->forward ().end (); fi++)
		    {
		      if (fi->type () != "init")
			{
			  ok = false;
			  break;
			}
		    }

		  // assert (x == xi && ok == true);

		  if (ok)
		    val |= TASK_INFO_VIRTUALOUTPORT_START;
		}
	    }

	  // DataStorememoryの場合
	  if (b->blocktype () == "DataStoreRead"
	      || b->blocktype () == "DataStoreWrite")
	    val |= TASK_INFO_USE_DATASTOREMEMORY;

	  size_t fn = b->forward ().size (); // 後段の数
	  size_t bn = b->backward ().size (); // 前段の数

	  if (fn == 0)
	    val |= TASK_INFO_ZFORW; // 後段なし
	  else if (fn == 1)
	    val |= TASK_INFO_1FORW; // 一つだけ

	  if (bn == 0)
	    val |= TASK_INFO_ZBACKW; // 前段なし
	  else if (bn == 1)
	    val |= TASK_INFO_1BACKW; // 一つだけ

	  if (binfo->upper () == NULL)
	    {
	      if (b->blocktype () == "Inport")
		val |= TASK_INFO_EXTIN; // 外部入力
	      else if (b->blocktype () == "Outport")
		val |= TASK_INFO_EXTOUT; // 外部出力
	    }

	  // 以下を結合可能なブロックの候補とする
	  // ・コードがない
	  // ・後段が一つ、前段がないか一つ
	  if ((val & TASK_INFO_NOCODE) == TASK_INFO_NOCODE
	      || ((val & TASK_INFO_1FORW) == TASK_INFO_1FORW
		  && (val & (TASK_INFO_ZBACKW|TASK_INFO_1BACKW)) != 0))
	    val |= TASK_INFO_CONCATENATABLE;

	  // S-Functionの場合
	  if ((val & TASK_INFO_SFUNCTION) == 0)
	    {
	      // 以下の場合、結合対象としない
	      // ・外部入力がある
	      // ・CONSTANT(後で定数伝播を行う)
	      // ・UPDATEを持つ
	      if (val
		  & (TASK_INFO_EXTIN|TASK_INFO_CONSTANT|TASK_INFO_HAS_UPDATE))
		val |= TASK_INFO_NOT_CONCAT;

	      // 以下の場合、結合対象の探索開始ブロックとしない
	      // ・コードがない
	      // ・CONSTANT(後で定数伝播を行う)
	      // ・UPDATEを持つ
	      if (val
		  & (TASK_INFO_NOCODE|TASK_INFO_CONSTANT|TASK_INFO_HAS_UPDATE))
		val |= TASK_INFO_NO_CONCAT_BASE;
	    }

	  binfo->task_info () = val;
	}

      // 5. CspNodeの作成
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  if (binfo == NULL)
	    continue;

	  int ind = binfo->id ();

	  // マージされているなら非対象
	  if (binfo->merge_to ().size ())
	    continue;

	  unsigned int& val = binfo->task_info ();

	  // Constantなら非対象
	  if (val & TASK_INFO_CONSTANT)
	    continue;

	  // 無視するブロックなら非対称
	  if (val & TASK_INFO_IGNORE_BLOCK)
	    {
	      if (verbose)
		std::cout << "Ignore Block[" << ind << "]: "
			    << binfo->block ()->blocktype ()
			    << ':'
			    << binfo->block ()->name () << std::endl;
	      continue;
	    }

	  // UPDATEがあるか
	  bool node_update = false;

	  if (binfo->has_update ()
	      && !binfo->is_sfunction ()
	      && !binfo->is_subsystem ()
	      && (val & TASK_INFO_MERGE_HEAD) == 0)
	    node_update = true;

	  // 結合可能なUPDATEを持つブロックで後段は一つ、かつレートが同
	  // じであれば、結合可能なので非対象
	  if (do_concat_delay_block != 0
	      && node_update == true && binfo->forward ().size () == 1)
	    {
	      const CspBlockInfo* f_binfo = binfo->forward ().begin ()->head ();
	      if (binfo->get_SampleTime() == f_binfo->get_SampleTime())
		continue;
	    }

	  // CspNodeの属性を設定する
	  const block_T* b = binfo->block ();
	  bool one_block = false;
	  unsigned int cn_info = 0;
	  if (binfo->is_subsystem ())
	    {
	      if (binfo->subsystem_type () == CspBlockInfo::ITERATOR)
		{
		  // Iterator SubSystemの場合
		  one_block = true;
		  cn_info = CSP_NODE_INFO_ITERATOR_SUBSYSTEM;
		}
	      else if (binfo->subsystem_type () == CspBlockInfo::STATEFLOW)
		{
		  // Stateflow(SubSystem)の場合
		  one_block = true;
		  cn_info = CSP_NODE_INFO_STATEFLOW_SUBSYSTEM;
		}
	      else
		{
		  // atomic/enabled/triggered/action subsystemは
		  // BLXML側での設定に従う
		  switch (binfo->subsystem_type ())
		    {
		    case CspBlockInfo::ATOMIC:
		      cn_info = CSP_NODE_INFO_ATOMIC_SUBSYSTEM;
		      break;
		    case CspBlockInfo::ENABLED:
		      cn_info = CSP_NODE_INFO_ENABLED_SUBSYSTEM;
		      break;
		    case CspBlockInfo::TRIGGERED:
		      cn_info = CSP_NODE_INFO_TRIGGERED_SUBSYSTEM;
		      break;
		    case CspBlockInfo::ENABLED_TRIGGERED:
		      cn_info = (CSP_NODE_INFO_ENABLED_SUBSYSTEM
				 | CSP_NODE_INFO_TRIGGERED_SUBSYSTEM);
		      break;
		    case CspBlockInfo::ACTION:
		      cn_info = CSP_NODE_INFO_ACTION_SUBSYSTEM;
		      break;

		    case CspBlockInfo::NONE:
		    case CspBlockInfo::ITERATOR:
		    case CspBlockInfo::STATEFLOW:
		      // NOT REACHED
		      break;
		    }

		  // SubSystem内のブロックが全てマージされ、単一の
		  // SubSystemになっているかどうか調べる
		  bool all_merged = true;
		  csp_block_list_const_iterator
		    mb (binfo->merge_blocks ().begin ());
		  csp_block_list_const_iterator
		    me (binfo->merge_blocks ().end ());
		  csp_block_list_const_iterator
		    lb (binfo->lower_block ().begin ());
		  csp_block_list_const_iterator
		    le (binfo->lower_block ().end ());

		  for (csp_block_list_const_iterator s (lb); s != le; s++)
		    {
		      if (std::find (mb, me, *s) == me)
			{
			  all_merged = false;
			  break;
			}
		    }

		  if (all_merged)
		    {
		      one_block = true;
		      cn_info |= CSP_NODE_INFO_ONE_SUBSYSTEM;
		    }
		}
	    }
	  else if (b->blocktype () == "SwitchCase")
	    {
	      // SwitchCaseの場合
	      one_block = true;
	      cn_info = CSP_NODE_INFO_SWITCHCASE;
	    }
	  else if (b->blocktype () == "If")
	    {
	      // Ifの場合
	      one_block = true;
	      cn_info = CSP_NODE_INFO_IF;
	    }
	  else if (val & TASK_INFO_MERGE_STATE_SUBSYS)
	    {
	      // 状態を持つSubSystemをマージしている場合は、単一のブロッ
	      // クとして扱う
	      one_block = true;
	    }

	  // CspNodeの対象がRateTransitionの場合
	  if (val & TASK_INFO_DLY_RATETRANSITION)
	    cn_info |= CSP_NODE_INFO_DLY_RATETRANSITION;

	  // エッジがあり、コードを持って入ればCspNodeとして作成する
	  if (binfo->forward ().size ()
	      || binfo->backward ().size ()
	      || (one_block && (val & TASK_INFO_NOCODE) == 0))
	    {
	      CspNode& c = CspNode::add_node (get_graph(), binfo);
	      c.oneblock (one_block);
	      if (node_update)
		c.info () |= CSP_NODE_INFO_HAS_UPDATE;
	      c.info () |= cn_info;
	      if (val & TASK_INFO_LOWRATE_PULSEGEN)
		c.info () |= CSP_NODE_INFO_LOWRATE_PULSEGEN;
	      val |= TASK_INFO_NODE_CREATED;
	      if (verbose)
		{
		  std::cout << "Create Node[" << binfo->id () << "]: "
			      << binfo->block ()->blocktype ()
			      << ":"
			      << binfo->block ()->name ();
		  if (one_block)
		    std::cout << "<oneblock>";
		  std::cout << std::endl;
		}
	    }
	}

      // 6. マージ対象のブロックをCspNodeへ結合
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  if (binfo == NULL)
	    continue;

	  CspBlockInfo* tbinfo = NULL;

	  // マージ先があれば、それに結合する
	  for (csp_block_list_const_iterator j (binfo->merge_to ().begin ());
	       j != binfo->merge_to ().end (); j++)
	    {
	      tbinfo = *j;

	      // マージ先はCspNodeとして作られていないので無視
	      // おそらくは他のブロックと結合することになっている
	      unsigned int val = tbinfo->task_info ();
	      if ((val & TASK_INFO_NODE_CREATED) == 0)
		continue;

	      // CspNodeがない。これは通常起こり得ないのだが
	      CspNode* c = tbinfo->get_node ();
	      if (c == NULL)
		{
		  std::cerr << "Error: CSP Process not found: "
			      << tbinfo->block ()->name ()
			      << '[' << tbinfo->id () << ']'
			      << " merge "
			      << binfo->block ()->name ()
			      << '[' << binfo->id () << ']'
			      << std::endl;
		  continue;
		}

	      // CspNode内のブロックとして結合
	      c->add_block (binfo);

	      // 結合したブロックのUPDATEと低レート化する
	      // PulseGeneratorの属性をCspNodeに設定する
	      if (tbinfo->has_update () && !tbinfo->is_sfunction ())
		c->info () |= CSP_NODE_INFO_HAS_UPDATE;

	      if (val & TASK_INFO_LOWRATE_PULSEGEN)
		c->info () |= CSP_NODE_INFO_LOWRATE_PULSEGEN;

	      // 状態を持つサブシステムをマージするケースでは、CspNode
	      // にSubSystemのタイプを設定する
	      if (val & TASK_INFO_MERGE_STATE_SUBSYS)
		{
		  if (binfo->is_subsystem ())
		    {
		      switch (binfo->subsystem_type ())
			{
			case BlockInfo::ENABLED:
			  c->info () |= CSP_NODE_INFO_ENABLED_SUBSYSTEM;
			  break;
			case BlockInfo::TRIGGERED:
			  c->info () |= CSP_NODE_INFO_TRIGGERED_SUBSYSTEM;
			  break;
			case BlockInfo::ENABLED_TRIGGERED:
			  c->info () |= (CSP_NODE_INFO_ENABLED_SUBSYSTEM
					 |CSP_NODE_INFO_TRIGGERED_SUBSYSTEM);
			  break;
			default: // none
			  break;
			}
		    }
		}

	      // マージするブロックがRateTransitionの場合
	      if (val & TASK_INFO_DLY_RATETRANSITION)
		c->info () |= CSP_NODE_INFO_DLY_RATETRANSITION;
	    }
	}

      // 7. Delayブロック(UPDATEを持つ)を結合する
      while (do_concat_delay_block)
	{
	  int change = 0;

	  blockinfo_pair_list_T yet;

	  for (csp_block_list_const_iterator i (block_list ().begin ());
	       i != block_list ().end (); i++)
	    {
	      CspBlockInfo* binfo = *i;
	      if (binfo == NULL)
		continue;

	      // 既に処理済み
	      if (binfo->get_nodes ().size () > 0
		  || binfo->merge_to ().size ())
		continue;

	      // 結合可能なUPDATEブロックではない
	      if (!binfo->has_update ()
		  || binfo->is_sfunction ()
		  || binfo->is_subsystem ()
		  || (binfo->task_info () & TASK_INFO_MERGE_HEAD) != 0
		  || binfo->forward ().size () != 1)
		continue;

	      for (csp_block_connect_list_T::iterator
		     fi (binfo->forward ().begin ());
		   fi != binfo->forward ().end (); fi++)
		{
		  CspBlockInfo* tbinfo = fi->head ();
		  CspNode* tc = tbinfo->get_node ();
		  if (tc == NULL)
		    {
		      // CspNodeがまだ作られていない場合、無視するブロッ
		      // クでなければ、そのブロックを未処理として保持
		      unsigned int tval = tbinfo->task_info ();
		      if (tval & TASK_INFO_IGNORE_BLOCK)
			continue;
		      yet.push_back (std::make_pair (binfo, tbinfo));
		    }
		  else if (tc->head_block ()->get_SampleTime()
			   != binfo->get_SampleTime())
		    {
		      // レートが同じでなければマージしない
		      continue;	// don't merge;
		    }
		  else
		    {
		      // CspNodeに結合
		      tc->add_block (binfo);
		      tc->info () |= CSP_NODE_INFO_HAS_UPDATE;
		      change = 1;
		    }
		}
	    }

	  // 未処理ブロックがないなら終了
	  if (yet.size () == 0)
	    break;

	  // 変更が行われていれば継続
	  if (change)
	    continue;

	  // 変更もなく、未処理があればエラー
	  for (blockinfo_pair_list_T::iterator i (yet.begin ());
	       i != yet.end (); i++)
	    {
	      std::cerr << "Error: Not Found propagate block: "
			  << i->first->block ()->name ()
			  << std::endl;
	      has_error = true;
	    }

	  return;
	}

      // 8. 定数伝播。Constantは後段へ結合する
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  if (binfo == NULL)
	    continue;

	  if (binfo->merge_to ().size ())
	    continue;

	  unsigned int val = binfo->task_info ();
	  if (val & TASK_INFO_CONSTANT)
	    {
	      // Constantのブロックは後段が含まれるCspNodeへ結合する
	      for (csp_block_connect_list_T::const_iterator
		     fi (binfo->forward ().begin ());
		   fi != binfo->forward ().end (); fi++)
		{
		  CspBlockInfo* tbinfo = fi->head ();
		  CspNode* tc = tbinfo->get_node ();
		  if (tc == NULL)
		    {
		      std::cerr << "Warning: Not Found propagate block: "
				  << tbinfo->block ()->name ()
				  << std::endl;

		      // no propagate
		      continue;
		    }

		  // add_block ignore duplicated
		  // (e.g. already merged by `incorporates')
		  tc->add_block (binfo);
		}
	    }
	}

      // 9. CspNode内のブロックをコードの行番号で出現順にソートする
      for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	   ci != CspNode::csp_node ().end (); ci++)
	{
	  CspNode& c = *ci;

	  if (!c.oneblock ())
	    {
	      std::sort (c.contain_blocks ().begin (),
			 c.contain_blocks ().end (), cmp_line);
	    }
	}

      // 10. 全てのブロックを行番号順にソートし、CspNodeの出現順を決める
      std::sort (block_sequence.begin (), block_sequence.end (), cmp_line);

      for (size_t i = 0; i < block_sequence.size (); i++)
	{
	  CspBlockInfo* binfo = block_sequence[i];
	  if (binfo)
	    {
	      // 二つ以上のブロックにマージされているブロックは非対象とする
	      const CspNodePtrList& cpl = binfo->get_nodes ();
	      if (cpl.size () != 1)
		continue;

	      CspNode* c = *cpl.begin ();
	      if (c && c->head_block () == binfo)
		c->pos (i);
	    }
	}
    }

    void
    CspReader::Make_Edges (void)
    {
      for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	   ci != CspNode::csp_node ().end (); ci++)
	{
	  CspNode& c = *ci;

	  // Trigger/Enable Subsystemのイベントのエッジを設定する
	  CspBlockInfo* binfo = c.head_block ();
	  CspBlockInfo* bi_sys = find_multirate_subsystem (binfo);
	  if ((binfo->task_info () & TASK_INFO_VIRTUALOUTPORT_START) == 0
	      && binfo != bi_sys && bi_sys != NULL)
	    {
	      CspNode* cn_sys = bi_sys->get_node ();
	      assert (cn_sys);
	      CspBlockInfo* xt_binfo = cn_sys->head_block ();
	      CspEdgeList::iterator xt_e = cn_sys->forw_edges ().end ();
	      CspEdgeList::iterator
		xt_x = std::find (cn_sys->forw_edges ().begin (), xt_e, &c);
	      if (xt_x == xt_e)
		{
		  std::pair < CspEdge*, CspEdge* >
		    x = cn_sys->add_edge (&c);
		  CspEdge* f_edge = x.first;
		  CspEdge* b_edge = x.second;
		  f_edge->new_connect (xt_binfo, binfo);
		  b_edge->new_connect (xt_binfo, binfo);
		  f_edge->info () |= CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT;
		  b_edge->info () |= CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT;
		  if (verbose)
		    std::cout << "Add event " << xt_binfo->block ()->name ()
				<< " -> " << binfo->block ()-> name ()
				<< std::endl;
		}
	    }

	  // CspNode内のブロックの接続関係からエッジを生成する
	  for (csp_block_list_const_iterator
		 j (c.contain_blocks ().begin ());
	       j != c.contain_blocks ().end (); j++)
	    {
	      CspBlockInfo* t_binfo = *j;

	      // Constantおよび無視するブロックは非対象
	      if (t_binfo->task_info () & TASK_INFO_CONSTANT)
		continue;

	      if (t_binfo->task_info () & TASK_INFO_IGNORE_BLOCK)
		continue;

	      // 後段のブロックを処理
	      for (csp_block_connect_list_T::iterator
		     f (t_binfo->forward ().begin ());
		   f != t_binfo->forward ().end (); f++)
		{
		  // エッジ(CspBlockConnect)のHEADブロック
		  CspBlockInfo* h_binfo = f->head ();

		  // CspNodeが作られていない場合は無視(マージされている
		  // か無視するブロック)
		  const CspNodePtrList& hcpl = h_binfo->get_nodes ();
		  if (hcpl.size () == 0)
		    continue;

		  // 一般にCspNodeは一つだが、複数のブロックにマージさ
		  // れているConstantへの初期値送信が存在する
		  assert (hcpl.size () == 1
			  || ((h_binfo->task_info () & TASK_INFO_CONSTANT)
			      && hcpl.size () >= 1));
		  for (CspNodePtrList::const_iterator cpli (hcpl.begin ());
		       cpli != hcpl.end (); cpli++)
		    {
		      CspNode* h_c = *cpli;

		      // 同じCspNodeなら不要
		      if (&c == h_c)
			continue;

		      // 対となる逆方向のCspBlockConnectがないなら異常
		      csp_block_connect_list_T::iterator
			t_e = h_binfo->backward ().end ();
		      csp_block_connect_list_T::iterator
			b = std::find (h_binfo->backward ().begin (), t_e, *f);

		      assert (b != t_e);

		      // エッジ(CspEdge)を新規に作るか、既存のものを見つける
		      CspEdge* forw_edge = NULL;
		      CspEdge* back_edge = NULL;
		      CspEdgeList::iterator h_e = c.forw_edges ().end ();
		      CspEdgeList::iterator
			l = std::find (c.forw_edges ().begin (), h_e, h_c);
		      if (l == h_e)
			{
			  // CspEdgeのペアを作成
			  std::pair< CspEdge*, CspEdge* >
			    ep = c.add_edge (h_c);
			  forw_edge = ep.first;
			  back_edge = ep.second;
			}
		      else
			{
			  // 既存のCspEdge
			  forw_edge = &*l;
			  back_edge = forw_edge->pair ();
			}

		      // HEADブロックにUpdateがあるならこのエッジはUpdateの
		      // 変数がある
		      if (h_binfo->has_update () && !h_binfo->is_sfunction ())
			{
			  forw_edge->info () |= CSP_EDGE_INFO_HAS_UPDATE;
			  back_edge->info () |= CSP_EDGE_INFO_HAS_UPDATE;
			}

		      // 条件付きSubSystemの初期値送信
		      if (f->link ()->type () == "init")
			{
			  forw_edge->info () |= CSP_EDGE_INFO_INITIAL_VALUE;
			  back_edge->info () |= CSP_EDGE_INFO_INITIAL_VALUE;
			}

		      // CspEdgeに、それぞれのCspBlockConnectを登録
		      forw_edge->block_connect_list ().push_back (&*f);
		      back_edge->block_connect_list ().push_back (&*b);
		    }
		}
	    }
	}

      // 孤立したブロック、末端(出力がない)ブロックを無視する場合
      if (ignore_isolated_block || ignore_leaf_block)
	{
	  int change;
	  do {
	    change = 0;

	    cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	    while (ci != CspNode::csp_node ().end ())
	      {
		CspNode& x = *ci;

		// 入力側では初期値送信のエッジはカウントしない
		size_t bedges = 0;
		for (CspEdgeList::const_iterator
		       ei (x.back_edges ().begin ());
		     ei != x.back_edges ().end (); ei++)
		  {
		    if ((ei->info () & CSP_EDGE_INFO_INITIAL_VALUE) == 0)
		      bedges++;
		  }

		// 単一化したブロックではなく、入力も出力もない場合は、
		// 孤立下ブロックとして削除する
		//
		// 単一化したブロックではなく、SubSystemでもなく、出力
		// がないブロックは、末端のブロックとして削除する
		if ((ignore_isolated_block != 0
		     && !x.oneblock ()
		     && x.forw_edges ().size () == 0
		     && bedges == 0)
		    || (ignore_leaf_block != 0
			&& !x.oneblock ()
			&& !x.head_block ()->is_subsystem ()
			&& x.forw_edges ().size () == 0))
		  {
		    if (verbose)
		      std::cout << "Remove Block["
				  << x.head_block ()->id ()
				  << "]: "
				  << x.head_block ()->block ()-> name ()
				  << std::endl;
		    ci = CspNode::remove_node (ci);
		    change = 1;
		  }
		else
		  ++ci;
	      }
	  } while (change);

#if 0
	  // 整合性をチェックする(デバッグ用)
	  std::vector < CspNode* > l;
	  for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	       ci != CspNode::csp_node ().end (); ci++)
	    {
	      CspNode& x = *ci;
	      l.push_back (&x);
	    }

	  for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	       ci != CspNode::csp_node ().end (); ci++)
	    {
	      for (CspEdgeList::iterator i (ci->forw_edges ().begin ());
		   i != ci->forw_edges ().end (); i++)
		{
		  CspNode* f = *i;
		  std::vector< CspNode* >::iterator
		      x = std::find (l.begin (), l.end (), f);
		  if (x == l.end ())
		    {
		      std::cout << "csp forw not found:"
				  << ci->id ()
				  << " -> "
				  << (void *)f
				  << std::endl;
		    }
		}

	      for (CspEdgeList::iterator i (ci->back_edges ().begin ());
		   i != ci->back_edges ().end (); i++)
		{
		  CspNode* f = *i;
		  std::vector< CspNode* >::iterator
		      x = std::find (l.begin (), l.end (), f);
		  if (x == l.end ())
		    {
		      std::cout << "csp back not found:"
				  << (void *)f
				  << " -> "
				  << ci->id ()
				  << std::endl;
		    }
		}
	    }
#endif
	}
    }


    int
    CspReader::Make_Context (void)
    {
      // Task0
      std::string t0_name ("t0");
      CspNode* cn_t0 = NULL;

      // extout
      std::string extout_name ("extout");
      CspNode* cn_extout = NULL;
      unsigned int extout_id = 0;

      // CspNod内の情報生成
      CspNode::correct_nodes ();

#if 0
      // Kalray用オプション
      if (code_target == 's')
	{
	  if ((need_extin || need_extout)
	      && do_create_t0 == 0)
	    {
	      std::cout << "Warning: force set --create-t0=y for Kalray MPAA"
			<< std::endl;
	      do_create_t0 = 1;
	    }

	  if (enable_multirate == 1)
	    enable_multirate = 0; // disable
	}
#endif

      // task0を生成するオプション指定
      if (do_create_t0)
	{
	  // task0の生成

	  CspGraph& g = get_graph();
	  cn_t0 = &CspNode::add_node (g, (CspBlockInfo*)NULL);
	  cn_t0->info () = CSP_NODE_INFO_TASK_0;
	  if (need_extin)
	    cn_t0->info () |= CSP_NODE_INFO_EXTIN;
	  cn_t0->id () = 0;

	  CspGraphVertex& gv_t0 = cn_t0->setup_vertex ();
	  boost::put(boost::vertex_name, g, gv_t0, t0_name);
	  boost::put(boost::vertex_BlockID, g, gv_t0, 0);
	  boost::put(boost::vertex_Label, g, gv_t0, t0_name);

	  if (enable_multirate)
	    cn_t0->rate ().rate () = 1; // base rate

	  // extoutの生成

	  extout_id = new_task_id++;

	  cn_extout = &CspNode::add_node (g, (CspBlockInfo*)NULL);
	  cn_extout->info () = (CSP_NODE_INFO_TASK_EXTOUT
				|CSP_NODE_INFO_EXTOUT);
	  cn_extout->id () = extout_id;

	  CspGraphVertex& gv_extout = cn_extout->setup_vertex ();
	  boost::put(boost::vertex_name, g, gv_extout, extout_name);
	  boost::put(boost::vertex_BlockID, g, gv_extout, extout_id);
	  boost::put(boost::vertex_Label, g, gv_extout, extout_name);

	  if (enable_multirate)
	    cn_extout->rate ().rate () = 1;
	}

      // DataStoreMemoryの処理
      for (dsm_map_T::iterator i (dsm_map ().begin ());
	   i != dsm_map ().end (); i++)
	{
	  dsm_entry& de = i->second;
	  const CspBlockInfo* binfo = de.blockinfo ();
	  const block_T* b = binfo->block ();

	  // 状態変数(DataStoreMemoryのオブジェクト)は一つしかないはず
	  assert (b->state ().size () == 1);

	  // DataStoreMemoryの状態変数の情報をC_CodeWriterに設定
	  block_T::state_const_iterator si (b->state ().begin ());
	  const param_T& dsmstate = *si;
	  param_ent_T& se = de.dsm_state ().add_param (binfo->id (),
						       &dsmstate);
	  const std::string& ss = dsmstate.storage ();
	  c_codewriter.dsmem ().add_storage (ss).add_member (&se);
	  if (!c_codewriter.state_storage ())
	    c_codewriter.state_storage () = ss;

	  // DataStoreMemoryのパラメタの情報をC_CodeWriteに設定
	  for (block_T::param_const_iterator pi (b->param ().begin ());
	       pi != b->param ().end (); pi++)
	    {
	      const param_T& dsmparam = *pi;
	      param_ent_T& pe = de.dsm_params ().add_param (binfo->id (),
							    &dsmparam);
	      const std::string& ps = dsmparam.storage ();
	      c_codewriter.dsmem ().add_storage (ps).add_member (&pe);
	      if (!c_codewriter.param_storage ())
		c_codewriter.param_storage () = ps;
	    }

	  // DataStoreMemoryの初期化コード(状態変数の初期化コード)を
	  // C_CodeWriteに設定
	  for (block_T::code_const_iterator ci (b->code ().begin ());
	       ci != b->code ().end (); ci++)
	    {
	      if (ci->type () == "init")
		{
		  assert (de.init ().length () == 0);
		  de.init () = *ci;
		  c_codewriter.dsmem_init ().push_back (*ci);
		}
	    }
	}

      // CspNodeとC_CodeWriterの情報設定
      for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	   ci != CspNode::csp_node ().end (); ci++)
	{
	  CspNode& cn = *ci;

	  // task0とextoutは無視
	  if (cn.info () & (CSP_NODE_INFO_TASK_0 | CSP_NODE_INFO_TASK_EXTOUT))
	    continue;

	  const CspBlockInfo* binfo = cn.head_block ();
	  const block_T* b = binfo->block ();
	  const std::string& bnam = b->name ();

	  CspNode* ccnp = &cn;

	  if (cn_extout != NULL
	      && b->blocktype () == "Outport"
	      && (binfo->task_info () & TASK_INFO_VIRTUALOUTPORT_START) == 0)
	    {
	      // extoutが作られている場合の外部出力
	      ccnp = cn_extout;
	    }
	  else if (b->blocktype () == "Inport"
		   || (b->blocktype () == "Outport"
		       && (binfo->task_info ()
			   & TASK_INFO_VIRTUALOUTPORT_START) == 0))
	    {
	      // Inportは外部入力を行い、Outportは外部出力を行う

	      // 頂点(vertex)のラベル設定
	      std::ostringstream oss;
	      oss << 't' << cn.id ();
	      if (label_only_task_id)
		{
		  std::string rate = binfo->get_SampleTime ();
		  if (enable_multirate)
		    oss << "\\\n(" << rate << ')';
		}
	      else
		oss << "\\\n" << b->name ();
	      std::string name = oss.str ();

	      boost::put (boost::vertex_Label,
			  get_graph(), cn.get_vertex(), name);

	      boost::put (boost::vertex_Shape,
			  get_graph(), cn.get_vertex(), "box");

	      // レート設定
	      if (enable_multirate)
		{
		  const SampleTime& st = binfo->get_SampleTime();
		  if (st.type () == SampleTime::ST_TYPE_VARIABLE)
		    cn.info () |= CSP_NODE_INFO_CHANGE_RATE;
		  else if (st.type () == SampleTime::ST_TYPE_DISCRETE)
		    {
		      cn.rate ().rate () = st.cvalue();
		      cn.rate ().offset () = st.coffset();
		    }
		  if (cn.rate ().rate () == 0)
		    cn.rate ().rate () = 1; // base rate
		}

	      if (b->blocktype () == "Inport")
		{
		  // task0が作られていなければ、このCspNodeが外部入力
		  if (cn_t0 == NULL)
		    cn.info () |= CSP_NODE_INFO_EXTIN;

		  cn.info () |= CSP_NODE_INFO_INPORT;

		  // task0が作られていれば情報はtask0に集約
		  if (cn_t0)
		    ccnp = cn_t0;
		}
	      else
		{
		  // extoutが作られていない場合の外部出力
		  cn.info () |= (CSP_NODE_INFO_EXTOUT|CSP_NODE_INFO_OUTPORT);
		}
	    }
	  else
	    {
	      // 通常のノード

	      // 頂点(vertex)のラベル設定
	      std::ostringstream oss;
	      if (label_only_task_id)
		{
		  std::string rate = binfo->get_SampleTime();
		  oss << 't' << cn.id ();
		  if (enable_multirate)
		    oss << "\\n(" << rate << ')';
		}
	      else
		oss << "\\\n" << bnam;

	      std::string name = oss.str ();
	      boost::put(boost::vertex_Label,
			 get_graph(), cn.get_vertex (), name);

	      // レート設定
	      if (enable_multirate)
		{
		  const SampleTime& st = binfo->get_SampleTime();
		  if (st.type () == SampleTime::ST_TYPE_VARIABLE)
		    cn.info () |= CSP_NODE_INFO_CHANGE_RATE;
		  else if (st.type () == SampleTime::ST_TYPE_DISCRETE)
		    {
		      cn.rate ().rate () = st.cvalue ();
		      cn.rate ().offset () = st.coffset ();
		    }

		  if (cn.rate ().rate () == 0)
		    cn.rate ().rate () = 1; // base rate
		}

	      // 頂点(vertex)のプロパティ設定
	      unsigned int val = binfo->task_info ();

	      const char *fillcolor = NULL;
	      if (cn.info () & (CSP_NODE_INFO_ENABLED_SUBSYSTEM
				|CSP_NODE_INFO_TRIGGERED_SUBSYSTEM
				|CSP_NODE_INFO_ITERATOR_SUBSYSTEM
				|CSP_NODE_INFO_STATEFLOW_SUBSYSTEM
				|CSP_NODE_INFO_SWITCHCASE
				|CSP_NODE_INFO_IF))
		fillcolor = "red";
	      else if (val & TASK_INFO_BIND_HEAD)
		fillcolor = "orange";
	      else if (cn.contain_blocks ().size () > 1)
		fillcolor = "yellow";

	      if (fillcolor)
		{
		  boost::put (boost::vertex_Style, get_graph (),
			      cn.get_vertex (), "filled");
		  boost::put (boost::vertex_FillColor, get_graph (),
			      cn.get_vertex (), fillcolor);
		}

	      if (cn.info () & CSP_NODE_INFO_HAS_UPDATE)
		boost::put (boost::vertex_FontColor, get_graph (),
			    cn.get_vertex (), "red");
	    }

	  // 構造体のブロックシグナルの構造体オブジェクト名を
	  // C_CodeWriteへ設定
	  for (ParamList::const_iterator pli (cn.signal ().begin ());
	       pli != cn.signal ().end (); pli++)
	    {
	      if (!c_codewriter.signal_storage ())
		{
		  c_codewriter.signal_storage () = pli->storage ();
		  break;
		}
	    }

	  // 状態変数の構造体オブジェクト名をC_CodeWriteへ設定
	  for (ParamList::const_iterator pli (cn.state ().begin ());
	       pli != cn.state ().end (); pli++)
	    {
	      if (!c_codewriter.state_storage ())
		{
		  c_codewriter.state_storage () = pli->storage ();
		  break;
		}
	    }

	  // Trigger Statesの構造体オブジェクト名をC_CodeWriteへ設定
	  for (ParamList::const_iterator pli (cn.trigger_state ().begin ());
	       pli != cn.trigger_state ().end (); pli++)
	    {
	      if (!c_codewriter.trigger_state_storage ())
		{
		  c_codewriter.trigger_state_storage () = pli->storage ();
		  break;
		}
	    }

	  // Invariant Signalsの初期値があれば、その属性設定と、構造体
	  // オブジェクト名をC_CodeWriteへ設定
	  bool has_value = true;
	  for (ParamList::const_iterator pli (cn.invariant_signal ().begin ());
	       pli != cn.invariant_signal ().end (); pli++)
	    {
	      if (!(*pli)->initcode ())
		has_value = false;
	      if (!c_codewriter.invsig_storage ())
		c_codewriter.invsig_storage () = pli->storage ();
	    }
	  if (has_value && cn.invariant_signal ().size ())
	    {
	      cn.info () |= CSP_NODE_INFO_INVSIG_INITVALUE_OK;
	      if (ccnp != &cn)
		ccnp->info () |= CSP_NODE_INFO_INVSIG_INITVALUE_OK;
	    }

	  // Parameterの初期値があれば、その属性設定と、構造体オブジェ
	  // クト名をC_CodeWriteへ設定
	  has_value = true;
	  for (ParamList::const_iterator pli (cn.param ().begin ());
	       pli != cn.param ().end (); pli++)
	    {
	      if (!(*pli)->initcode ())
		has_value = false;
	      if (!c_codewriter.param_storage ())
		c_codewriter.param_storage () = pli->storage ();
	    }
	  if (has_value && cn.param ().size ())
	    {
	      cn.info () |= CSP_NODE_INFO_PARAM_INITVALUE_OK;
	      if (ccnp != &cn)
		ccnp->info () |= CSP_NODE_INFO_PARAM_INITVALUE_OK;
	    }

	  // Constant Parameterの初期値があれば、その属性設定と、構造体
	  // オブジェクト名をC_CodeWriteへ設定
	  has_value = true;
	  for (ParamList::const_iterator pli (cn.const_param ().begin ());
	       pli != cn.const_param ().end (); pli++)
	    {
	      if (!(*pli)->initcode ())
		has_value = false;
	      if (!c_codewriter.cparam_storage ())
		c_codewriter.cparam_storage () = pli->storage ();
	    }
	  if (has_value && cn.const_param ().size ())
	    {
	      cn.info () |= CSP_NODE_INFO_CPARAM_INITVALUE_OK;
	      if (ccnp != &cn)
		ccnp->info () |= CSP_NODE_INFO_CPARAM_INITVALUE_OK;
	    }

	  // XMLからコードの取り込み
	  for (CodeList::iterator ci (cn.code ().begin ());
	       ci != cn.code ().end (); ci++)
	    {
	      code_ent_T& ce = *ci;
	      import_code (ccnp, ci->block (), ce);
	    }

	  // DataStoreMemoryのアクセスがあればその属性設定
	  for (dsm_info_list_T::const_iterator ci (cn.dsm ().begin ());
	       ci != cn.dsm ().end (); ci++)
	    {
	      dsm_map_T::iterator i = dsm_map ().find (*ci);
	      if (i != dsm_map ().end ())
		{
		  cn.info () |= CSP_NODE_INFO_HAS_DSMEM;
		  break;
		}
	    }

	  // 入出力変数の取り込みと属性設定
	  for (var_map_iterator vmi (cn.var ().begin ());
	       vmi != cn.var ().end (); vmi++)
	    {
	      var_ent_T& ve = vmi->second;

	      if (ve.mode () & VAR_MODE_AUTO)
		ccnp->auto_var ().add_varent (&ve);
	      else if (ve.mode () & VAR_MODE_OUT)
		{
		  PropagateVar& pov = ccnp->output_var ().add_varent (&ve);
		  CspVar* cv = pov;
		  if (cv->mode () & VAR_MODE_EXTOUT)
		    {
		      ccnp->extout ().push_back (&pov);
		      const optional_refstring& ors = cv->storage ();
		      if (ors && !c_codewriter.extout_storage ())
			c_codewriter.extout_storage () = ors;
		    }
		  else if (cv->storage () == c_codewriter.signal_storage ())
		    {
		      cv->mode () |= VAR_MODE_BIOSIG;
		      ParamList::const_iterator
			ci = cn.signal ().find_by_name (cv->name ());
		      if (ci == cn.signal ().end ())
			cn.signal ().add_param (param_ent_T (cn.id (),
							     cv->var ()));
		    }
		}
	      else
		{
		  PropagateVar& piv = ccnp->input_var ().add_varent (&ve);
		  CspVar* cv = piv;
		  if (cv->mode () & VAR_MODE_EXTIN)
		    {
		      ccnp->extin ().push_back (&piv);
		      const optional_refstring& ors = cv->storage ();
		      if (ors && !c_codewriter.extin_storage ())
			c_codewriter.extin_storage () = ors;
		    }
		  else if (cv->storage () == c_codewriter.signal_storage ())
		    {
		      cv->mode () |= VAR_MODE_BIOSIG;
		      ParamList::const_iterator
			ci = cn.signal ().find_by_name (cv->name ());
		      if (ci == cn.signal ().end ())
			cn.signal ().add_param (param_ent_T (cn.id (),
							     cv->var ()));
		    }
		}
	    }

	  // 出力変数に分割された配列があれば、その結合
	  for (PropagateVarMap::iterator mi (ccnp->output_var ().begin ());
	       mi != ccnp->output_var ().end (); mi++)
	    {
	      PropagateVar& pv = mi->second;
	      pv.compact_partial_array ();
	    }

	  // 入力変数に分割された配列があれば、その結合
	  for (PropagateVarMap::iterator mi (ccnp->input_var ().begin ());
	       mi != ccnp->input_var ().end (); mi++)
	    {
	      PropagateVar& pv = mi->second;
	      pv.compact_partial_array ();
	    }
	}

      // エッジの設定
      for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	   ci != CspNode::csp_node ().end (); ci++)
	{
	  if (ci->info () & (CSP_NODE_INFO_TASK_0|CSP_NODE_INFO_TASK_EXTOUT))
	    continue;

	  const CspBlockInfo* bi_tail = ci->head_block ();

	  for (CspEdgeList::iterator ei (ci->forw_edges ().begin ());
	       ei != ci->forw_edges ().end (); ei++)
	    {
	      CspNode* h = ei->node ();
	      CspGraphEdge& g_edge = ei->get_edge();

	      if (ei->info () & CSP_EDGE_INFO_HAS_UPDATE)
		{
		  // check tail -> head[delay, other]
		  //        +-------------^------^
		  //
		  // 上記の様にdelayと他のブロックに同じ変数が入力され
		  // るとき、UPDATEとはしない
		  // コード生成においては、通常updateの処理の前に入力を
		  // 行うが、この場合は、入力を最初に行う(少なくとも
		  // otherの前に)
		  if (check_delay_as_other_input (&*ci, h))
		    {
		      ei->info () &= ~CSP_EDGE_INFO_HAS_UPDATE;
		      ei->pair ()->info () &= ~CSP_EDGE_INFO_HAS_UPDATE;
		      h->info () &= ~CSP_NODE_INFO_INVAR_UPDATE;
		    }
		  else
		    h->info () |= CSP_NODE_INFO_INVAR_UPDATE;
		}

	      // 条件付きSubSystemの初期値送信の設定
	      if (ei->info () & CSP_EDGE_INFO_INITIAL_VALUE)
		{
		  ci->info () |= CSP_NODE_INFO_SEND_INITVAL;
		  h->info () |= CSP_NODE_INFO_RECV_INITVAL;
		}

	      // SubSystemのイベントがある場合の設定
	      if ((ci->info () & (CSP_NODE_INFO_TRIGGERED_SUBSYSTEM
				  |CSP_NODE_INFO_ENABLED_SUBSYSTEM)) != 0
		  && (ci->info () & CSP_NODE_INFO_ONE_SUBSYSTEM) == 0)
		{
		  ei->info () |= CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT;
		  ei->pair ()->info () |= CSP_EDGE_INFO_HAS_SUBSYSTEM_EVENT;
		  h->event_subsystem () = &*ci;
		  boost::put(boost::edge_color, get_graph (), g_edge, "red");
		}

	      // 非同期入力
	      const SampleTime&
		st_head = block_list ()[ci->id ()]->get_SampleTime ();
	      if (st_head.type () == SampleTime::ST_TYPE_ASYNCHRONOUS)
		{
		  ei->info () |= CSP_EDGE_INFO_ASYNC_INPUT;
		  ei->pair ()->info () |= CSP_EDGE_INFO_ASYNC_INPUT;
		}

	      // 伝播する変数を抽出しCspEdgeに設定
	      for (csp_block_list_iterator j (ci->contain_blocks ().begin ());
		   j != ci->contain_blocks ().end (); j++)
		{
		  CspBlockInfo* cbi_tail = *j;
		  for (csp_block_connect_list_T::iterator
			 bci (cbi_tail->forward ().begin ());
		       bci != cbi_tail->forward ().end (); bci++)
		    {
		      CspBlockConnect& bc = *bci;
		      CspBlockInfo* cbi_head = bc.head ();
		      const CspNodePtrList& hcpl = cbi_head->get_nodes ();
		      for (CspNodePtrList::const_iterator
			     ccni (hcpl.begin ());
			   ccni != hcpl.end (); ccni++)
			{
			  CspNode* cn_head = *ccni;
			  if (cn_head != h)
			    continue;
			  assign_variables (bc, &*ci, cbi_tail,
					    cn_head, cbi_head, *ei, false);
			}
		    }
		}
	    }

	  // task0がなければ継続
	  if (cn_t0 == NULL)
	    continue;

	  //
	  // task0がある場合のグラフのエッジ設定
	  //

	  if (bi_tail->block ()->blocktype () == "Inport")
	    {
	      std::pair< CspEdge*, CspEdge* > ep = ci->add_edge (cn_t0);
	      CspEdge* fedge = ep.first;
	      CspGraphEdge e = fedge->get_edge ();
	      boost::put (boost::edge_color, get_graph (), e, "green");

	      if (label_only_task_id == 0)
		add_var_list_label_to_edge (*fedge);
	    }
	  else if (ci->back_edges ().size () == 0)
	    {
	      std::pair< CspEdge*, CspEdge* > ep = ci->add_edge (cn_t0);
	      CspEdge* fedge = ep.first;
	      CspGraphEdge e = fedge->get_edge ();
	      boost::put (boost::edge_color, get_graph (), e, "red");

	      // channel
	      if (label_only_task_id == 0)
		add_label_to_edge (*fedge, "[EVENT]");
	    }
	}

      // 各CspNodeの入出力に分割された配列があれば、まとめる
      for (cspnode_list_iterator ci (CspNode::csp_node ().begin ());
	   ci != CspNode::csp_node ().end (); ci++)
	{
	  for (CspEdgeList::iterator ei (ci->forw_edges ().begin ());
	       ei != ci->forw_edges ().end (); ei++)
	    {
	      for (PropagateVarMap::iterator mi (ei->var ().begin ());
		   mi != ei->var ().end (); mi++)
		{
		  PropagateVar& pv = mi->second;
		  pv.compact_partial_array ();
		}
	    }
	  for (CspEdgeList::iterator ei (ci->back_edges ().begin ());
	       ei != ci->back_edges ().end (); ei++)
	    {
	      for (PropagateVarMap::iterator mi (ei->var ().begin ());
		   mi != ei->var ().end (); mi++)
		{
		  PropagateVar& pv = mi->second;
		  pv.compact_partial_array ();
		}
	    }
	}

      // CspNodeのポインタのベクタを生成
      CspNode::make_nodelist ();

      return 0;
    }

    void
    CspReader::Dump_Nodes (void)
    {
      for (cspnode_list_const_iterator ci (CspNode::csp_node ().begin ());
	   ci != CspNode::csp_node ().end (); ci++)
	{
	  const CspNode& c = *ci;
	  c.dump ();
	}
    }


    void
    CspReader::prepare_process (void)
    {
      need_extout = false;
      need_extin = false;

      tid_values.clear ();
      sorted_sample_times.clear ();
    }

    CspBlockInfo*
    CspReader::get_connect_block (const connection_T& c) const
    {
      CspBlockInfo* binfo = find_block (c.block ());
      if (binfo == NULL)
	return NULL;

      if (binfo->is_subsystem ()
	  && !binfo->is_atomic_subsystem ()
	  && !binfo->is_noblock_subsystem ())
	binfo = find_block (c.port ());

      return binfo;
    }

    bool
    CspReader::Inherit_SampleTime (const CspBlockInfo* from,
				   CspBlockInfo* to, int dir) const
    {
      bool change = false;

      const SampleTime& x = from->get_SampleTime ();
      SampleTime& p = to->get_SampleTime ();

      // 継承であれば、継承元からコピーする
      if (p.type () == SampleTime::ST_TYPE_INHERITED)
	{
	  p = x;
	  p.inherited () = true;

	  if (verbose)
	    {
	      if (dir)
		std::cout << "Inherit SampleTime: "
			    << to->block ()->blocktype ()
			    << ':'
			    << to->block ()->name ()
			    << '(' << to->id () << ')'
			    << " <- "
			    << from->block ()->blocktype ()
			    << ':'
			    << from->block ()->name ()
			    << '(' << from->id () << ')'
			    << '[' << x << ']'
			    << std::endl;
	      else
		std::cout << "Inherit SampleTime: "
			    << from->block ()->blocktype ()
			    << ':'
			    << from->block ()->name ()
			    << '(' << from->id () << ')'
			    << '[' << x << ']'
			    << " -> "
			    << to->block ()->blocktype ()
			    << ':'
			    << to->block ()->name ()
			    << '(' << to->id () << ')'
			    << std::endl;
	    }

	  // マージしているブロックに対してもサンプル時間をコピーする
	  (void)inherit_merge_blocks (to);

	  change = true;
	}

#if 0
      // check
      if (p.inherited ()
	  && p.type () == SampleTime::ST_TYPE_DISCRETE
	  && p != x)
	{
	  if (dir)
	    std::cout << "Warning: different inherit found: "
			<< to->block ()->name () << '(' << p << ')'
			<< " <- "
			<< from->block ()->name () << '(' << x << ')'
			<< std::endl;
	  else
	    std::cout << "Warning: different inherit found: "
			<< from->block ()->name () << '(' << x << ')'
			<< " -> "
			<< to->block ()->name () << '(' << p << ')'
			<< std::endl;
	}
#endif

      return change;
    }


    bool
    CspReader::Inherit_Subsystem_SampleTime (CspBlockInfo* subsys,
					     const CspBlockInfo* propagate,
					     bool force) const
    {
      const SampleTime& x = propagate->get_SampleTime ();
      bool change = false;

      // 継承元のサンプル時間が決定していない
      if (x.type () != SampleTime::ST_TYPE_DISCRETE)
	return false;

      // サンプル時間が継承可能(あるいはforce=true)ならサンプル時間を継
      // 承する
      SampleTime& s = subsys->get_SampleTime();
      if (force || s.type () == SampleTime::ST_TYPE_INHERITED)
	{
	  s = x;
	  s.inherited () = true;
	  change = true;
	  if (verbose)
	    {
	      std::cout << "Inherit in Subsystem: "
			  << propagate->block ()->blocktype ()
			  << ':'
			  << propagate->block ()->name ()
			  << '(' << propagate->id () << ')'
			  << '[' << x << ']'
			  << " => "
			  << subsys->block ()->blocktype ()
			  << ':'
			  << subsys->block ()->name ()
			  << '(' << subsys->id () << ')'
			  << std::endl;
	    }
	}
      else if (verbose
	       && s.type () != SampleTime::ST_TYPE_NONE
	       && s.type () != SampleTime::ST_TYPE_CONSTANT
	       && s.type () != SampleTime::ST_TYPE_CONTINUOUS
	       && s != x)
	{
	  std::cout << "Warning: Couldn't inherit in Subsystem: "
		      << subsys->block ()->blocktype ()
		      << ':'
		      << subsys->block ()->name ()
		      << '(' << subsys->id () << ')'
		      << '[' << s << ']'
		      << std::endl;
	}

      // SubSystemに含まれるブロックについて再帰的に実行
      for (csp_block_list_const_iterator
	     bi (subsys->lower_block ().begin ());
	   bi != subsys->lower_block ().end (); bi++)
	{
	  CspBlockInfo* binfo = *bi;

	  if (binfo == propagate)
	    continue;

	  if (Inherit_Subsystem_SampleTime (binfo, propagate, force))
	    change = true;
	}

      return change;
    }

    bool
    CspReader::check_input_inheritable (const SimulinkModel::XSD::ioport_T& input,
					const CspBlockInfo*& from,
					const SampleTime*& st,
					bool heuristic) const
    {
      for (ioport_T::connect_const_iterator ci (input.connect ().begin ());
	   ci != input.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* pbinfo = get_connect_block (c);
	  const block_T* pb = pbinfo->block ();

	  // Mux/Demux/Gogo/From/Merge
	  if (is_pass_block (pb))
	    {
	      const CspBlockInfo* tbinfo = from;
	      const SampleTime* tst = st;
	      if (!check_input_inheritable (pbinfo, tbinfo, tst, heuristic))
		return false;
	      pbinfo = tbinfo;
	      pb = pbinfo->block ();
	    }

	  const SampleTime& p = pbinfo->get_SampleTime();

	  switch (p.type ())
	    {
	    case SampleTime::ST_TYPE_NONE:
	      std::cout << "Warning: no input sampletime: "
			  << pb->blocktype () << ':' << pb->name ()
			  << std::endl;
	    case SampleTime::ST_TYPE_INHERITED:
	    case SampleTime::ST_TYPE_VARIABLE:
	    case SampleTime::ST_TYPE_HYBRID:
	      if (heuristic)
		continue;
	      return false;
	    case SampleTime::ST_TYPE_DISCRETE:
	      break;
	    case SampleTime::ST_TYPE_CONTINUOUS:
	    case SampleTime::ST_TYPE_FIXED:
	    case SampleTime::ST_TYPE_CONSTANT:
	    case SampleTime::ST_TYPE_TRIGGERED:
	    case SampleTime::ST_TYPE_ASYNCHRONOUS:
	    case SampleTime::ST_TYPE_INVALID: // nothing this case
	      continue;
	    }

	  if (*st == p)
	    continue;

	  if (st->type () == SampleTime::ST_TYPE_CONSTANT)
	    {
	      // stが定数なら受け入れ可能なのでpに候補を置き換える
	      st = &p;
	      from = pbinfo;
	    }

	  // Simulinkのサンプル時間の継承に従い、stがpの整数倍であれば、
	  // 受け入れ可能で、より速いレートであるpに候補を置き換える
	  //
	  // またはpがstの整数倍であれば、受け入れは可能
	  if (0 < p.cvalue () && p.cvalue () < st->cvalue ())
	    {
	      if (st->cvalue () % p.cvalue ())
		{
		  std::cout << "Warning: "
			      << "not integral multplication: "
			      << from->block ()->name ()
			      << '[' << *st << ']'
			      << " -> "
			      << pb->name ()
			      << '[' << p << ']'
			      << std::endl;
		  return false;
		}

	      st = &p;
	      from = pbinfo;
	    }
	  else if (p.cvalue () % st->cvalue ())
	    {
	      std::cout << "Warning: "
			  << "not integral multplication: "
			  << from->block ()->name ()
			  << '[' << *st << ']'
			  << " -> "
			  << pb->name ()
			  << '[' << p << ']'
			  << std::endl;
	      return false;
	    }
	}

      return true;
    }

    bool
    CspReader::check_input_inheritable (const CspBlockInfo* binfo,
					const CspBlockInfo*& from,
					const SampleTime*& st,
					bool heuristic) const
    {
      const block_T* b = binfo->block ();

      if (is_inport_type (b))
	{
	  // 接続先がポートの場合、その先に切り替える
	  const IOPort* port = find_inport_input (binfo);
	  if (port == NULL)
	    return false;
	  const ioport_T& input = *port->ioport_tag ();
	  return check_input_inheritable (input, from, st, heuristic);
	}

      // 入力に対して調べる
      for (block_T::input_const_iterator ii (b->input ().begin ());
	   ii != b->input ().end (); ii++)
	{
	  const ioport_T& input = *ii;
	  if (!check_input_inheritable (input, from, st, heuristic))
	    return false;
	}

      return true;
    }

    bool
    CspReader::check_output_inheritable (const SimulinkModel::XSD::ioport_T& output,
					 const CspBlockInfo*& to,
					 const SampleTime*& st) const
    {
      for (ioport_T::connect_const_iterator ci (output.connect ().begin ());
	   ci != output.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* fbinfo = find_block (c.block ());
	  const block_T* fb = fbinfo->block ();

	  // Mux/Demux/Goto/From/Merge
	  if (is_pass_block (fb))
	    {
	      const CspBlockInfo* tbinfo = to;
	      const SampleTime* tst = st;
	      if (!check_output_inheritable (fbinfo, tbinfo, tst))
		return false;
	      fbinfo = tbinfo;
	      fb = fbinfo->block ();
	    }
	  else if (fb->blocktype () == "RateTransition")
	    continue;	// no inherit
	  else if (fb->blocktype () == "Terminator"
		   || fb->blocktype () == "Scope"
		   || fb->blocktype () == "FloatingScope"
		   || fb->blocktype () == "Display"
		   || fb->blocktype () == "ToWorkspace"
		   || fb->blocktype () == "ToFile")
	    continue;	// skip this
	  else if (fbinfo->is_subsystem ())
	    {
	      CspBlockInfo::subsystem_type_T fst = fbinfo->subsystem_type ();
	      if (!fbinfo->is_atomic_subsystem ()
		  && !fbinfo->is_noblock_subsystem ())
		{
		  const IOPort* in = find_input_port (c.block (), c.port ());
		  const CspBlockInfo* inport = find_block (in->port ());
		  if (inport == NULL)
		    continue; // no input, skip this

		  if (fst == CspBlockInfo::ENABLED_TRIGGERED
		      || fst == CspBlockInfo::TRIGGERED)
		    {
		      if (inport != fbinfo->trigger_port ())
			continue; // ignore other port
		    }
		  else if (fst == CspBlockInfo::ACTION)
		    {
		      if (inport != fbinfo->action_port ())
			continue; // ignore other port
		    }

		  fbinfo = inport;
		  fb = fbinfo->block ();
		}
	    }

	  const SampleTime& f = fbinfo->get_SampleTime ();

	  switch (f.type ())
	    {
	    case SampleTime::ST_TYPE_NONE:
	      std::cout << "Warning: no output sampletime: "
			  << fb->blocktype () << ':' << fb->name ()
			  << std::endl;
	    case SampleTime::ST_TYPE_INHERITED:
	    case SampleTime::ST_TYPE_VARIABLE:
	    case SampleTime::ST_TYPE_HYBRID:
	      return false;
	    case SampleTime::ST_TYPE_DISCRETE:
	      break;
	    case SampleTime::ST_TYPE_CONTINUOUS:
	    case SampleTime::ST_TYPE_FIXED:
	    case SampleTime::ST_TYPE_CONSTANT:
	    case SampleTime::ST_TYPE_TRIGGERED:
	    case SampleTime::ST_TYPE_ASYNCHRONOUS:
	    case SampleTime::ST_TYPE_INVALID: // nothing this case
	      continue;
	    }

	  if (*st == f)
	    continue;

	  // stがfの整数倍であれば受け入れ可能で、候補をより速いレート
	  // であるfに置き換える
	  //
	  // それ以外の場合は継承不可。仮にfがstの整数倍であっても、遅
	  // いレートから速いレートへの伝播は、期待と異なる結果になりか
	  // ねない
	  if (0 < f.cvalue () && f.cvalue () < st->cvalue ())
	    {
	      if (st->cvalue () % f.cvalue ())
		{
		  std::cout << "Warning: "
			      << "not integral multplication: "
			      << to->block ()->name ()
			      << '[' << *st << ']'
			      << " <- "
			      << fb->name ()
			      << '[' << f << ']'
			      << std::endl;
		  return false;
		}

	      // acceptable
	      st = &f;
	      to = fbinfo;
	    }
	  else if (f.cvalue () % st->cvalue ())
	    {
	      std::cout << "Warning: "
			  << "not integral multplication: "
			  << to->block ()->name ()
			  << '[' << *st << ']'
			  << " <- "
			  << fb->name ()
			  << '[' << f << ']'
			  << std::endl;
	      return false;
	    }
	  else
	    return false;
	}

      return true;
    }


    bool
    CspReader::check_output_inheritable (const CspBlockInfo* binfo,
					 const CspBlockInfo*& to,
					 const SampleTime*& st) const
    {
      const block_T* b = binfo->block ();

      if (b->blocktype () == "Outport")
	{
	  // 出力がOutportの場合、その先に切り替える
	  const IOPort* port = find_outport_output (binfo);
	  if (port == NULL)
	    return false;
	  const ioport_T& output = *port->ioport_tag ();
	  return check_output_inheritable (output, to, st);
	}

      // 全ての出力について調べる
      for (block_T::output_const_iterator oi (b->output ().begin ());
	   oi != b->output ().end (); oi++)
	{
	  const ioport_T& output = *oi;
	  if (!check_output_inheritable (output, to, st))
	    return false;
	}

      return true;
    }

    bool
    CspReader::check_input_color (const SimulinkModel::XSD::ioport_T& input,
				  const int* color) const
    {
      for (ioport_T::connect_const_iterator ci (input.connect ().begin ());
	   ci != input.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* pbinfo = get_connect_block (c);
	  const block_T* pb = pbinfo->block ();

	  if (is_pass_block (pb))
	    {
	      if (!check_input_color (pbinfo, color))
		return false;
	    }
	  else
	    {
	      const SampleTime& st = pbinfo->get_SampleTime ();
	      // 未訪問かつ継承となっている場合、レートを決定できない
	      if (color[pbinfo->id ()] == 0
		  && st.type () == SampleTime::ST_TYPE_INHERITED)
		return false;
	    }
	}

      return true;
    }

    bool
    CspReader::check_input_color (const CspBlockInfo* binfo,
				  const int* color) const
    {
      const block_T* b = binfo->block ();

      if (is_inport_type (b))
	{
	  // inportの場合は、その先に切り替える
	  const IOPort* inp = find_inport_input (binfo);
	  if (inp)
	    {
	      const ioport_T* in = inp->ioport_tag ();
	      if (!check_input_color (*in, color))
		return false;
	    }
	}
      else
	{
	  // 全ての入力に対して調べる
	  for (block_T::input_const_iterator ii (b->input ().begin ());
	       ii != b->input ().end (); ii++)
	    {
	      if (!check_input_color (*ii, color))
		return false;
	    }
	}

      return true;
    }

#if 0
    // デバッグ用
    void
    CspReader::dump_rate (bool inherited_only) const
    {
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  SampleTime& x = binfo->get_SampleTime();
	  if (x.type () == SampleTime::ST_TYPE_INHERITED)
	    {
	      std::cout << "* " << b->blocktype () << ':' << b->name ()
			  << '(' << binfo->id() << ')' << std::endl;

	      for (block_T::input_const_iterator ii (b->input ().begin ());
		   ii != b->input ().end (); ii++)
		{
		  const ioport_T& in = *ii;
		  for (ioport_T::connect_const_iterator
			 ci (in.connect ().begin ());
		       ci != in.connect ().end (); ci++)
		    {
		      const connection_T& c = *ci;
		      const CspBlockInfo* pb = find_block (c.block ());
		      std::cout << "*   " << c.block ()
				  << '[' << pb->get_SampleTime () << ']'
				  << std::endl;
		    }
		}
	    }
	  else if (inherited_only == false)
	    {
	      std::cout << "| " << b->blocktype () << ':' << b->name ()
			  << '(' << binfo->id() << ')'
			  << '[' << binfo->get_SampleTime () << ']'
			  << std::endl;
	    }
	}
    }
#endif

    void
    CspReader::print_block_rate (const CspBlockInfo* binfo, int ind,
				 const std::string prefix) const
    {
      const std::string cind (ind * 2, ' ');
      const block_T* b = binfo->block ();
      std::cout << cind << prefix << b->blocktype () << ':' << b->name ()
		  << '(' << binfo->id () << ')'
		  << '[' << binfo->get_SampleTime () << ']';

      /// Pulse Generatorは、周期とパルス幅を表示する
      if (b->blocktype () == "DiscretePulseGenerator")
	{
	  const block_T::Period_optional& ppo = b->Period ();
	  const block_T::PulseWidth_optional& pwo = b->PulseWidth ();

	  if (ppo || pwo)
	    {
	      std::cout << "{";
	      bool flag = false;
	      if (ppo)
		{
		  std::cout << "Period=" << *ppo;
		  flag = true;
		}
	      if (pwo)
		{
		  if (flag)
		    std::cout << ",";
		  std::cout << "PulseWidth=" << *pwo;
		}
	      std::cout << "}";
	    }
	}
      std::cout << std::endl;
    }

    void
    CspReader::dump_block_rate_input (const CspBlockInfo* binfo, int ind) const
    {
      const block_T& b = *binfo->block ();
      if (is_inport_type (&b))
	{
	  const IOPort* port = find_inport_input (binfo);
	  if (port)
	    {
	      const ioport_T* in = port->ioport_tag ();
	      for (ioport_T::connect_const_iterator
		     ci (in->connect ().begin ());
		   ci != in->connect ().end (); ci++)
		{
		  const IOPort* port = find_output_port (ci->block (),
							 ci->port ());
		  if (port)
		    {
		      const CspBlockInfo*
			outport = find_block (ci->block ());
		      if (outport == NULL)
			continue;
		      const block_T* ob = port->block_tag ();
		      if (outport->is_subsystem ()
			  && !outport->is_atomic_subsystem ()
			  && !outport->is_noblock_subsystem ())
			{
			  outport = find_block (port->port ());
			  if (outport)
			    {
			      print_block_rate (outport, ind, "<- ");
			      dump_block_rate_input (outport, ind + 1);
			    }
			}
		      else
			{
			  print_block_rate (outport, ind, "<- ");
			  if (is_pass_block (ob))
			    dump_block_rate_input (outport, ind + 1);
			}
		    }
		}
	    }
	}
      else
	{
	  for (block_T::input_const_iterator i (b.input ().begin ());
	       i != b.input ().end (); i++)
	    {
	      for (ioport_T::connect_const_iterator
		     ci (i->connect ().begin ());
		   ci != i->connect ().end (); ci++)
		{
		  const CspBlockInfo* binfo = find_block (ci->block ());
		  print_block_rate (binfo, ind, "<- ");

		  const block_T *ib = binfo->block();
		  if (binfo->is_subsystem ()
		      && !binfo->is_atomic_subsystem ()
		      && !binfo->is_noblock_subsystem ())
		    {
		      const IOPort* out = find_output_port(ci->block (),
							   ci->port ());
		      if (out)
			{
			  const CspBlockInfo*
			    outport = find_block (out->port ());
			  if (outport)
			    {
			      print_block_rate (outport, ind + 1, "<- ");
			      dump_block_rate_input (outport, ind + 2);
			    }
			}
		    }
		  else if (is_pass_block (ib))
		    dump_block_rate_input (binfo, ind + 1);
		}
	    }
	}
    }

    void
    CspReader::dump_block_rate_output (const CspBlockInfo* binfo,
				       int ind) const
    {
      const block_T& b = *binfo->block ();
      if (b.blocktype () == "Outport")
	{
	  const IOPort* port = find_outport_output (binfo);
	  if (port)
	    {
	      const ioport_T* out = port->ioport_tag ();
	      for (ioport_T::connect_const_iterator
		     ci (out->connect ().begin ());
		   ci != out->connect ().end (); ci++)
		{
		  const IOPort* port = find_input_port (ci->block (),
							ci->port ());
		  if (port)
		    {
		      const CspBlockInfo* inport = find_block (ci->block ());
		      if (inport == NULL)
			continue;
		      const block_T* ib = port->block_tag ();
		      if (inport->is_subsystem ()
			  && !inport->is_atomic_subsystem ()
			  && !inport->is_noblock_subsystem ())
			{
			  inport = find_block (port->port ());
			  if (inport)
			    {
			      print_block_rate (inport, ind, "-> ");
			      dump_block_rate_output (inport, ind + 1);
			    }
			}
		      else
			{
			  print_block_rate (inport, ind, "-> ");
			  if (is_pass_block (ib))
			    dump_block_rate_output (inport, ind + 1);
			}
		    }
		}
	    }
	}
      else
	{
	  for (block_T::output_const_iterator i (b.output ().begin ());
	       i != b.output ().end (); i++)
	    {
	      for (ioport_T::connect_const_iterator
		     ci (i->connect ().begin ());
		   ci != i->connect ().end (); ci++)
		{
		  const CspBlockInfo* binfo = find_block (ci->block ());
		  print_block_rate (binfo, ind, "-> ");

		  const block_T *ob = binfo->block();
		  if (binfo->is_subsystem ()
		      && !binfo->is_atomic_subsystem ()
		      && !binfo->is_noblock_subsystem ())
		    {
		      const IOPort* in = find_input_port(ci->block (),
							 ci->port ());
		      if (in)
			{
			  const CspBlockInfo*
			    inport = find_block (in->port ());
			  if (inport)
			    {
			      print_block_rate (inport, ind + 1, "-> ");
			      dump_block_rate_output (inport, ind + 2);
			    }
			}
		    }
		  else if (is_pass_block (ob))
		    dump_block_rate_output (binfo, ind + 1);
		}
	    }
	}
    }

    void
    CspReader::dump_block_rate (const SimulinkModel::XSD::block_T& b,
				int ind, bool no_io) const
    {
      const std::string sind(ind * 2, ' ');
      const CspBlockInfo* binfo = find_block (b.name ());
      const SampleTime& st = binfo->get_SampleTime ();

      if (no_io == false
	  || b.blocks ().size () > 0
	  || b.code ().size () > 0)
	{
	  if (binfo->is_subsystem ()
	      && !binfo->is_atomic_subsystem ()
	      && !binfo->is_noblock_subsystem ())
	    {
	      if (st.type () == SampleTime::ST_TYPE_INHERITED
		  || st.type () == SampleTime::ST_TYPE_NONE)
		print_block_rate (binfo, ind, "- ");
	      else
		print_block_rate (binfo, ind, "+ ");
	    }
	  else if (st.type () == SampleTime::ST_TYPE_INHERITED)
	    print_block_rate (binfo, ind, "? ");
	  else if (st.type () == SampleTime::ST_TYPE_NONE
		   || st.type () == SampleTime::ST_TYPE_CONTINUOUS)
	    print_block_rate (binfo, ind, "- ");
	  else
	    print_block_rate (binfo, ind, "+ ");
	}

      if (no_io == false)
	{
	  dump_block_rate_input (binfo, ind + 2);
	  dump_block_rate_output (binfo, ind + 2);
	}

      for (block_T::blocks_const_iterator bsi (b.blocks ().begin ());
	   bsi != b.blocks ().end (); bsi++)
	dump_block_rates (*bsi, ind + 1, no_io);
    }

    void
    CspReader::dump_block_rates (const blocks_T& bs, int ind,
				 bool no_io) const
    {
      for (blocks_T::block_const_iterator bi (bs.block ().begin ());
	   bi != bs.block ().end (); bi++)
	{
	  const block_T& b = *bi;
	  dump_block_rate (b, ind, no_io);
	}
    }

    /// \brief 信号発生のブロックかどうか
    ///
    /// \param[in] b ブロック(BLXMLのblockタグ)
    ///
    /// 以下のブロックは信号を発生するブロック
    /// - DiscretePulseGenerator
    /// - Clock
    /// - DigitalClock
    /// - Step
    /// - Ramp
    /// - SignalGenerator
    ///
    /// \retval true 信号発生のブロック
    /// \retval false それ以外
    static inline bool
    is_signal_source_block (const block_T* b)
    {
      return (b->blocktype () == "DiscretePulseGenerator"
	      || b->blocktype () == "Clock"
	      || b->blocktype () == "DigitalClock"
	      || b->blocktype () == "Step"
	      || b->blocktype () == "Ramp"
	      || b->blocktype () == "SignalGenerator");
    }

    int
    CspReader::Inherit_SampleTime (void)
    {
      // ブロックのパラメタで、数値のものを抽出する
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  for (block_T::param_const_iterator pi (b->param ().begin ());
	       pi != b->param ().end (); pi++)
	    {
	      const param_T& p = *pi;
	      if (SampleTime::check_param (p.name ()))
		continue;	// already existing

	      const param_T::value_optional& v = p.value ();
	      if (!v)
		continue;	// no value

	      const std::string& sv = *v;
	      const char *cv = sv.c_str ();

	      if (*cv == '{')
		continue;	// ignore struct of array initialize

	      // read floating point number
	      char *x;
	      double pv = strtod (cv, &x);
	      if (*x == 0)
		SampleTime::set_param (p.name (), pv);
	    }
	}

      // blockタグのrate属性を取り出し、ブロックの種類に応じて
      // SampleTimeを設定する
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  const block_T::rate_optional& ro = b->rate ();

	  SampleTime& x = binfo->get_SampleTime ();

	  if (!ro)
	    {
	      if (b->blocktype () == "Clock")
		x.set ("0");
	      else if (b->blocktype () == "Width")
		x.set ("inf");
	      else if (b->blocktype () == "DiscretePulseGenerator"
		       || b->blocktype () == "S-Function"
		       || b->blocktype () == "ForIterator"
		       || b->blocktype () == "Concatenate"
		       || b->blocktype () == "Bias"
		       || b->blocktype () == "Reshape"
		       || (b->blocktype () == "SubSystem"
			   && (binfo->is_atomic_subsystem ()
			       || binfo->is_noblock_subsystem ()
			       || (binfo->subsystem_type ()
				   == CspBlockInfo::ENABLED_TRIGGERED)
			       || (binfo->subsystem_type ()
				   == CspBlockInfo::TRIGGERED)
			       || (binfo->subsystem_type ()
				   == CspBlockInfo::ACTION)
			       || (binfo->subsystem_type ()
				   == CspBlockInfo::STATEFLOW))))
		x.set ("-1");
	      else if (b->blocktype () == "ActionPort"
		       || b->blocktype () == "TriggerPort"
		       || b->blocktype () == "EnablePort")
		{
		  x.set ("-1");
		  x.output_acceptable (false);
		}
	      else if (is_pass_block (b)
		       || b->blocktype () == "Terminator"
		       || b->blocktype () == "SubSystem") // virtual?
		continue;	// none
	      else
		{
		  if (verbose)
		    std::cout << "None rate: " << b->blocktype ()
				<< ':' << b->name () << std::endl;
		  x.set ("-1");	// fail safe
		}
	    }
	  else if (b->blocktype () == "RateTransition")
	    {
	      // inherit from outport only
	      x.set ("-1");
	      x.output_acceptable (false);
	    }
	  else if (b->blocktype () == "TriggerPort")
	    x.set ("-1");	// not SampleTime!!, force inherit
	  else if (b->blocktype () == "DiscretePulseGenerator")
	    {
	      const block_T::PulseType_optional& pto = b->PulseType ();
	      if (pto && *pto != "Time based")
		x.set (*ro);
	      else
		x.set ("-1");
	    }
	  else
	    {
	      x.set (*ro);
	      if (x.type () == SampleTime::ST_TYPE_INVALID)
		{
		  std::cout << "Error: Invalid SampleTime: "
			      << b->name () << ": " << *ro << std::endl;
		  has_error = true;
		}
	    }

	  if (b->blocktype () == "From")
	    {
	      if (b->input ().empty ())
		x.input_acceptable (false);
	    }
	  else if (b->blocktype () == "Goto")
	    {
	      if (b->output ().empty ())
		x.output_acceptable (false);
	    }

	  // サンプル時間が数値またはシンボルで指定されている場合の処理
	  if (x.type () == SampleTime::ST_TYPE_DISCRETE)
	    {
	      // サンプル時間がシンボルの場合
	      if (x.svalue ().length ())
		{
		  // パラメタから値を探し、見つかったらその値を設定しシ
		  // ンボルをクリアする
		  double v = SampleTime::get_param (x.svalue ());
		  if (v == 0)
		    std::cout << "Warning: SampleTime: "
				<< x.svalue ()
				<< ": not found" << std::endl;
		  else
		    {
		      x.dvalue () = v;
		      x.svalue ().clear ();
		    }
		}

	      // 開始オフセットがシンボルの場合
	      if (x.soffset ().length ())
		{
		  // パラメタから値を探し、見つかったらその値を設定しシ
		  // ンボルをクリアする
		  double v = SampleTime::get_param (x.soffset ());
		  if (v == 0)
		    std::cout << "Warning: SampleTime: "
				<< x.soffset ()
				<< ": not found" << std::endl;
		  else
		    {
		      x.doffset () = v;
		      x.soffset ().clear ();
		    }
		}

	      // 数値である場合、で最速であれば、最速のサンプル時間を更
	      // 新する
	      if (x.dvalue () != 0
		  && (fast_sample_rate.dvalue () == 0
		      || fast_sample_rate.dvalue () > x.dvalue ()))
		{
		  fast_sample_rate = x;
		  fast_sample_rate.baserate () = true;
		}
	    }
	}

      if (is_error())
	return -1;

      // 最速のサンプル時間を、ベースレートの周期比にする
      if (fast_sample_rate.dvalue () != 0)
	fast_sample_rate.cvalue () = round (fast_sample_rate.dvalue ()
					    / SampleTime::get_tick ());

      // Triggered/Enabled SubSystemは出力側からの継承を禁止する
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL || !binfo->is_subsystem ())
	    continue;

	  CspBlockInfo::subsystem_type_T st = binfo->subsystem_type ();

	  if (binfo->is_atomic_subsystem ()
	      || binfo->is_noblock_subsystem ())
	    binfo->get_SampleTime ().output_acceptable (false);
	  else if (binfo->top_subsystem () == NULL
		   && (st == CspBlockInfo::TRIGGERED
		       || st == CspBlockInfo::ENABLED_TRIGGERED))
	    {
	      const block_T* b = binfo->block ();
	      for (block_T::output_const_iterator o (b->output ().begin ());
		   o != b->output ().end (); o++)
		{
		  const ioport_T& out = *o;
		  CspBlockInfo* outport = find_block (out.port ());
		  if (outport == NULL)
		    continue;
		  outport->get_SampleTime().output_acceptable (false);
		}
	    }
	}

      // ベースレートを単位として、サンプル時間を周期比にする
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  SampleTime& x = binfo->get_SampleTime ();

	  if (x.type () == SampleTime::ST_TYPE_DISCRETE)
	    {
	      if (x.svalue ().length () == 0)
		x.cvalue () = round (x.dvalue () / SampleTime::get_tick ());
	      if (x.soffset ().length () == 0)
		x.coffset () = round (x.doffset () / SampleTime::get_tick ());
	    }

	  // Pulse Generatorを低レート化する場合の処理
	  const block_T* b = binfo->block ();
	  if (b->blocktype () == "DiscretePulseGenerator"
	      && x.type () == SampleTime::ST_TYPE_INHERITED)
	    {
	      bool set_fast = true;
	      if (low_rate_pulsegenerator)
		{
		  const block_T::Period_optional& ppo = b->Period ();
		  const block_T::PulseWidth_optional& pwo = b->PulseWidth ();
		  const block_T::PulseType_optional& pto = b->PulseType ();
		  bool scalor_period = false;
		  double dpp = 0.0;

		  // PulseTypeが`Time based'で、スカラの場合のみ処理対象
		  // (ベクタは対象外)
		  if (ppo && pto && *pto == "Time based")
		    {
		      std::string spp = *ppo;
		      if (spp[0] != '[')
			{
			  char *p;
			  dpp = strtod (spp.c_str (), &p);
			  if ((*p == 0 || isspace (*p)) && dpp != 0.0)
			    scalor_period = true;
			}
		    }

		  if (scalor_period && pwo)
		    {
		      // PulseWidthからdutyを算出
		      double dhw = *pwo / 100.0;
		      double dlw = 1.0 - dhw;

		      // dutyからパルス幅の時間を算出
		      dhw *= dpp;
		      dlw *= dpp;

		      // highの時間とlowの時間の最大公約数を周期とする
		      unsigned long chw, clw, cd;
		      chw = round (dhw / SampleTime::get_tick ());
		      clw = round (dlw / SampleTime::get_tick ());
		      cd = SampleTime::gcd (chw, clw);
		      if (cd > 1)
			{
			  // 最大公約数が1を越えていれば、低レート化で
			  // きるので、PulseGeneratorのサンプル時間を作
			  // 成し、generated_sample_timesに登録する
			  x.cvalue () = cd;
			  x.coffset () = 0;
			  x.dvalue () = cd * SampleTime::get_tick ();
			  x.doffset () = 0;
			  x.type () = SampleTime::ST_TYPE_DISCRETE;
			  set_fast = false;
			  binfo->task_info () |= TASK_INFO_LOWRATE_PULSEGEN;
			  bool found = false;
			  for (std::vector < SampleTime* >::const_iterator
				 g (generated_sample_times.begin ());
			       g != generated_sample_times.end (); g++)
			    {
			      if (x == *(*g))
				{
				  found = true;
				  break;
				}
			    }
			  if (!found)
			    generated_sample_times.push_back (&x);
			}
		    }
		}

	      // 低レート化していなければ、サンプル時間は最速とする
	      if (set_fast)
		{
		  x = fast_sample_rate;
		  x.inherited () = true;
		}
	    }
	}

      // ブロックのコードからTIDを抽出
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();
	  if (b->code ().size () == 0)
	    continue;

	  const std::string& name = b->name ();

	  int tid = -1;
	  for (block_T::code_const_iterator cci (b->code ().begin ());
	       cci != b->code ().end (); cci++)
	    {
	      const code_T& code = *cci;
	      if (code.type () != "task" && code.type () != "update")
		continue;

	      // ブロックのtaskおよびupdateのコードから、TIDを参照して
	      // いるかパタンマッチを行い、TID番号を抽出する
	      const std::string& c_code = code;
	      codelist cl (c_code);
	      for (marklist_T::iterator mi (cl.mark_list ().begin ());
		   mi != cl.mark_list ().end (); mi++)
		{
		  c_mark& m = *mi;
		  if ((m.name () != "none" && m.name () != name)
		      || m.type () != 'S')
		    continue;

		  int xtid;
		  if (check_tid (m.beg (), m.end (), &xtid) != 0)
		    {
		      tid = xtid;
		      break;
		    }
		}
	    }

	  if (tid == -1)
	    continue;

	  // TID番号を設定
	  binfo->tid (tid);

	  // ブロックのサンプル時間が決まっていたら、それをTID番号をイ
	  // ンデクスとしたベクタに登録し、後でヒントとして使う
	  SampleTime& x = binfo->get_SampleTime ();
	  if (x.type () == SampleTime::ST_TYPE_DISCRETE)
	    {
	      if (tid_values.size () <= (size_t)tid)
		tid_values.resize (tid + 1, (SampleTime*)NULL);
	      if (tid_values[tid] == NULL)
		tid_values[tid] = &x;
	    }
	}

      // TID番号をもつブロックの処理
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  int tid = binfo->tid ();
	  if (tid == -1)
	    continue;

	  SampleTime& x = binfo->get_SampleTime ();

	  if (x.type () == SampleTime::ST_TYPE_DISCRETE)
	    continue;

	  // サンプル時間が決まっていない場合、登録されたヒントを調べ、
	  // あればそれを設定する

	  SampleTime* y = NULL;
	  if (tid_values.size () > (size_t)tid)
	    y = tid_values[tid];

	  // no TID hint
	  if (y == NULL)
	    continue;

	  x = *y;
	}

      // TIDにより抽出されたサンプル時間をsorted_SampleTimeに格納
      for (size_t i = 0; i < tid_values.size (); i++)
	{
	  SampleTime* st = tid_values[i];
	  if (st)
	    sorted_sample_times.push_back (sorted_SampleTime (i, st));
	}

      // 生成されたサンプル時間をsorted_SampleTimeに格納
      for (std::vector < SampleTime* >::const_iterator
	     i (generated_sample_times.begin ());
	   i != generated_sample_times.end (); i++)
	{
	  sorted_SampleTime g (sorted_SampleTime (~0U, *i));
	  if (std::find (sorted_sample_times.begin (),
			   sorted_sample_times.end (), g)
	      == sorted_sample_times.end ())
	    sorted_sample_times.push_back (g);
	}

      // 最速のレートを登録。これが最後に配置される
      sorted_sample_times.push_back (sorted_SampleTime (~0U,
							&fast_sample_rate));

      // サンプル時間をソート
      std::sort (sorted_sample_times.begin (), sorted_sample_times.end (),
		 sort_sampletime_func);

      // CONSTANTのサンプル時間をソートしたサンプル時間に登録
      for (csp_block_list_T::iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  if (binfo == NULL)
	    continue;
	  SampleTime& st = binfo->get_SampleTime();
	  if (st.type () == SampleTime::ST_TYPE_CONSTANT)
	    {
	      sorted_sample_times.push_back (sorted_SampleTime (~0U, &st));
	      break;
	    }
	}

      //
      // 継承解決のループ
      //

      // 変更があった場合はループを継続する
      bool inherited_change = true;

      // CONSTANTは継承しない。
      // ただし、継承が起きなくなったときに、一度
      // だけ、CONSTANTをINHETEDに変更し、継承を試す
      bool not_inherite_constant = false;

      while (inherited_change)
	{
	  inherited_change = false;

	  // 後段へ、サンプル時間の降順に継承を実施
	  for (std::vector < sorted_SampleTime >::const_iterator
		 s (sorted_sample_times.begin ());
	       s != sorted_sample_times.end (); s++)
	    {
	      const SampleTime* st = *s;
	      if (not_inherite_constant
		  && st->type () == SampleTime::ST_TYPE_CONSTANT)
		continue;

	      bool forward_change = true;
	      while (forward_change)
		{
		  forward_change = false;
		  for (csp_block_list_const_iterator
			 i (block_list ().begin ());
		       i != block_list ().end (); i++)
		    {
		      CspBlockInfo* binfo = *i;

		      if (binfo == NULL)
			continue;

		      SampleTime& x = binfo->get_SampleTime ();

		      // 継承できないサンプル時間はやらない
		      if (x.type () == SampleTime::ST_TYPE_NONE
			  || x.type () == SampleTime::ST_TYPE_INHERITED)
			continue;

		      // 現在試しているのサンプル時間と一致しない場合は
		      // やらない
		      if (*st != x)
			continue;

		      // Atomic SubSystemのブロックは継承を行わない
		      const CspBlockInfo* top = binfo->top_subsystem ();
		      if (top != NULL && top->is_atomic_subsystem ())
			continue;

		      const block_T* b = binfo->block ();

		      // AtomicではないTriggered SubSystemは継承を行わ
		      // ない(SubSystemではなくTriggerPortを参照する)
		      if (binfo->is_subsystem ()
			  && !binfo->is_atomic_subsystem ())
			{
			  CspBlockInfo::subsystem_type_T
			    fstype = binfo->subsystem_type ();
			  if (fstype != CspBlockInfo::TRIGGERED
			      || fstype != CspBlockInfo::ENABLED_TRIGGERED)
			    continue;
			}

		      CspBlockInfo* subsys = binfo->upper_subsystem();

		      if (b->blocktype () == "TriggerPort"
			  || b->blocktype () == "ActionPort")
			{
			  assert (subsys);
			  if (x.type () == SampleTime::ST_TYPE_CONSTANT
			      || x.type () == SampleTime::ST_TYPE_CONTINUOUS)
			    continue;

			  // ActionPortまたはTriggerPortの場合は、
			  // SubSystem内ブロック全体を継承対象とする
			  if (Inherit_Subsystem_SampleTime (subsys,
							    binfo, false))
			    {
			      inherited_change = true;
			      forward_change = true;
			    }
			}
		      else if (b->blocktype () == "EnablePort")
			{
			  assert (subsys);
			  if (x.type () == SampleTime::ST_TYPE_CONSTANT
			      || x.type () == SampleTime::ST_TYPE_CONTINUOUS)
			    continue;

			  // EnablePortの場合はSubSystemのブロックだけ
			  // を継承対象にする
			  if (Inherit_SampleTime (binfo, subsys, 0))
			    {
			      forward_change = true;
			      inherited_change = true;
			    }
			}
		      else if (b->blocktype () == "Outport")
			{
			  const IOPort* outp = find_outport_output (binfo);
			  if (outp == NULL)
			    continue;

			  // Outportはその先に継承を行う
			  const ioport_T& output = *outp->ioport_tag ();
			  inherit_forward (binfo, output,
					   forward_change, inherited_change);
			}
		      else
			{
			  // その他のブロックに対して継承を行う
			  inherit_forward (binfo, b,
					   forward_change, inherited_change);
			}
		    }
		}
	    }

	  // 継承が行われていればループを継続する
	  if (inherited_change)
	    continue;

	  // 前段へ、サンプル時間の降順に継承を実施
	  for (std::vector < sorted_SampleTime >::const_iterator
		 s (sorted_sample_times.begin ());
	       s != sorted_sample_times.end (); s++)
	    {
	      const SampleTime* st = *s;
	      if (not_inherite_constant
		  && st->type () == SampleTime::ST_TYPE_CONSTANT)
		continue;

	      bool backward_change = true;
	      while (backward_change)
		{
		  backward_change = false;
		  for (csp_block_list_const_iterator
			 i (block_list ().begin ());
		       i != block_list ().end (); i++)
		    {
		      CspBlockInfo* binfo = *i;
		      if (binfo == NULL)
			continue;

		      // Atomic SubSystemのブロックは継承を行わない
		      const CspBlockInfo* top = binfo->top_subsystem ();
		      if (top != NULL && top->is_atomic_subsystem ())
			continue;

		      // 継承できないサンプル時間はやらない
		      const SampleTime& x = binfo->get_SampleTime ();
		      if (x.type () == SampleTime::ST_TYPE_NONE
			  || x.type () == SampleTime::ST_TYPE_INHERITED
			  || x.type () == SampleTime::ST_TYPE_CONSTANT
			  || x.type () == SampleTime::ST_TYPE_CONTINUOUS)
			continue;

		      // 現在試しているのサンプル時間と一致しない場合は
		      // やらない
		      if (*st != x)
			continue;

		      const block_T* b = binfo->block ();
		      if (b->blocktype () == "Inport")
			{
			  // Inportはその先に景趣を行う
			  const IOPort* inp = find_inport_input (binfo);
			  if (inp == NULL)
			    continue;
			  const ioport_T& input = *inp->ioport_tag ();
			  inherit_backward (binfo, input,
					    backward_change, inherited_change);
			}
		      else
			{
			  // その他のブロックに対して継承を行う
			  inherit_backward (binfo, b,
					    backward_change, inherited_change);
			}
		    }
		}
	    }

	  // 継承が行われていればループを継続する
	  if (inherited_change)
	    continue;

	  // 継承が止まったので、継承できる範囲を色分けする
	  for (std::vector < sorted_SampleTime >::const_iterator
		 s (sorted_sample_times.begin ());
	       s != sorted_sample_times.end (); s++)
	    {
	      const SampleTime* st = *s;
	      if (st->type () == SampleTime::ST_TYPE_CONSTANT)
		continue;
	      for (csp_block_list_const_iterator i (block_list ().begin ());
		   i != block_list ().end (); i++)
		{
		  const CspBlockInfo* binfo = *i;
		  if (binfo == NULL)
		    continue;

		  // Atomic SubSystemはやらない
		  const CspBlockInfo* top = binfo->top_subsystem ();
		  if (top != NULL && top->is_atomic_subsystem ())
		    continue;

		  // 継承可能なサンプル時間の候補を持たない場合はやらない
		  const SampleTime* match = NULL;
		  if (!is_input_inherit_candidate (binfo, st, match)
		      || !is_output_inherit_candidate (binfo, st, match)
		      || match == NULL)
		    continue;

		  if (verbose)
		    {
		      std::cout << "Check coloring from: "
				  << binfo->block ()->blocktype ()
				  << ':'
				  << binfo->block ()->name ()
				  << '(' << binfo->id () << ')'
				  << '[' << *st << ']'
				  << std::endl;
		      // dump_block_rate (*binfo->block (), 1);
		    }

		  // ブロックの後段、前段についてサンプル時間の候補で色分け
		  std::vector < int > color (block_list ().size () + 1, 0);
		  color_forward (binfo, binfo, *st, color.data ());
		  color_backward (binfo, binfo, *st, color.data ());

		  // レートを決定できないブロックの除外処理
		  bool color_changed = true;
		  while (color_changed)
		    {
		      color_changed = false;

		      for (size_t id = 0; id < color.size (); id++)
			{
			  if (color[id] != 1)
			    continue;

			  const CspBlockInfo* xbinfo = block_list ()[id];

			  // レートが決定可能でない場合は、除外する
			  if (!check_input_color (xbinfo, color.data ()))
			    {
			      if (verbose)
				{
				  const SampleTime&
				    xst = xbinfo->get_SampleTime ();
				  std::cout << "Uncoloring: "
					      << xbinfo->block ()->blocktype ()
					      << ':'
					      << xbinfo->block ()->name ()
					      << '(' << xbinfo->id () << ')'
					      << '[' << xst << ']'
					      << std::endl;
				}
			      color[id] = 0;
			      color_changed = true;
			      break;
			    }
			}
		    }

		  // 残ったものについてサンプル時間を設定
		  for (size_t id = 0; id < color.size (); id++)
		    {
		      if (color[id] != 1)
			continue;

		      SampleTime& x = block_list ()[id]->get_SampleTime ();
		      if (x.type () == SampleTime::ST_TYPE_INHERITED)
			{
			  if (verbose)
			    {
			      CspBlockInfo* xb = block_list ()[id];
			      std::cout << "Coloring: "
					  << xb->block ()->blocktype ()
					  << ':'
					  << xb->block ()->name ()
					  << '(' << id << ')'
					  << '[' << *st << ']'
					  << std::endl;
			      // dump_block_rate (*xb->block (), 1);
			    }
			  x = *st;
			  x.inherited () = true;
			  inherited_change = true;
			}
		    }
		}
	    }

	  // 継承が行われていればループを継続する
	  if (inherited_change)
	    continue;

	  // 継承が止まったので、一度だけCONSTANTをINHERITEDに変更して
	  // もう一度試す
	  if (!not_inherite_constant)
	    {
	      not_inherite_constant = true;

	      bool change_back = false;
	      for (csp_block_list_const_iterator i (block_list ().begin ());
		   i != block_list ().end (); i++)
		{
		  CspBlockInfo* binfo = *i;
		  if (binfo == NULL)
		    continue;
		  SampleTime& st = binfo->get_SampleTime ();
		  if (st.inherited ()
		      && st.type () == SampleTime::ST_TYPE_CONSTANT)
		    {
		      if (verbose)
			std::cout << "Revert from Inf: "
				    << binfo->block ()->blocktype ()
				    << ':'
				    << binfo->block ()->name ()
				    << '(' << binfo->id () << ')'
				    << '[' << st << ']'
				    << std::endl;
		      st.inherited () = false;
		      st.type () = SampleTime::ST_TYPE_INHERITED;
		      change_back = true;
		    }
		}

	      if (change_back)
		inherited_change = true;
	    }

	  // TODO: heuristic
	}

      if (verbose)
	dump_block_rate ();

#if 0
      // debug
      if (verbose)
	{
	  if (inherited_change)
	    std::cout << "* --------" << std::endl;
	  else
	    std::cout << "# --------" << std::endl;
	  dump_rate (true);
	}
#endif

      // サンプル時間が決定していない信号発生のブロック
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();

	  SampleTime& x = binfo->get_SampleTime ();

	  // 低レート化したPulseGeneratorは除く
	  if (low_rate_pulsegenerator
	      && b->blocktype () == "DiscretePulseGenerator"
	      && x.type () == SampleTime::ST_TYPE_DISCRETE)
	    continue;

	  // 信号発生ブロックなら最速のレートにする
	  if (is_signal_source_block (b))
	    {
	      x = fast_sample_rate;
	      x.inherited () = true;
	      if (verbose)
		std::cout << "Set fast rate(Signal Source)[" << x << "]: "
			    << b->blocktype () << ':' << b->name ()
			    << '(' << binfo->id () << ')'
			    << std::endl;
	    }
	}

      // その他未決定なものの処理
      for (csp_block_list_const_iterator i (block_list ().begin ());
	   i != block_list ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;

	  if (binfo == NULL)
	    continue;

	  const block_T* b = binfo->block ();
	  SampleTime& x = binfo->get_SampleTime ();
	  if (x.type () != SampleTime::ST_TYPE_INHERITED)
	    continue;

	  const CspBlockInfo* top = binfo->top_subsystem ();
	  if (top != NULL && top->is_atomic_subsystem ())
	    {
	      // 最上位がAtomic SubSystemのブロックは不要だが、
	      // SubSystemのサンプル時間が決まっていれば一応それを設定
	      // する
	      const SampleTime& y = top->get_SampleTime ();
	      if (y.type () == SampleTime::ST_TYPE_DISCRETE)
		{
		  x = y;
		  x.inherited ();
		  if (verbose)
		    std::cout << "Inherit from Top Subsystem: ["
				<< y << "]: "
				<< b->blocktype () << ':' << b->name ()
				<< '(' << binfo->id () << ')'
				<< std::endl;
		  continue;
		}
	    }

	  // SubSystem内のブロックはSubSystemのサンプル時間が決まってい
	  // たらそれを設定する
	  const CspBlockInfo* subsys = binfo->upper_subsystem ();
	  if (subsys != NULL)
	    {
	      const SampleTime& y = subsys->get_SampleTime ();
	      if (y.type () == SampleTime::ST_TYPE_DISCRETE)
		{
		  x = y;
		  x.inherited () = true;
		  if (verbose)
		    std::cout << "Inherit from Upper Subsystem: ["
				<< y << "]: "
				<< b->blocktype () << ':' << b->name ()
				<< '(' << binfo->id () << ')'
				<< std::endl;
		  continue;
		}
	    }

	  // それでも残ってしまったものは最速のレートを設定する
	  x = fast_sample_rate;
	  x.inherited () = true;
	  if (verbose)
	    std::cout << "Set fast rate[" << x << "]: "
			<< b->blocktype () << ':' << b->name ()
			<< '(' << binfo->id () << ')'
			<< std::endl;
	}

      if (verbose)
	dump_block_rate (true);

      return is_error() ? -1 : 0;
    }

    void
    CspReader::inherit_forward (CspBlockInfo* binfo,
				const SimulinkModel::XSD::ioport_T& output,
				bool& forward_change, bool& inherited_change)
    {
      for (ioport_T::connect_const_iterator ci (output.connect ().begin ());
	   ci != output.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  CspBlockInfo* fb = find_block (c.block ());

	  if (fb == NULL)
	    continue;

	  const block_T* f = fb->block ();

	  if (is_pass_block (f))
	    {
	      inherit_forward (binfo, f, forward_change, inherited_change);
	      continue;
	    }

	  CspBlockInfo::subsystem_type_T fstype = fb->subsystem_type ();

	  if (f->blocktype () == "RateTransition")
	    continue; // not inherit forward, backward only

	  if (fb->is_subsystem ()
	      && !fb->is_atomic_subsystem ()
	      && !fb->is_noblock_subsystem ())
	    {
	      CspBlockInfo* port = find_block (c.port ());

	      if (fstype == CspBlockInfo::ENABLED_TRIGGERED
		  || fstype == CspBlockInfo::TRIGGERED)
		{
		  if (port != fb->trigger_port ())
		    continue;
		}
	      else if (fstype == CspBlockInfo::ACTION)
		{
		  if (port != fb->action_port ())
		    continue;
		}
	      fb = port;
	    }

	  const SampleTime& x = fb->get_SampleTime ();
	  if (x.type () != SampleTime::ST_TYPE_INHERITED
	      && x.type () != SampleTime::ST_TYPE_CONTINUOUS
	      && x.is_input_acceptable ())
	    continue;

	  const CspBlockInfo* from = binfo;
	  const SampleTime* st = &binfo->get_SampleTime ();
	  if (!check_input_inheritable (fb, from, st))
	    continue;

	  if (Inherit_SampleTime (from, fb, 0))
	    {
	      forward_change = true;
	      inherited_change = true;
	    }
	}
    }

    void
    CspReader::inherit_forward (CspBlockInfo* binfo,
				const SimulinkModel::XSD::block_T* b,
				bool& forward_change, bool& inherited_change)
    {
      for (block_T::output_const_iterator oi (b->output ().begin ());
	   oi != b->output ().end (); oi++)
	{
	  const ioport_T& output = *oi;
	  inherit_forward (binfo, output, forward_change, inherited_change);
	}
    }

    void
    CspReader::inherit_backward (CspBlockInfo* binfo,
				 const SimulinkModel::XSD::ioport_T& input,
				 bool& backward_change, bool& inherited_change)
    {
      for (ioport_T::connect_const_iterator ci (input.connect ().begin ());
	   ci != input.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  CspBlockInfo* pb = get_connect_block (c);

	  if (pb == NULL || pb == binfo)
	    continue;

	  const block_T* p = pb->block ();

	  if (is_pass_block (p))
	    {
	      inherit_backward (binfo, p, backward_change, inherited_change);
	      continue;
	    }

	  const SampleTime& x = pb->get_SampleTime ();
	  if (x.type () != SampleTime::ST_TYPE_INHERITED
	      && x.type () != SampleTime::ST_TYPE_CONTINUOUS
	      && x.type () != SampleTime::ST_TYPE_CONSTANT
	      && !x.is_output_acceptable ())
	    continue;

	  const CspBlockInfo* to = binfo;
	  const SampleTime* st = &binfo->get_SampleTime ();
	  if (!check_output_inheritable (pb, to, st))
	    continue;

	  if (Inherit_SampleTime (to, pb, 1))
	    {
	      backward_change = true;
	      inherited_change = true;
	    }
	}
    }

    void
    CspReader::inherit_backward (CspBlockInfo* binfo,
				 const SimulinkModel::XSD::block_T* b,
				 bool& backward_change, bool& inherited_change)
    {
      for (block_T::input_const_iterator ii (b->input ().begin ());
	   ii != b->input ().end (); ii++)
	{
	  const ioport_T& input = *ii;
	  inherit_backward (binfo, input, backward_change, inherited_change);
	}
    }

    bool
    CspReader::is_input_inherit_candidate (const SimulinkModel::XSD::ioport_T& input,
					   const SampleTime*& st,
					   const SampleTime*& match) const
    {
      assert (st);

      for (ioport_T::connect_const_iterator ci (input.connect ().begin ());
	   ci != input.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* pbinfo = get_connect_block (c);
	  const SampleTime& p = pbinfo->get_SampleTime ();
	  if (*st == p)
	    match = &p;
	  else if (!st->acceptable_rate (p) && !p.is_inheritable ()) // XXX
	    return false;
	}
      return true;
    }


    bool
    CspReader::is_input_inherit_candidate (const CspBlockInfo* binfo,
					   const SampleTime*& st,
					   const SampleTime*& match) const
    {
      if (binfo->is_subsystem ()
	  && !binfo->is_atomic_subsystem ()
	  && !binfo->is_noblock_subsystem ())
	return false;

      if (binfo->get_SampleTime ().type () != SampleTime::ST_TYPE_INHERITED)
	return false;

      const block_T* b = binfo->block ();

      if (is_inport_type (b))
	{
	  // 入力ポートの場合は、その先に切り替える
	  const IOPort* inp = find_inport_input (binfo);
	  if (inp)
	    {
	      const ioport_T* in = inp->ioport_tag ();
	      if (!is_input_inherit_candidate (*in, st, match))
		return false;
	    }
	}
      else
	{
	  // 入力全てに対して確認
	  for (block_T::input_const_iterator ii (b->input ().begin ());
	       ii != b->input ().end (); ii++)
	    {
	      if (!is_input_inherit_candidate (*ii, st, match))
		return false;
	    }
	}

      return true;
    }


    bool
    CspReader::is_output_inherit_candidate (const SimulinkModel::XSD::ioport_T& output,
					    const SampleTime*& st,
					    const SampleTime*& match) const
    {
      assert (st);

      for (ioport_T::connect_const_iterator ci (output.connect ().begin ());
	   ci != output.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* fbinfo = get_connect_block (c);
	  const SampleTime& f = fbinfo->get_SampleTime ();
	  if (*st == f)
	    match = &f;
	  else if (!st->acceptable_rate (f) && !f.is_inheritable ()) // XXX
	    return false;
	}
      return true;
    }


    bool
    CspReader::is_output_inherit_candidate (const CspBlockInfo* binfo,
					    const SampleTime*& st,
					    const SampleTime*& match) const
    {
      if (binfo->is_subsystem ()
	  && !binfo->is_atomic_subsystem ()
	  && !binfo->is_noblock_subsystem ())
	return false;

      if (binfo->get_SampleTime ().type () != SampleTime::ST_TYPE_INHERITED)
	return false;

      const block_T* b = binfo->block ();

      if (b->blocktype () == "Outport")
	{
	  const IOPort* outp = find_outport_output (binfo);
	  if (outp)
	    {
	      const ioport_T* out = outp->ioport_tag ();
	      if (!is_output_inherit_candidate (*out, st, match))
		return false;
	    }
	}
      else
	{
	  for (block_T::output_const_iterator oo (b->output ().begin ());
	       oo != b->output ().end (); oo++)
	    {
	      if (!is_output_inherit_candidate (*oo, st, match))
		return false;
	    }
	}

      return true;
    }


    bool
    CspReader::is_inherit_candidate (const CspBlockInfo* binfo,
				     const SampleTime*& x) const
    {
      const SampleTime* match = NULL;
      return (is_input_inherit_candidate (binfo, x, match)
	      || is_output_inherit_candidate (binfo, x, match));
    }


    bool
    CspReader::inherit_merge_blocks (CspBlockInfo* binfo) const
    {
      bool change = false;
      const SampleTime& f = binfo->get_SampleTime ();
      for (csp_block_list_const_iterator
	     mi (binfo->merge_blocks ().begin ());
	   mi != binfo->merge_blocks ().end (); mi++)
	{
	  CspBlockInfo* mbinfo = *mi;
	  SampleTime& x = mbinfo->get_SampleTime ();

	  if (x == f
	      || x.type () == SampleTime::ST_TYPE_CONSTANT
	      || x.type () == SampleTime::ST_TYPE_CONTINUOUS)
	    continue;

	  if (x.type () != SampleTime::ST_TYPE_INHERITED)
	    std::cout << "Warning: merged block: "
			<< mbinfo->block ()->name ()
			<< '(' << mbinfo->id () << ')'
			<< '[' << x << ']'
			<< ": different rate at "
			<< binfo->block ()->name ()
			<< '(' << binfo->id () << ')'
			<< '[' << f << ']'
			<< std::endl;
	  x = f;
	  x.inherited () = true;
	  change = true;
	  if (verbose)
	    std::cout << "Inherit merged block: "
			<< binfo->block ()->blocktype ()
			<< ':'
			<< binfo->block ()->name ()
			<< '(' << binfo->id () << ')'
			<< '[' << f << ']'
			<< " => "
			<< mbinfo->block ()->blocktype ()
			<< ':'
			<< mbinfo->block ()->name ()
			<< '(' << mbinfo->id () << ')'
			<< std::endl;
	}
      return change;
    }

    void
    CspReader::color_forward (const CspBlockInfo* head,
			      const SimulinkModel::XSD::ioport_T& output,
			      const SampleTime& st, int *color)
    {
      for (ioport_T::connect_const_iterator ci (output.connect ().begin ());
	   ci != output.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* fbinfo = get_connect_block (c);

	  const SampleTime* x = &st;
	  if (color[fbinfo->id ()] == 0
	      && fbinfo->get_SampleTime ().is_input_acceptable ()
	      && is_inherit_candidate (fbinfo, x))
	    color_forward (head, fbinfo, st, color);
	}
    }

    void
    CspReader::color_forward (const CspBlockInfo* head,
			      const CspBlockInfo* binfo,
			      const SampleTime& st, int *color)
    {
      color[binfo->id ()] = -1;

      const block_T* b = binfo->block ();

      if (b->blocktype () == "Outport")
	{
	  const IOPort* outp = find_outport_output (binfo);
	  if (outp)
	    {
	      const ioport_T* output = outp->ioport_tag ();
	      color_forward (head, *output, st, color);
	    }
	}
      else
	{
	  for (block_T::output_const_iterator oo (b->output ().begin ());
	       oo != b->output ().end (); oo++)
	    color_forward (head, *oo, st, color);
	}

      color[binfo->id ()] = 1;
    }

    void
    CspReader::color_backward (const CspBlockInfo* head,
			       const SimulinkModel::XSD::ioport_T& input,
			       const SampleTime& st, int *color)
    {
      for (ioport_T::connect_const_iterator ci (input.connect ().begin ());
	   ci != input.connect ().end (); ci++)
	{
	  const connection_T& c = *ci;
	  const CspBlockInfo* fbinfo = get_connect_block (c);

	  const SampleTime* x = &st;
	  if (color[fbinfo->id ()] == 0
	      && fbinfo->get_SampleTime ().is_output_acceptable ()
	      && is_inherit_candidate (fbinfo, x))
	    color_backward (head, fbinfo, st, color);
	}
    }


    void
    CspReader::color_backward (const CspBlockInfo* head,
			       const CspBlockInfo* binfo,
			       const SampleTime& st, int *color)
    {
      color[binfo->id ()] = -1;

      const block_T* b = binfo->block ();

      if (is_inport_type (b))
	{
	  const IOPort* outp = find_outport_output (binfo);
	  if (outp)
	    {
	      const ioport_T* output = outp->ioport_tag ();
	      color_backward (head, *output, st, color);
	    }
	}
      else
	{
	  for (block_T::output_const_iterator oo (b->output ().begin ());
	       oo != b->output ().end (); oo++)
	    color_backward (head, *oo, st, color);
	}

      color[binfo->id ()] = 1;
    }

    /* エッジのラベルを設定する */
    void
    CspReader::add_label_to_edge (CspEdge& e, const std::string& var)
    {
      long t_id = e.tail ().id ();
      long h_id = e.node ()->id ();

      std::ostringstream oss;
      oss << "ch_" << t_id << '_' << h_id;
      if (var.length ())
	oss << '(' << var << ')';

      boost::put (boost::edge_Label, get_graph (), e.get_edge (), oss.str ());
    }

    void
    CspReader::add_var_list_label_to_edge (CspEdge& ce)
    {
      std::string var;

      for (PropagateVarList::const_iterator i (ce.var ().order ().begin ());
	   i != ce.var ().order ().end (); i++)
	{
	  const PropagateVar* pv = *i;
	  if (var.size ())
	    var += ',';
	  var += (*pv)->name ();
	}

      add_label_to_edge (ce, var);
    }

    //
    // CspNodeのメンバ関数
    //

    std::pair < CspEdge*, CspEdge* >
    CspNode::add_edge (CspNode* c)
    {
      n_forw_edges.push_back (CspEdge (get_graph(), *this, c, true));
      CspEdge& f = n_forw_edges.back ();
      c->back_edges ().push_back (CspEdge (get_graph (), *c, this, false));
      CspEdge& b = c->back_edges ().back ();
      f.pair () = &b;
      b.pair () = &f;
      return std::make_pair (&f, &b);
    }

    void
    CspNode::clear (void)
    {
      for (csp_block_list_const_iterator i (n_contain_blocks.begin ());
	   i != n_contain_blocks.end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  binfo->remove_node (this);
	}
      if (n_valid_vertex)
	{
	  boost::clear_vertex (get_vertex (), get_graph ());
	  boost::remove_vertex (get_vertex (), get_graph ());
	}
    }

    CspNode&
    CspNode::add_node (CspGraph& g, CspBlockInfo* binfo)
    {
      n_csp_node.push_back (CspNode (g, binfo));
      CspNode& c = n_csp_node.back ();
      if (binfo)
	{
	  c.add_block (binfo);
	  c.id () = binfo->id ();
	}
      n_csp_node_count++;
      return c;
    }


    cspnode_list_iterator
    CspNode::remove_node (cspnode_list_iterator iter)
    {
      CspNode& x = *iter;
      x.disconnect ();
      x.clear ();
      n_csp_node_count--;
      return n_csp_node.erase (iter);
    }

    void
    CspNode::remove_node (CspNode& x)
    {
      x.disconnect ();
      x.clear ();
      n_csp_node_count--;
      n_csp_node.remove (x);
    }


    void
    CspNode::disconnect (void)
    {
      for (CspEdgeList::iterator f (forw_edges ().begin ());
	   f != forw_edges ().end (); f++)
	{
	  CspNode* c = *f;
	  c->del_back_edge (this);
	}

      for (CspEdgeList::iterator b (back_edges ().begin ());
	   b != back_edges ().end (); b++)
	{
	  CspNode* c = *b;
	  c->del_forw_edge (this);
	}
    }


    void
    CspNode::reconnect (CspNode* n)
    {
      CspEdgeList fmatch;
      CspEdgeList bmatch;

      for (CspEdgeList::iterator f (forw_edges ().begin ());
	   f != forw_edges ().end (); f++)
	fmatch.push_back (*f);

      for (CspEdgeList::iterator b (back_edges ().begin ());
	   b != back_edges ().end (); b++)
	bmatch.push_back (*b);

      for (CspEdgeList::iterator f (fmatch.begin ()); f != fmatch.end (); f++)
	{
	  // this-[fe]->this_forw[c]
	  CspEdge& fe = *f;
	  CspNode* c = fe.node ();
	  if (c != n && c != this)
	    {
	      // this<-[be]-this_forw[c]
	      CspEdge* be = c->get_back_edge (this);
	      assert (be);

	      CspEdge* nfe = NULL;
	      CspEdge* nbe = NULL;

	      // target[n]-[nfe]->this_forw[c]
	      // target[n]<-[nbe]-this_forw[c]
	      std::pair < CspEdge*, CspEdge* > p = n->get_edge (c);
	      if (p.first)
		{
		  nfe = p.first;
		  nbe = p.second;
		}
	      else
		{
		  p = n->add_edge (c);
		  nfe = p.first;
		  nbe = p.second;
		}

	      // this-[fe]->this_fow[c] => target[n]-[nfe]->this_forw[c]
	      nfe->copy_connect (fe.block_connect_list ());
	      // this<-[be]-this_forw[c] => target[n]<-[nbe]-this_forw[c]
	      nbe->copy_connect (be->block_connect_list ());
	    }
	  c->del_back_edge (this);
	}

      for (CspEdgeList::iterator b (bmatch.begin ()); b != bmatch.end (); b++)
	{
	  // this_back[c]-[be]->this
	  CspEdge& be = *b;
	  CspNode* c = be.node ();
	  if (c != n && c != this)
	    {
	      // this_back[c]-[fe]->this
	      CspEdge* fe = c->get_forw_edge (this);
	      assert (fe);

	      CspEdge* nfe = NULL;
	      CspEdge* nbe = NULL;

	      // this_back[c]-[nfe]->target[n]
	      // this_back[c]<-[nbe]-target[n]
	      std::pair < CspEdge*, CspEdge* > p = c->get_edge (n);
	      if (p.first)
		{
		  nfe = p.first;
		  nbe = p.second;
		}
	      else
		{
		  p = c->add_edge (n);
		  nfe = p.first;
		  nbe = p.second;
		}

	      // this_back[c]-[fe]->this => this_back[c]-[nfe]->target[n]
	      nfe->copy_connect (fe->block_connect_list ());
	      // this_back[c]<-[be]-this => this_back[c]<-[nbe]-target[n]
	      nbe->copy_connect (be.block_connect_list ());
	    }
	  c->del_forw_edge (this);
	}
    }


    void
    CspNode::copy_contain_blocks (const CspNode& x)
    {
      for (csp_block_list_const_iterator i (x.contain_blocks ().begin ());
	   i != x.contain_blocks ().end (); i++)
	{
	  CspBlockInfo* binfo = *i;
	  add_block (binfo);
	}
    }


    void
    CspNode::join (CspNode& x)
    {
      copy_contain_blocks (x);
      x.reconnect (this);
      n_csp_node.remove (x);
    }


    void
    CspNode::correct (const CspBlockInfo* bi)
    {
      const block_T* b = bi->block ();

      // vars
      for (block_T::var_const_iterator vi (b->var ().begin ());
	   vi != b->var ().end (); vi++)
	{
	  const var_T& v = *vi;

	  var_map_iterator x = n_var.find (v.name ());
	  if (x != n_var.end ())
	    continue;

	  // 単独の外部出力ポートの変数は出力である
	  var_mode_T vmode;
	  if (b->blocktype () == "Outport"
	      && bi->upper () == NULL
	      && v.mode () == "extout")
	    vmode = VAR_MODE_OUT;
	  else
	    vmode = VAR_MODE_AUTO;

	  n_var.add_var_map (v.name (), &v, vmode);
	}

      long id = bi->id ();

      // signals, states, params
      for (block_T::signal_const_iterator vi (b->signal ().begin ());
	   vi != b->signal ().end (); vi++)
	n_signal.add_param (id, &*vi);

      for (block_T::state_const_iterator vi (b->state ().begin ());
	   vi != b->state ().end (); vi++)
	n_state.add_param (id, &*vi);

      for (block_T::trigger_state_const_iterator
	     vi (b->trigger_state ().begin ());
	   vi != b->trigger_state ().end (); vi++)
	n_trigger_state.add_param (id, &*vi);

      for (block_T::invariant_signal_const_iterator
	     vi (b->invariant_signal ().begin ());
	   vi != b->invariant_signal ().end (); vi++)
	n_invariant_signal.add_param (id, &*vi);

      for (block_T::param_const_iterator vi (b->param ().begin ());
	   vi != b->param ().end (); vi++)
	n_param.add_param (id, &*vi);

      for (block_T::const_param_const_iterator vi (b->const_param ().begin ());
	   vi != b->const_param ().end (); vi++)
	n_const_param.add_param (id, &*vi);

      for (block_T::code_const_iterator ci (b->code ().begin ());
	   ci != b->code ().end (); ci++)
	n_code.add_code (bi, b->name (), &*ci);

      // peinfo
      if (head_block () == bi)
	{
	  block_T::peinfo_optional po = b->peinfo ();
	  if (po)
	    {
	      const std::string& peinfo = *po;
	      std::istringstream iss;
	      if (peinfo[0] == 'H')
		{
		  iss.str (std::string (peinfo, 1));
		  n_coretype = HARDWARE_CORE;
		}
	      else if (peinfo[0] == 'S')
		{
		  iss.str (std::string (peinfo, 1));
		  n_coretype = SOFTWARE_CORE;
		}
	      else
		{
		  iss.str (peinfo);
		  n_coretype = SOFTWARE_CORE;
		}
	      int n;
	      iss >> n;
	      if (n == -1 || (0 <= n && n < core_num))
		n_core = n;
	      else
		std::cout << "Warning: invalid peinfo[" << peinfo << "]: "
			  << b->name () << std::endl;
	    }
	}

      // DataStoreMemory Name
      unsigned long ds_attr = 0;
      if (b->blocktype () == "DataStoreRead")
	ds_attr = 1;
      else if (b->blocktype () == "DataStoreWrite")
	ds_attr = 2;
      if (ds_attr)
	{
	  const block_T::DataStoreName_optional& o = b->DataStoreName ();
	  if (o)
	    {
	      const std::string& nm = *o;
	      dsm_info_list_T::iterator e (n_dsm.end ());

	      if (std::find (n_dsm.begin (), e, nm) == e)
		n_dsm.push_back (dsm_info (nm, ds_attr));
	    }
	}
    }


    void
    CspNode::correct (void)
    {
      n_var.clear ();
      n_signal.clear ();
      n_state.clear ();
      n_trigger_state.clear ();
      n_invariant_signal.clear ();
      n_param.clear ();
      n_const_param.clear ();
      n_code.clear ();
      n_dsm.clear ();

      for (CspEdgeList::const_iterator ce (forw_edges ().begin ());
	   ce != forw_edges ().end (); ce++)
	{
	  const CspEdge& e = *ce;
	  for (csp_const_block_connect_list_T::const_iterator
		 l (e.block_connect_list ().begin ());
	       l != e.block_connect_list ().end (); l++)
	    {
	      const CspBlockConnect* bc = *l;
	      for (csp_var_list_T::const_iterator
		     vi (bc->var_list ().begin ());
		   vi != bc->var_list ().end (); vi++)
		{
		  const var_T* v = *vi;
		  const std::string& vname = v->name ();
		  var_map_iterator x = var ().find (vname);
		  if (x == var ().end ())
		    {
		      const CspBlockInfo* tail = bc->tail ();
		      const var_T* tv = tail->find_var (vname);
		      assert (tv);
		      var ().add_var_map (vname, tv, VAR_MODE_OUT);
		    }
		  else
		    x->second.mode () |= VAR_MODE_OUT;
		}
	    }
	}

      for (CspEdgeList::const_iterator ce (back_edges ().begin ());
	   ce != back_edges ().end (); ce++)
	{
	  const CspEdge& e = *ce;
	  for (csp_const_block_connect_list_T::const_iterator
		 l (e.block_connect_list ().begin ());
	       l != e.block_connect_list ().end (); l++)
	    {
	      const CspBlockConnect* bc = *l;
	      for (csp_var_list_T::const_iterator
		     vi (bc->var_list ().begin ());
		   vi != bc->var_list ().end (); vi++)
		{
		  const var_T* v = *vi;
		  const std::string& vname = v->name ();
		  var_map_iterator x = var ().find (vname);
		  if (x == var ().end ())
		    {
		      const CspBlockInfo* head = bc->head ();
		      const var_T* hv = head->find_var (vname);
		      assert (hv);
		      var ().add_var_map (vname, hv, VAR_MODE_IN);
		    }
		  else
		    x->second.mode () |= VAR_MODE_IN;
		}
	    }
	}

      for (csp_block_list_const_iterator i (n_contain_blocks.begin ());
	   i != n_contain_blocks.end (); i++)
	correct (*i);

      n_code.sort ();
    }

    void
    CspNode::correct_nodes (void)
    {
      for (cspnode_list_iterator ci (n_csp_node.begin ());
	   ci != n_csp_node.end (); ci++)
	ci->correct ();
    }

    void
    CspNode::make_nodelist (void)
    {
      int cn_index = 0;
      n_csp_nodelist.resize (n_csp_node.size (), (CspNode*)NULL);
      for (cspnode_list_iterator ci (n_csp_node.begin ());
	   ci != n_csp_node.end (); ci++)
	{
	  CspNode& cn = *ci;
	  n_csp_nodelist[cn_index] = &cn;
	  cn.n_index = cn_index++;
	}
    }

    void CspNode::dump (void) const
    {
      if (info () & CSP_NODE_INFO_TASK_0)
	std::cout << "<task0>" << std::endl;
      else if (info () & CSP_NODE_INFO_TASK_EXTOUT)
	std::cout << "<extout>" << std::endl;
      else
	{
	  const CspBlockInfo* binfo = head_block ();
	  const block_T* b = binfo->block ();

	  std::cout << "Node[" << id () << "]@" << pos () << ": "
		    << b->blocktype () << ":"
		    << b->name ()
		    << std::endl;

	  std::cout << "  Include: " << std::endl;
	  for (csp_block_list_const_iterator
		 j (contain_blocks ().begin ());
	       j != contain_blocks ().end (); j++)
	    {
	      const CspBlockInfo* tbinfo = *j;
	      const block_T* tb = tbinfo->block ();
	      int ct = 0;
	      if (tbinfo->has_update () && !tbinfo->is_sfunction ())
		ct |= 2;
	      for (block_T::code_const_iterator ci (tb->code ().begin ());
		   ci != tb->code ().end (); ci++)
		{
		  if (ci->type () == "task")
		    ct |= 1;
		}
	      std::cout << "   "
			<< (ct == 3 ? '+' : ct == 1 ? '*' : ' ')
			<< tb->blocktype ()
			<< ":"
			<< tb->name ()
			<< '['
			<< tbinfo->id ()
			<< ']';
	      int line = tbinfo->line_position ();
	      if (line != -1)
		std::cout << "<@" << line << '>';
	      std::cout << std::endl;
	    }
	}

      if (forw_edges().size ())
	{
	  std::cout << "  Forward:" << std::endl;
	  for (CspEdgeList::const_iterator j (forw_edges ().begin ());
	       j != forw_edges ().end (); j++)
	    {
	      const CspEdge& e = *j;
	      const CspNode* f = e.node ();
	      if (f->info () & CSP_NODE_INFO_TASK_0)
		std::cout << "    <task0>" << std::endl;
	      else if (f->info () & CSP_NODE_INFO_TASK_EXTOUT)
		std::cout << "    <extout>" << std::endl;
	      else
		{
		  const CspBlockInfo* tbinfo = f->head_block ();
		  const block_T* tb = tbinfo->block ();
		  std::cout << "    " << tb->blocktype ()
			    << ":"
			    << tb->name ()
			    << '['
			    << tbinfo->id ()
			    << ']'
			    << std::endl;
		}
	    }
	}

      if (back_edges().size ())
	{
	  std::cout << "  Backward:" << std::endl;
	  for (CspEdgeList::const_iterator j (back_edges ().begin ());
	       j != back_edges ().end (); j++)
	    {
	      const CspEdge& e = *j;
	      const CspNode* f = e.node ();
	      if (f->info () & CSP_NODE_INFO_TASK_0)
		std::cout << "    <task0>" << std::endl;
	      else if (f->info () & CSP_NODE_INFO_TASK_EXTOUT)
		std::cout << "    <extout>" << std::endl;
	      else
		{
		  const CspBlockInfo* tbinfo = f->head_block ();
		  const block_T* tb = tbinfo->block ();
		  std::cout << "    " << tb->blocktype ()
			    << ":"
			    << tb->name ()
			    << '['
			    << tbinfo->id ()
			    << ']'
			    << std::endl;
		}
	    }
	}

      if (signal ().size ())
	{
	  std::cout << "  Signals:" << std::endl;

	  for (ParamList::const_iterator pi (signal ().begin ());
	       pi != signal ().end (); pi++)
	    {
	      std::cout << "    " << (*pi)->storage()
			<< '.' << (*pi)->name ()
			<< std::endl;
	    }
	}

      if (state ().size ())
	{
	  std::cout << "  States:" << std::endl;

	  for (ParamList::const_iterator pi (state ().begin ());
	       pi != state ().end (); pi++)
	    {
	      std::cout << "    " << (*pi)->storage()
			<< '.' << (*pi)->name ()
			<< std::endl;
	    }
	}

      if (trigger_state ().size ())
	{
	  std::cout << "  Trigger States:" << std::endl;

	  for (ParamList::const_iterator pi (trigger_state ().begin ());
	       pi != trigger_state ().end (); pi++)
	    {
	      std::cout << "    " << (*pi)->storage()
			<< '.' << (*pi)->name ()
			<< std::endl;
	    }
	}

      if (invariant_signal ().size ())
	{
	  std::cout << "  Invariant Block Signals:" << std::endl;

	  for (ParamList::const_iterator
		 pi (invariant_signal ().begin ());
	       pi != invariant_signal ().end (); pi++)
	    {
	      std::cout << "    " << (*pi)->storage()
			<< '.' << (*pi)->name ()
			<< std::endl;
	    }
	}

      if (const_param ().size ())
	{
	  std::cout << "  Constant Params:" << std::endl;

	  for (ParamList::const_iterator pi (const_param ().begin ());
	       pi != const_param ().end (); pi++)
	    {
	      std::cout << "    " << (*pi)->storage()
			<< '.' << (*pi)->name ()
			<< std::endl;
	    }
	}

      if (param ().size ())
	{
	  std::cout << "  Params:" << std::endl;

	  for (ParamList::const_iterator pi (param ().begin ());
	       pi != param ().end (); pi++)
	    {
	      std::cout << "    " << (*pi)->storage()
			<< '.' << (*pi)->name ()
			<< std::endl;
	    }
	}

      if (var ().size ())
	{
	  std::cout << "  Vars:" << std::endl;

	  for (var_map_const_iterator vi (var ().begin ());
	       vi != var ().end (); vi++)
	    {
	      std::cout << "    " << vi->first
			  << " ["
			  << (((vi->second.mode () & VAR_MODE_BOTH)
			       == VAR_MODE_BOTH) ? "BOTH"
			      : (vi->second.mode () & VAR_MODE_OUT) ? "OUT"
			      : (vi->second.mode () & VAR_MODE_IN) ? "IN"
			      : "AUTO")
			  << "]"
			  << std::endl;
	    }
	}

      if (code ().size ())
	{
	  std::cout << "  Codes:" << std::endl;
	  for (CodeList::const_iterator ci (code ().begin ());
	       ci != code ().end (); ci++)
	    {
	      const code_T* cc = ci->code ();
	      std::cout << "    ---- " << cc->type ()
			  << "[@" << cc->line () << "] ----"
			  << std::endl;
	      std::cout << *cc;
	    }
	}

#if 0
      if (param_code ().size ())
	{
	  std::cout << "  Parameter Codes:" << std::endl;
	  for (CodeList::const_iterator ci (param_code ().begin ());
	       ci != param_code ().end (); ci++)
	    {
	      const code_T* cc = ci->code ();
	      std::cout << "    ---- " << cc->type ()
			  << "[@" << cc->line () << "] ----"
			  << std::endl;
	      std::cout << *cc;
	    }
	}

      if (const_param_code ().size ())
	{
	  std::cout << "  Constant Parameter Codes:" << std::endl;
	  for (CodeList::const_iterator
		 ci (const_param_code ().begin ());
	       ci != const_param_code ().end (); ci++)
	    {
	      const code_T* cc = ci->code ();
	      std::cout << "    ---- " << cc->type ()
			  << "[@" << cc->line () << "] ----"
			  << std::endl;
	      std::cout << *cc;
	    }
	}

      if (invariant_signal_code ().size ())
	{
	  std::cout << "  Invariant Block Signal Codes:" << std::endl;
	  for (CodeList::const_iterator
		 ci (invariant_signal_code ().begin ());
	       ci != invariant_signal_code ().end (); ci++)
	    {
	      const code_T* cc = ci->code ();
	      std::cout << "    ---- " << cc->type ()
			  << "[@" << cc->line () << "] ----"
			  << std::endl;
	      std::cout << *cc;
	    }
	}
#endif
    }

    // CspNode のブロック名を取得
    const std::string&
    CspNode::get_block_name (std::string& tmp) const
    {
      const CspBlockInfo* binfo = head_block ();

      if (binfo)
	return binfo->block ()->name ();

      std::ostringstream out;
      out << 't' << id ();

      tmp = out.str ();
      return tmp;
    }

    //
    // PartialArrayList
    //
    void
    PartialArrayList::compact (void)
    {
      if (size () <= 1)
	return;

      std::sort (begin (), end ());
      size_t c = size ();
      size_t p = 0;
      for (size_t n = 1; n < size (); n++)
	{
	  PartialArray& p_pa = (*this)[p];
	  PartialArray& n_pa = (*this)[n];
	  int p_end = p_pa.start () + p_pa.size ();
	  if (n_pa.start () <= p_end)
	    {
	      // 接続・重複なので前のものにマージ(必要ならサイズ更新)
	      int n_end = n_pa.start () + n_pa.size ();
	      if (p_end < n_end)
		p_pa.size () = n_end - p_pa.start ();
	      n_pa.start () = -1;
	      n_pa.size () = -1;
	      c--;
	    }
	  else
	    p = n;
	}

      if (c < size ())
	{
	  // 有効なデータを移動
	  // なお、(*this)[0]は有効なので(*this)[1]から開始
	  size_t t = 1;
	  for (size_t f = 1; f < size (); f++)
	    {
	      PartialArray& npa = (*this)[f];
	      if (npa.start () != -1)
		{
		  if (t != f)
		    (*this)[t] = npa;
		  t++;
		}
	    }
	  resize (c);
	}
    }

    //
    // PropagateVar
    //

    // 分割されている配列を結合する
    void
    PropagateVar::compact_partial_array (void)
    {
      pv_partial_array.compact ();

      // 分割されている状態なら終了
      if (pv_partial_array.size () != 1)
	return;

      // 配列全体にならなかったら終了
      const PartialArray& pa = pv_partial_array[0];
      if (pa.start () != 0 || (size_t)pa.size () != pv_var->size ())
	return;

      // 配列全体になったので、もう不要
      pv_var->mode () &= ~VAR_MODE_PARTIAL_ARRAY;
      pv_partial_array.clear ();
    }


    PropagateVar&
    PropagateVarMap::add_varent (CspVar* cv)
    {
      const var_T* xv = cv->var ();
      const std::string& xmode = xv->mode ();
      var_mode_T& vmode = cv->mode ();
      if (xmode == "extin")
	{
	  vmode |= VAR_MODE_EXTIN;
	  if (input_multirate)
	    vmode |= VAR_MODE_MULTIRATE;
	}
      else if (xmode == "extout")
	vmode |= VAR_MODE_EXTOUT;

      iterator mi = find (xv->name ());
      if (mi == end ())
	{
	  std::pair< iterator, bool >
	    res = insert (std::make_pair (xv->name (), PropagateVar (cv)));
	  mi = res.first;
	  pvm_order.push_back (&mi->second);
	}

      PropagateVar& pv = mi->second;

      // 分割された配列の開始と終了の情報の取り込み
      const var_T::start_optional& sto = xv->start ();
      if (sto)
	{
	  int st = *sto;
	  const var_T::end_optional& edo = xv->end ();
	  int sz = 1;
	  if (edo)
	    sz = *edo - st + 1;
	  pv.partial_array ().push_back (PartialArray (st, sz));

	  vmode |= VAR_MODE_PARTIAL_ARRAY;
	}

      return pv;
    }

    /// @}
  }
}
