// -*- mode: c++; coding: utf-8-unix; -*-

/// \file support_yaml.cxx
///
/// \brief YAML入力機能

#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <errno.h>
#include <string.h>
#include <assert.h>

#include "support_yaml.h"

namespace Simulink
{
  namespace CspTrans
  {
    namespace Support
    {
      namespace YAML
      {
	/// \addtogroup CSP_TRANSLATOR_SUPPORT_YAML
	///
	/// @{

	/// \brief コールバックを格納するマップ
	YAMLCallBackMap YAMLCallBack::cb_map;

	/// \brief YAMLのハッシュに値を追加する
	///
	/// \param[in] key ハッシュのキー
	/// \param[in] id 出現順のID
	/// \param[in] v 値となる YAMLObj のポインタ
	///
	/// \return YAMLMap の iterator
	YAMLMap::iterator
	YAMLMap::add (const std::string& key, int id, YAMLObj* v)
	{
	  std::pair < YAMLMap::iterator, bool >
	    r = insert (make_pair (YAMLMapKey (key, parser (), id, v), v));
	  return r.first;
	}

	/// \brief booleanの値を取得する
	///
	/// \retval true y/Y/yes/YES/on/ON/true/TRUE の場合
	/// \retval false それ以外
	bool
	YAMLScalar::get_bool (void) const
	{
	  return (casecmp ("y")
		  || casecmp ("yes")
		  || casecmp ("true")
		  || casecmp ("on"));
	}

	/// \brief longの値を取得する
	///
	/// \return スカラをlongに変換した値
	long
	YAMLScalar::get_long (void) const
	{
	  return strtol (c_str (), NULL, 0);
	}

	/// \brief unsigned longの値を取得する
	///
	/// \return スカラをunsigned longに変換した値
	unsigned long
	YAMLScalar::get_ulong (void) const
	{
	  return strtoul (c_str (), NULL, 0);
	}

	/// \brief floatの値を取得する
	///
	/// \return スカラをfloatに変換した値
	float
	YAMLScalar::get_float (void) const
	{
	  return strtof (c_str (), NULL);
	}

	/// \brief doubleの値を取得する
	///
	/// \return スカラをdoubleに変換した値
	double
	YAMLScalar::get_double (void) const
	{
	  return strtod (c_str (), NULL);
	}

	/// \brief 大文字小文字を区別しない文字列比較
	///
	/// \param[in] t 比較対象文字列
	///
	/// \return 比較した結果
	int
	YAMLScalar::casecmp (const std::string& t) const
	{
	  const std::string& s = *this;
	  std::string::const_iterator p1 (s.begin ());
	  std::string::const_iterator p2 (t.begin ());

	  while (p1 != s.end () && p2 != t.end ())
	    {
	      int c = toupper (*p1) - toupper (*p2);
	      if (c)
		return c;
	      p1++;
	      p2++;
	    }
	  if (p1 == s.end ())
	    {
	      if (p2 == t.end ())
		return 0;
	      return -1;
	    }
	  return 1;
	}

	/// \brief コールバックの実行
	///
	/// \param[in] parser
	///
	/// \return 0 成功
	/// \return 0以外 コールバックでエラー発生
	int
	YAMLCallBack::run_callbacks (YAMLParser* parser)
	{
	  YAMLSeq& root = parser->root ();
	  int errors = 0;
	  for (YAMLSeq::const_iterator i (root.begin ());
	       i != root.end (); i++)
	    {
	      YAMLObj* o = *i;
	      if (o->type () != YAMLObj::MAP)
		continue;

	      YAMLMap* m = *o;

	      for (YAMLMap::const_iterator mi (m->begin ());
		   mi != m->end (); mi++)
		{
		  const YAMLMapKey& k = mi->first;
		  const YAMLCallBackEntry* cbe = cb_map.get_entry (k);
		  if (cbe)
		    {
		      if ((cbe->callback ()) (mi->second))
			{
			  std::cerr << "Error: " << k << ": callback fail"
				    << std::endl;
			  errors++;
			}
		    }
		}
	    }

	  return errors;
	}

	/// \brief YAMLScalar の作成
	///
	/// \param[in] event yaml_event_t の参照
	///
	/// \return 作成した YAMLScalar の参照
	YAMLScalar&
	YAMLParser::make_scalar (const yaml_event_t& event)
	{
	  const char*
	    value = reinterpret_cast< char* > (event.data.scalar.value);
	  const char*
	    anchor = reinterpret_cast< char* > (event.data.scalar.anchor);

	  yaml_scalar.push_back (YAMLScalar (value, this));
	  YAMLScalar& s = yaml_scalar.back ();
	  if (anchor)
	    yaml_anchor_map[anchor] = &s;
	  return s;
	}

