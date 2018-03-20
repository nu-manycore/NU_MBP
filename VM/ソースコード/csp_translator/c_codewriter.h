// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef C_CODEWRITER_H
#define C_CODEWRITER_H

/// \file c_codewriter.h
///
/// \brief Cコードライタのヘッダファイル


/// \addtogroup CSP_TRANSLATOR
///
/// @{

/// \defgroup C_CODEWRITER Cコードライタ
///
/// \brief Cのコードを出力するためのライタ
///
/// 本モジュールは、Cのコード出力を支援するクラスで、主要な機能は以下の
/// 通りである。
///
/// xml_csp.h のクラスの変数や関数を保持するクラスは var_writer や
/// function_writer を継承し、その変数や関数の出力を適切なラッパクラス
/// 等を適切に選択することで、効率的なターゲットコード生成を行う
///
/// AutoStorage は、Embedded Coderが生成する各構造体オブジェクト単位に
/// var_writer をまとめたもので、Cコードにおける構造体出力は、指示を含
/// めた AutoStorage_wrapper を用いる。
/// AutoStorageMap は構造体オブジェクト単位の連想配列である。
///
/// なおDatastoreについては上記とは別に、DataStoreMemory用の
/// AutoStorageMap と初期化コードを持つ。これはDataStoreMemoryは状態変
/// 数に出力されるが、例えば、ブロックの状態変数を局所化し、
/// DataStoreMemoryは共有する、といったターゲットに対し、コード出力を容
/// 易にするために、他の AutoStorageMap と分離している。
///
/// task_attr_T はコード生成の際に参照する、タスク単位の様々な属性であ
/// り rate_info_T はタスクの一つのレート情報であり、タスク内のレート
/// は、そのベクタであり重複を許す rate_list_T に一旦保持される。実際の
/// タスク内レート task_rates_T は rate_list_T から生成される。
///
/// TaskSet は、Cのコード上でタスクとなる CspNode の集合と、およそ上記
/// の情報を保持したクラスで、これがCコードの生成時のタスク単位である。
/// 例えばeMCOSでは、コア割り当てされた CspNode を、単一のコアアフィニ
/// ティスレッドとして生成するために使用している。それ以外では TaskSet
/// は単一の CspNode になる。
///
/// TaskSetList は上記 TaskSet のポインタのベクタであり、生成するタスク
/// の要素を保持する。
///
/// 最後に C_CodeWriter は TaskSetList の他に、Embedded Coderが出力する、
/// ブロック固有のコードやデータ以外のコード(インクルードするファイル、
/// 関数やマクロ、型およびオブジェクトの宣言や定義等)を格納し、Cコード
/// 出力のためのデータを保持するクラスである。
///
/// @{

/// \defgroup C_OBJECT_EC_ATTR オブジェクト属性
///
/// \brief Cコードに出力するオブジェクト属性
///
/// @{

/// \brief Real-Time Model Objectのextern宣言
#define C_OBJECT_EC_ATTR_RTM_EXTERN	0x01

/// \brief Real-Time Model Objectのpointerの定義
#define C_OBJECT_EC_ATTR_RTM_POINTER	0x02

/// \brief Real-Time Model Objectのオブジェクトの定義
#define C_OBJECT_EC_ATTR_RTM_OBJECT	0x04

/// \brief オブジェクトはstruct宣言を伴う
#define C_OBJECT_EC_ATTR_WITH_STRUCT	0x08

/// \brief オブジェクトのvalue()にコードがある
#define C_OBJECT_EC_ATTR_VALUE_IS_CODE	0x10

/// @}

/// @}

/// @}

#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <deque>

// xml_csp.hのpredefine
#include "xml_csp_i.h"

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup C_CODEWRITER
    ///
    /// @{

#ifdef STRICT_STRING
    /// \brief char* から string への暗黙の変換を禁止するクラス
    ///
    /// このクラスはCSPトランスレータにおける機能的なクラスではなく、
    /// プログラムのコード品質を高める事を目的としている。
    ///
    /// このクラスを用いることで、クラスメンバに
    /// string の参照やポインタを持つクラスで、
    /// クラスインスタンスの寿命内に、char* から string
    /// へ変換された一時オブジェクトの寿命が尽きるような、
    /// リスクのあるコードをコンパイル時に検出可能にするためである。
    ///
    /// 従って <em><b>STRICT_STRING</b></em> を定義してコンパイルを行い、
    /// 暗黙の変換を排除したことを確認できたら
    /// <em><b>STRICT_STRING</b></em> を undefine してよい。
    ///
    /// \note
    /// - マクロ <em><b>STRICT_STRING</b></em> を定義すると利用可能
    /// - コンパイルエラーが発生する箇所では、
    ///   一時オブジェクトで安全な場合は、暗黙に変換が行われる
    ///   Cのリテラルではなく string の一時オブジェクトを明示する
    /// - 一時オブジェクトが使用できない場合は、別に定義した string を使うこと
    /// - マクロ <em><b>STRICT_STRING</b></em> が未定義の場合、
    ///   strict_string は std::string の typedef である
    class strict_string
    {
    public:
      /// \brief char* から string への変換を禁止するためのコンストラクタ
      ///
      /// string を期待する箇所に char* が使われていた場合、
      /// このコンストラクタの暗黙の使用は禁止されているので、
      /// コンパイルエラーになる。
      /// またコンストラクタの実体はなく、使用できない(リンクエラーになる)
      explicit strict_string (const char*);

      /// \brief string から strict_string への変換用コンストラクタ
      ///
      /// \param[in] s string の参照
      ///
      /// \attention
      /// 単にコンパイルエラー回避のために、
      /// 明示的に string の一時オブジェクトを使用してはならない。\n
      /// 例えばClassAが string の参照を持つ場合、以下はダメである。\n
      /// \n
      /// \code
      /// ClassA a (std::string ("aaa"));
      /// std::cout << a; // "aaa" の string は破棄されている
      /// \endcode
      /// \n
      /// 一時オブジェクトを明示的に使用する場合、
      /// 処理中にその寿命が有効であるか注意すること。\n
      /// 例えば、次は大丈夫。\n
      /// \n
      /// \code
      /// std::cout << classA (std::string ("aaa"));
      /// \endcode
      ///
      strict_string (const std::string& s) : ss_string (s) {}

      /// \brief string へのキャスト
      operator const std::string& () const { return ss_string; }

    private:
      /// \brief string の参照
      const std::string& ss_string;
    };
#else
    /// \brief strict_string の宣言
    ///
    /// マクロ STRICT_STRING が定義されない場合、
    /// strict_string は string であり、
    /// char* から string への変換を禁止しない。
    typedef std::string strict_string;
