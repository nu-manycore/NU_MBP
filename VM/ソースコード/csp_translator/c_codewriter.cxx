// -*- mode: c++; coding: utf-8-unix; -*-

/// \file c_codewriter.cxx
///
/// \brief Cコードライタモジュールのソース

#include <iostream>
#include <sstream>
#include <stack>
#include <iomanip>

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

#include "csp_translator.h"

using namespace SimulinkModel;
using namespace SimulinkModel::XSD;
using namespace Simulink::XML;

/// \addtogroup C_CODEWRITER
///
/// @{

/// \brief BLXMLの配列の部分要素が誤っている場合の対応
///
/// BLXMLで、Demuxにより入出力変数の配列の一部を取り出すが、その結果に
/// 誤りがあるものでは部分配列も全配列として扱うようにするための設定
#define BUG_BLXML_PARTIAL_ARRAY true

/// @}

namespace Simulink
{
  namespace CspTrans
  {

    /// \addtogroup C_CODEWRITER
    ///
    /// @{

    //
    // Writers
    //

    C_CodeWriter c_codewriter;

    // BLXMLの部分配列の情報が誤っているかどうか
    bool bug_partial_array = BUG_BLXML_PARTIAL_ARRAY;

    // indent
    const char* indent_writer::iw_cindent[] = {
      "",
      "  ",
      "    ",
      "      ",
      "\t",
    };

    // Export Global Variable Storage (empty)
    const std::string AutoStorage::export_global_storage ("");

    // null prefix
    const std::string AutoStorage::as_null_prefix ("");

    // 変数または構造体の空のプレフィクス/ポストフィクス
    const std::string var_basic_writer::vbw_nullfix ("");

    // indent writer
    std::ostream&
    indent_writer::writer(std::ostream& os, size_t ind)
    {
      const size_t s = sizeof iw_cindent / sizeof iw_cindent[0];
      if (s)
	{
	  const size_t m = s - 1;
	  while (ind)
	    {
	      size_t c = ind;
	      if (ind >= m)
		c = m;
	      os << iw_cindent[c];
	      ind -= c;
	    }
	}
      return os;
    }

    const indent_writer indent_writer::iw1 (1);
    const indent_writer indent_writer::iw2 (2);
    const indent_writer indent_writer::iw3 (3);
    const indent_writer indent_writer::iw4 (4);

    std::ostream&
    operator<< (std::ostream& s, const indent_writer& iw)
    {
      return iw.writer (s);
    }

    std::ostream&
    operator<< (std::ostream& s, const indent_writer* iw)
    {
      return iw->writer (s);
    }

    // optional_refstring
    std::ostream&
    operator<< (std::ostream& s, const optional_refstring& x)
    {
      if (x)
	return s << *x;
      return s;
    }

    // 変数の定義出力オペレータ
    std::ostream&
    operator<< (std::ostream& s, const var_defn_writer& vdw)
    {
      s << vdw.type () << ' '
	<< vdw.prefix () << vdw.name() << vdw.postfix ();
      size_t a = vdw.size();
      if (a)
	s << '[' << a << ']';
      return s;
    }

    // 変数のRHS出力オペレータ(配列インデクスなし。つまりそのまま)
    std::ostream&
    operator<< (std::ostream& s, const var_rhs_writer& vrw)
    {
      const optional_refstring& storage = vrw.storage ();
      s << vrw.prefix ();
      if (storage)
	s << *storage << vrw.postfix () << '.' << vrw.name();
      else
	s << vrw.name () << vrw.postfix ();
      return s;
    }

    // AutoStorage出力用ラッパクラスのコンストラクタ
    AutoStorage_wrapper::AutoStorage_wrapper(const AutoStorage* a,
					     const strict_string& prefix,
					     const strict_string& data_attr,
					     size_t n,
					     var_mode_T block_filter,
					     var_mode_T pass_filter)
      : asw_as (a), asw_prefix (prefix), asw_storage (a->storage ()),
	asw_data_attr (data_attr), asw_indent (n),
	asw_block_filter (block_filter), asw_pass_filter (pass_filter)
    {
    }

    AutoStorage_wrapper::AutoStorage_wrapper(const AutoStorage* a,
					     const strict_string& prefix,
					     const strict_string& storage,
					     const strict_string& data_attr,
					     size_t n,
					     var_mode_T block_filter,
					     var_mode_T pass_filter)
      : asw_as (a), asw_prefix (prefix), asw_storage (storage),
	asw_data_attr (data_attr), asw_indent (n),
	asw_block_filter (block_filter), asw_pass_filter (pass_filter)
    {
    }

    // AutoStorage用出力オペレータ
    std::ostream&
    operator<< (std::ostream& s, const AutoStorage_wrapper& asw)
    {
      size_t ind = asw.indent();
      size_t ind2 = ind;
      var_mode_T block_filter = asw.block_filter ();
      var_mode_T pass_filter =  asw.pass_filter ();

      indent_writer iw(ind);

      bool has_storage = asw.storage ().length () > 0;
      bool has_prefix = asw.prefix ().length () > 0;
      bool has_data_attr = asw.data_attr ().length () > 0;

      bool out = false;

      if (has_storage)
	ind2++;

      const indent_writer iw2 (ind2);

      for (AutoStorage::const_iterator i (asw->begin ());
	   i != asw->end (); i++)
	{
	  const var_writer* v = *i;
	  const CspVar* cv = static_cast< const CspVar* > (v);

	  if (pass_filter != 0 && (cv->mode () & pass_filter) != 0)
	    ;			// PASS
	  else if (block_filter != 0 && (cv->mode () & block_filter) != 0)
	    continue;		// BLOCK

	  if (has_storage && !out)
	    {
	      s << iw;
	      if (has_prefix)
		s << asw.prefix ();
	      if (asw->is_const ())
		s << "const ";
	      s << "struct {\n";
	      out = true;
	    }

	  s << iw2;
	  if (!has_storage && has_prefix)
	    s << asw.prefix ();
	  s << v->defn_writer () << ";\n";
	}

      if (!has_storage || !out)
	return s;

      s << iw << "} " << asw.storage ();

      if (has_data_attr)
	s << ' ' << asw.data_attr ();

      if (asw->has_initcode ())
	{
	  s << " = {\n";

	  for (AutoStorage::const_iterator i (asw->begin ());
	       i != asw->end (); i++)
	    {
	      const var_writer* v = *i;
	      const optional_refstring& io = v->initcode ();
	      if (io)
		s << *io << '\n';
	    }

	  s << iw << '}';
	}

      s << ";\n";

      return s;
    }

    // 関数用出力オペレータ(そのまま)
    std::ostream&
    operator<< (std::ostream& s, const function_basic_writer& fbw)
    {
      const SimulinkModel::XSD::function_T::code_optional& c = fbw.code ();
      if (c)
	s << *c;
      return s;
    }

    // rate_T の出力オペレータ
    std::ostream&
    operator<< (std::ostream& s, const rate_T& x)
    {
      Rate_offset_T o = x.offset ();
      if (o)
	s << '[' << x.rate () << ',' << o << ']';
      else
	s << x.rate ();
      return s;
    }

    // text_list_T の出力オペレータ
    std::ostream&
    operator<< (std::ostream& s, const text_list_T& tl)
    {
      for (text_list_T::const_iterator i (tl.begin ()); i != tl.end (); i++)
	s << *i;
      return s;
    }

    const char*
    skip_space_comment (const char* s)
    {
      bool com = false;

      for (; *s; s++)
	{
	  if (!com && s[0] == '/' && s[1] == '*')
	    {
	      com = true;
	      s++;
	      continue;
	    }
	  else if (com)
	    {
	      if (s[0] == '*' && s[1] == '/')
		{
		  com = false;
		  s++;
		}
	      continue;
	    }

	  if (!isspace (*(const unsigned char*)s))
	    break;
	}
      return s;
    }

    /// \brief 指定した単語にマッチする regex オブジェクトの生成
    ///
    /// \param[out] regpat 設定された regex オブジェクト
    /// \param[in] word 単語
    bool
    make_word_regex_pattern (std::tr1::regex& regpat,
			     const std::string& word)
    {
      try {
	regpat = std::string ("\\b" + word + "\\b");
	return true;
      }
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return false;
    }