	/// \brief YAMLMap の作成
	///
	/// \param[in] event yaml_event_t の参照
	///
	/// \return 作成した YAMLMap の参照
	YAMLMap&
	YAMLParser::make_map (const yaml_event_t& event)
	{
	  const char*
	    anchor = reinterpret_cast< char* >
	    (event.data.mapping_start.anchor);

	  yaml_map.push_back (YAMLMap (this));
	  YAMLMap& m = yaml_map.back ();
	  if (anchor)
	    yaml_anchor_map[anchor] = &m;
	  return m;
	}

	/// \brief YAMLSeq の作成
	///
	/// \param[in] event yaml_event_t の参照
	///
	/// \return 作成した YAMLSeq の参照
	YAMLSeq&
	YAMLParser::make_seq (const yaml_event_t& event)
	{
	  const char*
	    anchor = reinterpret_cast< char* >
	    (event.data.sequence_start.anchor);

	  yaml_seq.push_back (YAMLSeq (this));
	  YAMLSeq& s = yaml_seq.back ();
	  if (anchor)
	    yaml_anchor_map[anchor] = &s;
	  return s;
	}

	/// \brief YAMLのハッシュのパース
	///
	/// \param[in] map ハッシュの参照
	///
	/// \retval 0 成功
	/// \retval 0以外 失敗
	int
	YAMLParser::parse (YAMLMap& map)
	{
	  yaml_event_t event;
	  int ret = 0;

	  while (1)
	    {
	      if (!yaml_parser_parse (&parser, &event))
		return -1;

	      std::string key;

	      switch (event.type)
		{
		case YAML_MAPPING_END_EVENT:
#ifdef PARSE_DEBUG
		  ind--;
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_MAPPING_END_EVENT"
			    << std::endl;
#endif
		  yaml_event_delete (&event);
		  return 0;

		case YAML_SCALAR_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_SCALAR_EVENT("
			    << reinterpret_cast< char* > (event.data.scalar.value)
			    << ')'
			    << std::endl;
#endif
		  key = reinterpret_cast< char* > (event.data.scalar.value);
		  break;

		default:
		  std::cerr << "Error: couldn't handle yaml_event_type_e: "
			    << event.type << std::endl;
		  yaml_event_delete (&event);
		  return -1;
		}

	      yaml_event_delete (&event);

	      if (!yaml_parser_parse (&parser, &event))
		return -1;

	      switch (event.type)
		{
		case YAML_SCALAR_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_SCALAR_EVENT("
			    << reinterpret_cast< char* > (event.data.scalar.value)
			    << ')'
			    << std::endl;
#endif
		  {
		    YAMLScalar& s = make_scalar (event);
		    map.add (key, idnum++, &s);
		    yaml_event_delete (&event);
		  }
		  break;

		case YAML_ALIAS_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_ALIAS_EVENT"
			    << std::endl;
#endif
		  {
		    const char*
		      anchor = reinterpret_cast< char* >
		      (event.data.alias.anchor);
		    YAMLObj* o = get_anchor (anchor);
		    if (o == NULL)
		      {
			std::cerr << "Error: anchor not found: "
				  << anchor << std::endl;
			yaml_event_delete (&event);
			return -1;
		      }
		    map.add (key, idnum++, o);
		    yaml_event_delete (&event);
		  }

		  break;

		case YAML_MAPPING_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_MAPPING_START_EVENT" << std::endl;
		  ind++;
#endif
		  {
		    YAMLMap& m = make_map (event);
		    map.add (key, idnum++, &m);
		    yaml_event_delete (&event);
		    ret = parse (m);
		    if (ret)
		      return ret;
		  }
		  break;

		case YAML_SEQUENCE_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_SEQUENCE_START_EVENT"
			    << std::endl;
		  ind++;
#endif
		  {
		    YAMLSeq& s = make_seq (event);
		    map.add (key, idnum++, &s);
		    yaml_event_delete (&event);
		    ret = parse (s);
		    if (ret)
		      return ret;
		  }
		  break;

		default:
		  std::cerr << "Error: couldn't handle yaml_event_type_e: "
			    << event.type << std::endl;
		  yaml_event_delete (&event);
		  return -1;
		}
	    }

	  // NOTREACHED
	  return 0;
	}

	/// \brief YAMLのシーケンスのパース
	///
	/// \param[in] seq シーケンスの参照
	///
	/// \retval 0 成功
	/// \retval 0以外 失敗
	int
	YAMLParser::parse (YAMLSeq& seq)
	{
	  yaml_event_t event;

	  int ret = 0;

	  while (1)
	    {
	      if (!yaml_parser_parse (&parser, &event))
		return -1;

	      switch (event.type)
		{
		case YAML_SEQUENCE_END_EVENT:
#ifdef PARSE_DEBUG
		  ind--;
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_SEQUENCE_END_EVENT"
			    << std::endl;
#endif
		  yaml_event_delete (&event);
		  return 0;

		case YAML_SCALAR_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_SCALAR_EVENT("
			    << reinterpret_cast< char* > (event.data.scalar.value)
			    << ')'
			    << std::endl;
#endif
		  {
		    YAMLScalar& s = make_scalar (event);
		    seq.push_back (&s);
		    yaml_event_delete (&event);
		  }
		  break;

		case YAML_SEQUENCE_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_SEQUENCE_START_EVENT"
			    << std::endl;
		  ind++;
#endif
		  {
		    YAMLSeq& s = make_seq (event);
		    seq.push_back (&s);
		    yaml_event_delete (&event);
		    ret = parse (s);
		    if (ret)
		      return ret;
		  }
		  break;

		case YAML_MAPPING_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_MAPPING_START_EVENT" << std::endl;
		  ind++;
#endif
		  {
		    YAMLMap& m = make_map (event);
		    seq.push_back (&m);
		    yaml_event_delete (&event);
		    ret = parse (m);
		    if (ret)
		      return ret;
		  }
		  break;

		default:
		  std::cerr << "Error: couldn't handle yaml_event_type_e: "
			    << event.type << std::endl;
		  yaml_event_delete (&event);
		  return -1;
		}
	    }

	  // NOTREACHED
	  return 0;
	}

	/// \brief YAMLファイルのパース
	///
	/// \param[in] file 入力YAMLファイル名
	///
	/// \retval 0 成功
	/// \retval 0以外 失敗
	int
	YAMLParser::parse (const char* file)
	{
	  FILE *in = fopen (file, "rb");

	  if (in == NULL)
	    {
	      std::cerr << file << ": " << strerror (errno) << std::endl;
	      return -1;
	    }

	  yaml_parser_set_input_file (&parser, in);

	  yaml_event_t event;
	  int state = 1;
	  int ret = -1;

	  while (state)
	    {
	      if (!yaml_parser_parse (&parser, &event))
		{
		  ret = -1;	// error
		  break;
		}

	      switch (event.type)
		{
		case YAML_STREAM_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_STREAM_START_EVENT" << std::endl;
		  ind++;
#endif
		  yaml_event_delete (&event);
		  break;

		case YAML_STREAM_END_EVENT:
#ifdef PARSE_DEBUG
		  ind--;
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_STREAM_END_EVENT" << std::endl;
#endif
		  state = 0;
		  ret = 0;
		  yaml_event_delete (&event);
		  break;

		case YAML_DOCUMENT_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_DOCUMENT_START_EVENT" << std::endl;
		  ind++;
#endif
		  yaml_event_delete (&event);
		  break;

		case YAML_DOCUMENT_END_EVENT:
#ifdef PARSE_DEBUG
		  ind--;
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_DOCUMENT_END_EVENT" << std::endl;
#endif
		  yaml_event_delete (&event);
		  break;

		case YAML_MAPPING_START_EVENT:
#ifdef PARSE_DEBUG
		  std::cout << yaml_dumper_indent (ind)
			    << "YAML_MAPPING_START_EVENT" << std::endl;
		  ind++;
#endif
		  {
		    yaml_map.push_back (YAMLMap (this));
		    YAMLMap& m = yaml_map.back ();
		    if (event.data.mapping_start.anchor)
		      {
			std::string a (reinterpret_cast< char* >
				       (event.data.mapping_start.anchor));
			yaml_anchor_map[a] = &m;
#ifdef PARSE_DEBUG
			std::cout << "anchor: " << a << std::endl;
#endif
		      }
		    y_root.push_back (&m);
		    yaml_event_delete (&event);
		    int r = parse (m);
		    if (r)
		      {
			state = 0;
			ret = r;
		      }
		  }
		  break;

		case YAML_SEQUENCE_START_EVENT:
		  {
#ifdef PARSE_DEBUG
		    ind--;
		    std::cout << yaml_dumper_indent (ind)
			      << "YAML_SEQUENCE_START_EVENT"
			      << std::endl;
#endif
		    yaml_seq.push_back (YAMLSeq (this));
		    YAMLSeq &s = yaml_seq.back ();
		    if (event.data.sequence_start.anchor)
		      {
			std::string a (reinterpret_cast< char* >
				       (event.data.sequence_start.anchor));
			yaml_anchor_map[a] = &s;
#ifdef PARSE_DEBUG
			std::cout << "anchor: " << a << std::endl;
#endif
		      }
		    y_root.push_back (&s);
		    yaml_event_delete (&event);
		    int r = parse (s);
		    if (r)
		      {
			state = 0;
			ret = r;
		      }
		  }
		  break;

		default:
		  std::cerr << "unknown event: "
			    << event.type << std::endl;
		  assert (!1);
		}
	    }

	  yaml_parser_delete (&parser);

	  fclose (in);

	  if (ret == 0)
	    parse_ok = true;

	  return ret;
	}

	/// \brief アンカーに対応する YAMLObj の取得
	///
	/// \param[in] a アンカー名
	///
	/// \retval NULL 対応するアンカーがない
	/// \retval それ以外 アンカーに対応する YAMLObj のポインタ
	YAMLObj*
	YAMLParser::get_anchor (const std::string& a) const
	{
	  YAMLAnchorMap::const_iterator x = yaml_anchor_map.find (a);
	  if (x == yaml_anchor_map.end ())
	    return NULL;
	  return x->second;
	}

	/// \brief 指定したインデントを出力するオペレータ
	///
	/// \param[in] os 出力ストリーム
	/// \param[in] i yaml_dumper_indent の参照
	std::ostream&
	operator<< (std::ostream& os, const yaml_dumper_indent& i)
	{
	  size_t ind = i;
	  while (ind--)
	    os << "  ";
	  return os;
	}

	/// YAMLScalar の出力オペレータ
	///
	/// \param[in] os 出力ストリーム
	/// \param[in] s YAMLScalar の参照
	std::ostream&
	operator<< (std::ostream& os, const YAMLScalar& s)
	{
	  const std::string& x = s;
	  os << x;
	  if (s.id () != -1)
	    os << '<' << s.id () << '>';
	  return os;
	}

	/// \brief YAMLMapKey の出力オペレータ
	///
	/// \param[in] os 出力ストリーム
	/// \param[in] k YAMLMapKey の参照
	std::ostream&
	operator<< (std::ostream& os, const YAMLMapKey& k)
	{
	  const std::string& x = k;
	  os << x;
	  if (k.id () != -1)
	    os << '<' << k.id () << '>';
	  return os;
	}

	/// \brief YAMLSeq の出力オペレータ
	/// \param[in] os 出力ストリーム
	/// \param[in] im YAMLMap をラップした yaml_dumper の参照
	std::ostream&
	operator<< (std::ostream& os, const yaml_dumper< YAMLMap >& im)
	{
	  const YAMLMap& m = im;
	  size_t ind = im.indent ();
	  const yaml_dumper_indent iw (ind);

	  std::vector< const YAMLMapKey* > vl;
	  vl.reserve (m.size ());
	  for (YAMLMap::const_iterator i (m.begin ()); i != m.end (); i++)
	    vl.push_back (&i->first);
	  std::sort (vl.begin (), vl.end (), YAMLMapKey::idcmp);

	  for (std::vector< const YAMLMapKey* >::const_iterator
		 x (vl.begin ());
	       x != vl.end (); x++)
	    {
	      const YAMLMapKey& k = **x;
	      const YAMLObj* v = *k;

	      os << iw << k;

	      if (v->type () == YAMLObj::SCALAR)
		{
		  const YAMLScalar* s = *v;
		  os << " = " << *s << '\n';
		}
	      else if (v->type () == YAMLObj::MAP)
		{
		  const YAMLMap* m = *v;
		  os << " = {\n"
		     << yaml_dumper< YAMLMap > (*m, ind+1)
		     << iw << "}\n";
		}
	      else if (v->type () == YAMLObj::SEQ)
		{
		  const YAMLSeq* s = *v;
		  os << " = [\n"
		     << yaml_dumper< YAMLSeq > (*s, ind+1)
		     << iw << "]\n";
		}
	    }
	  return os;
	}

	/// \brief YAMLMap の出力オペレータ
	/// \param[in] os 出力ストリーム
	/// \param[in] is YAMLSeq をラップした yaml_dumper の参照
	std::ostream&
	operator << (std::ostream& os, const yaml_dumper < YAMLSeq >& is)
	{
	  const YAMLSeq& s = is;
	  size_t ind = is.indent ();
	  const yaml_dumper_indent iw (ind);

	  for (YAMLSeq::const_iterator x (s.begin ()); x != s.end (); x++)
	    {
	      const YAMLObj* v = *x;
	      if (v->type () == YAMLObj::SCALAR)
		{
		  const YAMLScalar* ss = *v;
		  os << iw << *ss << "\n";
		}
	      else if (v->type () == YAMLObj::MAP)
		{
		  const YAMLMap* m = *v;
		  os << iw << "{\n"
		     << yaml_dumper< YAMLMap > (*m, ind+1)
		     << iw << "}\n";
		}
	      else if (v->type () == YAMLObj::SEQ)
		{
		  const YAMLSeq* s = *v;
		  os << "[\n"
		     << yaml_dumper< YAMLSeq > (*s, ind+1)
		     << iw << "]\n";
		}
	    }
	  return os;
	}
	// @}
      }
    }
  }
}