#endif

    /// \brief オプショナルな値をもつクラスのテンプレート
    template <class X>
    class optional_template
    {
    public:
      /// \brief optional_templateのデフォルトコンストラクタ
      ///
      /// #ot_valid をfalseにする
      optional_template () : ot_valid (false) {}

      /// \brief optional_templateのコンストラクタ
      ///
      /// \param[in] x 値
      ///
      /// xを #ot_value に設定し、 #ot_valid をtrueにする
      optional_template (const X& x) : ot_valid (true), ot_value (x) {}

      /// \brief optional_templateのコンストラクタ
      ///
      /// \param[in] valid 値の有効(true)/無効(false)の真理値
      /// \param[in] x 値
      ///
      /// validを #ot_valid に設定し、xを #ot_value に設定する
      optional_template (bool valid, const X& x)
	: ot_valid (valid), ot_value (x)
      {}

      /// \brief optional_templateのコピーコンストラクタ
      ///
      /// \param[in] x コピー元の optional_template
      ///
      /// xをコピーする
      optional_template (const optional_template<X>& x)
	: ot_valid (x.ot_valid), ot_value (x.ot_value)
      {}

      /// \brief boolへのキャスト。値が有効か無効か調べる
      ///
      /// \retval true 値は有効
      /// \retval false 値は無効
      operator bool () const { return ot_valid; }

      /// \brief 値を取得
      ///
      /// \return 値
      const X& getv (void) const { return ot_value; }

      /// \brief 値の設定
      ///
      /// \param[in] x 値
      ///
      /// xを #ot_value に設定し、 #ot_valid をtrue(有効)にする
      void setv(const X& x) { ot_valid = true; ot_value = x; }

      /// \brief 有効無効と値を設定
      ///
      /// \param[in] v 有効無効の真理値
      /// \param[in] x 値
      ///
      /// vを #ot_valid に設定し、xを #ot_value に設定する
      ///
      /// 主に値を無効な値をコピーするために使う
      void setv(bool v, const X& x) { ot_valid = v; ot_value = x; }

      /// \brief 値を取得するための参照オペレータ(参照用)
      ///
      /// \return 値のconstなリファレンス
      const X& operator* () const { return ot_value; }

      /// \brief 値を取得するための参照オペレータ(設定用)
      ///
      /// \return 値のリファレンス
      X& operator* () { return ot_value; }

      /// \brief 値のポインタを取得するための参照オペレータ(参照用)
      ///
      /// \return 値のconstなポインタ
      const X* operator-> () const { return &ot_value; }

      /// \brief 値のポインタを取得するための参照オペレータ(設定用)
      ///
      /// \return 値のポインタ
      X* operator-> () { return &ot_value; }

      /// \brief 代入オペレータ
      ///
      /// \param[in] x コピー元の optional_template
      ///
      /// \return 更新された optional_template のリファレンス
      optional_template<X>& operator= (const optional_template<X>& x) {
	this->setv (x.ot_valid, x.ot_value);
	return *this;
      }

      /// \brief 比較オペレータ
      ///
      /// \param[in] x 比較先 optional_template
      ///
      /// \return true 一致
      /// \return false 不一致
      ///
      /// 無効同士の比較はfalseを返す
      bool operator== (const optional_template<X>& x) const
      {
	return ot_valid && x.ot_valid && ot_value == x.ot_value;
      }

    private:
      /// \brief 値が有効(true)/無効(false)
      bool ot_valid;

      /// \brief 値
      X ot_value;
    };

    //
    // writers
    //

    /// \brief インデントのみのクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa operator<<(std::ostream& s, const indent_writer& iw)
    /// \sa operator<<(std::ostream& s, const indent_writer* iw)
    class indent_writer
    {
    public:
      /// \brief indent_writer コンストラクタ
      ///
      /// \param[in] n インデント数(省略時0)
      ///
      /// 1インデントは2スペースである
      indent_writer(size_t n = 0) : iw_ind (n) {}

      /// \brief インデント数を取得
      ///
      /// \return インデント数
      size_t indent (void) const { return iw_ind; }

      /// \brief インデントを出力する静的メンバ関数
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] ind インデント数
      ///
      /// \return 出力ストリーム
      static std::ostream& writer(std::ostream& os, size_t ind);

      /// \brief インデントを出力する
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \return 出力ストリーム
      std::ostream&
      writer(std::ostream& os) const { return writer (os, iw_ind); }

      /// \brief インデント数1の静的メンバ
      static const indent_writer iw1;

      /// \brief インデント数2の静的メンバ
      static const indent_writer iw2;

      /// \brief インデント数3の静的メンバ
      static const indent_writer iw3;

      /// \brief インデント数4の静的メンバ
      static const indent_writer iw4;

    public:
      /// \brief インデント数
      size_t iw_ind;

      /// \brief インデント文字列
      ///
      /// 0-4の範囲で、1インデントは2スペースとし、4インデントはTAB(8ス
      /// ペース)としている
      static const char* iw_cindent[];
    };

    /// \brief indent_write の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] iw indent_writer
    ///
    /// \return 出力ストリーム
    ///
    /// \sa indent_writer
    std::ostream& operator<< (std::ostream& s, const indent_writer& iw);

    /// \brief indent_writer のポインタの出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] iw indent_writer のポインタ
    ///
    /// \return 出力ストリーム
    ///
    /// \sa indent_writer
    std::ostream& operator<< (std::ostream& s, const indent_writer* iw);

    /// \brief stringのポインタを保持するクラス
    ///
    /// optional_template を継承している
    ///
    /// 出力ストリームに対し "<<" オペレータで出力することができる
    ///
    /// \sa optional_template
    /// \sa operator<<(std::ostream& s, const optional_refstring& x)
    class optional_refstring
      : public optional_template< const std::string* >
    {
    public:
      /// \brief optional_refstring のデフォルトコンストラクタ
      optional_refstring()
	: optional_template < const std::string* > (false, NULL)
      {}

      /// \brief 文字列の参照オペレータ
      ///
      /// \return 文字列のconstなリファレンス
      const std::string& operator* () const { return *this->getv (); }

      /// \brief 文字列のポインタの参照オペレータ
      ///
      /// \return 文字列のconstなポインタ
      const std::string* operator-> () const { return this->getv (); }

      /// \brief 代入オペレータ
      ///
      /// \param[in] x 文字列
      ///
      /// \return 更新された optional_refstring のリファレンス
      optional_refstring& operator= (const strict_string& x) {
	const std::string& s = x;
	this->setv (&s);
	return *this;
      }

      /// \brief 比較(等号)オペレータ
      ///
      /// \param[in] x 比較先 optional_refstring
      ///
      /// \return true 一致
      /// \return false 不一致
      ///
      /// 無効なもの同士の比較はfalseを返す
      bool operator== (const optional_refstring& x) const
      { return (*this && x && *this->getv () == *x.getv ()); }

      /// \brief 比較(less than)オペレータ
      ///
      /// \param[in] x 比較先 optional_refstring
      ///
      /// \return true 自分の方が先
      /// \return false それ以外
      ///
      /// 無効なもの同士の比較はfalseを返す
      bool operator< (const optional_refstring& x) const
      { return (*this && x && *this->getv () < *x.getv ()); }
    };

    /// \brief optional_refstring の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] x 出力する optional_refstring
    ///
    /// \return 出力ストリーム
    ///
    /// \sa optional_refstring
    std::ostream& operator<< (std::ostream& s, const optional_refstring& x);

    /// \brief 変数・パラメタのコード出力用の基底のライタクラス
    ///
    /// var_defn_writer および var_rhs_writer がvirtualで継承し、
    /// var_writer が var_defn_writer および var_rhs_writer を継承する
    ///
    /// このクラスには出力用オペレータはない
    ///
    /// \sa var_defn_writer
    /// \sa var_rhs_writer
    /// \sa var_writer
    class var_basic_writer
    {
    public:
      /// \brief var_basic_writer のデフォルトコンストラクタ
      var_basic_writer () {}

      /// \brief 構造体名の設定
      ///
      /// \param[in] st 構造体名
      ///
      /// 変数(構造体メンバ)が含まれる構造体名の設定
      ///
      /// \attention
      /// \a st
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      void set_storage (const strict_string& st)
      {
	vbw_storage = st;
      }

      /// \brief 初期値コードの設定
      ///
      /// \param[in] iv 初期値コード
      ///
      /// 変数(構造体メンバ)の初期値コードの設定
      ///
      /// \attention
      /// \a iv
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      void set_initcode (const strict_string& iv)
      {
	vbw_initcode = iv;
      }

      /// \brief 変数の型を返す純粋仮想関数
      ///
      /// \return 変数の型
      virtual const std::string& type (void) const = 0;

      /// \brief 変数名を返す純粋仮想関数
      ///
      /// \return 変数名
      virtual const std::string& name (void) const = 0;

      /// \brief 配列サイズを返す純粋仮想関数
      ///
      /// \retval 0 配列ではない
      /// \retval 0以外 配列サイズ
      virtual size_t size (void) const = 0;

      /// \brief 構造体名を取得
      ///
      /// \return 構造体名の optional_refstring
      ///
      /// optional_refstring がfalseなら構造体名がない変数(exported global)
      const optional_refstring& storage (void) const { return vbw_storage; }

      /// \brief 初期値コードを取得
      ///
      /// \return 初期値コードの optional_refstring
      ///
      /// optional_refstring がfalseなら初期値コードがない。
      /// state,parameter, const parameter等は初期値コードをもつ
      const optional_refstring& initcode (void) const { return vbw_initcode; }

      /// \brief ポインタとして表示する際、'&' が必要かを判定する
      ///
      /// \retval true '&'が必要
      /// \retval true '&'が不要(配列等)
      virtual bool need_amp (void) const { return size () == 0; }

      /// \brief 変数または構造体名のプレフィクスの取得
      ///
      /// \return 変数または構造体名のプレフィクス
      virtual const std::string& prefix (void) const { return vbw_nullfix; }

      /// \brief 変数または構造体名のポストフィクスの取得
      ///
      /// \return 変数または構造体名のポストフィクス
      virtual const std::string& postfix (void) const { return vbw_nullfix; }

    protected:
      /// \brief 変数または構造体名の空のプレフィクス/ポストフィクス
      static const std::string vbw_nullfix;

    private:
      /// \brief 構造体名の optional_refstring
      optional_refstring vbw_storage;

      /// \brief 初期値コードの optional_refstring
      optional_refstring vbw_initcode;
    };

    /// \brief インデント表示をするためのラッパクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa operator<<(std::ostream& s, const indent_wrapper<W>& ww)
    template <class W>
    class indent_wrapper
    {
    public:
      /// \brief indent_wrapper のコンストラクタ
      ///
      /// \param[in] w 表示するもの
      /// \param[in] ind インデント数
      indent_wrapper(const W& w, size_t ind) : iw_w (w), iw_ind (ind) {}

      /// \brief 表示するものへのキャスト
      ///
      /// \return 表示するもの
      ///
      /// 表示するものを取得する
      operator const W& () const { return iw_w; }

      /// \brief インデント数の取得
      ///
      /// \return インデント数
      size_t indent(void) const { return iw_ind; }

    private:
      /// \brief 表示するもののリファレンス
      const W& iw_w;

      /// \brief インデント数
      size_t iw_ind;
    };

    /// \brief indent_wrapper を生成するテンプレート関数
    ///
    /// \param[in] w 表示するもの
    /// \param[in] ind インデント数
    ///
    /// \return 表示するものを包んだ indent_wrapper
    ///
    /// \sa indent_wrapper
    template <class W>
    indent_wrapper< W >
    make_indent_wrapper(const W& w, size_t ind)
    {
      return indent_wrapper< W > (w, ind);
    }

    /// \brief indent_wrapper を表示する出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] ww 表示する indent_wrapper で包まれたもの
    ///
    /// \return 出力ストリーム
    ///
    /// \sa indent_wrapper
    template <class W>
    std::ostream& operator<< (std::ostream& s, const indent_wrapper<W>& ww)
    {
      indent_writer::writer (s, ww.indent ());
      const W& w = ww;
      return s << w;
    }

    /// \brief staticを表示するためのラッパクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa operator<<(std::ostream& s, const static_wrapper<W> sw)
    template <class W>
    class static_wrapper
    {
    public:
      /// \brief static_wrapper のコンストラクタ
      ///
      /// \param[in] w 表示するもの
      static_wrapper (const W& w) : sw_w (w) {}

      /// \brief 表示するものへのキャスト
      ///
      /// \return 表示するもの
      ///
      /// 表示するものを取得する
      operator const W& () const { return sw_w; }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      ///
      /// \return static_wrapper を包んだ indent_wrapper
      const indent_wrapper< static_wrapper< W > >
      set_indent (size_t n) const
      {
	return indent_wrapper< static_wrapper< W > > (*this, n);
      }

    private:
      /// \brief 表示するもの
      const W& sw_w;
    };

    /// \brief static_wrapper の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] sw static_wrapper で包まれたもの
    ///
    /// \return 出力ストリーム
    ///
    /// \sa static_wrapper
    template <class W>
    std::ostream& operator<< (std::ostream& s, const static_wrapper<W> sw)
    {
      const W& w = sw;
      return s << "static " << w;
    }

    /// \brief 変数のポインタ(アドレス)出力用のラッパクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \note
    /// このラッパを試用できるクラスは need_amp() を実装する必要がある
    ///
    /// \sa var_basic_writer
    /// \sa var_rhs_wrapper
    /// \sa operator<<(std::ostream& s, const var_pointer_wrapper<W>& vpw)
    template <class W>
    class var_pointer_wrapper
    {
    public:
      /// \brief var_pointer_wrapper のコンストラクタ
      ///
      /// \param[in] w 表示するもの
      var_pointer_wrapper (const W& w) : vpw_w (w) {}

      /// \brief 表示するものへのキャスト
      ///
      /// \return 表示するもの
      ///
      /// 表示するものを取得する
      operator const W& () const { return vpw_w; }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      ///
      /// \return var_pointer_wrapper を包んだ indent_wrapper
      const indent_wrapper< var_pointer_wrapper< W > >
      set_indent (size_t n) const
      {
	return indent_wrapper < var_pointer_wrapper< W > > (*this, n);
      }

    private:
      /// \brief 表示するもの
      const W& vpw_w;
    };

    /// \brief 変数のポインタ(アドレス)の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] vpw var_pointer_wrapper で包まれたもの
    ///
    /// \return 出力ストリーム
    ///
    /// \sa var_pointer_wrapper
    template <class W>
    std::ostream&
    operator<< (std::ostream& s, const var_pointer_wrapper<W>& vpw)
    {
      const W& pw = vpw;

      if (pw.need_amp ())
	s << '&';
      return s << pw;
    }

    /// \brief 左辺または右辺に表示する、配列の要素を表示するためのラッパ
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// 表示は一次元表示なので二次元以上は var_rhs_wrapper をネストすれ
    /// ば実現できるが、事実上一次元だけで十分である
    ///
    /// \sa operator<<(std::ostream& s, const var_rhs_wrapper<W>& vrw)
    template <class W, class S>
    class var_rhs_wrapper
    {
    public:
      /// \brief var_rhs_wrapper のコンストラクタ
      ///
      /// \param[in] w 表示するもの
      /// \param[in] idx 配列のインデクス
      ///
      var_rhs_wrapper(const W& w, const S& idx) : vrw_w (w), vrw_idx (idx) {}

      /// \brief 表示するものへのキャスト
      ///
      /// \return 表示するもの
      ///
      /// 表示するものを取得する
      operator const W& () const { return vrw_w; }

      /// \brief 配列のインデクスを取得
      ///
      /// \return 配列のインデクス
      const S& index(void) const { return vrw_idx; }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      ///
      /// \return var_rhs_wrapper を包んだ indent_wrapper
      const indent_wrapper< var_rhs_wrapper< W, S > >
      set_indent (size_t n) const
      {
	return indent_wrapper < var_rhs_wrapper< W, S > > (*this, n);
      }

      /// \brief ポインタ形式にする
      ///
      /// \return var_rhs_wrapper を持った var_pointer_wrapper
      const var_pointer_wrapper< var_rhs_wrapper< W, S > >
      pointer (void) const
      {
	return var_pointer_wrapper< var_rhs_wrapper< W, S > >(*this);
      }

      /// \brief ポインタとして表示する際、'&' が必要かを判定する
      ///
      /// \retval true '&'が必要
      /// \retval true '&'が不要(配列等)
      bool need_amp (void) const { return true; }

    private:
      /// \brief 表示するもの
      const W& vrw_w;

      /// \brief 配列のインデクス
      const S& vrw_idx;
    };

    /// \brief 配列の要素の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] vrw var_rhs_wrapper で包まれたもの
    ///
    /// \return 出力ストリーム
    ///
    /// \sa var_rhs_wrapper
    template <class W, class S>
    std::ostream&
    operator<< (std::ostream& s, const var_rhs_wrapper<W, S>& vrw)
    {
      const W& w = vrw;
      const S& idx = vrw.index ();

      s << w;
      size_t a = w.size();
      if (a)
	s << '[' << idx << ']';
      return s;
    }

    /// \brief 変数宣言(変数定義)のライタクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// var_basic_writer を継承する
    ///
    /// \sa var_basic_writer
    /// \sa operator<<(std::ostream& s, const var_defn_writer& vdw)
    class var_defn_writer
      : public virtual var_basic_writer
    {
    public:
      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      const indent_wrapper<var_defn_writer>
      set_indent(size_t n) const
      {
	return indent_wrapper<var_defn_writer>(*this, n);
      }
    };

    /// \brief 変数宣言用出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] vdw var_defn_writer
    ///
    /// \return 出力ストリーム
    ///
    /// \sa var_defn_writer
    std::ostream& operator<< (std::ostream& s, const var_defn_writer& vdw);

    /// \brief 配列のインデクスを設定するテンプレート関数
    ///
    /// \param[in] w var_rhs_writer 等のライタ
    /// \param[in] n インデクス
    ///
    /// \return var_rhs_wrapper のインスタンス
    template <class W, class S>
    static inline var_rhs_wrapper< W, S >
    make_rhs_wrapper (const W& w, const S& n)
    {
      return var_rhs_wrapper< W, S >(w, n);
    }

    /// \brief RHS(またはLHS)のライタクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// var_basic_writerを継承する
    ///
    /// 配列の要素の出力は var_rhs_wrapperを使用する\n
    /// 以下の形式で使用可
    ///  - rhs.set_index().set_indent()
    ///  - rhs.set_index()
    ///  - rhs
    ///
    /// ポインタ化はvar_pointer_wrapperを使用する\n
    /// 以下の形式で使用可
    /// - rhs.set_index().pointer().set_indent()
    /// - rhs.set_index().pointer()
    /// - rhs.pointer().set_index()
    /// - rhs.pointer()
    ///
    /// \sa var_basic_writer
    /// \sa operator<<(std::ostream& s, const var_rhs_writer& vrw)
    class var_rhs_writer
      : public virtual var_basic_writer
    {
    public:
      /// \brief 配列のインデクスを設定する
      ///
      /// \param[in] n 配列のインデクス(数値)
      ///
      /// \return var_rhs_writer を包んだ var_rhs_wrapper
      const var_rhs_wrapper< var_rhs_writer, size_t >
      set_index (size_t n) const
      {
	return make_rhs_wrapper (*this, n);
      }

      /// \brief 配列のインデクスを設定する
      ///
      /// \param[in] s 配列のインデクス(文字列)
      ///
      /// \return var_rhs_writer を包んだ var_rhs_wrapper
      ///
      /// \attention
      /// \a s
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      const var_rhs_wrapper< var_rhs_writer, std::string >
      set_index (const strict_string& s) const
      {
	return make_rhs_wrapper (*this, static_cast< const std::string& > (s));
      }

      /// \brief ポインタ形式にする
      ///
      /// \return var_rhs_writer を持った var_pointer_wrapper
      const var_pointer_wrapper < var_rhs_writer >
      pointer (void) const
      {
	return var_pointer_wrapper < var_rhs_writer > (*this);
      }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      ///
      /// \return var_rhs_writer を包んだ indent_wrapper
      const indent_wrapper<var_rhs_writer>
      set_indent(size_t n) const
      {
	return indent_wrapper<var_rhs_writer>(*this, n);
      }
    };

    /// \brief RHS用出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] vrw var_rhs_writer
    ///
    /// \return 出力ストリーム
    ///
    /// \sa var_rhs_writer
    std::ostream& operator<< (std::ostream& s, const var_rhs_writer& vrw);

    /// \brief 変数用ライタクラス
    ///
    /// var_defn_writer と var_rhs_writer を継承する
    ///
    /// \sa var_defn_writer
    /// \sa var_rhs_writer
    /// \sa var_basic_writer
    class var_writer
      : public var_defn_writer,
	public var_rhs_writer
    {
    public:
      // 目的のライタを取得

      /// \brief var_defn_writer を取得
      ///
      /// \return var_defn_writer
      const var_defn_writer& defn_writer (void) const { return *this; }

      /// \brief var_rhs_writer を取得
      ///
      /// \return var_rhs_writer
      const var_rhs_writer& rhs_writer (void) const { return *this; }
    };


    /// \brief ダブルバッファ用変数宣言(変数定義)のライタクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa var_defn_writer
    /// \sa operator<<(std::ostream& s, const var_doublebuffer_writer < V, P >& vdbw)
    /// \sa make_doublebuffer()
    template < class V, class P >
    class var_doublebuffer_writer
    {
    public:
      /// \brief var_doublebuffer_writer のコンストラクタ
      ///
      /// \param[in] vw var_defn_writer や var_rhs_writer の参照
      /// \param[in] plane 面数を示すもの(数値、文字列等)
      var_doublebuffer_writer (const V& vw, P plane)
	: vdbw_vw (vw), vdbw_planes (plane)
      {
      }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      const indent_wrapper<var_doublebuffer_writer>
      set_indent (size_t n) const
      {
	return indent_wrapper<var_doublebuffer_writer>(*this, n);
      }

      /// \brief バッファの面数を設定する
      ///
      /// \param[in] n 面数を示すもの
      ///
      /// \return このインスタンスの参照
      var_doublebuffer_writer&
      set_planes (P n)
      {
	vdbw_planes = n;
	return *this;
      }

      /// \brief 配列のインデクスを設定する
      ///
      /// \param[in] n 配列のインデクス
      ///
      /// \return var_rhs_writer を包んだ var_rhs_wrapper
      const var_rhs_wrapper< var_doublebuffer_writer, size_t >
      set_index (size_t n) const
      {
	return make_rhs_wrapper (*this, n);
      }

      /// \brief 配列のインデクスを設定する
      ///
      /// \param[in] n 配列のインデクス
      ///
      /// \return var_rhs_writer を包んだ var_rhs_wrapper
      ///
      /// \attention
      /// \a n
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      const var_rhs_wrapper< var_doublebuffer_writer, std::string >
      set_index (const strict_string& n) const
      {
	return make_rhs_wrapper (*this, static_cast< const std::string& > (n));
      }

      /// \brief var_defn_writer や var_rhs_writer への変換オペレータ
      ///
      /// \return var_defn_writerの参照
      operator const V& () const { return vdbw_vw; }

      /// \brief 面数の取得
      ///
      /// \return 面数を示すもの
      const P& planes (void) const { return vdbw_planes; }

      /// \brief 配列サイズを返す
      ///
      /// \retval 0 配列ではない
      /// \retval 0以外 配列サイズ
      size_t size (void) const { return vdbw_vw.size (); }

    private:
      /// \brief var_defn_writer や var_rhs_writer の参照
      const V& vdbw_vw;

      /// \brief 面数を示すもの(数値や変数名等)
      P vdbw_planes;
    };

    /// \brief var_defn_writer用ダブルバッファ変数の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] vdbw var_doublebuffer_writer
    ///
    /// \return 出力ストリーム
    ///
    /// \sa var_doublebuffer_writer
    /// \sa var_defn_writer
    template < class P >
    std::ostream& operator<< (std::ostream& s,
			      const var_doublebuffer_writer <
			      var_defn_writer, P >& vdbw)
    {
      const var_defn_writer& vdw = vdbw;
      s << vdw.type () << ' '
	<< vdw.prefix () << vdw.name () << vdw.postfix ()
	<< '[' << vdbw.planes () << ']';
      size_t a = vdw.size ();
      if (a)
	s << '[' << a << ']';
      return s;
    }

    /// \brief var_defn_writer用ダブルバッファ変数の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] vdbw var_doublebuffer_writer
    ///
    /// \return 出力ストリーム
    ///
    /// \sa var_doublebuffer_writer
    /// \sa var_defn_writer
    template < class P >
    std::ostream& operator<< (std::ostream& s,
			      const var_doublebuffer_writer <
			      var_rhs_writer, P >& vdbw)
    {
      const var_rhs_writer& vrw = vdbw;
      s << vrw << '[' << vdbw.planes () << ']';
      return s;
    }

    /// \brief ダブルバッファ用ライタクラスを作成するテンプレート関数
    ///
    /// \param[in] vw var_defn_writer や var_rhs_writer
    /// \param[in] p 面を示すもの(数値や変数名等)
    ///
    /// \return var_doublebuffer_writer のインスタンス
    ///
    /// \sa var_doublebuffer_writer
    template < class V, class P>
    static inline var_doublebuffer_writer < V, P >
    make_doublebuffer (const V& vw, P p)
    {
      return var_doublebuffer_writer< V, P > (vw, p);
    }

    /// \brief 変数または構造体名のラッパクラス
    template < class W >
    class var_name_wrapper
      : public W
    {
    public:
      /// \brief var_name_wrapper のコンストラクタ
      ///
      /// \param[in] w var_basic_writer を継承しているライタ
      /// \param[in] prefix 変数または構造体名のプレフィクス
      /// \param[in] postfix 変数または構造体名のポストフィクス
      ///
      /// \attention
      /// \a prefix および \a postfix
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      var_name_wrapper (const W& w, const strict_string& prefix,
			const strict_string& postfix)
	: W (w), vnw_prefix (prefix), vnw_postfix (postfix)
      {
      }

      /// \brief var_name_wrapper のコンストラクタ
      ///
      /// \param[in] w var_basic_writer を継承しているライタ
      /// \param[in] prefix 変数または構造体名のプレフィクス
      ///
      /// \attention
      /// \a prefix
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      var_name_wrapper (const W& w, const strict_string& prefix)
	: W (w), vnw_prefix (prefix), vnw_postfix (w.postfix ())
      {
      }

      /// \brief var_name_wrapper のコンストラクタ
      ///
      /// 第二引数に数値を与えることで、postfixを省略する
      ///
      /// \param[in] w var_basic_writer を継承しているライタ
      /// \param[in] postfix 変数または構造体名のポストフィクス
      ///
      /// \attention
      /// \a postfix
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      var_name_wrapper (const W& w, int, const strict_string& postfix)
	: W (w), vnw_prefix (w.prefix ()), vnw_postfix (postfix)
      {
      }

      /// \brief 変数または構造体のプレフィクスの取得
      ///
      /// \return 変数または構造体のプレフィクスの取得
      const std::string& prefix (void) const { return vnw_prefix; }

      /// \brief 変数または構造体のポストフィクスの取得
      ///
      /// \return 変数または構造体のポストフィクスの取得
      const std::string& postfix (void) const { return vnw_postfix; }

    private:
      /// \brief 変数または構造体名のプレフィクス
      const std::string& vnw_prefix;

      /// \brief 変数または構造体名のポストフィクス
      const std::string& vnw_postfix;
    };

    class AutoStorage;

    /// \brief AutoStorageの出力時のインデント等を設定するラッパクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa operator<<(std::ostream& s, const AutoStorage_wrapper& asw)
    class AutoStorage_wrapper
    {
    public:
      /// \brief AutoStorage_wrapper のコンストラクタ
      ///
      /// \param[in] a AutoStorageのポインタ
      /// \param[in] prefix 構造体名またはメンバの前に出力するプレフィクス
      /// \param[in] data_attr データ定義するものに対して出力するコンパイラ属性
      /// \param[in] ind インデント数
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// 表示の際は pass_filter が優先される
      ///
      /// \attention
      /// \a prefix および \a data_attr
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      AutoStorage_wrapper(const AutoStorage* a,
			  const strict_string& prefix,
			  const strict_string& data_attr,
			  size_t ind,
			  var_mode_T block_filter = 0,
			  var_mode_T pass_filter = 0);

      /// \brief 構造体名を指定した AutoStorage_wrapper のコンストラクタ
      ///
      /// \param[in] a AutoStorageのポインタ
      /// \param[in] prefix 構造体名またはメンバの前に出力するプレフィクス
      /// \param[in] storage 出力する構造体名
      /// \param[in] data_attr データ定義するものに対して出力するコンパイラ属性
      /// \param[in] ind インデント数
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// 構造体名を変更して出力するために使用する
      /// \attention
      /// \a prefix, \a storage および \a data_attr
      /// はこのオブジェクトが破棄されるまで存在しなければならない
      AutoStorage_wrapper(const AutoStorage* a,
			  const strict_string& prefix,
			  const strict_string& storage,
			  const strict_string& data_attr,
			  size_t ind,
			  var_mode_T block_filter = 0,
			  var_mode_T pass_filter = 0);

      /// \brief AutoStoreageのポインタを取得する参照オペレータ
      ///
      /// \return AutoStorageのポインタ
      const AutoStorage* operator-> () const { return asw_as; }

      /// \brief AutoStorageを取得する参照オペレータ
      ///
      /// \return AutoStorage
      const AutoStorage& operator* () const { return *asw_as; }

      /// \brief 構造体名を取得
      ///
      /// \return 構造体名
      const std::string& storage (void) const { return asw_storage; }

      /// \brief 構造体またはメンバ名に付けるプレフィクスを取得
      ///
      /// \return 構造体またはメンバ名に付けるプレフィクス
      const std::string& prefix (void) const { return asw_prefix; }

      ///
      /// \brief 構造体または変数に対するデータ属性
      const std::string& data_attr (void) const { return asw_data_attr; }

      /// \brief インデント数の取得
      ///
      /// \return インデント数
      size_t indent (void) const { return asw_indent; }

      /// \brief block_filterを取得
      ///
      /// \return 表示をしない(ブロックする)変数属性(var_mode_T)
      ///
      /// block_filter よりも pass_filter が優先される
      var_mode_T block_filter (void) const { return asw_block_filter; }

      /// \brief pass_filterを取得
      ///
      /// \return 表示をする変数属性(var_mode_T)
      ///
      /// block_filter よりも pass_filter が優先される
      var_mode_T pass_filter (void) const { return asw_pass_filter; }

    private:
      /// \brief AutoStorageのポインタ
      const AutoStorage* asw_as;

      /// \brief 構造体名またはメンバの前に出力するプレフィクス
      const std::string& asw_prefix;

      /// \brief 出力する構造体名
      const std::string& asw_storage;

      /// \brief データ属性
      const std::string& asw_data_attr;

      /// \brief インデント数
      size_t asw_indent;

      /// \brief 表示しない変数属性
      var_mode_T asw_block_filter;

      /// \brief 表示する変数属性
      var_mode_T asw_pass_filter;
    };

    /// \brief AutoStorage_wrapper 出力用オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] asw AutoStorage_wrapper
    ///
    /// \return 出力ストリーム
    ///
    /// \sa AutoStorage_wrapper
    std::ostream&
    operator<< (std::ostream& s, const AutoStorage_wrapper& asw);

    /// \brief AutoStorageクラス
    ///
    /// Embedded Coderが生成する、ブロックシグナル、パラメタ、状態変数
    /// 等を構造体にまとめるためライタの集合(実体は var_writer のポイン
    /// タの配列)
    ///
    /// コード生成では、これを用いてEmbedded Coderが生成したCコードの変
    /// 数やパラメタを出力する
    class AutoStorage
      : public std::vector < var_writer* >
    {
      /// \brief 名前が同じvar_writerを見つけるための叙述関数
      class var_writer_same_name
      {
      public:
	/// \brief var_writer_same_name のコンストラクタ
	///
	/// \param[in] vw var_writer
	var_writer_same_name (const var_writer* vw)
	  : sn_name (vw->name ()) {}

	/// \brief 同じ名前の var_writer を見つけるオペレータ
	///
	/// \param[in] vw 比較する var_writer
	///
	/// \retval true 同じ名前
	/// \retval false 異なる名前
	bool operator() (const var_writer* vw) const
	{
	  return sn_name == vw->name ();
	}

      private:
	/// \brief 比較する名前
	const std::string& sn_name;
      };

    public:
      /// \brief AutoStorage のコンストラクタ
      ///
      /// \param[in] s 構造体名
      /// \param[in] c const かどうか
      ///
      /// \attention
      /// \a s の構造体名は
      /// このオブジェクトが破棄されるまで存在しなければならない
      AutoStorage (const strict_string& s, bool c = false)
	: as_storage (s), as_has_initcode (false), as_is_const (c)
      {}

      /// \brief 構造体名を取得
      ///
      /// \return 構造体名
      const std::string& storage (void) const { return as_storage; }

      /// \brief メンバを追加する
      ///
      /// \param[in] p 追加するメンバの var_writer
      void add_member (var_writer* p)
      {
	if (find_by_name (p) == end ())
	  {
	    push_back (p);

	    const optional_refstring& io = p->initcode ();
	    if (io && io->size ())
	      as_has_initcode = true;
	  }
      }

      /// \brief 初期値を持つものかどうか
      ///
      /// \retval true 初期値をもつ(ParameterやConst Parameter等)
      /// \retval false 初期値を持たない(stateやexpoted global等)
      bool has_initcode (void) const { return as_has_initcode; }

      /// \brief constかどうかを返す
      ///
      /// \retval true 構造体は const である
      /// \retval false 構造体は const ではない
      bool is_const (void) const { return as_is_const; }

      /// \brief 同じ名前の var_writer を探し、反復子を返す
      ///
      /// \param[in] vw 探す名前を持った var_writer
      ///
      /// \retval end() 見つからなかった
      /// \retval それ以外 見つけた
      const_iterator find_by_name (const var_writer* vw) const
      {
	return find_if (begin (), end (), var_writer_same_name (vw));
      }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// \return AutoStorage_wrapper
      const AutoStorage_wrapper
      set_indent (size_t n,
		  var_mode_T block_filter = 0,
		  var_mode_T pass_filter = 0) const
      {
	return AutoStorage_wrapper(this, as_null_prefix, as_null_prefix, n,
				   block_filter, pass_filter);
      }

      /// \brief インデントと共にデータ属性を設定する
      ///
      /// \param[in] n インデント数
      /// \param[in] data_attr データ定義するものに対して出力するコンパイラ属性
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// \return AutoStorage_wrapper
      ///
      /// \attention
      /// \a data_attr
      /// はこのインスタンスが破棄されるまでなくなってはならない。
      const AutoStorage_wrapper
      set_indent (size_t n,
		  const strict_string& data_attr,
		  var_mode_T block_filter = 0,
		  var_mode_T pass_filter = 0) const
      {
	return AutoStorage_wrapper(this, as_null_prefix, data_attr, n,
				   block_filter, pass_filter);
      }

      /// \brief インデントとともにプレフィクスを設定する
      ///
      /// \param[in] prefix 構造体名またはメンバの前に出力するプレフィクス
      /// \param[in] n インデント数
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// \return AutoStorage_wrapper
      ///
      /// \attention
      /// \a prefix
      /// はこのオブジェクトが破棄されるまで存在しなければならない
      const AutoStorage_wrapper
      set_indent (const strict_string& prefix, size_t n,
		  var_mode_T block_filter = 0,
		  var_mode_T pass_filter = 0) const
      {
	return AutoStorage_wrapper(this, prefix, as_null_prefix, n,
				   block_filter, pass_filter);
      }

      /// \brief インデント、プレフィクスとともにデータ属性を設定する
      ///
      /// \param[in] prefix 構造体名またはメンバの前に出力するプレフィクス
      /// \param[in] n インデント数
      /// \param[in] data_attr データ定義するものに対して出力するコンパイラ属性
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// \return AutoStorage_wrapper
      ///
      /// \attention
      /// \a prefix および \a data_attr
      /// はこのオブジェクトが破棄されるまで存在しなければならない
      const AutoStorage_wrapper
      set_indent (const strict_string& prefix, size_t n,
		  const strict_string& data_attr,
		  var_mode_T block_filter = 0,
		  var_mode_T pass_filter = 0) const
      {
	return AutoStorage_wrapper(this, prefix, data_attr, n,
				   block_filter, pass_filter);
      }

      /// \brief インデント、プレフィクスとともに構造体名を設定する
      ///
      /// \param[in] prefix 構造体名またはメンバの前に出力するプレフィクス
      /// \param[in] storage 出力する構造体名
      /// \param[in] n インデント数
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// \return AutoStorage_wrapper
      ///
      /// \attention
      /// \a prefix と \a storage
      /// はこのオブジェクトが破棄されるまで存在しなければならない
      const AutoStorage_wrapper
      set_indent (const strict_string& prefix, const strict_string& storage,
		  size_t n,
		  var_mode_T block_filter = 0,
		  var_mode_T pass_filter = 0) const
      {
	return AutoStorage_wrapper(this, prefix, storage, as_null_prefix, n,
				   block_filter, pass_filter);
      }

      /// \brief インデント、プレフィクス、構造体名と共にデータ属性を設定する
      ///
      /// \param[in] prefix 構造体名またはメンバの前に出力するプレフィクス
      /// \param[in] storage 出力する構造体名
      /// \param[in] n インデント数
      /// \param[in] data_attr データ定義するものに対して出力するコンパイラ属性
      /// \param[in] block_filter 表示しないvar_mode_Tの属性
      /// \param[in] pass_filter 表示するvar_mode_Tの属性
      ///
      /// \return AutoStorage_wrapper
      ///
      /// \attention
      /// \a prefix と \a storage および \a data_attr
      /// はこのオブジェクトが破棄されるまで存在しなければならない
      const AutoStorage_wrapper
      set_indent (const strict_string& prefix, const strict_string& storage,
		  size_t n, const strict_string& data_attr,
		  var_mode_T block_filter = 0,
		  var_mode_T pass_filter = 0) const
      {
	return AutoStorage_wrapper(this, prefix, storage, data_attr, n,
				   block_filter, pass_filter);
      }

      /// \brief export globalの変数のための構造体名の静的メンバ。
      /// 実際は空文字列
      static const std::string export_global_storage;

    private:
      /// \brief 空文字列の静的メンバ
      static const std::string as_null_prefix;

      /// \brief 構造体名
      const std::string& as_storage;

      /// \brief 初期値コードをもつかどうか
      bool as_has_initcode;

      /// \brief const かどうか
      bool as_is_const;
    };

    /// \brief 構造体名をキーとした AutoStorage の連想配列
    class AutoStorageMap
      : public std::map < std::string, AutoStorage >
    {
    public:
      /// \brief 構造体名を指定した AutoStorage を作成
      ///
      /// \param[in] s 構造体名
      /// \param[in] c const かどうか
      ///
      /// \return AutoStorage
      AutoStorage&
      add_storage (const std::string& s, bool c = false)
      {
	iterator i = find (s);
	if (i != end ())
	  return i->second;
	std::pair< iterator,  bool>
	  x = insert (std::make_pair (s, AutoStorage (s, c)));
	return x.first->second;
      }

      /// \brief 指定した構造体名(string)の AutoStorage のポインタを取
      /// 得(設定用)
      ///
      /// \param[in] s 構造体名(string)
      ///
      /// \retval NULL 存在しない
      /// \retval NULL以外 AutoStorage のポインタ
      AutoStorage*
      get_storage (const std::string& s)
      {
	iterator i = find (s);
	if (i != end ())
	  return &i->second;
	return NULL;
      }

      /// \brief 指定した構造体名(string)の AutoStorage のポインタを取
      /// 得(参照用)
      ///
      /// \param[in] s 構造体名(string)
      ///
      /// \retval NULL 存在しない
      /// \retval NULL以外 AutoStorage のポインタ
      const AutoStorage*
      get_storage (const std::string& s) const
      {
	const_iterator i = find (s);
	if (i != end ())
	  return &i->second;
	return NULL;
      }

      /// \brief 指定した構造体名(optional_refstring)の AutoStorage の
      /// ポインタを取得(設定用)
      ///
      /// \param[in] r 構造体名(optional_refstring)
      ///
      /// \retval NULL 存在しない
      /// \retval NULL以外 AutoStorage のポインタ
      AutoStorage*
      get_storage (const optional_refstring& r)
      {
	if (!r)
	  return NULL;
	return get_storage (*r);
      }

      /// \brief 指定した構造体名(optional_refstring)の AutoStorage の
      /// ポインタを取得(参照用)
      ///
      /// \param[in] r 構造体名(optional_refstring)
      ///
      /// \retval NULL 存在しない
      /// \retval NULL以外 AutoStorage のポインタ
      const AutoStorage*
      get_storage (const optional_refstring& r) const
      {
	if (!r)
	  return NULL;
	return get_storage (*r);
      }
    };

    //
    // 関数
    //

    /// \brief 文字列中の空白とCのコメントを読み飛ばす関数
    ///
    /// \param[in] s 文字列
    ///
    /// \return 読み飛ばし後の文字列
    ///
    /// 文字列中のCのコメント、空白文字を読み飛ばした後のアドレスを返す
    const char* skip_space_comment (const char* s);

    /// \brief 関数からstaticを取るためのラッパクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa function_defn_writer
    /// \sa function_decl_writer
    /// \sa operator<<(std::ostream& s,
    ///     const function_discard_static_wrapper< W >& fdsw)
    template <class W>
    class function_discard_static_wrapper
    {
    public:
      /// \brief function_discard_static_wrapper のコンストラクタ
      ///
      /// \param[in] w 表示するもの
      function_discard_static_wrapper (const W& w) : fdsw_w (w) {}

      /// \brief 表示するものへのキャスト
      ///
      /// \return 表示するもの
      ///
      /// 表示するものを取得する
      operator const W& () const { return fdsw_w; }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      ///
      /// \return 表示するものを包んだ indent_wrapper
      const indent_wrapper< function_discard_static_wrapper< W > >
      set_indent (size_t n) const
      {
	return indent_wrapper< function_discard_static_wrapper< W >
			       > (*this, n);
      }

    private:
      /// \brief 表示するもの
      const W& fdsw_w;
    };

    /// \brief staticを削除する関数用出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] fdsw 表示するものを包んだ function_discard_static_wrapper
    ///
    /// \return 出力ストリーム
    ///
    /// \sa function_discard_static_wrapper
    template <class W>
    std::ostream&
    operator<< (std::ostream& s,
		const function_discard_static_wrapper< W >& fdsw)
    {
      const W& fw = fdsw;
      const SimulinkModel::XSD::function_T::code_optional& c = fw.code ();
      if (c)
	{
	  const char* code = (*c).c_str ();
	  const char* n = skip_space_comment (code);
	  if (strncmp ("static", n, 6) == 0
	      && (isspace (((unsigned char*)n)[6])))
	    {
	      // "static"を持つ場合は、削除
	      if (n != code)
		s << std::string (code, n - code);
	      n += 7;		// static + space
	      s << n;
	    }
	  else
	    s << *c;
	}
      return s;
    }

    /// \brief 関数にstaticをつけるためのラッパクラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// \sa operator<< (std::ostream& s,
    ///     const function_static_wrapper< W >& fsw)
    template <class W>
    class function_static_wrapper
    {
    public:
      /// \brief function_static_wrapper のコンストラクタ
      ///
      /// \param[in] w 表示するもの
      function_static_wrapper (const W& w) : fsw_w (w) {}

      /// \brief 表示するものへのキャスト
      ///
      /// \return 表示するもの
      ///
      /// 表示するものを取得する
      operator const W& () const { return fsw_w; }

      /// \brief インデントを設定する
      ///
      /// \param[in] n インデント数
      ///
      /// \return function_static_wrapper を包んだ indent_wrapper
      const indent_wrapper< function_static_wrapper< W > >
      set_indent (size_t n) const
      {
	return indent_wrapper< function_static_wrapper< W > > (*this, n);
      }

    private:
      /// \brief 表示するもの
      const W& fsw_w;
    };

    /// \brief staticを付与する関数用出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] fsw 表示するものを包んだ function_static_wrapper
    ///
    /// \return 出力ストリーム
    ///
    /// \sa function_static_wrapper
    template <class W>
    std::ostream&
    operator<< (std::ostream& s,
		const function_static_wrapper< W >& fsw)
    {
      const W& fw = fsw;
      SimulinkModel::XSD::function_T::code_optional& c = fw.code ();
      if (c)
	{
	  const char* code = (*c).c_str ();
	  const char* n = skip_space_comment (code);
	  if (strncmp ("static", n, 6) != 0
	      || !isspace (((unsigned char*)n)[6]))
	    {
	      // "static"を持たない場合は"static"を付与
	      if (code != n)
		s << std::string (code, n - code);
	      s << "static ";
	      s << n;
	    }
	  else
	    s << *c;
	}
      return s;
    }

    /// \brief 関数定義/宣言のライタの基底クラス
    ///
    /// 出力ストリームに対し "<<" オペレータで出力する
    ///
    /// function_defn_writer と function_decl_writer から継承され、表示
    /// の際にこのクラスを使用する。
    ///
    /// var_basic_writer とは異なり、このクラスは function_defn_writer
    /// および function_decl_writer では共有されず、別のものになる
    ///
    /// \sa function_defn_writer
    /// \sa function_decl_writer
    /// \sa operator<< (std::ostream& s, const function_basic_writer& f)
    class function_basic_writer
    {
    public:
      /// \brief BLXMLのfunctionタグを取得する仮想関数
      ///
      /// \return BLXMLのfunctionタグのポインタ
      ///
      /// function_defn_writer と function_decl_writer では、使用する
      /// BLXMLのfunctionタグが異なるため、この仮想関数を経由して
      /// functionタグにアクセスする
      virtual const SimulinkModel::XSD::function_T*
      get_function (void) const = 0;

      /// \brief 関数の型を取得
      ///
      /// \return 関数の型
      const std::string& type (void) const { return get_function ()->type (); }

      /// \brief 関数名を取得
      ///
      /// \return 関数名
      const std::string& name (void) const { return get_function ()->name (); }

      /// \brief BLXMLのfunctionタグのcodeを取得する
      ///
      /// \return BLXMLのfunctionタグのcode
      const SimulinkModel::XSD::function_T::code_optional&
      code (void) const { return get_function ()->code (); }

      /// \brief 真理値へのキャスト
      ///
      /// \retval true 有効なBLXMLのfunctionタグをもつ
      /// \retval false BLXMLのfunctionタグを持たない
      ///
      /// このオブジェクトが有効か無効かの判定に用いる
      operator bool() const { return get_function () != NULL; }
    };

    /// \brief 関数定義/宣言用出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] f function_basic_writer
    ///
    /// \return 出力ストリーム
    ///
    /// \sa function_basic_writer
    std::ostream& operator<< (std::ostream& s, const function_basic_writer& f);

    /// \brief 関数定義用ライタクラス
    ///
    /// function_basic_writer を継承し function_writer から継承される
    ///
    /// \sa function_basic_writer
    /// \sa function_writer
    class function_defn_writer
      : public function_basic_writer
    {
    public:
      /// \brief function_defn_writer のデフォルトコンストラクタ
      function_defn_writer () : fdefn (NULL) {}

      /// \brief function_defn_writer のコンストラクタ
      ///
      /// \param[in] defn BLXMLのfunctionタグ(defn)のポインタ
      function_defn_writer (const SimulinkModel::XSD::function_T* defn)
	: fdefn (defn)
      {
      }

      /// \brief BLXMLのfunctionタグのポインタ(defn)を取得する
      ///
      /// \return BLXMLのfunctionタグのポインタ(defn)
      const SimulinkModel::XSD::function_T*
      get_defn (void) const
      {
	return fdefn;
      }

      /// \brief BLXMLのfunctionタグ(defn)を取得する関数
      ///
      /// \return BLXMLのfunctionタグ(defn)のポインタ
      ///
      /// function_basic_writer の virtual を上書きする
      const SimulinkModel::XSD::function_T*
      get_function (void) const
      {
	return fdefn;
      }

      /// \brief BLXMLのfunctionタグ(defn)のポインタを設定する
      ///
      /// \param[in] f BLXMLのfunctionタグ(defn)のポインタ
      void set_defn (const SimulinkModel::XSD::function_T* f) { fdefn = f; }

    private:
      /// \brief BLXMLのfunctionタグ(defn)のポインタ
      const SimulinkModel::XSD::function_T* fdefn;
    };

    /// \brief 関数宣言用ライタクラス
    ///
    /// function_basic_writer を継承し function_writer から継承される
    ///
    /// \sa function_basic_writer
    /// \sa function_writer
    class function_decl_writer
      : public function_basic_writer // not virtual
    {
    public:
      /// \brief function_decl_writer のデフォルトコンストラクタ
      function_decl_writer () : fdecl (NULL) {}

      /// \brief function_decl_writer のコンストラクタ
      ///
      /// \param[in] decl BLXMLのfunctionタグ(decl)のポインタ
      function_decl_writer (const SimulinkModel::XSD::function_T* decl)
	: fdecl (decl)
      {
      }

      /// \brief BLXMLのfunctionタグのポインタ(decl)を取得する
      ///
      /// \return BLXMLのfunctionタグのポインタ(decl)
      const SimulinkModel::XSD::function_T*
      get_decl (void) const
      {
	return fdecl;
      }

      /// \brief BLXMLのfunctionタグ(decl)を取得する関数
      ///
      /// \return BLXMLのfunctionタグ(decl)のポインタ
      ///
      /// function_basic_writer の virtual を上書きする
      const SimulinkModel::XSD::function_T*
      get_function (void) const
      {
	return fdecl;
      }

      /// \brief BLXMLのfunctionタグ(decl)のポインタを設定する
      ///
      /// \param[in] f BLXMLのfunctionタグ(decl)のポインタ
      void set_decl (const SimulinkModel::XSD::function_T* f) { fdecl = f; }

    private:
      /// \brief BLXMLのfunctionタグ(decl)のポインタ
      const SimulinkModel::XSD::function_T* fdecl;
    };

    /// \brief 関数のライタ全体
    ///
    /// function_ent_T から継承される
    ///
    /// function_defn_writer と function_decl_writer を継承するが
    /// function_basic_writer はそれぞれ別のものを持つ
    ///
    /// \sa function_defn_writer
    /// \sa function_decl_writer
    /// \sa function_basic_writer
    /// \sa function_ent_T
    class function_writer
      : public function_defn_writer,
	public function_decl_writer
    {
    public:
      /// \brief function_writer のコンストラクタ
      ///
      /// \param[in] defn BLXMLのfunctionタグ(defn)のポインタ
      /// \param[in] decl BLXMLのfunctionタグ(decl)のポインタ
      ///
      /// 実際にはどちらか一方のポインタが指定されて呼ばれ、一方は後か
      /// ら設定する
      function_writer (const SimulinkModel::XSD::function_T* defn = NULL,
		       const SimulinkModel::XSD::function_T* decl = NULL)
	: function_defn_writer (defn), function_decl_writer (decl)
      {
      }

      /// \brief function_defn_writer を取得する
      ///
      /// \return function_defn_writer
      const function_defn_writer& defn_writer (void) const { return *this; }

      /// \brief function_decl_writer を取得する
      ///
      /// \return function_decl_writer
      const function_decl_writer& decl_writer (void) const { return *this; }

      /// \brief function_defn_writer の function_basic_writer を取得する
      ///
      /// \return function_defn_writer の function_basic_writer
      const function_basic_writer& defn_basic_writer (void) const
      { return defn_writer (); }

      /// \brief function_decl_writer の function_basic_writer を取得する
      ///
      /// \return function_decl_writer の function_basic_writer
      const function_basic_writer& decl_basic_writer (void) const
      { return decl_writer (); }
    };

    /// \brief 変数やパラメタの入れ物のクラス
    class var_param_ptr_ent
    {
    public:
      /// \brief 入っているものの種別
      ///
      /// - VAR は CspVar* を保持
      /// - PARAM は param_ent_T* を保持
      enum ent_type { VAR, PARAM };

      /// \brief 変数を指定したコンストラクタ
      ///
      /// \param[in] v 変数(CspVar*)
      var_param_ptr_ent (const CspVar* v) : type (VAR) { var = v; }

      /// \brief パラメタを指定したコンストラクタ
      ///
      /// \param[in] v パラメタ(param_ent_T*)
      var_param_ptr_ent (const param_ent_T* v) : type (PARAM) { param = v; }

      /// \brief 入っているものの種類を取得
      ent_type get_type (void) const { return type; }

      /// \brief CspVarのポインタへのキャスト
      ///
      /// \return CspVarのポインタ
      operator const CspVar* () const { return var; }

      /// \brief param_ent_Tのポインタへのキャスト
      ///
      /// \return param_ent_Tのポインタ
      operator const param_ent_T* () const { return param; }

    private:
      /// \brief 入っているものの種類
      ent_type type;

      /// \brief ポインタを保持するunion
      union {
	/// \brief CspVarのポインタ
	const CspVar* var;

	/// \brief param_ent_Tのポインタ
	const param_ent_T* param;
      }; // noname
    };

    /// \brief 変数やパラメタのリスト
    ///
    /// \sa var_param_ptr_ent
    class var_param_list_T
      : public std::list< var_param_ptr_ent >
    {
    };

    /// \brief 変数やパラメタのマップ
    ///
    /// \sa var_param_ptr_ent
    class var_param_map_T
      : public std::map< std::string, var_param_ptr_ent >
    {
    public:
      /// \brief マップに登録
      ///
      /// \param[in] name 名前
      /// \param[in] ent 変数やパラメタの var_param_ptr_ent
      ///
      /// \return 登録した var_param_ptr_ent
      var_param_ptr_ent&
      add_map (const std::string& name, const var_param_ptr_ent& ent);
    };

    /// \brief 関数のエントリ
    ///
    /// Cコード生成のために function_writer を継承する
    ///
    /// \sa function_writer
    class function_ent_T
      : public function_writer
    {
    public:
      /// \brief function_ent_T のコンストラクタ
      ///
      /// \param[in] defn BLXMLのfunctionタグ(defn)のポインタ
      /// \param[in] decl BLXMLのfunctionタグ(decl)のポインタ
      ///
      /// 実際にはどちらか一方のポインタが指定されて呼ばれ、一方は後か
      /// ら設定する
      function_ent_T (const SimulinkModel::XSD::function_T* defn = NULL,
		      const SimulinkModel::XSD::function_T* decl = NULL)
	: function_writer (defn, decl), fc_entry_id (-1)
      {
      }

      /// \brief 出現順に付けた番号を取得(参照用)
      ///
      /// \return 出現順に付けた番号
      int entry_id (void) const { return fc_entry_id; }

      /// \brief 出現順に付ける番号のリファレンスを取得(設定用)
      ///
      /// \return 出現順に付ける番号のリファレンス
      int& entry_id (void) { return fc_entry_id; }

      /// \brief 呼び出す関数のリストを取得(参照用)
      ///
      /// \return 呼び出す関数のリストのconstなリファレンス
      const function_ptr_list_T&
      call_functions (void) const
      {
	return fc_call_functions;
      }

      /// \brief 呼び出す関数のリストを取得(設定用)
      ///
      /// \return 呼び出す関数のリストのリファレンス
      function_ptr_list_T& call_functions (void) { return fc_call_functions; }

   private:
      /// \brief 出現順につける番号
      int fc_entry_id;

      /// \brief 呼び出す関数のリスト
      function_ptr_list_T fc_call_functions;
    };

    /// \brief 関数名をキーとした関数(function_ent_T)の連想配列と、定義・
    /// 宣言の出現順のベクタを持ったクラス
    ///
    /// \sa function_ent_T
    /// \sa function_ptr_list_T
    class function_map_T
      : public std::map < std::string, function_ent_T >
    {
    public:
      /// \brief 連想配列に関数を追加
      ///
      /// \param[in] name 関数名
      /// \param[in] f BLXMLのfunctionタグ
      void add_function_map (const std::string& name,
			     const SimulinkModel::XSD::function_T& f);

      /// \brief 呼び出す関数のリストを作成する
      void correct (void);

      /// \brief 関数定義の出現順リストを取得(設定用)
      ///
      /// \return 関数定義の出現順リスト
      function_ptr_list_T& defn_list (void) { return fm_defn_list; }

      /// \brief 関数定義の出現順リストを取得(参照用)
      ///
      /// \return 関数定義の出現順リスト
      const function_ptr_list_T&
      defn_list (void) const { return fm_defn_list; }

      /// \brief 関数宣言の出現順リストを取得(設定用)
      ///
      /// \return 関数宣言の出現順リスト
      function_ptr_list_T& decl_list (void) { return fm_decl_list; }

      /// \brief 関数宣言の出現順リストを取得(参照用)
      ///
      /// \return 関数宣言の出現順リスト
      const function_ptr_list_T&
      decl_list (void) const { return fm_decl_list; }

    private:
      /// \brief 関数定義の出現順リスト
      function_ptr_list_T fm_defn_list;
      /// \brief 関数宣言の出現順リスト
      function_ptr_list_T fm_decl_list;
    };

    //
    // taskの属性
    //

    /// \brief タスクの属性のクラス
    class task_attr_T
      : protected std::vector< bool >
    {
      /// \brief タスクの属性の種別
      ///
      /// 詳細は対応するメンバ関数参照
      enum attr_index
	{
	  NEED_RTM = 0,
	  HAS_STATE, HAS_TRIGGER_STATE,
	  HAS_INVSIG, HAS_PARAM, HAS_CPARAM, HAS_BIOSIG,
	  HAS_EXTIN, HAS_EXTOUT,
	  HAS_INVAR, HAS_OUTVAR, HAS_AUTOVAR,
	  HAS_FUNCTION, HAS_DATASTORE, HAS_VARIABLE_RATE,
	  OUT_RTM,
	  OUT_STATE, OUT_TRIGGER_STATE,
	  OUT_INVSIG, OUT_PARAM, OUT_CPARAM, OUT_BIOSIG,
	  OUT_EXTIN, OUT_EXTOUT,
	  OUT_INVAR, OUT_OUTVAR, OUT_AUTOVAR,
	  MULTIRATE_HEAD,
	  DOUBLE_BUFFER,
	  SEND_INIT,
	  MAX_ATTR
	};

    public:
      /// \brief 構造体オブジェクトの種別
      ///
      /// - INVSIG はinvariant block signal
      /// - PARAM はパラメタ
      /// - C_PARAM は定数パラメタ
      /// - STATE は状態変数
      /// - TRIGGER はTrigger State
      /// - BIOSIG はBlock  Signal
      /// - EXTIN は外部入力変数
      /// - EXTOUT は外部出力変数
      enum obj_type
	{
	  INVSIG, PARAM, C_PARAM, STATE, TRIGGER, BIOSIG, EXTIN, EXTOUT
	};

      /// \brief task_attr_T のコンストラクタ
      ///
      /// \param[in] n boolのベクタのサイズ
      task_attr_T (size_t n = MAX_ATTR)
	: std::vector< bool >(n > MAX_ATTR ? n : MAX_ATTR, false)
      {
      }

      /// \brief 処理対象のCspNodeを指定したコンストラクタ
      ///
      /// \param[in] cn 処理対象のCspNodeのポインタ
      /// \param[in] n boolのベクタのサイズ
      ///
      /// 処理対象のCspNodeに対して check_task() を呼ぶ
      ///
      /// \sa check_task()
      task_attr_T (const CspNode* cn, size_t n = MAX_ATTR)
	: std::vector< bool > (n > MAX_ATTR ? n : MAX_ATTR, false)
      {
	check_task (cn);
      }

      /// \brief Real-Time Model Objectが必要かどうか
      ///
      /// \retval true Real-Time Model Objectが必要
      /// \retval false Real-Time Model Objectは不要
      bool need_rtm (void) const { return (*this)[NEED_RTM]; }

      /// \brief Real-Time Model Objectが必要かどうか設定する
      ///
      /// \param[in] f trueなら必要、falseなら不要
      void need_rtm (bool f) { (*this)[NEED_RTM] = f; }

      /// \brief 状態変数を持つかどうか
      ///
      /// \retval true 状態変数を持つ
      /// \retval false 状態変数を持たない
      bool has_state (void) const { return (*this)[HAS_STATE]; }

      /// \brief 状態変数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_state (bool f) { (*this)[HAS_STATE] = f; }

      /// \brief Trigger Stateを持つかどうか
      ///
      /// \retval true Trigger Stateを持つ
      /// \retval false Trigger Stateを持たない
      bool has_trigger_state (void) const { return (*this)[HAS_TRIGGER_STATE]; }

      /// \brief Trigger Stateを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_trigger_state (bool f) { (*this)[HAS_TRIGGER_STATE] = f; }

      /// \brief invariant block signalを持つかどうか
      ///
      /// \retval true invariant block signalを持つ
      /// \retval false invariant block signalを持たない
      bool has_invsig (void) const { return (*this)[HAS_INVSIG]; }

      /// \brief invariant block signalを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_invsig (bool f) { (*this)[HAS_INVSIG] = f; }

      /// \brief パラメタを持つかどうか
      ///
      /// \retval true パラメタを持つ
      /// \retval false パラメタを持たない
      bool has_param (void) const { return (*this)[HAS_PARAM]; }

      /// \brief パラメタを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_param (bool f) { (*this)[HAS_PARAM] = f; }

      /// \brief 定数パラメタを持つかどうか
      ///
      /// \retval true 定数パラメタを持つ
      /// \retval false 定数パラメタを持たない
      bool has_cparam (void) const { return (*this)[HAS_CPARAM]; }

      /// \brief 定数パラメタを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_cparam (bool f) { (*this)[HAS_CPARAM] = f; }

      /// \brief ブロックシグナルを持つかどうか
      ///
      /// \retval true ブロックシグナルを持つ
      /// \retval false ブロックシグナルを持たない
      bool has_biosig (void) const { return (*this)[HAS_BIOSIG]; }

      /// \brief ブロックシグナルを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_biosig (bool f) { (*this)[HAS_BIOSIG] = f; }

      /// \brief 外部入力変数を持つかどうか
      ///
      /// \retval true 外部入力変数を持つ
      /// \retval false 外部入力変数を持たない
      bool has_extin (void) const { return (*this)[HAS_EXTIN]; }

      /// \brief 外部入力変数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_extin (bool f) { (*this)[HAS_EXTIN] = f; }

      /// \brief 外部出力変数を持つかどうか
      ///
      /// \retval true 外部出力変数を持つ
      /// \retval false 外部出力変数を持たない
      bool has_extout (void) const { return (*this)[HAS_EXTOUT]; }

      /// \brief 外部出力変数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_extout (bool f) { (*this)[HAS_EXTOUT] = f; }

      /// \brief 入力変数を持つかどうか
      ///
      /// \retval true 入力変数を持つ
      /// \retval false 入力変数を持たない
      bool has_invar (void) const { return (*this)[HAS_INVAR]; }

      /// \brief 入力変数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_invar (bool f) { (*this)[HAS_INVAR] = f; }

      /// \brief 出力変数を持つかどうか
      ///
      /// \retval true 出力変数を持つ
      /// \retval false 出力変数を持たない
      bool has_outvar (void) const { return (*this)[HAS_OUTVAR]; }

      /// \brief 出力変数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_outvar (bool f) { (*this)[HAS_OUTVAR] = f; }

      /// \brief 自動変数を持つかどうか
      ///
      /// \retval true 自動変数を持つ
      /// \retval false 自動変数を持たない
      bool has_autovar (void) const { return (*this)[HAS_AUTOVAR]; }

      /// \brief 自動変数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_autovar (bool f) { (*this)[HAS_AUTOVAR] = f; }

      /// \brief 関数を持つかどうか
      ///
      /// \retval true 関数を持つ
      /// \retval false 関数を持たない
      bool has_functions (void) const { return (*this)[HAS_FUNCTION]; }

      /// \brief 関数を持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_functions (bool f) { (*this)[HAS_FUNCTION] = f; }

      /// \brief DataStoreを持つかどうか
      ///
      /// \retval true DataStoreを持つ
      /// \retval false DataStoreを持たない
      bool has_datastore (void) const { return (*this)[HAS_DATASTORE]; }

      /// \brief DataStoreを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_datastore (bool f) { (*this)[HAS_DATASTORE] = f; }

      /// \brief 可変レートを持つかどうか
      ///
      /// \retval true 可変レートを持つ
      /// \retval false 可変レートを持たない
      bool has_variable_rate (void) const { return (*this)[HAS_VARIABLE_RATE]; }

      /// \brief 可変レートを持つかどうか設定する
      ///
      /// \param[in] f trueなら持つ、falseなら持たない
      void has_variable_rate (bool f) { (*this)[HAS_VARIABLE_RATE] = f; }

      /// \brief Real-Time Model Object をタスクの関数の外側に出力する
      /// かどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_rtm (void) const { return (*this)[OUT_RTM]; }

      /// \brief Real-Time Model Object を関数の外側に出力するかどうか
      /// 設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_rtm (bool f) { (*this)[OUT_RTM] = f; }

      /// \brief 状態変数をタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_state (void) const { return (*this)[OUT_STATE]; }

      /// \brief 状態変数をタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_state (bool f) { (*this)[OUT_STATE] = f; }

      /// \brief Trigger Stateをタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_trigger_state (void) const { return (*this)[OUT_TRIGGER_STATE]; }

      /// \brief Trigger Stateをタスクの関数の外側に出力するかどうか設
      /// 定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_trigger_state (bool f) { (*this)[OUT_TRIGGER_STATE] = f; }

      /// \brief invariant block signalをタスクの関数の外側に出力するか
      /// どうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_invsig (void) const { return (*this)[OUT_INVSIG]; }

      /// \brief invariant block signalをタスクの関数の外側に出力するか
      /// どうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_invsig (bool f) { (*this)[OUT_INVSIG] = f; }

      /// \brief パラメタをタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_param (void) const { return (*this)[OUT_PARAM]; }

      /// \brief パラメタをタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_param (bool f) { (*this)[OUT_PARAM] = f; }

      /// \brief 定数パラメタをタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_cparam (void) const { return (*this)[OUT_CPARAM]; }

      /// \brief 定数パラメタをタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_cparam (bool f) { (*this)[OUT_CPARAM] = f; }

      /// \brief ブロックシグナルをタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_biosig (void) const { return (*this)[OUT_BIOSIG]; }

      /// \brief ブロックシグナルをタスクの関数の外側に出力するかどうか
      /// 設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_biosig (bool f) { (*this)[OUT_BIOSIG] = f; }

      /// \brief 外部入力変数をタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_extin (void) const { return (*this)[OUT_EXTIN]; }

      /// \brief 外部入力変数をタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_extin (bool f) { (*this)[OUT_EXTIN] = f; }

      /// \brief 外部出力変数をタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_extout (void) const { return (*this)[OUT_EXTOUT]; }

      /// \brief 外部出力変数をタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_extout (bool f) { (*this)[OUT_EXTOUT] = f; }

      /// \brief 入力変数をタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_invar (void) const { return (*this)[OUT_INVAR]; }

      /// \brief 入力変数をタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_invar (bool f) { (*this)[OUT_INVAR] = f; }

      /// \brief 出力変数をタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_outvar (void) const { return (*this)[OUT_OUTVAR]; }

      /// \brief 出力変数をタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_outvar (bool f) { (*this)[OUT_OUTVAR] = f; }

      /// \brief 自動変数をタスクの関数の外側に出力するかどうか
      ///
      /// \retval true 外側に出力する
      /// \retval false 外側に出力しない
      bool out_autovar (void) const { return (*this)[OUT_AUTOVAR]; }

      /// \brief 自動変数をタスクの関数の外側に出力するかどうか設定する
      ///
      /// \param[in] f trueなら外側に出力し、falseなら外側に出力しない
      void out_autovar (bool f) { (*this)[OUT_AUTOVAR] = f; }

      /// \brief レートの切り替わる先頭かどうか
      ///
      /// \retval true レートが切り替わる先頭である
      /// \retval false レートが切り替わる先頭ではない
      bool multirate_head (void) const { return (*this)[MULTIRATE_HEAD]; }

      /// \brief レートの切り替わる先頭かどうかを設定する
      ///
      /// \param[in] f trueならレートが切り替わる先頭で、falseならそう
      /// ではない
      void multirate_head (bool f) { (*this)[MULTIRATE_HEAD] = f; }

      /// \brief ダブルバッファかどうか
      ///
      /// \retval true ダブルバッファである
      /// \retval false ダブルバッファではない
      bool double_buffer (void) const { return (*this)[DOUBLE_BUFFER]; }

      /// \brief ダブルバッファかどうかを設定する
      ///
      /// \param[in] f trueならダブルバッファ、falseならそうではない
      void double_buffer (bool f) { (*this)[DOUBLE_BUFFER] = f; }

      /// \brief オフセットによる初期値送信が必要かどうか
      ///
      /// \retval true 初期値を送信する
      /// \retval false 初期値を送信しない
      bool send_init (void) const { return (*this)[SEND_INIT]; }

      /// \brief オフセットによる初期値送信が必要かどうかを設定する
      void send_init (bool f) { (*this)[SEND_INIT] = f; }

      /// \brief タスクの関数の外側に出力する変数やパラメタのマップを取
      /// 得(参照用)
      ///
      /// \return タスクの関数の外側に出力する変数やパラメタのマップ
      const var_param_map_T& out_signal (void) const { return ta_out_sig; }

      /// \brief タスクの関数の外側に出力する変数やパラメタのマップを取
      /// 得(設定用)
      ///
      /// \return タスクの関数の外側に出力する変数やパラメタのマップ
      var_param_map_T& out_signal (void) { return ta_out_sig; }

      /// \brief 対象関数のコードに、マッチ対象の構造体オブジェクトが含
      /// まれる場合にその情報を設定する
      ///
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] fcode 対象の関数のコード
      /// \param[in] regpat マッチ対象の構造体オブジェクトの regex
      /// \param[in] type 対象の構造体オブジェクトの種別
      ///
      /// コード中に、マッチ対象の構造体オブジェクトがあれば、オブジェ
      /// クトの種別に従って、関数の外側に構造体オブジェクトを出力する
      /// ように設定する
      void check_function_obj (const CspNode* node,
			       const std::string& fcode,
			       const std::tr1::regex& regpat,
			       obj_type type);
      /// \brief 対象の CspNode の関数のコードに、マッチ対象の構造体オ
      /// ブジェクトが含まれる場合にその情報を設定する
      ///
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] regpat マッチ対象の構造体オブジェクトの regex
      /// \param[in] type 対象の構造体オブジェクトの種別
      ///
      /// CspNode で呼び出す関数のコードについて check_function_obj()
      /// を実行する
      void check_function_obj (const CspNode* node,
			       const std::tr1::regex& regpat,
			       obj_type type);

      /// \brief 対象の CspNode の関数のコードに、指定した構造体オブジェ
      /// クト名が含まれる場合にその情報を設定する
      ///
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] object 構造体オブジェクト名
      /// \param[in] type 対象の構造体オブジェクトの種別
      ///
      /// マッチ対象の構造体オブジェクトの regex を
      /// make_word_regex_pattern() で作成し check_function_obj() を実
      /// 行する
      void check_function_obj (const CspNode* node,
			       const std::string& object,
			       obj_type type);

      /// \brief 対象の CspNode の関数のコードに、指定した構造体オブジェ
      /// クト名が含まれる場合にその情報を設定する
      ///
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] params ParamList
      /// \param[in] type 対象の構造体オブジェクトの種別
      ///
      /// ParamListの構造体オブジェクト名で check_function_obj() を実行
      /// する
      void check_function_obj (const CspNode* node,
			       const ParamList& params,
			       obj_type type);

      /// \brief 対象の CspNode に対し、各属性の設定を行う
      ///
      /// \param[in] node 対象の CspNode のポインタ
      void check_task (const CspNode* node);

    private:
      /// \brief タスクの関数の外側に出力する変数やパラメタのマップ
      var_param_map_T ta_out_sig;
    };

    /// \brief ブロックのレートとオフセットの基底クラス
    ///
    /// ブロックのレートとオフセットを保持し、比較用のオペレータを提供する
    class rate_T
    {
    public:
      /// \brief rate_T のデフォルトコンストラクタ
      rate_T () : r_rate (0), r_offset (0) {}

      /// \brief rate_T のレート、オフセットを指定したコンストラクタ
      ///
      /// \param[in] r レート
      /// \param[in] o オフセット
      rate_T (unsigned int r, unsigned int o)
	: r_rate (r), r_offset (o)
      {
      }

      /// \brief rate_T のレートを指定したコンストラクタ
      ///
      /// \param[in] rate レート
      rate_T (const rate_T& rate)
	: r_rate (rate.r_rate), r_offset (rate.r_offset)
      {
      }

      /// \brief ブロックのレートの取得(参照用)
      ///
      /// \return ブロックのレート
      Rate_rate_T rate (void) const { return r_rate; }

      /// \brief ブロックのレートのリファレンスの取得(設定用)
      ///
      /// \return ブロックのレートのリファレンス
      Rate_rate_T& rate (void) { return r_rate; }

      /// \brief ブロックのレートのオフセット取得(参照用)
      ///
      /// \return ブロックのレートのオフセット
      Rate_offset_T offset (void) const { return r_offset; }

      /// \brief ブロックのレートのオフセットのリファレンスの取得(設定用)
      ///
      /// \return ブロックのレートのオフセットリファレンス
      Rate_offset_T& offset (void) { return r_offset; }

      /// \brief rate_T との比較(equal)オペレータ
      ///
      /// \param[in] x 比較先の rate_T
      ///
      /// \retval true 一致
      /// \retval false 不一致
      bool operator== (const rate_T& x) const
      {
	return r_rate == x.r_rate && r_offset == x.r_offset;
      }

      /// \brief rate_T との比較(not equal)オペレータ
      ///
      /// \param[in] x 比較先の rate_T
      ///
      /// \retval true 不一致
      /// \retval false 一致
      bool operator!= (const rate_T& x) const
      {
	return r_rate != x.r_rate || r_offset != x.r_offset;
      }

      /// \brief rate_T との比較(less than)オペレータ
      ///
      /// \param[in] x 比較先の rate_T
      ///
      /// \retval true 比較先の rate_T より小さい
      /// \retval false 比較先の rate_T と同じか大きい
      bool operator< (const rate_T& x) const
      {
	if (r_rate < x.r_rate)
	  return true;
	else if (r_rate > x.r_rate)
	  return false;
	return r_offset < x.r_offset;
      }

      /// \brief rate_T との比較(greater than)オペレータ
      ///
      /// \param[in] x 比較先の rate_T
      ///
      /// \retval true 比較先の rate_T より大きい
      /// \retval false 比較先の rate_T と同じか小さい
      bool operator> (const rate_T& x) const
      {
	if (r_rate > x.r_rate)
	  return true;
	else if (r_rate < x.r_rate)
	  return false;
	return r_offset > x.r_offset;
      }

    private:
      /// \brief レート
      Rate_rate_T r_rate;

      /// \brief オフセット
      Rate_offset_T r_offset;
    };

    /// \brief rate_T の出力オペレータ
    ///
    /// \param[in] s 出力ストリーム
    /// \param[in] x 出力する rate_T
    ///
    /// \return 出力ストリーム
    ///
    /// \sa rate_T
    std::ostream& operator<< (std::ostream& s, const rate_T& x);

    /// \brief タスクのレートの情報
    ///
    /// レートと同じレートの使用数を持ち、レートの順や使用数の順でソー
    /// トするエントリとして使用する
    ///
    /// \sa rate_T
    /// \sa rate_list_T
    class rate_info_T
      : public rate_T
    {
    public:
      /// \brief rate_info_T のコンストラクタ
      ///
      /// \param[in] r レート
      /// \param[in] o オフセット
      rate_info_T (unsigned int r, unsigned int o)
	: rate_T (r, o), ri_count (0)
      {
      }

      /// \brief rate_info_T のコンストラクタ
      ///
      /// \param[in] rate レート
      rate_info_T (const rate_T& rate)
	: rate_T (rate), ri_count (0)
      {
      }

      /// \brief 使用数の取得(参照用)
      ///
      /// \return 使用数
      unsigned int count (void) const { return ri_count; }

      /// \brief 使用数のリファレンスの取得(設定用)
      ///
      /// \return 使用数のリファレンス
      unsigned int& count (void) { return ri_count; }

    private:
      /// \brief 使用数
      unsigned int ri_count;
    };

    /// \brief rate_info_Tのソート用ベクタ
    ///
    /// \sa rate_info_T
    class rate_list_T
      : public std::vector< rate_info_T >
    {
    public:
      /// \brief レートの大きい順にソート
      void sort_gt (void)
      {
	if (size () > 1)
	  ::std::sort (begin (), end (), compare_gt);
      }

      /// \brief 使用数の多い順にソート
      void sort_count (void)
      {
	if (size () > 1)
	  ::std::sort (begin (), end (), compare_count);
      }

      /// \brief レートをベクタに追加
      ///
      /// \param[in] rate レート
      void add_rate (const rate_T& rate);

    private:
      /// \brief レートの大きい順にソートするための比較関数
      ///
      /// \param[in] a レート1
      /// \param[in] b レート2
      ///
      /// \return true レート1が大きい
      /// \return false それ以外
      static bool compare_gt (const rate_info_T& a, const rate_info_T& b)
      {
	return a > b;
      }

      /// \brief 使用数の多い順にソートするための比較関数
      ///
      /// \param[in] a レート1
      /// \param[in] b レート2
      ///
      /// \return true レート1の使用数が多い
      /// \return false それ以外
      static bool compare_count (const rate_info_T& a, const rate_info_T& b)
      {
	return a.count () > b.count ();
      }
    };

    /// \brief タスク内のレートのリスト
    ///
    /// 例えばコア割り当てされたタスク内に複数のレートを持たせる必要が
    /// ある
    ///
    /// \sa rate_T
    class task_rates_T
      : public std::deque< rate_T >
    {
    public:
      /// \brief レートからインデクスを取得
      ///
      /// \param[in] rate レート
      ///
      /// \retval ~0U 見つからない
      /// \retval それ以外 リストのインデクス
      ///
      /// タスク内のレートを制御する変数のインデクスに使用する
      unsigned int
      find_index_by_value (const rate_T& rate) const
      {
	for (unsigned int n = 0; n < size (); n++)
	  {
	    const rate_T& r = (*this)[n];
	    if (r == rate)
	      return n;
	  }
	return ~0U;
      }
    };

    /// \brief 生成するタスクの単位のクラス
    ///
    /// コア割り当てされた複数の CspNode を、単一のタスクにするような使
    /// い方を行う
    ///
    /// \sa task_attr_T
    /// \sa CspNodeList
    class TaskSet
      : public task_attr_T, public CspNodeList
    {
    public:
      /// \brief デフォルトコンストラクタ
      TaskSet() {}

      /// \brief タスク内の CspNode のポインタのリストを取得(参照用)
      ///
      /// \return タスク内の CspNode のポインタのリスト
      const CspNodeList& cspnodelist (void) const { return *this; }

      /// \brief タスク内の CspNode のポインタのリストを取得(設定用)
      ///
      /// \return タスク内の CspNode のポインタのリスト
      CspNodeList& cspnodelist (void) { return *this; }

      /// \brief タスク属性(task_attr_T)の取得(参照用)
      ///
      /// \return タスク属性(task_attr_T)
      const task_attr_T& attr (void) const { return *this; }

      /// \brief タスク属性(task_attr_T)の取得(設定用)
      ///
      /// \return タスク属性(task_attr_T)
      task_attr_T& attr (void) { return *this; }

      /// \brief コア番号の取得
      ///
      /// \return コア番号を取得。CspNodeListの最初のコア番号を返す
      int core (void) const;

      /// \brief コアのタイプの取得
      ///
      /// \return コアのタイプ
      CoreType coretype (void) const;

      /// \brief TaskSet に CspNode を追加する
      ///
      /// \param[in] n 追加する CspNode
      void add_node (CspNode* n);

      /// \brief TaskSetの情報を設定する
      ///
      /// 含まれる CspNode について以下を行う
      ///  -# task_attr_T::check_task() を実行
      ///  -# CspNode が持つ構造体オブジェクトの情報を TaskSet の
      ///     #tf_autostorage に取り込む
      ///  -# 入力変数、出力変数、自動変数を #tf_autostorage に取り込む
      ///  -# Triggered/Enabled SubSystemのブロックであれば、その
      ///     SubSystemの状態変数を #tf_autostorage に取り込む
      ///  -# DataStoreがあれば、それを #tf_dsm_storage に取り込む
      ///
      /// \sa task_attr_T::check_task()
      /// \sa #tf_autostorage
      /// \sa #tf_dsm_storage
      void setup_taskset (void);

      /// \brief 指定した CspNode が、この TaskSet に含まれるか調べる
      ///
      /// \param[in] n 対象の CspNode のポインタ
      ///
      /// \retval true 含まれる
      /// \retval false 含まれない
      bool is_in_sets (const CspNode* n) const
      {
	const CspNodeList& thislist = *this;
	return std::find (thislist.begin (),
			  thislist.end (), n) != thislist.end ();
      }

      /// \brief TaskSet 内の重複を含む全レートのリスト(rate_list_T)を
      /// 取得(参照用)
      ///
      /// \return 重複を含む全レートのリスト(rate_list_T)
      const rate_list_T& rate_list (void) const { return tf_rate_list; }

      /// \brief TaskSet内の重複を含む全レートのリスト(rate_list_T)を取
      /// 得(設定用)
      ///
      /// \return 重複を含む全レートのリスト(rate_list_T)
      rate_list_T& rate_list (void) { return tf_rate_list; }

      /// \brief TaskSet のレートの最大公約数(基本レート)を取得(参照用)
      ///
      /// \return TaskSet のレートの最大公約数(基本レート)
      const rate_T& rate (void) const { return tf_rate; }

      /// \brief TaskSet のレートの最大公約数(基本レート)のリファレンス
      /// を取得(設定用)
      ///
      /// \return TaskSet のレートの最大公約数(基本レート)のリファレン
      /// ス
      rate_T& rate (void) { return tf_rate; }

      /// \brief TaskSet 内のレートのリストの取得(参照用)
      ///
      /// \return TaskSet 内のレートのリスト
      const task_rates_T& ratebyid (void) const { return tf_ratebyid; }

      /// \brief TaskSet 内のレートのリストの取得(設定用)
      ///
      /// \return TaskSet 内のレートのリスト
      task_rates_T& ratebyid (void) { return tf_ratebyid; }

      /// \brief TaskSet 内のレートのリストにレートを追加
      ///
      /// \param[in] rate 追加するレート
      void
      add_rate (const rate_T& rate)
      {
	tf_rate_list.add_rate (rate);
      }

      /// \brief TaskSet内のレートのリストのインデクスからレートを取得
      ///
      /// \param[in] idx レートのリストのインデクス
      ///
      /// \return レート
      const rate_T&
      idx2rate (unsigned int idx) const
      {
	return tf_ratebyid[idx];
      }

      /// \brief TaskSet 内のレートのリストのレートからインデクスを取得
      ///
      /// \param[in] rate レート
      ///
      /// \retval ~0U 見つからなかった
      /// \retval それ以外 レートのリストのインデクス
      unsigned int rate2idx (const rate_T& rate) const
      {
	return tf_ratebyid.find_index_by_value (rate);
      }

      /// \brief TaskSet 内の最小のレートが TaskSet のレートの最大公約
      /// 数(基本レート)かどうか調べる
      ///
      /// \retval true 最小レートがレートの最大公約数
      /// \retval false そうではない
      ///
      /// falseの場合、タスク内はレートの最大公約数で駆動し、レートの数
      /// だけ時間を管理する必要がある。true の場合は最小レートで駆動す
      /// るので、管理する時間は一つ少なくなる
      bool minrate_is_taskrate (void) const
      {
	return (tf_ratebyid.size () > 0 && tf_ratebyid[0] == tf_rate);
      }

      /// \brief TaskSet の AutoStorage を取得(参照用)
      ///
      /// \return TaskSet の AutoStorage
      const AutoStorageMap& autostorage (void) const { return tf_autostorage; }

      /// \brief TaskSet の AutoStorage を取得(設定用)
      ///
      /// \return TaskSet の AutoStorage
      AutoStorageMap& autostorage (void) { return tf_autostorage; }

      /// \brief TaskSet のDataStoreの AutoStorage を取得(参照用)
      ///
      /// \return TaskSet のDataStoreの AutoStorage
      const AutoStorageMap& dsm_storage (void) const { return tf_dsm_storage; }

      /// \brief TaskSet のDataStoreの AutoStorage を取得(設定用)
      ///
      /// \return TaskSet のDataStoreの AutoStorage
      AutoStorageMap& dsm_storage (void) { return tf_dsm_storage; }

      /// \brief DataStoreの初期化コードを取得(参照用)
      ///
      /// \return DataStoreの初期化コード
      const std::string& dsm_init (void) const { return tf_dsm_init; }

      /// \brief DataStoreの初期化コードを取得(設定用)
      ///
      /// \return DataStoreの初期化コード
      std::string& dsm_init (void) { return tf_dsm_init; }

      /// \brief TaskSet 内のレートのリストと最大公約数のレートの算出を
      /// 行う
      ///
      /// \return レートの最大公約数
      ///
      /// 次の処理を行う
      /// -# #tf_rate_list を大きい順にソートする
      /// -# #tf_ratebyid の先頭に一番大きいものを入れる
      /// -# その値を現在の最大公約数とする
      /// -# #tf_rate_list を順に処理し、値が変わったら #tf_ratebyid に入れる
      ///    - 前の最大公約数と変わった値から、新しい最大公約数を求める
      /// -# 最大公約数を #tf_rate に格納し、その値を返す
      rate_T& select_rate (void);

    private:
      /// \brief TaskSet 内の重複を含む全レートのリスト
      rate_list_T tf_rate_list;

      /// \brief TaskSet のレートの最大公約数(基本レート)
      ///
      /// 例えば #tf_ratebyid に 2, 3 があったとすると #tf_rate は 1 に
      /// なる
      rate_T tf_rate;

      /// \brief TakSet 内の昇順のレートのリスト
      ///
      /// ただし #tf_rate が例えば1になってしまっている場合のように、
      /// #tf_rate は含まれていないことがある
      task_rates_T tf_ratebyid;

      /// \brief TaskSet の AutoStorage
      AutoStorageMap tf_autostorage;

      /// \brief TaskSet のDataStoreの AutoStorage
      ///
      // Embedded Coderの出力では、DataStoreは状態変数として出力される。
      // ターゲットによっては、ブロックの状態変数は局所化され、
      // DataStoreの状態変数は共有される、といった場合に対応するため
      // TaskSet の AutoStorageMap とは別にDataStore用の
      // AutoStorageMap を持つ。
      AutoStorageMap tf_dsm_storage; // state, param

      /// \brief DataStoreの初期化コード
      std::string tf_dsm_init;

      /// 最大公約数を計算する
      ///
      /// \param[in] x 値1
      /// \param[in] y 値2
      ///
      /// \return 値1と値2の最大公約数
      static unsigned int gcd (unsigned int x, unsigned int y);
    };

    /// \brief TaskSet のポインタのベクタのみを保持する基底クラス
    class baseTaskSetList
      : public std::vector< TaskSet* >
    {
    };


    /// \brief TaskSet のポインタのベクタ
    ///
    /// Cコードを生成する際、このリストの要素である TaskSet をタスクの
    /// 単位としてコード生成を行う
    class TaskSetList
      : public baseTaskSetList
    {
    public:
      /// \brief 指定した CspNode を含む新たな TaskSet を生成し
      /// TaskSetList に追加する
      ///
      /// \param[in] n 登録する CspNode のポインタ
      void
      add_node (CspNode *n)
      {
	ts_store.push_back (TaskSet());
	TaskSet& ts = ts_store.back ();
	push_back (&ts);
	ts.add_node (n);
      }

      /// \brief TaskSetList を指定サイズに設定する
      ///
      /// これは例えば、コア数分の TaskSet をあらかじめ生成するといった
      /// ことに使用する
      void
      set_size (size_t n)
      {
	clear ();
	resize (n, NULL);
	ts_store.clear ();
	for (size_t i = 0; i < n; i++)
	  {
	    ts_store.push_back (TaskSet ());
	    TaskSet& ts = ts_store.back ();
	    (*this)[i] = &ts;
	  }
      }

    private:
      /// \brief TaskSet のストア
      ///
      /// 実体はここに格納され、ポインタのベクタから参照される
      std::list< TaskSet > ts_store;
    };

    /// \brief Embedded Coderが生成するマクロ等、ブロック固有のコード以
    /// 外のコードテキストを保持するクラス
    class text_list_T
      : public std::vector< std::string >
    {
    };

    /// \brief text_list_T 専用出力オペレータ
    ///
    std::ostream& operator<< (std::ostream& s, const text_list_T& tl);

    /// \brief Embedded Coderが生成する構造体の型宣言や定義を保持するクラス
    ///
    /// 代表的なものとしては Real-Time Model Objectの宣言や定義である
    class c_object_ent_T
    {
    public:
      /// \brief c_object_ent_T のコンストラクタ
      ///
      /// \param[in] obj BLXMLのobject/deftypeのタグのポインタ
      /// \param[in] value 保持しているテキストのポインタ
      /// \param[in] attr C_OBJECT_EC_ATTR の属性
      ///
      /// \sa C_OBJECT_EC_ATTR
      c_object_ent_T (const SimulinkModel::XSD::object_T* obj,
		      const std::string* value, int attr = 0)
	: co_object (obj), co_value (value), co_attr (attr)
      {
      }

      /// \brief BLXMLのobject/deftypeのタグの参照用オペレータ
      ///
      /// \return BLXMLのobject/deftypeのタグのリファレンス
      const SimulinkModel::XSD::object_T& operator* () const
      { return *co_object; }

      /// \brief BLXMLのobject/deftypeのタグのポインタ参照用オペレータ
      ///
      /// \return BLXMLのobject/deftypeのタグのポインタ
      const SimulinkModel::XSD::object_T* operator-> () const
      { return co_object; }

      /// \brief BLXMLのobject/deftypeのタグのポインタへのキャスト
      ///
      /// \return BLXMLのobject/deftypeのタグのポインタ
      operator const SimulinkModel::XSD::object_T* () const
      {
	return co_object;
      }

      /// \brief BLXMLのobject/deftypeのタグのポインタの取得
      ///
      /// \return BLXMLのobject/deftypeのタグのポインタ
      const SimulinkModel::XSD::object_T* object (void) const
      {
	return co_object;
      }

      /// \brief BLXMLのobject/deftypeのタグのテキストのポインタ取得
      ///
      /// object/deftype に設定されているテキスト、または属性valueのポ
      /// インタを取得する。(コード生成時には、基本的にそのまま出力する
      /// テキスト情報である)
      ///
      /// \return BLXMLのobject/deftypeのタグのテキストのポインタ
      const std::string* value (void) const { return co_value; }

      /// \brief Cコードに出力するオブジェクト属性を取得する(参照用)
      ///
      /// \return Cコードに出力するオブジェクト属性
      ///
      /// \sa C_OBJECT_EC_ATTR
      int attr (void) const { return co_attr; }

      /// \brief Cコードに出力するオブジェクト属性のリファレンスを取得
      /// する(設定用)
      ///
      /// \return Cコードに出力するオブジェクト属性のリファレンス
      int& attr (void) { return co_attr; }

    private:
      /// \brief BLXMLのobject/deftypeのタグのポインタ
      const SimulinkModel::XSD::object_T* co_object;

      /// \brief object/deftype に設定されているテキスト、または属性
      /// valueのポインタ
      const std::string* co_value;

      /// \brief Cコードに出力するオブジェクト属性
      ///
      /// \sa C_OBJECT_EC_ATTR
      int co_attr;
    };

    /// \brief c_object_ent_T のベクタのクラス
    ///
    /// \sa c_object_ent_T
    class c_object_list_T
      : public std::vector< c_object_ent_T >
    {
    };

    /// \brief BLXMLのincludeタグのポインタのベクタ
    class c_include_list_T
      : public std::vector< const SimulinkModel::XSD::include_T* >
    {
    public:
      /// \brief 置換するファイルとテキストのマップ
      class text_map : public std::map< std::string, const char* >
      {
      };

      /// \brief 置換するマップの参照
      const text_map& get_text_map (void) const { return cil_text_map; }

      /// \brief 置換するマップの取得
      text_map& get_text_map (void) { return cil_text_map; }

    private:
      /// \brief 置換するテキストのマップ
      text_map cil_text_map;
    };

    /// \brief コマンドラインオプションで設定され、生成するCコードに出
    /// 力するユーザ指定のインクルードファイルのベクタ
    class option_include_list_T
      : public std::vector< std::string >
    {
    public:
      /// \brief ヘッダファイル名を追加する
      ///
      /// \param[in] file ヘッダファイル名
      void add_header_file (const char* file)
      {
	push_back (std::string (file));
      }
    };

    /// \brief Cコード生成用ライタおよび出力データを保持するクラス
    class C_CodeWriter
    {
    public:
      /// \brief C_CodeWriter のデフォルトコンストラクタ
      C_CodeWriter () {}

      /// \brief コア割り当てされた TaskSetList の取得(参照用)
      ///
      /// TaskSetList のインデクスはコア番号となっている
      ///
      /// \return コア番号をインデクスとした TaskSet のconstなリファレンス
      ///
      /// \sa TaskSetList
      const TaskSetList& core_assigned_taskset_list (void) const
      { return cw_core_assigned_tasksetlist; }

      /// \brief コア割り当てされた TaskSetList の取得(設定用)
      ///
      /// \return コア番号をインデクスとした TaskSet のリファレンス
      ///
      /// \sa TaskSetList
      TaskSetList& core_assigned_taskset_list (void)
      { return cw_core_assigned_tasksetlist; }

      /// \brief コア割り当てされていないタスク用の TaskSetList の取得
      /// (参照用)
      ///
      /// \return コア割り当てされていないタスクの TaskSet のconstなリ
      /// ファレンス
      ///
      /// \sa TaskSetList
      const TaskSetList&
      taskset_list (void) const { return cw_tasksetlist; }

      /// \brief コア割り当てされていないタスク用の TaskSetList の取得
      /// (設定用)
      ///
      /// \return コア割り当てされていないタスクの TaskSet のリファレン
      /// ス
      ///
      /// \sa TaskSetList
      TaskSetList& taskset_list (void) { return cw_tasksetlist; }

      /// \brief ヘッダファイルの二重インクルード判定のテキスト情報取得
      /// (参照用)
      ///
      /// \return ヘッダファイルの二重インクルード判定のテキスト情報取の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T&
      header_preamble (void) const { return cw_header_preamble; }

      /// \brief ヘッダファイルの二重インクルード判定のテキスト情報取得
      /// (設定用)
      ///
      /// \return ヘッダファイルの二重インクルード判定のテキスト情報取
      /// のリファレンス
      ///
      /// \sa text_list_T
      text_list_T& header_preamble (void) { return cw_header_preamble; }

      /// \brief Real-Time Model Object用マクロのテキスト情報取得
      /// (参照用)
      ///
      /// \return Real-Time Model Object用マクロのテキスト情報の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& rt_macro (void) const { return cw_rt_macro; }

      /// \brief Real-Time Model Object用マクロのテキスト情報取得
      /// (設定用)
      ///
      /// \return Real-Time Model Object用マクロのテキスト情報の
      /// リファレンス
      ///
      /// \sa text_list_T
      text_list_T& rt_macro (void) { return cw_rt_macro; }

      /// \brief Real-Time Model Objectの型宣言のテキスト情報取得
      /// (参照用)
      ///
      /// \return Real-Time Model Objectの型宣言のテキスト情報の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& rt_type (void) const { return cw_rt_type; }

      /// \brief Real-Time Model Objectの型宣言のテキスト情報取得
      /// (設定用)
      ///
      /// \return Real-Time Model Objectの型宣言のテキスト情報の
      /// リファレンス
      ///
      /// \sa text_list_T
      text_list_T& rt_type (void) { return cw_rt_type; }

      /// \brief Real-Time Model Objectの更新コードのテキスト情報取得
      /// (参照用)
      ///
      /// \return Real-Time Model Objectの更新コードのテキスト情報の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& rt_code (void) const { return cw_rt_code; }

      /// \brief Real-Time Model Objectの更新コードのテキスト情報取得
      /// (設定用)
      ///
      /// \return Real-Time Model Objectの更新コードのテキスト情報の
      /// リファレンス
      ///
      /// \sa text_list_T
      text_list_T& rt_code (void) { return cw_rt_code; }

      /// \brief Real-Time Model Objectの初期化コードのテキスト情報取得
      /// (参照用)
      ///
      /// \return Real-Time Model Objectの初期化コードのテキスト情報の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& rt_init (void) const { return cw_rt_init; }

      /// \brief Real-Time Model Objectの初期化コードのテキスト情報取得
      /// (設定用)
      ///
      /// \return Real-Time Model Objectの初期化コードのテキスト情報の
      /// リファレンス
      ///
      /// \sa text_list_T
      text_list_T& rt_init (void) { return cw_rt_init; }

      /// \brief 共通の実行コードのテキスト情報取得(参照用)
      ///
      /// \return 共通の実行コードのテキスト情報のconstなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& common (void) const { return cw_common; }

      /// \brief 共通の実行コードのテキスト情報取得(設定用)
      ///
      /// \return 共通の実行コードのテキスト情報のリファレンス
      ///
      /// \sa text_list_T
      text_list_T& common (void) { return cw_common; }

      /// \brief 共通の初期化コードのテキスト情報取得(参照用)
      ///
      /// \return 共通の初期化コードのテキスト情報のconstなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& common_init (void) const { return cw_common_init; }

      /// \brief 共通の初期化コードのテキスト情報取得(設定用)
      ///
      /// \return 共通の初期化コードのテキスト情報のリファレンス
      ///
      /// \sa text_list_T
      text_list_T& common_init (void) { return cw_common_init; }

      /// \brief タスクのコードの先頭に出力するテキスト情報(参照用)
      ///
      /// \return タスクのコードの先頭に出力するテキスト情報の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& preamble (void) const { return cw_preamble; }

      /// \brief タスクのコードの先頭に出力するテキスト情報(設定用)
      ///
      /// \return タスクのコードの先頭に出力するテキスト情報の
      /// リファレンス
      ///
      /// \sa text_list_T
      text_list_T& preamble (void) { return cw_preamble; }

      /// \brief 共通マクロのテキスト情報(参照用)
      ///
      /// \return 共通マクロのテキスト情報のconstなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& c_macro (void) const { return cw_c_macro; }

      /// \brief 共通マクロのテキスト情報(設定用)
      ///
      /// \return 共通マクロのテキスト情報のリファレンス
      ///
      /// \sa text_list_T
      text_list_T& c_macro (void) { return cw_c_macro; }

      /// \brief BLXMLのdeftypeタグのリスト取得(参照用)
      ///
      /// \return BLXMLのdeftypeタグのリストのconstなリファレンス
      ///
      /// \sa c_object_ent_T
      const c_object_list_T& c_deftype (void) const { return cw_c_deftype; }

      /// \brief BLXMLのdeftypeタグのリスト取得(設定用)
      ///
      /// \return BLXMLのdeftypeタグのリストのリファレンス
      ///
      /// \sa c_object_ent_T
      c_object_list_T& c_deftype (void) { return cw_c_deftype; }

      /// \brief BLXMLのobjectタグのリスト取得(参照用)
      ///
      /// \return BLXMLのobjectタグのリストのconstなリファレンス
      ///
      /// \sa c_object_ent_T
      const c_object_list_T& c_object (void) const { return cw_c_object; }

      /// \brief BLXMLのobjectタグのリスト取得(設定用)
      ///
      /// \return BLXMLのobjectタグのリストのリファレンス
      ///
      /// \sa c_object_ent_T
      c_object_list_T& c_object (void) { return cw_c_object; }

      /// \brief 関数の連想配列の取得(参照用)
      ///
      /// \return 関数の連想配列のconstなリファレンス
      ///
      /// \sa function_map_T
      const function_map_T& functions (void) const { return cw_functions; }

      /// \brief 関数の連想配列の取得(設定用)
      ///
      /// \return 関数の連想配列のリファレンス
      ///
      /// \sa function_map_T
      function_map_T& functions (void) { return cw_functions; }

      /// \brief Real-Time Model Objectのコードから呼び出される関数の
      /// リスト取得(参照用)
      ///
      /// \return Real-Time Model Objectのコードから呼び出される
      /// 関数のリストのconstなリファレンス
      ///
      /// \sa function_ptr_list_T
      const function_ptr_list_T&
      rt_code_func (void) const { return cw_rt_code_func; }

      /// \brief Real-Time Model Objectのコードから呼び出される関数の
      /// リスト取得(設定用)
      ///
      /// \return Real-Time Model Objectのコードから呼び出される
      /// 関数のリストのリファレンス
      ///
      /// \sa function_ptr_list_T
      function_ptr_list_T& rt_code_func (void) { return cw_rt_code_func; }

      /// \brief 共通コードから呼び出される関数のリスト取得(参照用)
      ///
      /// \return 共通コードから呼び出される関数のリストの
      /// constなリファレンス
      ///
      /// \sa function_ptr_list_T
      const function_ptr_list_T&
      common_func (void) const { return cw_common_func; }

      /// \brief 共通コードから呼び出される関数のリスト取得(設定用)
      ///
      /// \return 共通コードから呼び出される関数のリストのリファレンス
      ///
      /// \sa function_ptr_list_T
      function_ptr_list_T& common_func (void) { return cw_common_func; }

      /// \brief BLXMLのincludeタグのリストを取得(参照用)
      ///
      /// \return BLXMLのincludeタグのリストのconstなリファレンス
      ///
      /// \sa c_include_list_T
      const c_include_list_T&
      include_file(void) const { return cw_include_file; }

      /// \brief BLXMLのincludeタグのリストを取得(設定用)
      ///
      /// \return BLXMLのincludeタグのリストのリファレンス
      ///
      /// \sa c_include_list_T
      c_include_list_T& include_file(void) { return cw_include_file; }

      /// \brief DataStore用の AutoStorageMap を取得(参照用)
      ///
      /// \return DataStore用の AutoStorageMap のconstなリファレンス
      ///
      /// \sa AutoStorageMap
      const AutoStorageMap& dsmem (void) const { return cw_dsmem; }

      /// \brief DataStore用の AutoStorageMap を取得(設定用)
      ///
      /// \return DataStore用の AutoStorageMap のリファレンス
      ///
      /// \sa AutoStorageMap
      AutoStorageMap& dsmem (void) { return cw_dsmem; }

      /// \brief DataStoreの初期化コードのテキスト情報を取得(参照用)
      ///
      /// \return DataStoreの初期化コードのテキスト情報の
      /// constなリファレンス
      ///
      /// \sa text_list_T
      const text_list_T& dsmem_init (void) const { return cw_dsmem_init; }

      /// \brief DataStoreの初期化コードのテキスト情報を取得(設定用)
      ///
      /// \return DataStoreの初期化コードのテキスト情報のリファレンス
      ///
      /// \sa text_list_T
      text_list_T& dsmem_init (void) { return cw_dsmem_init; }

      /// \brief 外部入力変数の構造体名を取得(参照用)
      ///
      /// \return 外部入力変数の構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& extin_storage (void) const
      { return cw_extin_storage; }

      /// \brief 外部入力変数の構造体名を取得(設定用)
      ///
      /// \return 外部入力変数の構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& extin_storage (void) { return cw_extin_storage; }

      /// \brief 外部出力変数の構造体名を取得(参照用)
      ///
      /// \return 外部出力変数の構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& extout_storage (void) const
      { return cw_extout_storage; }

      /// \brief 外部出力変数の構造体名を取得(設定用)
      ///
      /// \return 外部出力変数の構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& extout_storage (void) { return cw_extout_storage; }

      /// \brief ブロックシグナルの構造体名を取得(参照用)
      ///
      /// \return ブロックシグナルの構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& signal_storage (void) const
      { return cw_signal_storage; }

      /// \brief ブロックシグナルの構造体名を取得(設定用)
      ///
      /// \return ブロックシグナルの構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& signal_storage (void) { return cw_signal_storage; }

      /// \brief 状態変数の構造体名を取得(参照用)
      ///
      /// \return 状態変数の構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& state_storage (void) const
      { return cw_state_storage; }

      /// \brief 状態変数の構造体名を取得(設定用)
      ///
      /// \return 状態変数の構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& state_storage (void) { return cw_state_storage; }

      /// \brief Trigger Stateの構造体名を取得(参照用)
      ///
      /// \return Trigger Stateの構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& trigger_state_storage (void) const
      { return cw_trigger_state_storage; }

      /// \brief Trigger Stateの構造体名を取得(設定用)
      ///
      /// \return Trigger Stateの構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& trigger_state_storage (void)
      { return cw_trigger_state_storage; }

      /// \brief invariant block signalの構造体名を取得(参照用)
      ///
      /// \return invariant block signalの構造体名の
      /// optional_refstring のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& invsig_storage (void) const
      { return cw_invsig_storage; }

      /// \brief invariant block signalの構造体名を取得(設定用)
      ///
      /// \return invariant block signalの構造体名の
      /// optional_refstring のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& invsig_storage (void) { return cw_invsig_storage; }

      /// \brief パラメタの構造体名を取得(参照用)
      ///
      /// \return パラメタの構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring& param_storage (void) const
      { return cw_param_storage; }

      /// \brief パラメタの構造体名を取得(設定用)
      ///
      /// \return パラメタの構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& param_storage (void) { return cw_param_storage; }

      /// \brief 定数パラメタの構造体名を取得(参照用)
      ///
      /// \return 定数パラメタの構造体名の optional_refstring
      /// のconstなリファレンス
      ///
      /// \sa optional_refstring
      const optional_refstring cparam_storage (void) const
      { return cw_cparam_storage; }

      /// \brief 定数パラメタの構造体名を取得(設定用)
      ///
      /// \return 定数パラメタの構造体名の optional_refstring
      /// のリファレンス
      ///
      /// \sa optional_refstring
      optional_refstring& cparam_storage (void) { return cw_cparam_storage; }

      /// \brief ターゲット向け出力ヘッダファイルのリスト取得(参照用)
      ///
      /// \return ターゲット向け出力ヘッダファイルのリストの
      /// constなリファレンス
      ///
      /// \sa option_include_list_T
      const option_include_list_T& xc_include_headers (void) const
      { return cw_xc_include_headers; }

      /// \brief ターゲット向け出力ヘッダファイルのリスト取得(設定用)
      ///
      /// \return ターゲット向け出力ヘッダファイルのリストのリファレンス
      ///
      /// \sa option_include_list_T
      option_include_list_T& xc_include_headers (void)
      { return cw_xc_include_headers; }

      /// \brief 生成Cコード向け出力ヘッダファイルのリスト取得(参照用)
      ///
      /// \return 生成C向け出力ヘッダファイルのリストの
      /// constなリファレンス
      ///
      /// \sa option_include_list_T
      const option_include_list_T& c_include_headers (void) const
      { return cw_c_include_headers; }

      /// \brief 生成Cコード向け出力ヘッダファイルのリスト取得(設定用)
      ///
      /// \return 生成C向け出力ヘッダファイルのリストのリファレンス
      ///
      /// \sa option_include_list_T
      option_include_list_T& c_include_headers (void)
      { return cw_c_include_headers; }

    private:
      //
      // task list
      //

      /// \brief コア割り当てされた TaskSetList
      TaskSetList cw_core_assigned_tasksetlist;

      /// \brief コア割り当てされていないタスク用の TaskSetList
      TaskSetList cw_tasksetlist;

      //
      // Embedded Coder Texts and Datas
      //

      /// \brief ヘッダファイルの二重インクルード判定のテキスト情報
      text_list_T cw_header_preamble;

      /// \brief Real-Time Model Object用マクロのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_rt_macro;

      /// \brief Real-Time Model Objectの型宣言のテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_rt_type;

      /// \brief Real-Time Model Objectの更新コードのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_rt_code;

      /// \brief Real-Time Model Objectの初期化コードのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_rt_init;

      /// \brief 共通の実行コードのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_common;

      /// \brief 共通の初期化コードのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_common_init;

      /// \brief タスクのコードの先頭に出力するテキスト情報
      ///
      /// step関数の先頭に出力されている自動変数等のコード
      ///
      /// \sa text_list_T
      text_list_T cw_preamble;

      /// \brief 共通マクロのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_c_macro;

      /// \brief BLXMLのdeftypeタグのリスト
      ///
      /// \sa c_object_list_T
      c_object_list_T cw_c_deftype;

      /// \brief BLXMLのobjectタグのリスト
      ///
      /// \sa c_object_list_T
      c_object_list_T cw_c_object;

      /// \brief 間数名をキーとした関数情報の連想配列
      ///
      /// \sa function_map_T
      function_map_T cw_functions;

      /// \brief Real-Time Model Objectのコードから呼び出される関数のリ
      /// スト
      ///
      /// \sa function_ptr_list_T
      function_ptr_list_T cw_rt_code_func;

      /// \brief 共通コードから呼び出される関数のリスト
      ///
      /// \sa function_ptr_list_T
      function_ptr_list_T cw_common_func;

      /// \brief BLXMLのincludeタグのリスト
      ///
      /// \sa c_include_list_T
      c_include_list_T cw_include_file;

      /// \brief DataStore用の AutoStorageMap
      ///
      /// \sa AutoStorageMap
      AutoStorageMap cw_dsmem;

      /// \brief DataStoreの初期化コードのテキスト情報
      ///
      /// \sa text_list_T
      text_list_T cw_dsmem_init;

      //
      // AutoStorageMapのキーともなる構造体名
      //

      /// \brief 外部入力変数の構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_extin_storage;

      /// \brief 外部出力変数の構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_extout_storage;

      /// \brief ブロックシグナルの構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_signal_storage;

      /// \brief 状態変数の構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_state_storage;

      /// \brief Trigger Stateの構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_trigger_state_storage;

      /// \brief invariant block signalの構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_invsig_storage;

      /// \brief パラメタの構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_param_storage;

      /// \brief 定数パラメタの構造体名
      ///
      /// \sa optional_refstring
      optional_refstring cw_cparam_storage;

      //
      // コマンドラインオプション
      //

      /// \briefターゲット向け出力ヘッダファイルのリスト
      ///
      /// コマンドラインオプション -I で指定されたヘッダファイルのリスト
      ///
      /// \sa option_include_list_T
      option_include_list_T cw_xc_include_headers;

      /// \brief生成Cコード向け出力ヘッダファイルのリスト
      ///
      /// コマンドラインオプション -i で指定されたヘッダファイルのリスト
      ///
      /// \sa option_include_list_T
      option_include_list_T cw_c_include_headers;
    };

    /// \brief C_CodeWriterの静的オブジェクト
    extern C_CodeWriter c_codewriter;

    /// \brief BLXMLの部分配列の情報が誤っているかどうか
    extern bool bug_partial_array;

    /// \defgroup C_CODEWRITER_SUB Cコードライタサブルーチン
    ///
    /// \brief Cコードライタのサブルーチン
    ///
    /// @{

    /// \brief 変数の出力モード
    ///
    /// ユーザーが作成する必要がある部分のコメントの種別を選択する
    /// - VOUT_EXTIN
    ///   - 外部入力変数
    /// - VOUT_EXTOUT
    ///   - 外部出力変数
    enum var_out_type
      {
	VOUT_EXTIN, VOUT_EXTOUT
      };


    /// \brief 始点終点といった範囲のベクタ
    typedef std::vector < std::pair < int, int > > range_T;


    /// \brief 処理済みチェック用リスト(マーク用ベクタ)
    typedef std::vector< bool > mark_T;


    /// \brief CspNode を出力処理したかどうかを管理する連想配列
    ///
    /// - キーは CspNode のポインタ
    /// - 値は、任意のビットの値
    class CspNode_map_T
      : public std::map < CspNode*, unsigned long >
    {
    public:
      /// \brief 対象の CspNode の値の確認
      ///
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] mask 確認するビットのマスク値
      ///
      /// \retval 0 CspNode が未登録か、mask で指定されたビットが0

      /// CspNode の連想配列からその CspNode に設定された値を取り出し、
      /// 指定したマスク値のいずれかのビットが設定されていたらその値を
      /// 返す。
      /// CspNode がまだ未登録か、またはマスク値がビット設定されていな
      /// ければ0を返す
      int
      check (CspNode* node, unsigned long mask)
      {
	iterator i = find (node);
	if (i == end ())
	  {
	    insert (std::make_pair (node, mask));
	    return 0;
	  }

	if (i->second & mask)
	  return mask;

	i->second |= mask;
	return 0;
      }
    };


    /// \brief マクロプリプロセッサのdefine/undefine
    /// を出力するための文字列リスト
    class def_list_T
      : public std::list < std::string >
    {
    public:
      /// \brief 登録されている文字列全てにundefineを出力する
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \sa mcos_output_define()
      void undefine (std::ostream& os) const;
    };


    /// \brief DataStoreMemory用 の regex
    ///
    /// DataStoreMemory の正規表現から生成した regex
    extern std::tr1::regex datastore_memory_regex;


    /// \brief PulseGenerator用の regex
    ///
    /// PulseGenerator のコードを書き換えるための正規表現から作成された
    /// regex
    extern std::tr1::regex pulse_generator_code_regex;


    /// \brief タスクIDをコード生成時に使用するフォーマットにする
    ///
    /// \param[in] id タスクID
    ///
    /// \return 左に'0'をパディングした4桁の十進数にしたstring
    std::string mk_task_id (long id);


    /// \brief PulseGeneratorを書き換えるための正規表現を構築し
    /// regex を生成する
    ///
    /// \retval true 成功
    /// \retval false 失敗
    ///
    /// \sa pulse_generator_code_regex
    /// \sa check_pulse_generator_code()
    bool prepare_pulse_generator_regex (void);

    /// \brief PulseGeneratorのコードを調べ書き換えるべき文字列の範囲返す
    ///
    /// \param[in] code PulseGeneratorのコード
    /// \param[out] cmp_beg カウンタの比較演算の開始位置
    /// \param[out] cmp_end カウンタの比較演算の終了位置
    /// \param[out] duty_beg Dutyの開始位置
    /// \param[out] duty_end Dutyの終了位置
    /// \param[out] if_beg if文の開始位置
    /// \param[out] if_end if文の終了位置
    /// \param[out] amp 状態変数のampのメンバ名
    /// \param[out] period 状態変数のPeriodのメンバ名
    /// \param[out] duty 状態変数のDutyのメンバ名
    /// \param[out] counter 状態変数のカウンタのメンバ名
    ///
    /// \retval true PulseGeneratorの書き換え対象のコード
    /// \retval false そうでない
    ///
    /// PulseGeneratorのコードに pulse_generator_code_regex を適用し、
    /// PulseGeneratorの情報と、書き換えるコード位置を取得する
    ///
    /// \sa pulse_generator_code_regex
    /// \sa prepare_pulse_generator_regex
    bool
    check_pulse_generator_code (const char* code,
				int& cmp_beg, int& cmp_end,
				int& duty_beg, int& duty_end,
				int& if_beg, int& if_end,
				std::string& amp,
				std::string& period,
				std::string& duty,
				std::string& counter);


    /// \brief
    /// コード中のDataStoreのアクセスを見つけるため、
    /// 構造体名とメンバ名の正規表現から regex を作成する
    ///
    /// \retval true 成功
    /// \retval false エラー
    ///
    /// \sa datastore_memory_regex
    bool make_dsmem_regpat (void);


    /// \brief コードにから DataStore の書き換え範囲を探索する
    ///
    /// \param[in] code 対象のコード
    /// \param[out] list 書き換え対象の範囲
    ///
    /// \retval true DataStoreの書き換え対象のコード
    /// \retval false そうでない
    ///
    /// DataStoreの正規表現の regex を適用し、コード中の DataStoreがあれば、
    /// その書き換え範囲を返す
    ///
    /// \sa datastore_memory_regex
    bool check_datastore_code (const char* code, range_T& list);


    /// \brief
    /// ユーザーが作成する必要がある外部入出力がある旨のコメントの出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] type 出力するコメントの種類
    /// \param[in] ind インデント
    void generate_extvar_user_codes (std::ostream& os,
				     var_out_type type, int ind);


    /// \brief 処理済みマークをクリアする
    ///
    /// \param[in,out] mark 処理済みマークのベクタ
    /// \param[in] n マークの数の最大値
    ///
    /// \sa mark_T
    static inline void
    reset_mark (mark_T& mark, size_t n)
    {
      mark.clear();
      mark.resize (n, false);
    }

    /// \brief 関数定義を出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] fe 関数のエントリ
    /// \param[in] mark 処理済みマーク用ベクタ
    /// \param[in] outer タスク関数のスコープの外を示す真理値
    ///
    /// 関数の定義を出力する。ただし、出力済みならば出力しない。マルチ
    /// レートの処理の場合、Embedded Coderが生成する rate_scheduler()
    /// は不要なので出力しない。
    /// なお outer が false の場合は、gccのNested Functionとして出力す
    /// るため、static宣言があれば削除する
    void output_function_defn_code (std::ostream& os,
				    const function_ent_T* fe, mark_T& mark,
				    bool outer);


    /// \brief 関数の宣言を出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] fe 関数のエントリ
    /// \param[in] mark 処理済みマーク用ベクタ
    /// \param[in] outer タスク関数のスコープの外を示す真理値
    ///
    /// 関数の宣言を出力する。ただし、出力済みならば出力しない。マルチ
    /// レートの処理の場合、Embedded Coderが生成する rate_scheduler()
    /// は不要なので出力しない。
    /// なお outer が false の場合は、gccのNested Functionとして出力す
    /// るため、static宣言があれば削除する
    void output_function_decl_code (std::ostream& os,
				    const function_ent_T* fe, mark_T& mark,
				    bool outer);


    /// \brief
    /// 関数や変数に対するプリプロセッサの置換用マクロを出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] name 出力マクロ名
    /// \param[in] attr defineで定義するものに対して出力するコンパイラ用属性
    /// \param[in] postfix マクロ名に付与するもの
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// プリプロセッサの define を出力する。gcc の場合はNested
    /// Function等でタスク関数内に全てを局所化できるが、そうでないコン
    /// パイラを想定し、同名の関数をプリプロセッサのdefine/undefを行う
    /// 事で、コードを変更すること無く利用できるようにする。
    ///
    /// \sa def_list_T::undefine()
    void define_macro (std::ostream& os,
		       const std::string& name, const std::string& attr,
		       const std::string& postfix,
		       def_list_T* define_list = NULL);


    /// \brief 関数出力用のサブルーチン
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] funcs 関数リスト
    /// \param[in] mark 処理済みマークのベクタ
    /// \param[in] defn 定義を出力するかどうかの真理値
    ///            (trueなら定義、falseなら宣言)
    /// \param[in] ind インデント
    /// \param[in,out] comout コメントを出力するか否かの真理値のリファレンス
    /// \param[in] code_attr 定義するものに対して出力するコンパイラ属性
    /// \param[in] postfix プリプロセッサマクロで関数名につけるpostfix
    /// \param[in] outer タスク関数外かどうかを示す真理値
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// 指定された関数リストを出力する。また、関数から呼び出される関数
    /// についても出力する。
    void output_function_sub (std::ostream& os,
			      const function_ptr_list_T& funcs,
			      mark_T& mark, bool defn,
			      int ind, bool& comout,
			      const std::string& code_attr,
			      const std::string& postfix,
			      bool outer = false,
			      def_list_T* define_list = NULL);


    /// \brief タスクで使用する関数を出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode
    /// \param[in] mark 処理済みマークのベクタ
    /// \param[in] defn 定義を出力するかどうかの真理値
    ///            (trueなら定義、falseなら宣言)
    /// \param[in] ind インデント
    /// \param[in,out] comout コメントを出力するか否かの真理値のリファレンス
    /// \param[in] code_attr 定義するものに対して出力するコンパイラ属性
    /// \param[in] postfix プリプロセッサマクロで関数名につけるpostfix
    /// \param[in] outer タスク関数外かどうかを示す真理値
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// 処理済み関数リストをクリアし CspNode の関数リストを出力する
    ///
    /// \sa reset_mark
    /// \sa output_function_sub
    void generate_task_function (std::ostream& os, CspNode *node,
				 mark_T& mark, bool defn,
				 int ind, bool& comout,
				 const std::string& code_attr,
				 const std::string& postfix,
				 bool outer = false,
				 def_list_T* define_list = NULL);


    /// \brief
    /// Real-Time Model Objectの初期化コードをタスクのコードに出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] ind インデント
    void generate_task_rtw_initializer (std::ostream& os, int ind);


    /// \brief Real-Time Model Objectの定義をタスクのコードに出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] ind インデント
    /// \param[in] stdef 宣言時の型修飾(static)
    /// \param[in] data_attr 定義するものに対して出力するコンパイラのデータ属性
    /// \param[in] code_attr 定義するものに対して出力するコンパイラのコード属性
    /// \param[in] postfix プリプロセッサマクロで定義するものにつけるpostfix
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// Real-Time Model Objectの構造体、ポインタを出力する。postfixが指
    /// 定されている場合は、置換用マクロを出力する
    ///
    /// \sa def_list_T::undefine()
    void generate_task_rtw_context (std::ostream& os, int ind,
				    const std::string& stdef,
				    const std::string& data_attr,
				    const std::string& code_attr,
				    const std::string& postfix,
				    def_list_T* define_list = NULL);


    /// \brief イベントが必要な SubSystem の実行状態用変数を出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] ind インデント
    /// \param[in,out] cnmap CspNode の出力制御用連想配列
    ///
    /// イベントが必要な SubSystem 用の、実行状態を保持するための変数を
    /// 出力する。生成コードでは、この変数が0以外の時、ブロックのコード
    /// を実行するようにコード生成する
    ///
    /// \sa CspNode_map_T
    void generate_task_subsystem_variables (std::ostream& os, CspNode* node,
					    int ind,
					    CspNode_map_T* cnmap = NULL);


    /// \brief タスクの初期化コードを出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] ind インデント
    /// \param[in,out] cnmap CspNode の出力制御用連想配列
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    ///
    /// タスクに含まれる対象の CspNode の状態変数の初期化および
    /// CspNode の初期化コードを出力する
    int generate_task_context_initializer (std::ostream& os, CspNode* node,
					   int ind,
					   CspNode_map_T* cnmap = NULL);


    /// \brief Real-Time Model Objectの更新コードを出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] ind インデント
    ///
    /// Real-Time Model Objectの更新コードを出力する。ただしrate_scheduler
    /// の呼び出しはマルチレート時は行わない
    void generate_update_rtw (std::ostream& os, int ind);

    /// \brief Simulinkモデル用のヘッダファイルをmarkの前までinclude文を出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] inclist インクルードファイルのリスト
    /// \param[in,out] mark 開始位置入力、出力後停止した位置を返す
    ///
    /// Simulinkモデル用のヘッダファイルの前までinclude文を出力。
    /// モデル用ヘッダファイルはBLXMLに取り込まれているので、
    /// それは出力しない(その位置で停止し \a mark に返す)
    ///
    /// \a mark に-1を指定した場合、開始位置を0とする。
    /// つまりこの場合、 \a mark が -1 のままであれば、
    /// 停止しなかった事を意味する
    void generate_includes (std::ostream& os,
			    const c_include_list_T& inclist, int& mark);


    /// \brief 定義されている型の出力
    ///
    /// \param[in] os 出力ストリーム
    ///
    /// Simulinkモデル用のヘッダファイルおよびソースファイルに
    /// 定義されていた方を出力する
    void generate_deftype (std::ostream& os);


    /// \brief 利用者指定のinclude文の出力
    ///
    /// \param[in] os 出力ストリーム
    void generate_user_includes (std::ostream& os);


    /// \brief タスク関数のタスク実行部のコードを生成する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] blocklist 出力対象のブロックリスト
    /// \param[in] ind インデント
    /// \param[in] dsm_prefix DataStoreMemoryの状態変数のプレフィクス
    ///
    /// 以下を行う
    /// -# イベントが必要なSubSysteであれば SubSystemの実行状態の変数を0にする
    ///    - 後で実行可能か判定する
    /// -# タスクのコードを出力する
    ///    -# イベントが必要なSubSystemのブロックであれば、通知された実行状
    ///       態をチェックするコードを出力する
    ///    -# 断片化されたコードの区切りで、イベントが必要なSubSystemであれ
    ///       ば、実行状態の変数を1とする
    ///    -# 通常の断片化されたコードで、低レート化するPulseGeneratorのコー
    ///       ドの場合は、置換を行う
    ///    -# 通常の断片化されたコードで、DataStoreのコードであれば、共有さ
    ///       れたDataStoreにアクセスするようコードの置換を行う
    /// -# 実行終了のマクロを出力する
    ///
    /// blocklitがnull pointerなら全てのブロックが対象だが、
    /// そうでない場合はそれで指定されたブロックのコードを出力する
    void
    generate_task_exec_code (std::ostream& os, CspNode* node,
			     const csp_block_list_T* blocklist,
			     int ind, const std::string& dsm_prefix);


    /// \brief Update部のコードを出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象となる CspNode のポインタ
    /// \param[in] blocklist 出力対象のブロックリスト
    /// \param[in] ind インデント
    /// \param[in] do_rtm_out Real-Time Model Object
    /// の更新コードを出力するかどうか
    ///
    /// Delay等のUpdate部のコードを出力する。また、do_rtm_outに従って、
    /// マルチレートの場合のタスクの時間更新と、Real-Time Model Object
    /// の更新コードを出力する。
    /// なお、イベントが必要なSubSystemのブロックの場合はUpdateも
    /// SubSystemの実行状態に従う
    ///
    /// blocklitがnull pointerなら全てのブロックが対象だが、
    /// そうでない場合はそれで指定されたブロックのコードを出力する
    ///
    /// \sa generate_update_rtw()
    void
    generate_task_update_code (std::ostream& os, CspNode* node,
			       const csp_block_list_T* blocklist,
			       int ind, bool do_rtm_out = true);


    /// \brief スレッドのタスクの定義・宣言の出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] taskset タスク情報 TaskSet のポインタ
    /// \param[in] outer タスク関数外かどうかを示す真理値
    /// \param[in] postfix プリプロセッサマクロで定義するものにつけるpostfix
    /// \param[in] code_attr コード定義するものに対して出力するコンパイラ属性
    /// \param[in] data_attr データ定義するものに対して出力するコンパイラ属性
    /// \param[in] rodata_attr 読み込み専用のデータ定義するものに対して
    /// 出力するコンパイラ属性
    /// \param[in,out] define_list 出力マクロ名を登録するリスト
    ///
    /// \a outer がfalseの場合は、タスク関数内での定義を行う。\n
    /// \a outer がtrueの場合は、タスクの固有の変数等と関数が、タスク関数
    /// 以外の関数(SubSystemが関数として外出しされているような場合)から
    /// 参照できる必要がある場合には、それらはタスク関数外で定義される
    /// 必要がある。具体的には、他と衝突しないように参照する名前に
    /// \a postfix を付けたものをdefineし、後でundefする。
    ///
    /// \a code_attr と \a data_attr と \a rodata_attr は \a outer
    /// がtrueの場合に設定されている必要があり、
    /// falseの場合は空文字列が期待である
    ///
    /// \sa task_attr_T
    /// \sa generate_task_subsystem_variables()
    /// \sa generate_task_rtw_context()
    void generate_task_definitions (std::ostream& os, const TaskSet* taskset,
				    bool outer,
				    const std::string& postfix,
				    const std::string& code_attr,
				    const std::string& data_attr,
				    const std::string& rodata_attr,
				    def_list_T* define_list = NULL);

    /// @}

    /// @}
  }
}

#endif	// C_CODEWRITER_H