    /// \brief 文字列の指定した範囲に指定した regex がマッチするかどうか
    ///
    /// \param[in] regpat 設定済み regex
    /// \param[in] beg 文字列の開始位置
    /// \param[in] end 文字列の終了位置
    ///
    /// \retval true regex がマッチ
    /// \retval false マッチせず
    static bool
    has_object (const std::tr1::regex& regpat,
		const char* beg, const char* end)
    {
      try
	{
	  std::tr1::cmatch mr;
	  return std::tr1::regex_search (beg, end, mr, regpat);
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}
      return false;
    }

    /// \brief 文字列の指定した範囲に指定したオブジェクト名があるかかどうか
    ///
    /// \param[in] obj 探すオブジェクト名
    /// \param[in] beg 文字列の開始位置
    /// \param[in] end 文字列の終了位置
    ///
    /// \retval true オブジェクト名がある
    /// \retval false オブジェクト名がない
    static bool
    has_object (const std::string& obj, const char* beg, const char* end)
    {
      std::tr1::regex regpat;
      return (make_word_regex_pattern (regpat, obj)
	      && has_object (regpat, beg, end));
    }

    /// \brief 文字列中に指定した regex がマッチするかどうか
    ///
    /// \param[in] regpat 設定済み regex
    /// \param[in] code 文字列
    ///
    /// \retval true regex がマッチ
    /// \retval false マッチせず
    static bool
    has_object (const std::tr1::regex& regpat, const std::string& code)
    {
      const char* beg = skip_space_comment (code.c_str ());
      const char* end = beg + strlen (beg);

      return has_object (regpat, beg, end);
    }

    var_param_ptr_ent&
    var_param_map_T::add_map (const std::string& name,
			      const var_param_ptr_ent& ent)
    {
      std::pair< var_param_map_T::iterator, bool >
	res = insert (std::make_pair (name, ent));
      return res.first->second;
    }

    // <function>をマップに登録し、decl, defnのリストにも登録する
    void
    function_map_T::add_function_map (const std::string& name,
				      const function_T& f)
    {
      iterator mi = find (name);
      if (mi == end ())
	{
	  // 連想配列(map)にないので新規登録
	  const function_T* decl = NULL;
	  const function_T* defn = NULL;
	  if (f.type () == "decl")
	    decl = &f;
	  else
	    defn = &f;

	  int xid = size ();
	  std::pair< iterator, bool >
	    res = insert (std::make_pair (name,
					  function_ent_T (defn, decl)));

	  function_ent_T& xfe = res.first->second;
	  xfe.entry_id () = xid;
	  if (defn)
	    fm_defn_list.push_back (&xfe);
	  if (decl)
	    fm_decl_list.push_back (&xfe);
	}
      else
	{
	  // 連想配列(map)のエントリを更新する
	  function_ent_T& fe = mi->second;
	  if (f.type () == "decl")
	    {
	      fe.set_decl (&f);
	      function_ptr_list_T::const_iterator
		i = std::find (fm_decl_list.begin (),
			       fm_decl_list.end (), &fe);
	      if (i == fm_decl_list.end ())
		fm_decl_list.push_back (&fe);
	    }
	  else
	    {
	      fe.set_defn (&f);
	      function_ptr_list_T::const_iterator
		i = std::find (fm_defn_list.begin (),
			       fm_defn_list.end (), &fe);
	      if (i == fm_defn_list.end ())
		fm_defn_list.push_back (&fe);
	    }
	}
    }

    // 呼び出す関数のリストを設定する
    void
    function_map_T::correct (void)
    {
      for (iterator i (begin ()); i != end (); i++)
	{
	  function_ent_T& fe = i->second;
	  const function_T* f = fe.get_defn ();

	  // Declarationは処理しない
	  if (f == NULL)
	    continue;

	  // 呼び出す関数について処理
	  for (function_T::function_const_iterator
		 lfi (f->function ().begin ());
	       lfi != f->function ().end (); lfi++)
	    {
	      const function_T& lf = *lfi;
	      iterator j = find (lf.name ());

	      // 定義も宣言もない関数は無視。ただし本来これはassertでも
	      // よいはずである。
	      if (j == end ())
		continue;

	      function_ent_T& lfe = j->second;

	      // 定義されている呼び出し関数
	      if (lfe.get_defn ())
		fe.call_functions ().push_back (&lfe);
	    }
	}
    }

    //
    // task_attr_T
    //

    void
    task_attr_T::check_function_obj (const CspNode* node,
				     const std::string& fcode,
				     const std::tr1::regex& regpat,
				     obj_type type)
    {
      if (has_object (regpat, fcode))
	{
	  switch (type)
	    {
	    case INVSIG:
	      out_invsig (true);
	      break;
	    case PARAM:
	      out_param (true);
	      break;
	    case C_PARAM:
	      out_cparam (true);
	      break;
	    case STATE:
	      out_state (true);
	      break;
	    case TRIGGER:
	      out_trigger_state (true);
	      break;
	    case BIOSIG:
	      out_biosig (true);
	      break;
	    case EXTIN:
	      out_extin (true);
	      break;
	    case EXTOUT:
	      out_extout (true);
	      break;
	    }
	}
    }

    void
    task_attr_T::check_function_obj (const CspNode* node,
				     const std::tr1::regex& regpat,
				     obj_type type)
    {
      for (function_ptr_list_T::const_iterator
	     fi (node->functions().begin ());
	   fi != node->functions().end (); fi++)
	{
	  const function_ent_T* func = *fi;
	  const function_defn_writer* fdefn = func;
	  if (*fdefn)
	    {
	      const function_T::code_optional& fcode = fdefn->code ();
	      if (fcode)
		check_function_obj (node, *fcode, regpat, type);
	    }
	}
    }

    void
    task_attr_T::check_function_obj (const CspNode* node,
				     const std::string& object,
				     obj_type type)
    {
      std::tr1::regex regpat;
      if (make_word_regex_pattern (regpat, object))
	check_function_obj (node, regpat, type);
    }

    void
    task_attr_T::check_function_obj (const CspNode* node,
				     const ParamList& params,
				     obj_type type)
    {
      ParamList::const_iterator pi (params.begin ());
      if (pi != params.end ())
	check_function_obj (node, pi->storage (), type);
    }

