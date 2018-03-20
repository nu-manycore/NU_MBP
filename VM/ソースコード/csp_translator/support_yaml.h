// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef SUPPRT_YAML_H
#define SUPPRT_YAML_H

/// \file support_yaml.h
///
/// \brief YAML入力機能

#include <string>
#include <list>
#include <vector>
#include <map>

#include <yaml.h>

namespace Simulink
{
  namespace CspTrans
  {
    namespace Support
    {
      /// \addtogroup CSP_TRANSLATOR_SUPPORT
      ///
      /// @{

      //
      // YAMLの名前空間とdoxygenのグループの設定
      //

      /// \defgroup CSP_TRANSLATOR_SUPPORT_YAML YAML入力機能
      ///
      /// \brief YAML入力機能
      ///
      /// @{

      /// \brief YAML入力機能の名前空間
      namespace YAML
      {
      }

      /// @}

      /// @}
    }
  }
}


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

	class YAMLParser;
	class YAMLScalar;
	class YAMLMap;
	class YAMLSeq;
	class YAMLMapKey;

	/// \brief YAMLのオブジェクトの基底クラス
	///
	/// 継承されたクラスのコンテナ格納のためのクラスで、
	/// 継承されたクラスの識別、変換機能を提供する。
	///
	/// YAMLScalar, YAMLMap, YAMLSeq, YAMLMapKey
	/// の各クラスが本クラスを継承する
	///
	/// \sa YAMLScalar
	/// \sa YAMLMap
	/// \sa YAMLSeq
	/// \sa YAMLMapKey
	class YAMLObj
	{
	public:
	  /// \brief YAMLのオブジェクトの種別
	  enum Type
	    {
	      /// \brief スカラ
	      SCALAR,
	      /// \brief マッピング(ハッシュ)
	      MAP,
	      /// \brief シーケンス
	      SEQ,
	      /// \brief マッピングのキー
	      MAPKEY
	    };

	  /// \brief コンストラクタ
	  ///
	  /// \param[in] t 継承するクラスの種別
	  /// \param[in] p YAMLParser のポインタ
	  /// \param[in] id ハッシュのキーの出現順を識別するID
	  YAMLObj (Type t, YAMLParser* p, int id = -1)
	    : o_type (t), o_parser (p), o_id (id)
	  {
	  }

	  /// \brief 継承したクラスの種別の取得
	  ///
	  /// \return 継承したクラスの種別
	  Type type (void) const { return o_type; }

	  /// \brief ハッシュのキーの出現順を識別するIDの取得
	  ///
	  /// \return 出現順を識別するID
	  int id (void) const { return o_id; }

	  /// \brief ハッシュのキーの出現順を識別するIDの設定
	  ///
	  /// \param[in] i ID
	  void id (int i) { o_id = i; }

	  /// \brief YAMLParser のポインタを取得
	  ///
	  /// \return YAMLParser のポインタ
	  YAMLParser* parser (void) const { return o_parser; }

	  // YAMLScalar のconstなポインタへの変換
	  const YAMLScalar* toScalar (void) const;

	  // YAMLMap のconstなポインタへの変換
	  const YAMLMap* toMap (void) const;

	  // YAMLSeq のconstなポインタへの変換
	  const YAMLSeq* toSeq (void) const;

	  // YAMLMapKey のconstなポインタへの変換
	  const YAMLMapKey* toMapKey (void) const;

	  // YAMLScalar のポインタへの変換
	  YAMLScalar* toScalar (void);

	  // YAMLMap のポインタへの変換
	  YAMLMap* toMap (void);

	  // YAMLSeq のポインタへの変換
	  YAMLSeq* toSeq (void);

	  // YAMLMapKey のポインタへの変換
	  YAMLMapKey* toMapKey (void);

	  /// \brief YAMLScalar のconstなポインタへのキャスト
	  ///
	  /// \return YAMLScalar のconstなポインタ
	  operator const YAMLScalar* () const { return toScalar (); }

	  /// \brief YAMLMap のconstなポインタへのキャスト
	  ///
	  /// \return YAMLMap のconstなポインタ
	  operator const YAMLMap* () const { return toMap (); }

	  /// \brief YAMLSeq のconstなポインタへのキャスト
	  ///
	  /// \return YAMLSeq のconstなポインタ
	  operator const YAMLSeq* () const { return toSeq (); }

	  /// \brief YAMLMapKey のconstなポインタへのキャスト
	  ///
	  /// \return YAMLMapKey のconstなポインタ
	  operator const YAMLMapKey* () const { return toMapKey (); }

	  /// \brief YAMLScalar のポインタへのキャスト
	  ///
	  /// \return YAMLScalar のポインタ
	  operator YAMLScalar*() { return toScalar (); }

	  /// \brief YAMLMap のポインタへのキャスト
	  ///
	  /// \return YAMLMap のポインタ
	  operator YAMLMap*() { return toMap (); }

	  /// \brief YAMLSeq のポインタへのキャスト
	  ///
	  /// \return YAMLSeq のポインタ
	  operator YAMLSeq*() { return toSeq (); }

	  /// \brief YAMLMapKey のポインタへのキャスト
	  ///
	  /// \return YAMLMapKey のポインタ
	  operator YAMLMapKey*() { return toMapKey (); }

	  /// \brief 出現順にソートするための比較関数
	  ///
	  /// \param[in] a 比較1
	  /// \param[in] b 比較2
	  ///
	  /// \retval true \a a が先
	  /// \retval false \a b が先
	  static bool
	  idcmp (const YAMLObj* a, const YAMLObj* b)
	  {
	    return a->id () < b->id ();
	  }

	private:
	  /// \brief 継承したクラスの種別
	  Type o_type;

	  /// \brief YAMLParser のポインタ
	  YAMLParser* o_parser;

	  /// \brief ハッシュのキーの出現順を識別するID
	  int o_id;
	};

	/// \brief YAMLMapKey クラス
	///
	/// このクラスは std::string と YAMLObj を継承した、
	/// ハッシュのキーとなるクラス。
	/// 値を示す YAMLObj のポインタも保持する
	///
	/// \sa YAMLObj
	class YAMLMapKey
	  : public std::string,
	    public YAMLObj
	{
	public:
	  /// \brief YAMLMapKey コンストラクタ
	  ///
	  /// \param[in] k ハッシュのキー
	  /// \param[in] p YAMLParser のポインタ
	  /// \param[in] id ハッシュのキーの出現順を識別するID
	  /// \param[in] v 値となる YAMLObj のポインタ
	  YAMLMapKey (const std::string& k,
		      YAMLParser* p, int id = -1, YAMLObj* v = NULL)
	    : std::string (k), YAMLObj (MAPKEY, p, id), k_value (v)
	  {
	  }

	  /// \brief 値の設定
	  ///
	  /// \param[in] v 値となる YAMLObj のポインタ
	  ///
	  /// \return このインスタンスの参照
	  YAMLMapKey& set (YAMLObj *v) { k_value = v; return *this; }

	  /// \brief 値の取得
	  ///
	  /// \return 値となる YAMLObj のポインタ
	  YAMLObj* get (void) const { return k_value; }

	  /// \brief 値へのキャスト
	  ///
	  /// \return 値となる YAMLObj のポインタ
	  YAMLObj* operator*() const { return k_value; }

	private:
	  /// \brief 値の YAMLObj のポインタ
	  YAMLObj* k_value;
	};

	/// \brief YAMLScalar クラス
	///
	/// YAMLのスカラを保持するクラスで
	/// std::string と YAMLObj を継承する
	///
	/// \sa YAMLObj
	class YAMLScalar
	  : public std::string,
	    public YAMLObj
	{
	public:
	  /// YAMLScalar のコンストラクタ
	  ///
	  /// \param[in] s 文字列
	  /// \param[in] p YAMLParser のポインタ
	  YAMLScalar (const std::string& s, YAMLParser* p)
	    : std::string (s), YAMLObj (SCALAR, p)
	  {
	  }

	  // booleanの値を取得する
	  bool get_bool (void) const;

	  // longの値を取得する
	  long get_long (void) const;

	  // unsigned longの値を取得する
	  unsigned long	get_ulong (void) const;

	  // floatの値を取得する
	  float get_float (void) const;

	  // doubleの値を取得する
	  double get_double (void) const;

	  // 大文字小文字を区別しない文字列比較
	  int casecmp (const std::string& s) const;
	};

	/// \brief YAMLMap クラス
	///
	/// このクラスはYAMLのハッシュで、 YAMLMapKey をキーとし、
	/// YAMLObj のポインタとのペアの std::map と、
	/// YAMLObj を継承する。
	///
	/// \sa YAMLMapKey
	/// \sa YAMLObj
	class YAMLMap
	  : public std::map < YAMLMapKey, YAMLObj* >,
	    public YAMLObj
	{
	public:
	  /// \brief YAMLMap のコンストラクタ
	  ///
	  /// \param[in] p YAMLParser のポインタ
	  YAMLMap (YAMLParser* p) : YAMLObj (MAP, p) {}

	  // YAMLのハッシュに値を追加する
	  YAMLMap::iterator add (const std::string& key, int id, YAMLObj* v);

	  /// \brief 値の取得
	  ///
	  /// \param[in] k YAMLMapKey
	  ///
	  /// \retval キーに対する値があれば YAMLObj のポインタ
	  /// \retval キーに対する値がなければ NULL
	  YAMLObj*
	  get (const YAMLMapKey& k) const
	  {
	    const_iterator i = find (k);
	    if (i == end ())
	      return NULL;
	    else
	      return i->second;
	  }

	  /// \brief 値の取得
	  ///
	  /// \param[in] x 文字列
	  ///
	  /// \retval キーに対する値があれば YAMLObj のポインタ
	  /// \retval キーに対する値がなければ NULL
	  ///
	  /// \sa YAMLMap::get(const YAMLMapKey& k) const
	  YAMLObj*
	  get (const std::string& x) const
	  {
	    return get (YAMLMapKey (x, parser ()));
	  }
	};

	/// \brief YAMLSeq クラス
	///
	/// このクラスはYAMLのシーケンスで、
	/// YAMLObj のポインタの std::vector と、
	/// YAMLObj を継承する。
	///
	/// \sa YAMLObj
	class YAMLSeq
	  : public std::vector < YAMLObj* >,
	    public YAMLObj
	{
	public:
	  /// \brief YAMLSeq のコンストラクタ
	  ///
	  /// \param[in] p YAMLParser のポインタ
	  YAMLSeq (YAMLParser* p) : YAMLObj (SEQ, p) {}
	};

	/// \brief YAMLScalar のconstなポインタへの変換
	///
	/// \return YAMLScalar のconstなポインタ
	inline const YAMLScalar*
	YAMLObj::toScalar (void) const
	{
	  return static_cast< const YAMLScalar* > (this);
	}

	/// \brief YAMLMap のconstなポインタへの変換
	///
	/// \return YAMLMap のconstなポインタ
	inline const YAMLMap*
	YAMLObj::toMap (void) const
	{
	  return static_cast< const YAMLMap* > (this);
	}

	/// \brief YAMLSeq のconstなポインタへの変換
	///
	/// \return YAMLSeq のconstなポインタ
	inline const YAMLSeq*
	YAMLObj::toSeq (void) const
	{
	  return static_cast< const YAMLSeq* > (this);
	}

	/// \brief YAMLMapKey のconstなポインタへの変換
	///
	/// \return YAMLMapKey のconstなポインタ
	inline const YAMLMapKey*
	YAMLObj::toMapKey (void) const
	{
	  return static_cast< const YAMLMapKey* > (this);
	}

	/// \brief YAMLScalar のポインタへの変換
	///
	/// \return YAMLScalar のポインタ
	inline YAMLScalar*
	YAMLObj::toScalar (void)
	{
	  return static_cast< YAMLScalar* > (this);
	}

	/// \brief YAMLMap のポインタへの変換
	///
	/// \return YAMLMap のポインタ
	inline YAMLMap*
	YAMLObj::toMap (void)
	{
	  return static_cast< YAMLMap* > (this);
	}

	/// \brief YAMLSeq のポインタへの変換
	///
	/// \return YAMLSeq のポインタ
	inline YAMLSeq*
	YAMLObj::toSeq (void)
	{
	  return static_cast< YAMLSeq* > (this);
	}

	/// \brief YAMLMapKey のポインタへの変換
	///
	/// \return YAMLMapKey のポインタ
	inline YAMLMapKey*
	YAMLObj::toMapKey (void)
	{
	  return static_cast< YAMLMapKey* > (this);
	}

	/// \brief アンカーのマップのクラス
	///
	/// このクラスはアンカーとそれに対応する YAMLObj のポインタの
	/// std::map を継承する。
	class YAMLAnchorMap
	  : public std::map < std::string, YAMLObj* >
	{
	};

	class YAMLParser;

	/// \brief YAMLのトップレベルのキーに対するコールバックの型
	typedef int (*YAMLCallBackFuncType) (YAMLObj*);

	/// \brief トップレベルのキーに対するコールバックを保持するクラス
	class YAMLCallBackEntry
	{
	public:
	  /// \brief YAMLCallBackEntry のデフォルトコンストラクタ
	  YAMLCallBackEntry () : cbe_cb (NULL) {}

	  /// \brief YAMLCallBackEntry のコンストラクタ
	  ///
	  /// \param[in] entry トップレベルのキー
	  /// \param[in] cb コールバック関数のアドレス
	  YAMLCallBackEntry (const std::string& entry,
			     YAMLCallBackFuncType cb)
	    : cbe_entry (entry), cbe_cb (cb)
	  {
	  }

	  /// \brief トップレベルのキーの設定
	  ///
	  /// \param[in] s トップレベルのキー
	  ///
	  /// \return このインスタンスの参照
	  YAMLCallBackEntry&
	  entry (const std::string& s)
	  {
	    cbe_entry = s;
	    return *this;
	  }

	  /// \brief コールバック関数の設定
	  ///
	  /// \param[in] cb コールバック関数のアドレス
	  ///
	  /// \return このインスタンスの参照
	  YAMLCallBackEntry&
	  callback (YAMLCallBackFuncType cb)
	  {
	    cbe_cb = cb;
	    return *this;
	  }

	  /// \brief トップレベルのキーの取得
	  ///
	  /// \return トップレベルのキーのconstな参照
	  const std::string& entry (void) const { return cbe_entry; }

	  /// \brief コールバック関数の取得
	  ///
	  /// \return コールバック関数のアドレス
	  YAMLCallBackFuncType callback (void) const { return cbe_cb; }

	private:
	  /// \brief トップレベルのキー
	  std::string cbe_entry;

	  /// \brief コールバック関数のアドレス
	  YAMLCallBackFuncType cbe_cb;
	};

	/// \brief YAMLのトップレベルのキーとコールバックのマップのクラス
	class YAMLCallBackMap
	  : public std::map < std::string, YAMLCallBackEntry >
	{
	public:
	  /// \brief トップレベルのキーとコールバックの登録
	  ///
	  /// \param[in] ent トップレベルのキー
	  /// \param[in] cb コールバック関数のアドレス
	  void
	  add_entry (const std::string& ent,
		     YAMLCallBackFuncType cb)
	  {
	    (*this)[ent].entry (ent).callback (cb);
	  }

	  /// \brief トップレベルのキーに対するコールバックの取得
	  ///
	  /// \param[in] ent トップレベルのキー
	  ///
	  /// \return YAMLCallBackEntry のポインタ
	  const YAMLCallBackEntry*
	  get_entry (const std::string& ent)
	  {
	    const_iterator i = find (ent);
	    if (i == end ())
	      return NULL;
	    return &i->second;
	  }
	};

	/// \brief YAMLCallBack のクラス
	///
	/// このクラスは static なメンバを持つクラスで、
	/// コールバックを保持するコンテナと、
	/// コールバックを実行する関数を持つ。
	///
	/// このクラスの目的は、複数のYAMLを入力した際にも対応できるよ
	/// うにするためである。
	class YAMLCallBack
	{
	public:
	  /// \brief コールバックの登録
	  ///
	  /// \param[in] ent トップレベルのキー
	  /// \param[in] cb コールバック関数のアドレス
	  static void
	  add_callback (const std::string& ent,
			YAMLCallBackFuncType cb)
	  {
	    cb_map.add_entry (ent, cb);
	  }

	  // コールバックの実行
	  static int run_callbacks (YAMLParser* parser);

	private:
	  /// \brief コールバックのマップ
	  static YAMLCallBackMap cb_map;
	};

	/// \brief YAMLParser のクラス
	///
	/// このクラスはYAMLのパーサーを提供する
	class YAMLParser
	{
	public:
	  /// \brief YAMLParser のデフォルトコンストラクタ
	  YAMLParser ()
	    : parse_ok (false), idnum (0), y_root (this), ind (0)
	  {
	    initialize ();
	  }

	  /// \brief YAMLParser のコンストラクタ
	  ///
	  /// \param[in] file 入力YAMLファイル名
	  YAMLParser (const char* file)
	    : parse_ok (false), idnum (0), y_root (this), ind (0)
	  {
	    initialize ();
	    parse (file);
	  }

	  /// \brief YAMLParser のデストラクタ
	  ~YAMLParser () { yaml_parser_delete (&parser); }

	  /// \brief YAMLParser のインスタンスを初期化する
	  void
	  initialize (void)
	  {
	    memset (&parser, 0, sizeof parser);
	    yaml_parser_initialize (&parser);
	  }

	  /// \brief YAMLのファイルのトップレベルのシーケンスを返す
	  ///
	  /// このクラスでは、ファイル中に複数のドキュメントであっても、
	  /// それらは単一のシーケンスとして保持する
	  ///
	  /// \return YAMLSeq の参照
	  YAMLSeq& root (void) { return y_root; }

	  // YAMLScalar の作成
	  YAMLScalar& make_scalar (const yaml_event_t& event);

	  // YAMLMap の作成
	  YAMLMap& make_map (const yaml_event_t& event);

	  // YAMLSeq の作成
	  YAMLSeq& make_seq (const yaml_event_t& event);

	  // YAMLファイルのパース
	  int parse (const char* file);

	  // YAMLのハッシュのパース
	  int parse (YAMLMap& map);

	  // YAMLのシーケンスのパース
	  int parse (YAMLSeq& seq);

	  // アンカーに対応する YAMLObj の取得
	  YAMLObj* get_anchor (const std::string& a) const;

	  /// \brief 利用可能かどうかの判定
	  ///
	  /// \retval true YAMLが読み込まれ、使用可能な状態
	  /// \retval false 未処理やエラーで使用不可能な状態
	  operator bool () const { return parse_ok; }

	private:
	  /// \brief 利用可能かどうか
	  bool parse_ok;

	  /// \brief 出現順を示す、単調増加するID
	  int idnum;

	  /// \brief YAMLファイルの最上位のアイテムを保持するシーケンス
	  YAMLSeq y_root;

	  /// \brief libyaml の yaml_parser_t
	  yaml_parser_t parser;

	  /// \brief 生成した YAMLScalar を保持する
	  std::list < YAMLScalar > yaml_scalar;

	  /// \brief 生成した YAMLMap を保持する
	  std::list < YAMLMap > yaml_map;

	  /// \brief 生成した YAMLSet を保持する
	  std::list < YAMLSeq > yaml_seq;

	  /// \brief アンカーと対応する YAMLObj のマップ
	  YAMLAnchorMap yaml_anchor_map;

	  /// デバッグ出力用に保持するインデント
	  int ind;		// debug
	};

	/// \brief YAMLをダンプする際のインデントに対応するクラス
	class yaml_dumper_indent
	{
	public:
	  /// \brief yaml_dumper_indent のコンストラクタ
	  ///
	  /// \param[in] i インデント
	  yaml_dumper_indent (size_t i) : ydi_i (i) {}

	  /// \brief インデントを取得するキャスト
	  ///
	  /// \return インデント
	  operator size_t () const { return ydi_i; }

	private:
	  /// \brief インデント
	  size_t ydi_i;
	};

	// 指定したインデントを出力するオペレータ
	std::ostream&
	operator<< (std::ostream& os, const yaml_dumper_indent& i);

	/// \brief yaml_dumper クラス
	///
	/// YAMLのオブジェクトをダンプするためのテンプレートクラスで、
	/// YAMLScalar, YAMLMap をラップする。
	///
	template < class W >
	class yaml_dumper
	{
	public:
	  /// \brief yaml_dumper のコンストラクタ
	  ///
	  /// \param[in] w ラップする対象
	  /// \param[in] i インデント
	  yaml_dumper (const W& w, size_t i) : yd_w (w), yd_i (i) {}

	  /// \brief ラップした対象の参照を取得する
	  ///
	  /// \return ラップした対象の参照
	  operator const W& () const { return yd_w; }

	  /// \brief インデントの取得
	  ///
	  /// \return インデント
	  size_t indent (void) const { return yd_i; }

	private:
	  /// \brief ラップした対象の参照
	  const W& yd_w;

	  /// \brief インデント
	  size_t yd_i;
	};

	// YAMLScalar の出力オペレータ
	std::ostream&
	operator<< (std::ostream& os, const YAMLScalar& s);

	// YAMLMapKey の出力オペレータ
	std::ostream&
	operator<< (std::ostream& os, const YAMLMapKey& k);

	// YAMLSeq の出力オペレータ
	std::ostream&
	operator<< (std::ostream& os, const yaml_dumper< YAMLSeq >& is);

	// YAMLMap の出力オペレータ
	std::ostream&
	operator<< (std::ostream& os, const yaml_dumper< YAMLMap >& im);

      }

      /// @}
    }
  }
}

#endif	// SUPPRT_YAML_H