    void
    task_attr_T::check_task (const CspNode* node)
    {
      const CspNode* ev_node = node->event_subsystem ();

      if (node->info () & CSP_NODE_INFO_NEED_RTM)
	need_rtm (true);

      if (node->state ().size ()
	  || (ev_node != NULL && ev_node->state ().size ()))
	{
	  has_state (true);
	  if (no_gcc && out_state () == false)
	    {
	      if (node->state ().size () == 0)
		check_function_obj (node, ev_node->state (), STATE);
	      else
		check_function_obj (node, node->state (), STATE);
	    }
	}

      if (node->trigger_state ().size ())
	{
	  has_trigger_state (true);
	  check_function_obj (node, node->trigger_state (), TRIGGER);
	}

      if (node->invariant_signal ().size ())
	{
	  has_invsig (true);
	  check_function_obj (node, node->invariant_signal (), INVSIG);
	}

      if (node->param ().size ())
	{
	  has_param (true);
	  check_function_obj (node, node->param (), PARAM);
	}

      if (node->const_param ().size ())
	{
	  has_cparam (true);
	  check_function_obj (node, node->const_param (), C_PARAM);
	}

      if (node->signal ().size ())
	{
	  has_biosig (true);
	  check_function_obj (node, node->signal (), BIOSIG);
	}

      if (node->extin ().size ())
	{
	  has_extin (true);
	  const PropagateVar* eiv = node->extin ()[0];
	  const CspVar* cv = *eiv;
	  const optional_refstring& storage = cv->storage ();
	  if (storage)
	    check_function_obj (node, *storage, EXTIN);
	}

      if (node->extout ().size ())
	{
	  has_extout (true);
	  const PropagateVar* eov = node->extout ()[0];
	  const CspVar* cv = *eov;
	  const optional_refstring& storage = cv->storage ();
	  if (storage)
	    check_function_obj (node, *storage, EXTOUT);
	}

      if (node->input_var ().size ())
	has_invar (true);
      if (node->output_var ().size ())
	has_outvar (true);
      if (node->auto_var ().size ())
	has_autovar (true);

      if (node->functions ().size ())
	has_functions (true);

      if (node->info () & CSP_NODE_INFO_HAS_DSMEM)
	has_datastore (true);
      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
	has_variable_rate (true);

      if (no_gcc)
	{
	  for (function_ptr_list_T::const_iterator
		 fi (node->functions ().begin ());
	       fi != node->functions ().end (); fi++)
	    {
	      const function_defn_writer* fdefn = *fi;
	      if (!*fdefn)
		continue;

	      const function_T::code_optional& fco = fdefn->code ();
	      if (!fco)
		continue;

	      const char* beg = skip_space_comment (fco->c_str ());
	      const char* end = beg + strlen (beg);

	      if (out_rtm () == false && need_rtm ())
		{
		  for (c_object_list_T::const_iterator
			 coi (c_codewriter.c_object ().begin ());
		       coi != c_codewriter.c_object ().end (); coi++)
		    {
		      const c_object_ent_T& coe = *coi;
		      if (coe.attr () & (C_OBJECT_EC_ATTR_RTM_OBJECT
					 |C_OBJECT_EC_ATTR_RTM_POINTER))
			{
			  const object_T* xo = coe;
			  if (has_object (xo->name (), beg, end))
			    {
			      out_rtm (true);
			      break;
			    }
			}
		    }
		}

	      for (PropagateVarMap::const_iterator
		     pi (node->input_var ().begin ());
		   pi != node->input_var ().end (); pi++)
		{
		  const PropagateVar& pv = pi->second;
		  const CspVar* cv = pv;
		  if (has_object (pi->first, beg, end))
		    out_signal ().add_map (pi->first, var_param_ptr_ent (cv));
		  out_invar (true);
		}

	      for (PropagateVarMap::const_iterator
		     pi (node->output_var ().begin ());
		   pi != node->output_var ().end (); pi++)
		{
		  const PropagateVar& pv = pi->second;
		  const CspVar* cv = pv;
		  if (has_object (pi->first, beg, end))
		    out_signal ().add_map (pi->first, var_param_ptr_ent (cv));
		  out_outvar (true);
		}

	      for (PropagateVarMap::const_iterator
		     pi (node->auto_var ().begin ());
		   pi != node->auto_var ().end (); pi++)
		{
		  const PropagateVar& pv = pi->second;
		  const CspVar* cv = pv;
		  if (has_object (pi->first, beg, end))
		    out_signal ().add_map (pi->first, var_param_ptr_ent (cv));
		  out_autovar (true);
		}
	    }
	}

      if (node->info () & CSP_NODE_INFO_MULTIRATE_HEAD)
	multirate_head (true);
    }

    void
    rate_list_T::add_rate (const rate_T& rate)
    {
      const rate_info_T ri (rate);
      rate_list_T::iterator i = std::find (begin (), end (), ri);
      if (i == end ())
	push_back (ri);
      else
	i->count ()++;
    }

    // TaskSet
    int
    TaskSet::core (void) const
    {
      const CspNodeList& nodelist = cspnodelist ();
      if (nodelist.size () == 0)
	return -1;
      return nodelist[0]->core ();
    }

    //  コアのタイプの取得
    CoreType
    TaskSet::coretype (void) const
    {
      const CspNodeList& nodelist = cspnodelist ();
      if (nodelist.size () == 0)
	return SOFTWARE_CORE;
      return nodelist[0]->coretype ();
    }

    void
    TaskSet::add_node (CspNode* n)
    {
      cspnodelist ().push_back (n);
      n->taskset () = this;
    }

    void
    TaskSet::setup_taskset (void)
    {
      AutoStorageMap& asmap = autostorage ();
      const CspNodeList& thislist = *this;
      for (CspNodeList::const_iterator i (thislist.begin ());
	   i != thislist.end (); i++)
	{
	  CspNode* cn = *i;
	  check_task (cn);

	  for (ParamList::iterator i (cn->signal ().begin ());
	       i != cn->signal ().end (); i++)
	    {
	      param_ent_T& pe = *i;
	      asmap.add_storage (pe->storage ()).add_member (&pe);
	    }

	  for (ParamList::iterator i (cn->state ().begin ());
	       i != cn->state ().end (); i++)
	    {
	      param_ent_T& pe = *i;
	      asmap.add_storage (pe->storage ()).add_member (&pe);
	    }

	  for (ParamList::iterator i (cn->trigger_state ().begin ());
	       i != cn->trigger_state ().end (); i++)
	    {
	      param_ent_T& pe = *i;
	      asmap.add_storage (pe->storage ()).add_member (&pe);
	    }

	  for (ParamList::iterator i (cn->invariant_signal ().begin ());
	       i != cn->invariant_signal ().end (); i++)
	    {
	      param_ent_T& pe = *i;
	      asmap.add_storage (pe->storage (), true).add_member (&pe);
	    }
	  for (ParamList::iterator i (cn->param ().begin ());
	       i != cn->param ().end (); i++)
	    {
	      param_ent_T& pe = *i;
	      asmap.add_storage (pe->storage ()).add_member (&pe);
	    }
	  for (ParamList::iterator i (cn->const_param ().begin ());
	       i != cn->const_param ().end (); i++)
	    {
	      param_ent_T& pe = *i;
	      asmap.add_storage (pe->storage (), true).add_member (&pe);
	    }

	  for (PropagateVarList::const_iterator
		 i (cn->auto_var ().order ().begin ());
	       i != cn->auto_var ().order ().end (); i++)
	    {
	      PropagateVar* pv = *i;
	      CspVar* cv = *pv;
	      const optional_refstring& storage = cv->storage ();
	      if (storage)
		asmap.add_storage (*storage).add_member (cv);
	      else
		asmap
		  .add_storage (AutoStorage::export_global_storage)
		  .add_member (cv);
	    }

	  for (PropagateVarList::const_iterator
		 i (cn->input_var ().order ().begin ());
	       i != cn->input_var ().order ().end (); i++)
	    {
	      PropagateVar* pv = *i;
	      CspVar* cv = *pv;
	      const optional_refstring& storage = cv->storage ();
	      if (storage)
		asmap.add_storage (*storage).add_member (cv);
	      else
		asmap
		  .add_storage (AutoStorage::export_global_storage)
		  .add_member (cv);
	    }

	  for (PropagateVarList::const_iterator
		 i (cn->output_var ().order ().begin ());
	       i != cn->output_var ().order ().end (); i++)
	    {
	      PropagateVar* pv = *i;
	      CspVar* cv = *pv;
	      const optional_refstring& storage = cv->storage ();
	      if (storage)
		asmap.add_storage (*storage).add_member (cv);
	      else
		asmap
		  .add_storage (AutoStorage::export_global_storage)
		  .add_member (cv);
	    }

	  // Triggered/Enabled SubSystemの状態変数
	  CspNode* cn_ev = cn->event_subsystem ();
	  if (cn_ev)
	    {
	      for (ParamList::iterator i (cn_ev->state ().begin ());
		   i != cn_ev->state ().end (); i++)
		{
		  param_ent_T& pe = *i;
		  asmap.add_storage (pe->storage ()).add_member (&pe);
		}
	    }

	  // DataStoreMemory
	  for (dsm_info_list_T::const_iterator i (cn->dsm ().begin ());
	       i != cn->dsm ().end (); i++)
	    {
	      dsm_map_T::iterator di = csp_reader.dsm_map ().find (*i);
	      if (di != csp_reader.dsm_map ().end ())
		{
		  dsm_entry& de = di->second;

		  dsm_init () = de.init ();

		  for (ParamList::iterator
			 si (de.dsm_state ().begin ());
		       si != de.dsm_state ().end (); si++)
		    {
		      param_ent_T& se = *si;
		      dsm_storage ()
			.add_storage (se->storage ()).add_member (&se);
		    }

		  for (ParamList::iterator
			 pi (de.dsm_params ().begin ());
		       pi != de.dsm_params ().end (); pi++)
		    {
		      param_ent_T& pe = *pi;
		      dsm_storage ()
			.add_storage (pe->storage ()).add_member (&pe);
		    }
		}
	    }
	}
    }

    rate_T&
    TaskSet::select_rate (void)
    {
      if (tf_rate_list.size () == 0)
	return rate ();

      tf_rate_list.sort_gt ();

      size_t n = 1;
      size_t last_rate = 0;
      Rate_rate_T gcd_rate = tf_rate_list[0].rate ();
      Rate_rate_T gcd_offset = tf_rate_list[0].offset ();
      tf_ratebyid.push_front (tf_rate_list[0]);
      for (; n < tf_rate_list.size (); n++)
	{
	  if (tf_rate_list[last_rate] == tf_rate_list[n])
	    continue;
	  tf_ratebyid.push_front (tf_rate_list[n]);
	  if (gcd_rate > 1)
	    {
	      // rateのGCDの算出
	      Rate_rate_T task_rate = tf_rate_list[n].rate ();
	      gcd_rate = gcd (gcd_rate, task_rate);

	      // offsetが0でない場合、offsetのGCDを算出
	      Rate_rate_T task_offset = tf_rate_list[n].offset ();
	      if (task_offset)
		{
		  // 時刻0からのずれ(周期に対する端数)が対象
		  task_offset %= task_rate;
		  if (task_offset)
		    {
		      if (gcd_offset)
			gcd_offset = gcd (gcd_offset, task_offset);
		      else
			gcd_offset = task_offset;
		    }
		}
	    }
	  last_rate = n;
	}

      // 最終的にoffsetとrateのGCDがタスクの最小周期になる
      if (gcd_rate > 1 && gcd_offset > 1)
	gcd_rate = gcd (gcd_rate, gcd_offset);

      rate ().rate () = gcd_rate;
      return rate ();
    }

    unsigned int
    TaskSet::gcd (unsigned int x, unsigned int y)
    {
      unsigned int t;
      while (y != 0)
	{
	  t = x % y;
	  x = y;
	  y = t;
	}
      return x;
    }

    /// @}

    /// \addtogroup C_CODEWRITER_SUB
    ///
    /// @{

    // DataStoreMemory用 の regex
    std::tr1::regex datastore_memory_regex;


    // PulseGenerator用の regex
    std::tr1::regex pulse_generator_code_regex;


    // 登録されている文字列全てにundefineを出力する
    void
    def_list_T::undefine (std::ostream& os) const
    {
      for (const_reverse_iterator i (rbegin ()); i != rend (); i++)
	os << "#undef " << *i << '\n';
    }


    // タスクIDをコード生成時に使用するフォーマットにする
    std::string
    mk_task_id (long id)
    {
      std::ostringstream oss;
      oss << std::setfill ('0') << std::setw (4) << id;
      return oss.str ();
    }

    // PulseGeneratorを書き換えるための正規表現を構築しregexを生成する
    bool
    prepare_pulse_generator_regex (void)
    {
      const optional_refstring& state_storage = c_codewriter.state_storage ();
      const optional_refstring& param_storage = c_codewriter.param_storage ();

      if (!state_storage || !param_storage)
	return true;

      const std::string pat ("[[:space:]]*"
			     "="
			     "[[:space:]]*"
			     "\\("
			     "[[:space:]]*"
			     + *state_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #1 - counter
			     "[[:space:]]*"
			     "(<)" // #2 -- cmp
			     "[[:space:]]*"
			     + *param_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #3 - duty
			     "[[:space:]]*"
			     "\\)"
			     "[[:space:]]*"
			     "&&"
			     "[[:space:]]*"
			     "\\("
			     + *state_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #4 - counter
			     "[[:space:]]*"
			     ">="
			     "[[:space:]]*"
			     "0"
			     "[[:space:]]*"
			     "\\)"
			     "[[:space:]]*"
			     "\\?"
			     "[[:space:]]*"
			     + *param_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #5 - amp
			     "[[:space:]]*"
			     ":"
			     "[[:space:]]*"
			     "0\\.0"
			     "[[:space:]]*"
			     ";"
			     "[[:space:]]*"

			     // if

			     "(" // #6 - if begin
			     "^"
			     "[[:blank:]]*"
			     "if"
			     "[[:space:]]*"
			     "\\("
			     "[[:space:]]*"
			     + *state_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #7 - counter
			     "[[:space:]]*"
			     ">="
			     "[[:space:]]*"
			     + *param_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #8 - period
			     "[[:space:]]*"
			     "-"
			     "[[:space:]]*"
			     "1\\.0"
			     "[[:space:]]*"
			     "\\)"
			     "[[:space:]]*"
			     "\\{"
			     "[[:space:]]*"
			     + *state_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #9 - counter
			     "[[:space:]]*"
			     "="
			     "[[:space:]]*"
			     "0"
			     "[[:space:]]*"
			     ";"
			     "[[:space:]]*"
			     "\\}"
			     "[[:space:]]*"
			     "else"
			     "[[:space:]]*"
			     "\\{"
			     "[[:space:]]*"
			     + *state_storage
			     + "[[:space:]]*"
			     "\\."
			     "[[:space:]]*"
			     "([_[:alpha:]][_[:alnum:]]*)" // #10 - counter
			     "[[:space:]]*"
			     "\\+\\+"
			     "[[:space:]]*"
			     ";"
			     "[[:space:]]*"
			     "\\}"
			     "[[:blank:]]*"
			     "$"
			     ")"
			     );
      try {
	pulse_generator_code_regex = pat;
	return true;
      }

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return false;
    }


    // PulseGeneratorのコードを調べ書き換えるべき文字列の範囲返す
    bool
    check_pulse_generator_code (const char* code,
				int& cmp_beg, int& cmp_end,
				int& duty_beg, int& duty_end,
				int& if_beg, int& if_end,
				std::string& amp,
				std::string& period,
				std::string& duty,
				std::string& counter)
    {
      std::tr1::cmatch mr;

      try
	{
	  if (!pulse_generator_code_regex.empty ()
	      && regex_search (code, mr, pulse_generator_code_regex))
	    {
	      const std::string c1 = mr[1];
	      const std::string c2 = mr[4];
	      const std::string c3 = mr[7];
	      const std::string c4 = mr[9];
	      const std::string c5 = mr[10];

	      if (c1 == c2 && c1 == c3 && c1 == c4 && c1 == c5)
		{
		  cmp_beg = mr[2].first - code;
		  cmp_end = mr[2].second - code;

		  duty_beg = mr[3].first - code;
		  duty_end = mr[3].second - code;
		  duty = mr[3];

		  if_beg = mr[6].first - code;
		  const char* p = mr[6].second;
		  while (*p && isspace ((unsigned char)*p))
		    {
		      if (*p++ == '\n')
			break;
		    }
		  if_end = p - code;

		  counter = c1;
		  amp = mr[5];
		  period = mr[8];
		  return true;
		}
	    }
	}
      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return false;
    }


    // コード中のDataStoreのアクセスを見つけるため、構造体名とメンバ名
    // の正規表現から regex を作成する
    bool
    make_dsmem_regpat (void)
    {
      const AutoStorage*
	dsm_as = c_codewriter.dsmem ()
	.get_storage (c_codewriter.state_storage ());

      if (dsm_as == NULL)
	return true;

      std::string pat ("\\b(" + dsm_as->storage () + ")\\.(");

      size_t n = 0;
      for (AutoStorage::const_iterator i (dsm_as->begin ());
	   i != dsm_as->end (); i++)
	{
	  const var_writer* vw = *i;
	  if (n++)
	    pat += '|';
	  pat += vw->name ();
	}
      pat += ')';

      try
	{
	  datastore_memory_regex = pat;
	  return true;
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return false;
    }


    // コードにから DataStore の書き換え範囲を探索する
    bool
    check_datastore_code (const char* code, range_T& list)
    {
      try
	{
	  std::tr1::cmatch mr;
	  const char* p = code;

	  while (regex_search (p, mr, datastore_memory_regex))
	    {
	      list.push_back (std::make_pair (mr[1].first - code,
					      mr[1].second - code));
	      p = mr[0].second;
	    }

	  return list.size () != 0;
	}

      catch (const std::tr1::regex_error& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (const std::exception& e)
	{
	  std::cerr << "Error in " << __FUNCTION__ << ":"
		    << e.what() << std::endl;
	}
      catch (...)
	{
	  std::cerr << "Error in " << __FUNCTION__ << std::endl;
	}

      return false;
    }


    // ユーザーが作成する必要がある外部入出力がある旨のコメントの出力
    void
    generate_extvar_user_codes (std::ostream& os, var_out_type type, int ind)
    {
      const indent_writer iw (ind);

      os << '\n'
	 << iw << "/****************************************************\n"
	 << iw << " *\n"
	 << iw
	 << ((type == VOUT_EXTIN)
	     ? " * You should insert some codes here for external input"
	     : " * You may insert some codes here for external output")
	 << ".\n"
	 << iw << " *\n"
	 << iw << " ****************************************************/"
	 << '\n';
    }

    // 関数定義を出力する
    void
    output_function_defn_code (std::ostream& os,
			       const function_ent_T* fe, mark_T& mark,
			       bool outer)
    {
      if (mark[fe->entry_id ()])
	return;

      const function_defn_writer& fdw = fe->defn_writer ();

      if (enable_multirate && fdw.name () == "rate_scheduler")
	return;

      mark[fe->entry_id ()] = true;

      if (!outer)
	os << '\n'
	   << function_discard_static_wrapper< function_defn_writer > (fdw);
      else
	os << '\n'
	   << fdw;
    }


    // 関数の宣言を出力する
    void
    output_function_decl_code (std::ostream& os,
			       const function_ent_T* fe, mark_T& mark,
			       bool outer)
    {
      if (mark[fe->entry_id ()])
	return;

      const function_decl_writer& fdw = fe->decl_writer ();

      if (enable_multirate && fdw.name () == "rate_scheduler")
	return;

      mark[fe->entry_id ()] = true;

      if (!outer)
	os << '\n'
	   << function_discard_static_wrapper< function_decl_writer > (fdw);
      else
	os << '\n'
	   << fdw;
    }

    // 関数や変数に対するプリプロセッサの置換用マクロを出力する
    void
    define_macro (std::ostream& os,
		  const std::string& name, const std::string& attr,
		  const std::string& postfix, def_list_T* define_list)
    {
      os << "#define " << name;
      if (attr.length ())
	os << ' ' << attr;
      os << ' ' << name << "__" << postfix << '\n';
      if (define_list)
	define_list->push_back (name);
    }

    // 関数出力用のサブルーチン
    void
    output_function_sub (std::ostream& os,
			 const function_ptr_list_T& funcs, mark_T& mark,
			 bool defn, int ind, bool& comout,
			 const std::string& code_attr,
			 const std::string& postfix,
			 bool outer, def_list_T* define_list)
    {
      for (function_ptr_list_T::const_iterator i (funcs.begin ());
	   i != funcs.end (); i++)
	{
	  const function_ent_T* fe = *i;
	  const function_basic_writer* fbw;
	  if (defn)
	    fbw = &fe->defn_basic_writer ();
	  else
	    fbw = &fe->decl_basic_writer ();

	  const std::string& fname = fbw->name ();

	  if (enable_multirate && fname == "rate_scheduler")
	    continue;

	  if (mark[fe->entry_id ()])
	    continue;

	  output_function_sub (os, fe->call_functions (), mark,
			       defn, ind, comout, code_attr, postfix,
			       outer, define_list);

	  if (comout)
	    {
	      os << '\n' << indent_writer(ind) << "/* functions */\n";
	      comout = false;
	    }

	  if (outer)
	    {
	      os << '\n';
	      define_macro (os, fname, code_attr, postfix, define_list);
	    }

	  if (defn)
	    output_function_defn_code (os, fe, mark, outer);
	  else
	    output_function_decl_code (os, fe, mark, outer);

	  if (outer)
	    {
	      os << "\n#undef " << fname << '\n';
	      std::string empty;
	      define_macro (os, fname, empty, postfix);
	    }
	}
    }

    // タスクで使用する関数を出力する
    void
    generate_task_function (std::ostream& os, CspNode *node,
			    mark_T& mark, bool defn, int ind, bool& comout,
			    const std::string& code_attr,
			    const std::string& postfix,
			    bool outer, def_list_T* define_list)
    {
      reset_mark (mark, c_codewriter.functions ().size ());
      output_function_sub (os, node->functions (), mark, defn, ind, comout,
			   code_attr, postfix, outer, define_list);
    }

    // Real-Time Model Objectの初期化コードをタスクのコードに出力
    void
    generate_task_rtw_initializer (std::ostream& os, int ind)
    {
      if (c_codewriter.rt_init ().size () == 0)
	return;

      /* initialize Real-Time Model */
      os << '\n'
	 << indent_writer (ind)
	 << "/* initialize Real-Time Model Object */\n"
	 << c_codewriter.rt_init ();
    }


    // Real-Time Model Objectの定義をタスクのコードに出力
    void
    generate_task_rtw_context (std::ostream& os, int ind,
			       const std::string& stdef,
			       const std::string& data_attr,
			       const std::string& rodata_attr,
			       const std::string& postfix,
			       def_list_T* define_list)
    {
      const c_object_ent_T* rtm_object = NULL;
      const c_object_ent_T* rtm_pointer = NULL;

      for (c_object_list_T::const_iterator
	     i (c_codewriter.c_object ().begin ());
	   i != c_codewriter.c_object ().end (); i++)
	{
	  const c_object_ent_T& co = *i;
	  if (co.attr () & C_OBJECT_EC_ATTR_RTM_OBJECT)
	    rtm_object = &co;
	  else if (co.attr () & C_OBJECT_EC_ATTR_RTM_POINTER)
	    rtm_pointer = &co;
	}

      // output Real Time Model Object and Pointer
      if (rtm_object != NULL && rtm_pointer != NULL)
	{
	  const object_T* x_ro = *rtm_object;
	  const object_T* x_rp = *rtm_pointer;
	  os << '\n'
	     << indent_writer (ind) << "/* Real-Time Model Object */\n";

	  if (postfix.length ())
	    {
	      define_macro (os, (*rtm_object)->name (), "", postfix,
			    define_list);
	      define_macro (os, (*rtm_pointer)->name(), "", postfix,
			    define_list);
	      os << '\n';
	    }

	  bool rtm_pointer_is_const = false;
	  if (x_rp)
	    {
	      const std::string& t = x_rp->type ();
	      std::string::size_type p = t.rfind ('*');
	      if (p != std::string::npos)
		{
		  p = t.find ("const", p + 1);
		  if (p != std::string::npos)
		    rtm_pointer_is_const = true;
		}
	    }

	  const std::string&
	    rtm_pointer_attr = rtm_pointer_is_const ? rodata_attr : data_attr;

	  os << indent_writer (ind) << stdef
	     << x_ro->type () << ' ' << x_ro->name ();
	  if (data_attr.length ())
	    os << ' ' << data_attr;
	  os << ";\n"
	     << indent_writer (ind) << stdef
	     << x_rp->type () << ' ' << x_rp->name ();
	  if (rtm_pointer_attr.length ())
	    os << ' ' << rtm_pointer_attr;
	  os << " = " << x_rp->value () << ";\n";
	}
    }


    // イベントが必要な SubSystem の実行状態用変数を出力する
    void
    generate_task_subsystem_variables (std::ostream& os, CspNode* node,
				       int ind, CspNode_map_T* cnmap)
    {
      CspNode* cn_ev = node->event_subsystem ();

      if (cn_ev != NULL
	  && cn_ev->need_subsystem_event ())
	{
	  if (cnmap == NULL || cnmap->check (cn_ev, 1) == 0)
	    {
	      std::string tmp;
	      const std::string& n = cn_ev->get_block_name (tmp);
	      std::string xx_task_id (mk_task_id (cn_ev->id ()));
	      os << '\n'
		 << indent_writer (ind) << "/* Block: " << n << " */\n"
		 << indent_writer (ind) << "int subsystem__"
		 << xx_task_id << "_state;\n";
	    }
	}

      if (node != cn_ev
	  && node->need_subsystem_event ())
	{
	  if (cnmap == NULL || cnmap->check (node, 1) == 0)
	    {
	      std::string tmp;
	      const std::string& n = node->get_block_name (tmp);
	      std::string my_task_id (mk_task_id (node->id ()));
	      os << '\n'
		 << indent_writer (ind) << "/* Block: " << n << " */\n"
		 << indent_writer (ind) << "int subsystem__" << my_task_id
		 << "_state;\n";
	    }
	}
    }


    // タスクの初期化コードを出力する
    int
    generate_task_context_initializer (std::ostream& os, CspNode* node,
				       int ind, CspNode_map_T* cnmap)
    {
      const indent_writer iw (ind);
      std::string xx_task_id;
      CspNode* cn_ev;

      std::string my_task_id (mk_task_id (node->id ()));

      cn_ev = node->event_subsystem ();
      if (cn_ev)
	xx_task_id = mk_task_id (cn_ev->id ());

      /* trigger subsystem initialize */
      if (node->need_subsystem_event ())
	{
	  if (cnmap == NULL || cnmap->check (node, 1) == 0)
	    {
	      os << '\n'
		 << iw
		 << "/* initialize triggered/enabled subsystem state */\n"
		 << iw
		 << "subsystem__" << my_task_id << "_state = 0;\n";
	    }
	}
      else if (cn_ev != NULL
	       && cn_ev->need_subsystem_event ())
	{
	  if (cnmap == NULL || cnmap->check (cn_ev, 1) == 0)
	    {
	      os << '\n'
		 << iw
		 << "/* initialize triggered/enabled subsystem state */\n"
		 << iw
		 << "subsystem__" << xx_task_id << "_state = 0;\n";
	    }
	}

      /* initialize state */
      CodeList::const_iterator
	stcode = node->code ().find_type_code ("state");
      if (stcode != node->code ().end ())
	{
	  os << '\n' << iw << "/* initialize states */\n";
	  for (; stcode != node->code ().end (); stcode++)
	    {
	      const code_ent_T& ce = *stcode;
	      const code_T* c = ce.code ();
	      if (c->type () != "state")
		break;
	      for (code_frag_list_T::const_iterator
		     i (ce.code_frag_list ().begin ());
		   i != ce.code_frag_list ().end (); i++)
		{
		  if (*i)
		    os << **i;
		}
	    }
	}

      /* initialize task context */
      CodeList::const_iterator
	inicode = node->code ().find_type_code ("init");
      if (inicode != node->code ().end ())
	{
	  os << '\n' << iw << "/* initialize task context */\n";
	  for (; inicode != node->code ().end (); inicode++)
	    {
	      const code_ent_T& ce = *inicode;
	      const code_T* c = ce.code ();
	      if (c->type () != "init")
		break;
	      for (code_frag_list_T::const_iterator
		     i (ce.code_frag_list ().begin ());
		   i != ce.code_frag_list ().end (); i++)
		{
		  if (*i)
		    os << **i;
		}
	    }
	}

      return 0;
    }


    // Real-Time Model Objectの更新コードを出力する
    void
    generate_update_rtw (std::ostream& os, int ind)
    {
      const indent_writer iw0 (ind);

      if (c_codewriter.rt_code ().size ())
	{
	  os << '\n'
	     << iw0 << "/* Update Real-Time Model */\n"
	     << iw0 << "{\n";

	  for (text_list_T::const_iterator
		 i (c_codewriter.rt_code ().begin ());
	       i != c_codewriter.rt_code ().end (); i++)
	    {
	      const std::string& s = *i;

	      const char *beg = skip_space_comment (s.c_str ());
	      if (enable_multirate
		  && strncmp (beg, "rate_scheduler", 14) == 0)
		continue;
	      os << s;
	    }

	  os << iw0 << "}\n";
	}
    }

    // Simulinkモデル用のヘッダファイルをmarkの前までinclude文を出力
    void
    generate_includes (std::ostream& os,
		       const c_include_list_T& inclist, int& mark)
    {
      bool first = true;
      for (size_t i = (mark < 0) ? 0 : mark; i < inclist.size (); i++)
	{
	  const include_T* ie = inclist[i];
	  const include_T::mark_optional& mo = ie->mark ();
	  if (mo && *mo)
	    {
	      mark = i;
	      break;
	    }
	  if (first)
	    {
	      first = false;
	      os << '\n';
	    }
	  const c_include_list_T::text_map&
	    tmap = c_codewriter.include_file().get_text_map ();
	  c_include_list_T::text_map::const_iterator
	    ti = tmap.find (ie->file());
	  if (ti == tmap.end ())
	    os << "#include " << ie->file () << "\n";
	  else
	    os << ti->second;
	}
    }

    // 定義されている型の出力
    void
    generate_deftype (std::ostream& os)
    {
      if (c_codewriter.c_deftype ().size ())
	{
	  os << '\n';

	  for (c_object_list_T::const_iterator
		 i (c_codewriter.c_deftype ().begin ());
	       i != c_codewriter.c_deftype ().end (); i++)
	    {
	      const c_object_ent_T& coe = *i;

	      if (coe.attr () & C_OBJECT_EC_ATTR_VALUE_IS_CODE)
		os << *coe.value ();
	      else
		{
		  os << "typedef " << coe->type () << ' ' << coe->name ();
		  const object_T::size_optional& so = coe->size ();
		  if (so)
		    os << '[' << *so << ']';
		  os << ";\n";
		}
	    }
	}
    }


    // 利用者指定のinclude文の出力
    void
    generate_user_includes (std::ostream& os)
    {
      if (c_codewriter.c_include_headers ().size ())
	{
	  os << '\n';
	  for (option_include_list_T::const_iterator
		 i (c_codewriter.c_include_headers ().begin ());
	       i != c_codewriter.c_include_headers ().end (); i++)
	    {
	      const std::string& inc = *i;
	      size_t len = inc.size ();

	      if (len > 0
		  && ((inc[0] == '<' && inc[len - 1] == '>')
		      || (inc[0] == '"' && inc[len - 1] == '"')))
		os << "#include " << inc << "\n";
	      else
		os << "#include \"" << inc << "\"\n";
	    }
	}
    }


    /// \brief タスク関数のタスク実行部のコードを生成する
    void
    generate_task_exec_code (std::ostream& os, CspNode* node,
			     const csp_block_list_T* blocklist,
			     int ind, const std::string& dsm_prefix)
    {
      std::string my_task_id (mk_task_id (node->id ()));

      const indent_writer iw0 (ind);

      if (node->need_subsystem_event ())
	{
	  os << '\n'
	     << iw0
	     << "subsystem__" << my_task_id << "_state = 0;\n";
	}

      std::string xx_task_id;
      CspNode* cn_ev = node->event_subsystem ();
      if (cn_ev)
	xx_task_id = mk_task_id (cn_ev->id ());

      int ind2 = ind + 1;

      const indent_writer iw1 (ind2);

      CodeList::const_iterator ci = node->code ().find_type_code ("task");
      if (ci != node->code ().end ())
	{
	  bool out_code = false;
	  bool event_first = true;
	  for (; ci != node->code ().end (); ci++)
	    {
	      const code_ent_T& code = *ci;
	      if (code->type () != "task")
		break;

	      if (blocklist != NULL
		  && !blocklist->checkinfo (code.blockinfo ()))
		continue;

	      if (out_code == false)
		{
		  os << '\n'
		     << iw0 << "/* C code */\n"
		     << iw0;

		  if (cn_ev != NULL
		      && cn_ev->need_subsystem_event ())
		    os << "if (subsystem__" << xx_task_id << "_state) ";
		  os << "{\n";
		  out_code = true;
		}

	      for (code_frag_list_T::const_iterator
		     i (code.code_frag_list ().begin ());
		   i != code.code_frag_list ().end (); i++)
		{
		  const code_frag_T& cf = *i;
		  if (!cf)
		    {
		      if (event_first && node->need_subsystem_event ())
			{
			  os << iw1 << "/*@ Set event from SubSystem @*/\n";

			  os << iw1 << "subsystem__" << my_task_id
			     << "_state = 1;\n";
			  os << iw1 << "/*@ End of Subsystem event @*/\n";
			  event_first = false;
			}
		    }
		  else if (enable_multirate
			   && (node->info ()
			       & CSP_NODE_INFO_LOWRATE_PULSEGEN))
		    {
		      const char* s = cf->c_str ();
		      int cmp_beg = -1, cmp_end = -1;
		      int duty_beg = -1, duty_end = -1;
		      int if_beg = -1, if_end = -1;
		      std::string amp;
		      std::string period;
		      std::string duty;
		      std::string counter;
		      if (check_pulse_generator_code (s,
						      cmp_beg, cmp_end,
						      duty_beg, duty_end,
						      if_beg, if_end,
						      amp, period, duty,
						      counter))
			{
			  const char* b = s + if_beg;
			  const char* p = b;

			  while (isspace ((unsigned char)*p))
			    p++;

			  std::string pind (p - b, ' ');

			  const std::string
			    pc (*c_codewriter.state_storage ()
				+ "." + counter);
			  const std::string
			    pp (*c_codewriter.param_storage ()
				+ "." + period);

			  // 元のコードはベースレートでカウントアップし、
			  // Period と PulseWidth に従って値を出力している。
			  // このコード書き換えでは、PulseGeneratorのブロッ
			  // クはPeriod と PulseWidth で決定されるレートで
			  // 動作するようにコードを書き換える
			  os << std::string (s, 0, cmp_beg)
			     << "<="
			     << std::string (s, cmp_end, duty_end - cmp_end)
			     << " - 1.0"
			     << std::string (s, duty_end, if_beg - duty_end)
			     << pind << pc << " += "
			     << node->rate ().rate () << ";\n"
			     << pind << "if (" << pc << " > "
			     << pp << " - 1.0) {\n"
			     << pind << "  " << pc << " = 0;\n"
			     << pind << "}\n";

			  os << s + if_end;
			}
		      else
			os << *cf << '\n';
		    }
		  else if (node->info () & CSP_NODE_INFO_HAS_DSMEM)
		    {
		      const char* s = cf->c_str ();
		      bool dsmout = false;

		      if (!datastore_memory_regex.empty ())
			{
			  // insert dsm_prefix before DataStoreMemory
			  range_T vlist;
			  if (check_datastore_code (s, vlist))
			    {
			      const char* p = s;
			      for (range_T::const_iterator
				     di (vlist.begin ());
				   di != vlist.end (); di++)
				{
				  size_t o = s + di->first - p;
				  os << std::string (p, 0, o) << dsm_prefix;
				  p += o;
				}
			      os << p << '\n'; // 残り
			      dsmout = true;
			    }
			}

		      if (!dsmout)
			os << *cf << '\n';
		    }
		  else
		    os << *cf << '\n';
		}
	    }

	  // イベントが必要かつイベントが生成されなかった場合の対応
	  if (event_first && node->need_subsystem_event ())
	    {
	      os << iw1 << "/*@ Set event from SubSystem @*/\n";

	      os << iw1 << "subsystem__" << my_task_id
		 << "_state = 1;\n";
	      os << iw1 << "/*@ End of Subsystem event @*/\n";
	    }

	  if (out_code)
	    os << iw0 << "}\n";
	}

      os << '\n'
	 << "#ifdef TASK_EXECUTE_" << my_task_id << "_COMPLETED\n"
	 << iw0 << "TASK_EXECUTE_" << my_task_id << "_COMPLETED();\n"
	 << "#endif\n";
    }


    // Update部のコードを出力する
    void
    generate_task_update_code (std::ostream& os, CspNode* node,
			       const csp_block_list_T* blocklist,
			       int ind, bool do_rtm_out)
    {
      std::string my_task_id (mk_task_id (node->id ()));

      CspNode* cn_ev = node->event_subsystem ();
      std::string xx_task_id;
      if (cn_ev)
	xx_task_id = mk_task_id (cn_ev->id ());

      const indent_writer iw0 (ind);
      const indent_writer iw1 (ind + 1);

      CodeList::const_iterator ci = node->code ().find_type_code ("update");

      /* update */
      if (ci != node->code ().end ())
	{
	  bool out_code = false;
	  for (; ci != node->code ().end (); ci++)
	    {
	      const code_ent_T& code = *ci;
	      if (code->type () != "update")
		break;

	      if (blocklist != NULL
		  && !blocklist->checkinfo (code.blockinfo ()))
		continue;

	      if (out_code == false)
		{
		  os << '\n'
		     << iw0 << "/* Update */\n";

		  if (cn_ev
		      && cn_ev->need_subsystem_event ())
		    os << iw0 << "if (subsystem__" << xx_task_id << "_state) ";
		  os << iw0 << "{\n";
		  out_code = true;
		}

	      for (code_frag_list_T::const_iterator
		     i (code.code_frag_list ().begin ());
		   i != code.code_frag_list ().end (); i++)
		{
		  const code_frag_T& cf = *i;
		  if (!cf)
		    os << iw1 << "/*(DELIMITER)*/\n"; // XXX - delimiter
		  else
		    os << *cf << '\n';
		}
	    }

	  if (out_code)
	    os << iw0 << "}\n";

	  os << '\n'
	     << "#ifdef TASK_UPDATE_" << my_task_id << "_COMPLETED\n"
	     << iw0 << "TASK_UPDATE_" << my_task_id << "_COMPLETED();\n"
	     << "#endif\n";
	}

      if (do_rtm_out)
	{
	  // task time
	  if (enable_multirate)
	    {
	      if (node->info () & CSP_NODE_INFO_CHANGE_RATE)
		{
		  os << "#ifdef TASK_CHANGE_RATE_" << my_task_id << '\n'
		     << indent_writer::iw2
		     << "TASK_CHANGE_RATE_" << my_task_id
		     << "(task__time, task__rate_t" << my_task_id << ");\n"
		     << "#endif\n"
		     << indent_writer::iw2
		     << "task__time += task__rate_t" << my_task_id << ";\n";
		}
	      else
		os << indent_writer::iw2
		   << "task__time += " << node->rate ().rate () << ";\n";
	    }

	  if ((node->info () & CSP_NODE_INFO_NEED_RTM) != 0)
	    generate_update_rtw (os, ind);
	}
    }


    //  スレッドのタスクの定義・宣言の出力
    void
    generate_task_definitions (std::ostream& os, const TaskSet* taskset,
			       bool outer,
			       const std::string& postfix,
			       const std::string& code_attr,
			       const std::string& data_attr,
			       const std::string& rodata_attr,
			       def_list_T* define_list)
    {
      const task_attr_T& task_attr = *taskset;

      std::string stdef;		// static か空
      int ind = 1;

      if (outer)
	ind = 0;

      const indent_writer iw0 (ind);

      // --staticalize またはタスク関数外なら、関数や構造体に static を
      // 付与する
      if (do_staticalize || outer)
	stdef = "static ";

      // タスク関数外かつReal-Time Model Objectを外に出す、
      // または、タスク関数内でReal-Time Model Objectを外に出さないで、
      // かつReal-Time Model Objectの出力が必要な場合は、Real-Time
      // Model Objectを出力する
      if ((outer && task_attr.out_rtm ())
	  || (!outer && !task_attr.out_rtm () && task_attr.need_rtm ()))
	generate_task_rtw_context (os, ind, stdef, data_attr, rodata_attr,
				   postfix, define_list);

      // タスク関数外かつ状態変数を外に出す、
      // または、タスク関数内で状態変数を外に出さない、かつ状態変数を持
      // つ場合は状態変数を出力する
      if ((outer && task_attr.out_state ())
	  || (!outer && !task_attr.out_state () && task_attr.has_state ()))
	{
	  const optional_refstring&
	    state_storage = c_codewriter.state_storage ();
	  assert (state_storage);
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (state_storage);
	  assert (as);

	  os << '\n'
	     << iw0 << "/* states */\n";

	  // タスク関数外であれば、状態変数の構造体名をマクロで置換する
	  if (outer)
	    define_macro (os, *state_storage, "", postfix, define_list);

	  os << as->set_indent (stdef, ind, data_attr);
	}

      // タスクの関数外かつTrigger Stateを外に出す、
      // または、タスクの関数内でTrigger Stateを外に出さない、かつ
      // Trigger Stateを持つ場合はTrigger Stateを出力する
      if ((outer && task_attr.out_trigger_state ())
	  || (!outer && !task_attr.out_trigger_state ()
	      && task_attr.has_trigger_state ()))
	{
	  const optional_refstring&
	    trigger_state_storage = c_codewriter.trigger_state_storage ();
	  assert (trigger_state_storage);
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (trigger_state_storage);
	  assert (as);

	  os << '\n'
	     << iw0 << "/* trigger states */\n";

	  // タスク関数外であれば、Trigger Stateの構造体名をマクロで置
	  // 換する
	  if (outer)
	    define_macro (os, *trigger_state_storage, "", postfix,
			  define_list);

	  os << as->set_indent (stdef, ind, data_attr);
	}

      // タスクの関数外かつブロックシグナルを外に出す、
      // または、タスクの関数内でブロックシグナルを外に出さない、かつブ
      // ロックシグナルを持つ場合はブロックシグナルを出力する
      if ((outer && task_attr.out_biosig ())
	  || (!outer && !task_attr.out_biosig ()
	      && task_attr.has_biosig ()))
	{
	  const optional_refstring&
	    signal_storage = c_codewriter.signal_storage ();
	  assert (signal_storage);
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (signal_storage);
	  assert (as);

	  os << '\n'
	     << iw0 << "/* Block I/O Signals */\n";

	  // タスク関数外であれば、ブロックシグナルの構造体名をマクロで
	  // 置換する
	  if (outer)
	    define_macro (os, *signal_storage, "", postfix, define_list);

	  os << as->set_indent (stdef, ind, data_attr);
	}

      // タスクの関数外かつ定数パラメタを外に出す、
      // または、タスクの関数内で定数パラメタを外に出さない、かつ定数パ
      // ラメタを持つ場合は定数パラメタを出力する
      if ((outer && task_attr.out_cparam ())
	  || (!outer && !task_attr.out_cparam ()
	      && task_attr.has_cparam ()))
	{
	  const optional_refstring&
	    cparam_storage = c_codewriter.cparam_storage ();
	  assert (cparam_storage);
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (cparam_storage);
	  assert (as);
	  const std::string& attr = as->is_const () ? rodata_attr : data_attr;

	  os << '\n'
	     << iw0 << "/* constant params */\n";

	  // タスク関数外であれば、定数パラメタの構造体名をマクロで置換
	  // する
	  if (outer)
	    define_macro (os, *cparam_storage, "", postfix, define_list);

	  os << as->set_indent (stdef, ind, attr);
	}

      // タスクの関数外かつパラメタを外に出す、
      // または、タスクの関数内でパラメタを外に出さない、かつパラメタを
      // 持つ場合はパラメタを出力する
      if ((outer && task_attr.out_param ())
	  || (!outer && !task_attr.out_param ()
	      && task_attr.has_param ()))
	{
	  const optional_refstring&
	    param_storage = c_codewriter.param_storage ();
	  assert (param_storage);
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (param_storage);
	  assert (as);

	  os << '\n'
	     << iw0 << "/* params */\n";

	  // タスク関数外であれば、パラメタの構造体名をマクロで置換する
	  if (outer)
	    define_macro (os, *param_storage, "", postfix, define_list);

	  os << as->set_indent (stdef, ind, data_attr);
	}

      // タスクの関数外かつinvariant block signalを外に出す、
      // または、タスクの関数内でinvariant block signalを外に出さない、
      // かつinvariant block signalを持つ場合はinvariant block signalを
      // 出力する
      if ((outer && task_attr.out_invsig ())
	  || (!outer && !task_attr.out_invsig ()
	      && task_attr.has_invsig ()))
	{
	  const optional_refstring&
	    invsig_storage = c_codewriter.invsig_storage ();
	  assert (invsig_storage);
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (invsig_storage);
	  assert (as);
	  const std::string& attr = as->is_const () ? rodata_attr : data_attr;

	  os << '\n'
	     << iw0 << "/* invariant Signals */\n";

	  // タスク関数外であれば、invariant block signalの構造体名をマ
	  // クロで置換する
	  if (outer)
	    define_macro (os, *invsig_storage, "", postfix, define_list);

	  os << as->set_indent (stdef, ind, attr);
	}

      // タスクの関数外かつ外部入力変数を外に出す、
      // または、タスクの関数内で外部入力変数を外に出さない、かつ
      // 外部入力変数を持つ場合は外部入力変数を出力する
      if ((outer && task_attr.out_extin ())
	  || (!outer && !task_attr.out_extin ()
	      && task_attr.has_extin ()))
	{
	  const optional_refstring&
	    extin_storage = c_codewriter.extin_storage ();
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (extin_storage);
	  if (as)
	    {
	      os << '\n'
		 << iw0 << "/* external input */\n";

	      // タスク関数外であれば、外部入力変数の構造体名をマクロで
	      // 置換する
	      if (outer)
		define_macro (os, *extin_storage, "", postfix, define_list);

	      os << as->set_indent (stdef, ind, data_attr);
	    }
	}

      // タスクの関数外かつ外部出力変数を外に出す、
      // または、タスクの関数内で外部出力変数を外に出さない、かつ
      // 外部出力変数を持つ場合は外部出力変数を出力する
      if ((outer && task_attr.out_extout ())
	  || (!outer && !task_attr.out_extout ()
	      && task_attr.has_extout ()))
	{
	  const optional_refstring&
	    extout_storage = c_codewriter.extout_storage ();
	  const AutoStorage*
	    as = taskset->autostorage ().get_storage (extout_storage);
	  if (as)
	    {
	      os << '\n'
		 << iw0 << "/* external output */\n";

	      // タスク関数外であれば、外部出力変数の構造体名をマクロで
	      // 置換する
	      if (outer)
		define_macro (os, *extout_storage, "", postfix, define_list);

	      os << as->set_indent (stdef, ind, data_attr);
	    }
	}

      // その他、入出力変数となっているものの出力
      const AutoStorage* export_globals = taskset->autostorage ()
	.get_storage (AutoStorage::export_global_storage);

      if (export_globals)
	{
	  bool first = true;
	  for (AutoStorage::const_iterator i (export_globals->begin ());
	       i != export_globals->end (); i++)
	    {
	      const var_writer* v = *i;

	      // タスク関数外に出力する変数になっているかどうか。
	      //
	      // out_signal()には、ブロックのコード以外(例えば
	      // SubSystemの関数)などから参照される変数が設定される
	      var_param_map_T::const_iterator
		vpmi = task_attr.out_signal ().find (v->name ());
	      var_param_map_T::const_iterator
		vpmie = task_attr.out_signal ().end ();

	      // タスク関数外の場合で、タスク関数外の関数から参照されて
	      // いない変数、および、タスク関数内の場合で、逆にタスク関
	      // 数外の関数から参照されている変数は出力しない
	      if ((outer && vpmi == vpmie)
		  || (!outer && vpmi != vpmie))
		continue;

	      if (first)
		{
		  os << '\n'
		     << iw0 << "/* input/output/auto variables */\n";
		  first = false;
		}

	      // タスク関数外の出力の場合は変数名をマクロで置換する
	      if (outer)
		  define_macro (os, v->name (), "", postfix, define_list);

	      os << iw0 << stdef << v->defn_writer ();
	      if (data_attr.length ())
		os << ' ' << data_attr;
	      os << ";\n";
	    }
	}
    }


    /// @}
  }
}
