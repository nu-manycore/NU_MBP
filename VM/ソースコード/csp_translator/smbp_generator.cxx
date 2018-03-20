// -*- mode: c++; coding: utf-8-unix; -*-

/// \file smbp_generator.cxx
///
/// \brief SystemBuilder向けコード生成

#include <string>
#include <deque>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>

#include <errno.h>
#include <string.h>

/// \brief weakシンボルにするかどうかのマクロ
///
/// ここでは SMBP_WEAK を空にしてweakシンボルを使わないようにする
#define SMBP_WEAK

#include "csp_translator.h"
#include "atk2_generator_i.h"

/// \addtogroup CSP_TRANSLATOR
///
/// @{

/// \defgroup SMBP_GENERATOR SystemBuilderコード生成
///
/// \brief SystemBuilder用コード生成機能
///
/// @{

/// \defgroup SMBP_GENERATOR_C_CODE SystemBuilder向けCコード生成本体
///
/// \brief SystemBuilder向けコード生成本体
///
/// @{

/// \brief ダブルバッファの面数
///
/// 他と同様に2面とする
#define DB_PLANE	2

/// @}

/// @}

/// @}

using namespace SimulinkModel::XSD;
using namespace Simulink::XML;

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup SMBP_GENERATOR
    ///
    /// @{

    /// \defgroup SMBP_GENERATOR_OPTIONS SystemBuilder向けコード生成用オプション変数
    ///
    /// \brief SystemBuilder用コード生成コマンドラインオプション変数
    ///
    /// \sa CSP_TRANSLATOR_MAIN_OPTIONS
    ///
    /// @{

    /// \brief このモデルのSystemBuilder用YAMLファイル
    const char* smbp_yaml = NULL;

    /// \brief このモデルの出力コードのファイル名のベース名
    const char* smbp_prefix = NULL;

    /// \brief longは64bit
    bool smbp_long_is_64bit = false;

    /// \brief realは64bit
    bool smbp_real_is_64bit = false;

    /// \brief 64bit longのアラインメントの指定。デフォルトでは4バイト
    size_t smbp_long64_align = 4;

    /// \brief doubleのアラインメントの指定。デフォルトでは4バイト
    size_t smbp_real64_align = 4;

    /// \brief 構造体のアラインメントの指定。デフォルトでは4バイト
    size_t smbp_struct_align = 4;

    /// \brief SytemBuilder用のメモリチャネルのアラインメント指定。
    /// デフォルトでは64バイト
    size_t smbp_mem_align = 64;

    /// @}

    /// \brief SystemBuilder用コード生成の名前空間
    namespace SMBP
    {
      /// \addtogroup SMBP_GENERATOR_C_CODE
      ///
      /// @{

      /// \brief NBCチャネルのプレフィクス
      static const std::string nbc_prefix = "nbc_";

      /// \brief BCチャネルのプレフィクス
      static const std::string bc_prefix = "bc_";

      /// \brief MEMチャネルのプレフィクス
      static const std::string mem_prefix = "mem_";

      /// \brief SystemBuilderのSWコアタイプ
      static std::string sw_core_type = "Nios";

      /// \brief SystemBuilderのHWコアタイプ
      static std::string hw_core_type = "FPGA";

      /// \brief SystemBuilderのHW_FREQ_MHZ
      static std::string hw_freq_mhz;

      /// \brief CyclicSyncのinit_startの値
      static std::string cyclic_sync_init_start = "true";

      /// \brief CyclicSyncのsw_base_nsecの値
      static std::string cyclic_sync_sw_base_nsec;

      /// \brief CyclicSyncのhw_base_nsecの値
      static std::string cyclic_sync_hw_base_nsec = "1000";

      /// \brief ダブルバッファの出力面の変数名を生成する
      ///
      /// \param[in] chan チャネル名
      ///
      /// \return 出力面の変数名
      inline std::string
      mk_out_side (const std::string& chan)
      {
	return std::string ("task__ch_" + chan + "_out_side");
      }

      /// \brief ダブルバッファの入力面の変数名を生成する
      ///
      /// \param[in] chan チャネル名
      ///
      /// \return 入力面の変数名
      inline std::string
      mk_in_side (const std::string& chan)
      {
	return std::string ("task__ch_" + chan + "_in_side");
      }

      /// \brief チャネルの次駆動時刻の変数名を生成する
      ///
      /// \param[in] chan チャネル名
      ///
      /// \return チャネルの時刻の変数名
      inline std::string
      mk_chan_time (const std::string& chan)
      {
	return  std::string ("task__ch_" + chan + "_time");
      }

      /// \brief 要素のベクタ
      class ElemVec
	: public std::vector < std::string >
      {
      public:
	/// \brief デフォルトコンストラクタ
	ElemVec () {}

	/// \brief 二つメンバ用コンストラクタ
	ElemVec (const std::string& i, const std::string& j)
	{
	  push_back (i);
	  push_back (j);
	}
      };

      /// \brief Embedded Coderの型と、サイズおよびアラインメントの情報
      class SizeAlignInfo
      {
      public:
	/// \brief 型の特性
	enum TypeInfo
	  {
	    /// \brief 整数型。intに対して型変換なし
	    TYPE_INT,

	    /// \brief 浮動小数点型。intに対して型変換あり
	    TYPE_FLOAT
	  };

	/// \brief SizeAlignInfo のデフォルトコンストラクタ
	SizeAlignInfo ()
	  : sai_size (4), sai_allign (4), sai_typeinfo (TYPE_INT)
	{
	}

	/// \brief SizeAlignInfo のコンストラクタ
	///
	/// \param[in] size サイズ(バイト)
	/// \param[in] align アラインメント(バイト)
	/// \param[in] t 型の特性
	SizeAlignInfo (size_t size, size_t align, TypeInfo t = TYPE_INT)
	  : sai_size (size), sai_allign (align), sai_typeinfo (t)
	{
	}

	/// \brief SizeAlignInfo のコンストラクタ
	///
	/// \param[in] size サイズ(バイト)
	/// \param[in] align アラインメント(バイト)
	/// \param[in] elem 要素のベクタ
	/// \param[in] t 型の特性
	SizeAlignInfo (size_t size, size_t align, const ElemVec& elem,
		       TypeInfo t = TYPE_INT)
	  : sai_size (size), sai_allign (align), sai_typeinfo (t),
	    sai_elements (elem)
	{
	}

	/// \brief SizeAlignInfo のコピーコンストラクタ
	///
	/// \param[in] sai SizeAlignInfoの参照
	SizeAlignInfo (const SizeAlignInfo& sai)
	  : sai_size (sai.sai_size), sai_allign (sai.sai_allign),
	    sai_typeinfo (sai.sai_typeinfo),
	    sai_elements (sai.sai_elements)
	{
	}

	/// \brief 型のサイズの取得
	///
	/// \return サイズ(バイト)
	size_t size (void) const { return sai_size; }

	/// \brief 型のサイズ設定
	///
	/// \param[in] s サイズ(バイト)
	///
	/// \return このインスタンスの参照
	SizeAlignInfo& size (size_t s) { sai_size = s; return *this; }

	/// \brief 型のアラインメント取得
	///
	/// \return アラインメント(バイト)
	size_t align (void) const { return sai_allign; }

	/// \brief 型のアラインメント設定
	///
	/// \param[in] a アラインメント(バイト)
	///
	/// \return このインスタンスの参照
	SizeAlignInfo& align (size_t a) { sai_allign = a; return *this; }

	/// \brief 型の要素の取得(参照用)
	///
	/// \return 要素のベクタ
	const ElemVec& elements (void) const { return sai_elements; }

	/// \brief 型の要素の取得(設定用)
	///
	/// \return 要素のベクタ
	ElemVec& elements (void) { return sai_elements; }

	/// \brief 型の要素の設定
	///
	/// \param[in] e 要素
	///
	/// \return このインスタンスの参照
	SizeAlignInfo& elements (const std::string& e)
	{
	  sai_elements.push_back (e); return *this;
	}

	/// \brief 型のタイプ情報の取得
	///
	/// \return 型のタイプ情報
	TypeInfo typeinfo (void) const { return sai_typeinfo; }

	/// \brief 型のタイプ情報の設定
	///
	/// \param[in] t タイプ情報
	///
	/// \return このインスタンスの参照
	SizeAlignInfo&
	typeinfo (TypeInfo t)
	{
	  sai_typeinfo = t;
	  return *this;
	}

	/// \brief 型のサイズとアラインメントの設定
	///
	/// \param[in] s サイズ(バイト)
	/// \param[in] a アラインメント(バイト)
	/// \param[in] t 型の特性
	SizeAlignInfo& set (size_t s, size_t a, TypeInfo t = TYPE_INT)
	{
	  sai_size = s;
	  sai_allign = a;
	  sai_typeinfo = t;
	  return *this;
	}

	/// \brief 型のサイズとアラインメントの設定
	///
	/// \param[in] s サイズ(バイト)
	/// \param[in] a アラインメント(バイト)
	/// \param[in] e 要素のベクタ
	/// \param[in] t 型の特性
	SizeAlignInfo& set (size_t s, size_t a, const ElemVec& e,
			    TypeInfo t = TYPE_INT)
	{
	  sai_size = s;
	  sai_allign = a;
	  sai_typeinfo = t;
	  sai_elements = e;
	  return *this;
	}

	/// \brief 型のサイズとアラインメントの設定
	///
	/// \param[in] s SizeAlignInfo の参照
	///
	/// \return このインスタンスの参照
	SizeAlignInfo& set (const SizeAlignInfo& s)
	{
	  return set (s.sai_size, s.sai_allign, s.sai_elements,
		      s.sai_typeinfo);
	}

	/// \brief 代入オペレータ
	///
	/// \param[in] s SizeAlignInfo の参照
	///
	/// \return このインスタンスの参照
	SizeAlignInfo& operator= (const SizeAlignInfo& s) { return set (s); }

	/// \brief 比較オペレータ
	///
	/// \param[in] s SizeAlignInfo の参照
	///
	/// \retval true 同じ値のSizeAlignInfo
	/// \retval false 異なるSizeAlignInfo
	bool operator== (const SizeAlignInfo& s) const
	{
	  if (sai_size != s.sai_size
	      || sai_allign != s.sai_allign
	      || sai_elements != s.sai_elements
	      || sai_typeinfo != s.sai_typeinfo)
	    return false;

	  const std::string* ttype = type ();
	  const std::string* stype = s.type ();

	  return ((ttype == NULL && stype == NULL)
		  || (ttype != NULL && stype != NULL && *stype == *ttype));
	}

	/// \brief SystemBuilderのチャネルの特定の型かどうか
	///
	/// SystemBuiler の型の場合、継承されたクラスの関数が使われる
	///
	/// \retval true SystemBuilderの型
	/// \retval false それ以外
	virtual bool has_sb_type (void) const { return false; }

	/// \brief SystemBuilderの型名の取得
	///
	/// \return 型名称のポインタ
	virtual const std::string* type (void) const { return NULL; }

      private:
	/// \brief サイズ。バイト単位
	size_t sai_size;

	/// \brief アラインメント。バイト単位
	size_t sai_allign;

	/// \brief 型の特性
	TypeInfo sai_typeinfo;

	/// \brief 要素
	ElemVec sai_elements;
      };

      /// \brief SystemBuilderの型とサイズの情報
      class SBSizeAlignInfo : public SizeAlignInfo
      {
      public:
	/// \brief SBSizeAlignInfo のデフォルトコンストラクタ
	SBSizeAlignInfo () : ssai_flag (false) {}

	/// \brief SBSizeAlignInfo のコンストラクタ
	///
	/// \param[in] t 型の名称
	/// \param[in] s サイズ(バイト)
	/// \param[in] a アラインメント(バイト)
	/// \param[in] f has_sb_type(void) で返す真理値
	SBSizeAlignInfo (const std::string& t, size_t s, size_t a,
			 bool f)
	  : SizeAlignInfo (s, a), ssai_type (t), ssai_flag (f)
	{
	}

	/// \brief SBSizeAlignInfo のコンストラクタ
	///
	/// \param[in] t 型の名称
	/// \param[in] s SizeAlignInfo の参照
	/// \param[in] f has_sb_type(void) で返す真理値
	SBSizeAlignInfo (const std::string& t, const SizeAlignInfo& s,
			 bool f)
	  : SizeAlignInfo (s), ssai_type (t), ssai_flag (f)
	{
	}

	/// \brief SystemBuilderのチャネルの特定の型かどうかを返す
	///
	/// SizeAlignInfo の
	/// SizeAlignInfo::has_sb_type(void) を上書きする
	///
	/// \retval true SystemBuilderで扱える型
	/// \retval false SystemBuilderで扱えない
	bool has_sb_type (void) const { return ssai_flag; }

	/// \brief SystemBuilderの型かどうかを設定する
	///
	/// \return このインスタンスの参照
	///
	/// \sa SizeAlignInfo::has_sb_type(void)
	SBSizeAlignInfo& has_sb_type (bool f) { ssai_flag = f; return *this; }

	/// \brief SystemBuilderのチャネルの特定の型の名称の取得
	///
	/// SizeAlignInfo の
	/// SizeAlignInfo::has_sb_type(void) を上書きする
	///
	/// \return 型の名称
	///
	/// \sa SizeAlignInfo::type(void)
	const std::string* type (void) const { return &ssai_type; }

	/// \brief 型の名称設定
	///
	/// \param[in] t 型の名称
	///
	/// \return このインスタンスの参照
	SBSizeAlignInfo&
	type (const std::string& t)
	{
	  ssai_type = t; return *this;
	}

      private:
	/// \brief 型の名称
	std::string ssai_type;

	/// \brief SystemBuilderの通信可能か型かどうか
	bool ssai_flag;
      };

      /// \brief SystemBuilderの型のマップのクラス
      class SystemBuilderTypeMap
	: public std::map < std::string, SBSizeAlignInfo >
      {
      public:
	// SystemBulderの、指定した型の情報を取得
	SBSizeAlignInfo* get (const std::string& t);

	// SystemBulderの、指定した型の情報の変更
	SBSizeAlignInfo* set (const std::string& t, size_t s, size_t a,
			      SizeAlignInfo::TypeInfo
			      ti = SizeAlignInfo::TYPE_INT);

	// SystemBulderの、指定した型の情報の変更
	SBSizeAlignInfo* set (const std::string& t, size_t s, size_t a,
			      const ElemVec& e,
			      SizeAlignInfo::TypeInfo
			      ti = SizeAlignInfo::TYPE_INT);
      };

      /// \brief SystemBulderの、指定した型の情報を取得
      ///
      /// \param[in] t 型の名称
      ///
      /// \retval NULL 指定した型名称がない
      /// \retval それ以外 指定した型名称に対する型情報
      SBSizeAlignInfo*
      SystemBuilderTypeMap::get (const std::string& t)
      {
	SystemBuilderTypeMap::iterator i = find (t);
	if (i == end ())
	  return NULL;
	return &i->second;
      }

      /// \brief SystemBulderの、指定した型の情報の変更
      ///
      /// \param[in] t 型の名称
      /// \param[in] s サイズ(バイト)
      /// \param[in] a アラインメント(バイト)
      /// \param[in] ti 型のタイプ情報
      ///
      /// \return 指定した型名称に対する型情報
      SBSizeAlignInfo*
      SystemBuilderTypeMap::set (const std::string& t, size_t s, size_t a,
				 SizeAlignInfo::TypeInfo ti)
      {
	SBSizeAlignInfo* v = get (t);
	if (v)
	  v->set (s, a, ti);
	else
	  {
	    SBSizeAlignInfo& n = (*this)[t];
	    n.type (t).set (s, a, ti);
	    v = &n;
	  }
	return v;
      }

      /// \brief SystemBulderの、指定した型の情報の変更
      ///
      /// \param[in] t 型の名称
      /// \param[in] s サイズ(バイト)
      /// \param[in] a アラインメント(バイト)
      /// \param[in] e ElemVec の参照
      /// \param[in] ti 型のタイプ情報
      ///
      /// \return 指定した型名称に対する型情報
      SBSizeAlignInfo*
      SystemBuilderTypeMap::set (const std::string& t, size_t s, size_t a,
				 const ElemVec& e, SizeAlignInfo::TypeInfo ti)
      {
	SBSizeAlignInfo* v = get (t);
	if (v)
	  v->set (s, a, e, ti);
	else
	  {
	    SBSizeAlignInfo& n = (*this)[t];
	    n.type (t).set (s, a, e, ti);
	    v = &n;
	  }
	return v;
      }

      /// \brief EmbeddedCoderが使用する型の情報
      class EmbeddedCoderType
      {
      public:
	/// \brief EmbeddedCoderType のデフォルトコンストラクタ
	EmbeddedCoderType () : ect_sai (NULL) {}

	/// \brief EmbeddedCoderType のコンストラクタ
	///
	/// \param[in] typenam EmbeddedCoderの型名称
	/// \param[in] sai SystemBuilderの型
	EmbeddedCoderType (const std::string& typenam,
			   const SizeAlignInfo* sai)
	  : ect_type (typenam), ect_sai (sai)
	{
	}

	/// \brief EmbeddedCoderの型名称の取得
	///
	/// \return EmbeddedCoderの型名称
	const std::string& type (void) const { return ect_type; }

	/// \brief EmbeddedCoderの型名称の設定
	///
	/// \param[in] t EmbeddedCoderの型名称
	///
	/// \return このインスタンスの参照
	EmbeddedCoderType& type (const std::string& t)
	{
	  ect_type = t;
	  return *this;
	}

	/// \brief SystemBuilderの型の取得
	///
	/// \return SizeAlignInfo のポインタ
	const SizeAlignInfo* sizealign (void) const { return ect_sai; }

	/// \brief SystemBuilderの型の取得
	///
	/// \param[in] t SizeAlignInfo のポインタ
	///
	/// \return このインスタンスの参照
	EmbeddedCoderType& sizealign (const SizeAlignInfo* t)
	{
	  ect_sai = t;
	  return *this;
	}

      private:
	/// \brief EmbeddedCoderの型名称
	std::string ect_type;

	/// \brief SizeAlignInfo のポインタ
        const SizeAlignInfo* ect_sai;
      };

      /// \brief EmbeddedCoderの型のマップのクラス
      class EmbeddedCoderTypeMap
	: public std::map < std::string, EmbeddedCoderType >
      {
      public:
	// EmbeddedCoderの、指定した型の取得
	EmbeddedCoderType* get (const std::string& t);

	// EmbeddedCoderの、指定した型の設定
	EmbeddedCoderType* set (const std::string& t,
				const SizeAlignInfo* sai);
      };

      /// \brief EmbeddedCoderの、指定した型の取得
      ///
      /// \param[in] t EmbeddedCoderの型名称
      ///
      /// \retval NULL 型名称がない
      /// \retval それ以外 型名称に対応する型
      EmbeddedCoderType*
      EmbeddedCoderTypeMap::get (const std::string& t)
      {
	EmbeddedCoderTypeMap::iterator i = find (t);
	if (i == end ())
	  return NULL;
	return &i->second;
      }

      /// \brief EmbeddedCoderの、指定した型の設定
      ///
      /// \param[in] t EmbeddedCoderの型名称
      /// \param[in] sai SizeAlignInfo のポインタ
      ///
      /// \return 型名称に対応する型のポインタ
      EmbeddedCoderType*
      EmbeddedCoderTypeMap::set (const std::string& t,
				 const SizeAlignInfo* sai)
      {
	EmbeddedCoderType* v = get (t);
	if (v)
	  v->type (t).sizealign (sai);
	else
	  v = &(*this)[t].type (t).sizealign (sai);
	return v;
      }

      /// \brief 作成された SizeAlignInfo の格納用クラス
      class SizeAlignInfoList
	: public std::list< SizeAlignInfo >
      {
      public:
	/// \brief 一致する SizeAlignInfo を探す
	///
	/// \param[in] sai SizeAlignInfo の参照
	///
	/// \retval NULL 一致する SizeAlignInfo がない
	/// \retval それ以外 一致する SizeAlignInfo のポインタ
	SizeAlignInfo*
	match (const SizeAlignInfo& sai)
	{
	  iterator i = std::find (begin (), end (), sai);
	  if (i == end ())
	    return NULL;
	  return &*i;
	}

	/// \brief 指定した SizeAlignInfo の登録されているポインタを返す
	///
	/// \param[in] sai SizeAlignInfo の参照
	///
	/// \return 登録されている SizeAlignInfo のポインタ
	SizeAlignInfo*
	get (const SizeAlignInfo& sai)
	{
	  SizeAlignInfo* v = match (sai);
	  if (v)
	    return v;
	  push_back (sai);
	  SizeAlignInfo& nv = back ();
	  return &nv;
	}

	/// \brief 指定した SizeAlignInfo の登録されているポインタを返す
	///
	/// \param[in] s サイズ
	/// \param[in] a アライン
	/// \param[in] e ElemVec の参照
	/// \param[in] t 型の特性
	///
	/// \return 登録されている SizeAlignInfo のポインタ
	SizeAlignInfo*
	get (size_t s, size_t a, const ElemVec& e,
	     SizeAlignInfo::TypeInfo t = SizeAlignInfo::TYPE_INT)
	{
	  return get (SizeAlignInfo (s, a, e, t));
	}
      };

      /// \brief Embedded Coderの型名と SizeAlignInfo のテーブル
      struct ec_typeinfo_type
      {
	/// \brief 型名の参照
	const std::string& type;

	/// \brief SizeAlignInfo の参照
	const SizeAlignInfo& sai;
      };

      //
      // データ型に関する定義
      //

      /// \brief SystemBuilderの型のマップ
      static SystemBuilderTypeMap sb_sizealign;

      /// \brief EmbeddedCoderの型のマップ
      static EmbeddedCoderTypeMap ec_sizealign;

      /// \brief Cのcharのリテラル
      static const std::string c_char ("char");

      /// \brief Cのshortのリテラル
      static const std::string c_short ("short");

      /// \brief Cのintのリテラル
      static const std::string c_int ("int");

      /// \brief Cのlongのリテラル
      static const std::string c_long ("long");

      /// \brief Cのfloatのリテラル
      static const std::string c_float ("float");

      /// \brief Cのdoubleのリテラル
      static const std::string c_double ("double");

      /// \brief Cのunsigned charのリテラル
      static const std::string c_uchar ("unsigned char");

      /// \brief Cのunsigned shortのリテラル
      static const std::string c_ushort ("unsigned short");

      /// \brief Cのunsigned intのリテラル
      static const std::string c_uint ("unsigned int");

      /// \brief Cのunsigned longのリテラル
      static const std::string c_ulong ("unsigned long");

      //
      // Embedded Coderのrtwtypes.hの型名(SystemBuilderで通信可のもの)
      //

      /// \brief Embedded Codeのrtwtypes.hのint8_T。signed charである
      static const std::string ec_i8T ("int8_T");

      /// \brief Embedded Codeのrtwtypes.hのint16_T。shortである
      static const std::string ec_i16T ("int16_T");

      /// \brief Embedded Codeのrtwtypes.hのint32_T。intである
      static const std::string ec_i32T ("int32_T");

      /// \brief Embedded Codeのrtwtypes.hのuint8_T。unsigned charである
      static const std::string ec_ui8T ("uint8_T");

      /// \brief Embedded Codeのrtwtypes.hのuint16_T。unsigned shortである
      static const std::string ec_ui16T ("uint16_T");

      /// \brief Embedded Codeのrtwtypes.hのuint32_T。unsigned intである
      static const std::string ec_ui32T ("uint32_T");

      /// \brief Embedded Codeのrtwtypes.hのreal32_T。floatである
      static const std::string ec_r32T ("real32_T");

      /// \brief Embedded Codeのrtwtypes.hのboolean_T。unsigned charである
      static const std::string ec_blT ("boolean_T");

      /// \brief Embedded Codeのrtwtypes.hのint_T。intである。
      static const std::string ec_iT ("int_T");

      /// \brief Embedded Codeのrtwtypes.hのuint_T。unsigned intである。
      static const std::string ec_uiT ("uint_T");

      /// \brief Embedded Codeのrtwtypes.hのchar_T。charである。
      static const std::string ec_cT ("char_T");

      /// \brief Embedded Codeのrtwtypes.hのuchar_T。unsigned charである。
      static const std::string ec_ucT ("uchar_T");

      /// \brief Embedded Codeのrtwtypes.hのbyte_T。charである。
      static const std::string ec_BT ("byte_T");

      //
      // Embedded Coderのrtwtypes.hの型名(SystemBuilderで通信不可のもの)
      //

      /// \brief Embedded Codeのrtwtypes.hのtime_T。doubleである。
      static const std::string ec_tT ("time_T");

      /// \brief Embedded Codeのrtwtypes.hのreal64_T。doubleである。
      static const std::string ec_r64T ("real64_T");

      /// \brief Embedded Codeのrtwtypes.hのulonglong_T。64bitである。
      static const std::string ec_ullT ("ulonglong_T");

      /// \brief Embedded Codeのrtwtypes.hのcreal32_T。float x2 である。
      static const std::string ec_cr32T ("creal32_T");

      /// \brief Embedded Codeのrtwtypes.hのcreal64_T。double x2 である。
      static const std::string ec_cr64T ("creal64_T");

      /// \brief Embedded Codeのrtwtypes.hのcint8_T。int8_T x2 である。
      static const std::string ec_ci8T ("cint8_T");

      /// \brief Embedded Codeのrtwtypes.hのcint16_T。int16_T x2 である。
      static const std::string ec_ci16T ("cint16_T");

      /// \brief Embedded Codeのrtwtypes.hのcint32_T。int32_T x2 である。
      static const std::string ec_ci32T ("cint32_T");

      /// \brief Embedded Codeのrtwtypes.hのcint64_T。int64_T x2 である。
      static const std::string ec_ci64T ("cint64_T");

      /// \brief Embedded Codeのrtwtypes.hのcuint8_T。uint8_T x2 である。
      static const std::string ec_cui8T ("cuint8_T");

      /// \brief Embedded Codeのrtwtypes.hのcuint16_T。uint16_T x2 である。
      static const std::string ec_cui16T ("cuint16_T");

      /// \brief Embedded Codeのrtwtypes.hのcuint32_T。uint32_T x2 である。
      static const std::string ec_cui32T ("cuint32_T");

      /// \brief Embedded Codeのrtwtypes.hのcuint64_T。uint64_T x2 である。
      static const std::string ec_cui64T ("cuint64_T");

      //
      // Embedded Coderのrtwtypes.hの型名(ターゲットにより異なる)
      //

      /// \brief Embedded Codeのrtwtypes.hのint64_T。longである。
      static const std::string ec_i64T ("int64_T");

      /// \brief Embedded Codeのrtwtypes.hのuint64_T。unsigned longである。
      static const std::string ec_ui64T ("uint64_T");

      /// \brief Embedded Codeのrtwtypes.hのulong_T。unsigned longである。
      static const std::string ec_ulT ("ulong_T");

      /// \brief Embedded Codeのrtwtypes.hのreal_T。floatまたはdoubleである。
      static const std::string ec_rT ("real_T");

      /// \brief Embedded Codeのrtwtypes.hのcreal_T。real_T x2である。
      static const std::string ec_crT ("creal_T");

      //
      // 8, 16, 32, 64bitの単一の型、および構造体のサイズ
      //

      /// \brief Embedded Coderの複素数の要素
      static const ElemVec ec_imaginary ("re", "ie");

      /// \brief 単一の8bitの型
      static const SizeAlignInfo ec_u8Tinfo (1, 1); // unit (8bit)

      /// \brief 単一の16bitの型
      static const SizeAlignInfo ec_u16Tinfo (2, 2); // unit (16bit)

      /// \brief 単一の32bitの型
      static const SizeAlignInfo ec_u32Tinfo (4, 4); // unit (32bit)

      /// \brief 単一の64bitの型
      static SizeAlignInfo ec_u64Tinfo (8, 4); // unit (64bit)

      /// \brief 32bitの浮動小数点
      static SizeAlignInfo ec_r32Tinfo (4, 4, SizeAlignInfo::TYPE_FLOAT);

      /// \brief 64bitの浮動小数点
      static SizeAlignInfo ec_r64Tinfo (8, 4, SizeAlignInfo::TYPE_FLOAT);

      /// \brief 8bitの構造体の型
      static const SizeAlignInfo ec_s8Tinfo (1, 1, ec_imaginary);

      /// \brief 16bitの構造体の型
      static const SizeAlignInfo ec_s16Tinfo (2, 2, ec_imaginary);

      /// \brief 32bitの構造体の型
      static const SizeAlignInfo ec_s32Tinfo (4, 4, ec_imaginary);

      /// \brief 64bitの構造体の型
      static SizeAlignInfo ec_s64Tinfo (8, 4, ec_imaginary);

      /// \brief 64bitの浮動小数点の構造体の型
      static SizeAlignInfo ec_sr32Tinfo (8, 4, ec_imaginary,
					 SizeAlignInfo::TYPE_FLOAT);

      /// \brief 64bitの浮動小数点の構造体の型
      static SizeAlignInfo ec_sr64Tinfo (8, 4, ec_imaginary,
					 SizeAlignInfo::TYPE_FLOAT);

      /// \brief Embedded Coderでターゲットに依存しない型
      static const ec_typeinfo_type ec_typeinfo[] = {
	{ ec_tT, ec_u64Tinfo, },     // 64bit (time_T)
	{ ec_r64T, ec_r64Tinfo, },   // 64bit (real64_T)
	{ ec_ullT, ec_u64Tinfo, },   // 64bit (unsigned long long)
	{ ec_cr32T, ec_s32Tinfo, },  // 32bit x 2 (creal32_T)
	{ ec_cr64T, ec_sr64Tinfo, }, // 64bit x 2 (creal64_T)
	{ ec_ci8T, ec_s8Tinfo, },    // 8bit x 2 (cint8_T)
	{ ec_ci16T, ec_s16Tinfo, },  // 16bit x 2 (cint16_T)
	{ ec_ci32T, ec_s32Tinfo, },  // 32bit x 2 (cint32_T)
	{ ec_cui8T, ec_s8Tinfo, },   // 8bit x 2 (cuint8_T)
	{ ec_cui16T, ec_s16Tinfo, }, // 16bit x 2 (cuint16_T)
	{ ec_cui32T, ec_s32Tinfo, }, // 32bit x 2 (cuint32_T)
      };

      /// \brief SizeAlignInfo の登録用リスト
      ///
      /// これにはあらかじめ想定されるものが予約登録され、
      /// ないものは新規登録される
      static SizeAlignInfoList sizealign_list;

      /// \brief 型に関するのデフォルトの初期化
      static void
      sizealign_default_init (void)
      {
	{
	  // long64, real64のアラインメント設定
	  ec_u64Tinfo.align (smbp_long64_align);
	  ec_r64Tinfo.align (smbp_real64_align);

	  // 64bitの複素数型の構造体のアラインメント設定。
	  // 構造体のアラインメントと型のアラインメントの大きい方を選択
	  size_t cuint64_t_align = ((smbp_long64_align > smbp_struct_align)
				    ? smbp_long64_align : smbp_struct_align);
	  size_t creal64_t_align = ((smbp_real64_align > smbp_struct_align)
				    ? smbp_real64_align : smbp_struct_align);
	  ec_s64Tinfo.align (cuint64_t_align);
	  ec_sr64Tinfo.align (creal64_t_align);
	}

	// 予約した型の登録
	sizealign_list.push_back (ec_u8Tinfo);   // 8bitの型
	sizealign_list.push_back (ec_u16Tinfo);  // 16bitの型
	sizealign_list.push_back (ec_u32Tinfo);  // 32bitの型
	sizealign_list.push_back (ec_u64Tinfo);  // 64bitの型
	sizealign_list.push_back (ec_r32Tinfo);  // float
	sizealign_list.push_back (ec_r64Tinfo);  // double
	sizealign_list.push_back (ec_s8Tinfo);	 // 8bit複素数
	sizealign_list.push_back (ec_s16Tinfo);  // 16bit複素数
	sizealign_list.push_back (ec_s32Tinfo);  // 32bit複素数
	sizealign_list.push_back (ec_s64Tinfo);  // 64bit複素数
	sizealign_list.push_back (ec_sr32Tinfo); // 32bit浮動小数点複素数
	sizealign_list.push_back (ec_sr64Tinfo); // 64bit浮動小数点複素数

	// SystemBuilderの型の初期化
	SizeAlignInfo&
	  sai_i8 = sb_sizealign[c_char].type (c_char).set (ec_u8Tinfo);
	SizeAlignInfo&
	  sai_i16 = sb_sizealign[c_short].type (c_short).set (ec_u16Tinfo);
	SizeAlignInfo&
	  sai_i32 = sb_sizealign[c_int].type (c_int).set (ec_u32Tinfo);
	SizeAlignInfo&
	  sai_u8 = sb_sizealign[c_uchar].type (c_uchar).set (ec_u8Tinfo);
	SizeAlignInfo&
	  sai_u16 = sb_sizealign[c_ushort].type (c_ushort).set (ec_u16Tinfo);
	SizeAlignInfo&
	  sai_u32 = sb_sizealign[c_uint].type (c_uint).set (ec_u32Tinfo);
	SizeAlignInfo&
	  sai_float = sb_sizealign[c_float].has_sb_type (true)
	  .type (c_float).set (ec_r32Tinfo);
	SizeAlignInfo&
	  sai_double = sb_sizealign[c_double].type (c_double).set (ec_r64Tinfo);

	if (!smbp_long_is_64bit)
	  {
	    sb_sizealign[c_long].type (c_int).set (ec_u32Tinfo);
	    sb_sizealign[c_ulong].type (c_uint).set (ec_u32Tinfo);
	  }
	else
	  {
	    sb_sizealign[c_long].type (c_long).set (ec_u64Tinfo);
	    sb_sizealign[c_ulong].type (c_ulong).set (ec_u64Tinfo);
	  }

	// Embedded Coderの8/16/32bitの型とSystemBuilderの型のペアリング
	ec_sizealign[ec_i8T].type (ec_i8T).sizealign (&sai_i8);
	ec_sizealign[ec_i16T].type (ec_i16T).sizealign (&sai_i16);
	ec_sizealign[ec_i32T].type (ec_i32T).sizealign (&sai_i32);
	ec_sizealign[ec_ui8T].type (ec_ui8T).sizealign (&sai_u8);
	ec_sizealign[ec_ui16T].type (ec_ui16T).sizealign (&sai_u16);
	ec_sizealign[ec_ui32T].type (ec_ui32T).sizealign (&sai_u32);
	ec_sizealign[ec_r32T].type (ec_r32T).sizealign (&sai_float);

	ec_sizealign[ec_blT].type (ec_blT).sizealign (&sai_u8);
	ec_sizealign[ec_iT].type (ec_iT).sizealign (&sai_i32);
	ec_sizealign[ec_uiT].type (ec_uiT).sizealign (&sai_u32);
	ec_sizealign[ec_cT].type (ec_cT).sizealign (&sai_i8);
	ec_sizealign[ec_ucT].type (ec_ucT).sizealign (&sai_u8);
	ec_sizealign[ec_BT].type (ec_BT).sizealign (&sai_i8);

	// Embedded Coderの型の初期化
	for (size_t n = 0;
	     n < sizeof ec_typeinfo / sizeof ec_typeinfo[0]; n++)
	  {
	    const ec_typeinfo_type& t = ec_typeinfo[n];
	    ec_sizealign[t.type].type (t.type).sizealign (&t.sai);
	  }

	//
	// Embedded Coder の　long の型に関わる初期化
	//

	const SizeAlignInfo* uli; // unit
	const SizeAlignInfo* sli; // struct

	if (smbp_long_is_64bit)
	  {
	    uli = &ec_u64Tinfo;
	    sli = &ec_s64Tinfo;
	  }
	else
	  {
	    uli = &sai_i32;
	    sli = &ec_s32Tinfo;
	  }

	// ulonng_T
	ec_sizealign[ec_ulT].type (ec_ulT).sizealign (uli);

	// int64_T/uint64_T
	ec_sizealign[ec_i64T].type (ec_i64T).sizealign (uli);
	ec_sizealign[ec_ui64T].type (ec_ui64T).sizealign (uli);

	// cint64_T/cuint64_T
	ec_sizealign[ec_ci64T].type (ec_ci64T).sizealign (sli);
	ec_sizealign[ec_cui64T].type (ec_cui64T).sizealign (sli);

	//
	// Embedded Coder の real_T の型に関わる初期化
	//

	const SizeAlignInfo* uri; // unit
	const SizeAlignInfo* sri;

	if (smbp_real_is_64bit)
	  {
	    // doubleのalignを考慮した構造体
	    uri = &sai_double;
	    sri = &ec_sr64Tinfo;
	  }
	else
	  {
	    uri = &sai_float;
	    sri = &ec_s32Tinfo;
	  }

	// real_T
	ec_sizealign[ec_rT].type (ec_rT).sizealign (uri);

	// creal_T
	ec_sizealign[ec_crT].type (ec_crT).sizealign (sri);
      }

      using namespace Support::YAML;

      /// \brief YAML の型の "element" の処理
      ///
      /// \param[in] elem ElemVec の参照
      /// \param[in] mkey 主キー名
      /// \param[in] seq YAMLSeq の参照
      static int
      yaml_element_get (ElemVec& elem,
			const std::string& mkey, const YAMLSeq* seq)
      {
	int data_error = 0;
	for (YAMLSeq::const_iterator i (seq->begin ()); i != seq->end (); i++)
	  {
	    const YAMLObj* veo = *i;
	    if (veo->type () != YAMLObj::SCALAR)
	      {
		std::cerr << mkey << ": element/element not scalar"
			  << std::endl;
		data_error++;
	      }
	    if (data_error == 0)
	      elem.push_back (*veo->toScalar ());
	  }
	return data_error;
      }

      /// \brief YAMLのSystemBuilderTypesを処理するコールバック関数
      ///
      /// \attention C言語の基本型に対し、設定ファイル(YAML)に、この設
      /// 定は不要。\n (C言語の基本型は定義済みのため)
      ///
      /// 読み込むフォーマットは以下の通り
      /// - SystemBuilderTypesはハッシュ
      /// - 型名がキーで、以下が定義済み。\n
      ///   char, short, int, long,
      ///   unsigned char, unsigned short, unsigned int, unsigned long,
      ///   float
      ///   - long, unsigned long は CSPトランスレータの --smbp-long64
      ///     に影響を受ける(指定時はlongを64bitとして扱う)
      /// - ハッシュで属性を与える\n
      ///   属性は以下の通り。
      ///   - size\n
      ///     型の(一要素の)サイズ(バイト)[必須]
      ///   - align\n
      ///     型の(一要素の)アラインメント(バイト)。省略時はsizeと同じ
      ///   - element\n
      ///     構造体の場合、メンバをシーケンスで指定する。
      ///     メンバは配列を持てない
      ///   - float
      ///     (一要素の)型が浮動小数点型の場合に指定する
      ///   .
      /// .
      ///
      /// SystemBuilderで取り扱える型には以下の制約がある
      /// - 構造体のメンバは、全て同一の型でなければならない。
      ///   (MemoryChannelが、同一の型を扱うため)
      /// - 構造体のメンバには、配列は指定できない
      /// - doubleも定義されているが動作保証しない
      /// .
      ///
      /*! \verbatim
          SystemBuilderTypes:
            IntStruct: { size: 4, align: 4, element: [ a, b, c ] }
            FloatStruct: { size: 4, align: 4, element: [ x, y, z ], float }
          \endverbatim */
      ///
      /// \param[in] obj YAMLObj のポインタ
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      ///
      /// \sa CSP_TRANSLATOR_MAIN_OPTIONS --config-file
      static int
      yaml_cb_SystemBuilderTypes (YAMLObj* obj)
      {
	int data_error = 0;
	const std::string mkey ("SystemBuilderTypes");
	if (obj != NULL && obj->type () == YAMLObj::MAP)
	  {
	    const YAMLMap* sbtm = *obj;
	    for (YAMLMap::const_iterator sbti (sbtm->begin ());
		 data_error == 0 && sbti != sbtm->end ();
		 sbti++)
	      {
		// これがSystemBuilderの型名称になる
		const std::string& k = sbti->first;

		// こちらはその属性。mappiingでなければならない
		const YAMLObj* v = sbti->second;
		if (v->type () != YAMLObj::MAP)
		  {
		    std::cerr << mkey << ": not map" << std::endl;
		    data_error++;
		    break;
		  }
		const YAMLMap* vm = *v;

		///
		// SystemBuilderの型名に対し、サイズとアラインを設定する
		//

		size_t v_size = 4;
		const YAMLObj* vso = vm->get ("size");
		if (vso != NULL && vso->type () == YAMLObj::SCALAR)
		  v_size = vso->toScalar ()->get_ulong ();
		else
		  {
		    std::cerr << mkey << ": " << k << ": size not defined"
			      << std::endl;
		    data_error++;
		  }

		size_t v_align = v_size;
		const YAMLObj* vao = vm->get ("align");
		if (vao)
		  {
		    if (vao->type () == YAMLObj::SCALAR)
		      v_align = vao->toScalar ()->get_ulong ();
		    else
		      {
			std::cerr << mkey << ": " << k
				  << ": align not scalar"
				  << std::endl;
			data_error++;
		      }
		  }

		ElemVec v_elem;
		const YAMLObj* veo = vm->get ("element");
		if (veo)
		  {
		    if (veo->type () != YAMLObj::SEQ)
		      {
			std::cerr << mkey << ": " << k
				  << ": element not sequence"
				  << std::endl;
			data_error++;
		      }
		    else
		      {
			int r = yaml_element_get (v_elem, mkey,
						  veo->toSeq ());
			if (r)
			  data_error++;
		      }
		  }

		SizeAlignInfo::TypeInfo v_type = SizeAlignInfo::TYPE_INT;
		const YAMLObj* vfo = vm->get ("float");
		if (vfo)
		  v_type = SizeAlignInfo::TYPE_FLOAT;

		if (data_error == 0)
		  sb_sizealign.set (k, v_size, v_align, v_elem, v_type);
	      }
	  }

	if (data_error)
	  return -1;

	return 0;
      }

      /// \brief YAMLのEmbeddedCoderTypesを処理するコールバック関数
      ///
      /// \attention 基本的な型に対し、設定ファイル(YAML)に、この設定は不要
      ///
      /// 読み込むフォーマットは以下の通り
      /// - EmbeddedCoderTypesはハッシュ
      /// - 型名がキーで、以下の型が定義済み。\n
      ///   int8_T, uint8_T, int16_T, uint16_T, int32_T, uint32_T,
      ///   real32_T, real64_T, real_T, time_T, boolean_T, int_T,
      ///   uint_T, ulong_T, char_T, uchar_T, byte_T,
      ///   creal32_T, creal64_T, creal_T, cint8_T, cuint8_T,
      ///   cint16_T, cuint16_T, cint32_T, cuint32_T
      ///   - \a real_T, \a creal_T はCSPトランスレータの \a --smbp-real64
      ///     に影響を受ける(指定時はreal64_Tと同じになる)
      ///   - \a ulong_T は CSPトランスレータの \a --smbp-long64
      ///     に影響を受ける(指定時はlongを64bitとして扱う)
      /// - ハッシュで属性を与える\n
      ///   属性は以下の通り。
      ///   - ref\n
      ///     Embedded Coderの型をSystemBuilderと型を一致させるために使
      ///     用し、参照するSystemBuilder側の型を指定する。\n
      ///     refを指定した場合 \a size と \a align は無視する
      ///   - size\n
      ///     信号が構造体型等の場合、型のサイズを指定する
      ///   - align
      ///     信号が構造体型等の場合、型のアラインを指定する
      ///   .
      /// .
      ///
      /// 例えば以下のようになる
      ///
      /*! \verbatim
          EmbeddedCoderTypes:
            IntStruct: { ref: IntStruct }
	    FloatStruct: { ref: FloatStruct }
	  \endverbatim */
      /// \param[in] obj YAMLObj のポインタ
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      ///
      /// \sa CSP_TRANSLATOR_MAIN_OPTIONS --config-file
      static int
      yaml_cb_EmbeddedCoderTypes (YAMLObj* obj)
      {
	int data_error = 0;
	const std::string mkey ("EmbeddedCoderTypes");

	if (obj != NULL && obj->type () == YAMLObj::MAP)
	  {
	    const YAMLMap* ectm = *obj;
	    for (YAMLMap::const_iterator ecti (ectm->begin ());
		 data_error == 0 && ecti != ectm->end (); ecti++)
	      {
		const std::string& k = ecti->first;
		const YAMLObj* v = ecti->second;
		if (v->type () != YAMLObj::MAP)
		  {
		    std::cerr << mkey << ": not map" << std::endl;
		    data_error++;
		    break;
		  }

		SizeAlignInfo* sai = NULL;

		const YAMLMap* vm = *v;

		const YAMLObj* vto = vm->get ("ref");
		if (vto != NULL && vto->type () == YAMLObj::SCALAR)
		  {
		    /// SystemBuilderの型を参照する場合を優先
		    const std::string& sbtype = *vto->toScalar ();
		    sai = sb_sizealign.get (sbtype);
		    if (sai == NULL)
		      {
			std::cerr << mkey << ": " << k << ": type "
				  << sbtype << ": not found"
				  << std::endl;
			data_error++;
		      }
		  }
		else
		  {
		    // SystemBuilderの型を参照していないので、sizeと
		    // alignを取得する

		    size_t v_size = 4;
		    const YAMLObj* vso = vm->get ("size");
		    if (vso != NULL
			&& vso->type () == YAMLObj::SCALAR)
		      v_size = vso->toScalar ()->get_ulong ();
		    else
		      {
			std::cerr << mkey << ": " << k << ": size not defined"
				  << std::endl;
			data_error++;
		      }

		    size_t v_align = v_size;
		    const YAMLObj* vao = vm->get ("align");
		    if (vao)
		      {
			if (vao->type () == YAMLObj::SCALAR)
			  v_align = vao->toScalar ()->get_ulong ();
			else
			  {
			    std::cerr << mkey << ": " << k
				      << ": align not scalar"
				      << std::endl;
			    data_error++;
			  }
		      }

		    ElemVec v_elem;
		    const YAMLObj* veo = vm->get ("element");
		    if (veo)
		      {
			if (veo->type () != YAMLObj::SEQ)
			  {
			    std::cerr << mkey << ": " << k
				      << ": element not sequence"
				      << std::endl;
			    data_error++;
			  }
			else
			  {
			    int r = yaml_element_get (v_elem, mkey,
						      veo->toSeq ());
			    if (r)
			      data_error++;
			  }
		      }

		    SizeAlignInfo::TypeInfo v_type = SizeAlignInfo::TYPE_INT;
		    const YAMLObj* vfo = vm->get ("float");
		    if (vfo)
		      v_type = SizeAlignInfo::TYPE_FLOAT;

		    // サイズに対応する SizeAlignInfo のポインタを取得
		    if (data_error == 0)
		      sai = sizealign_list.get (v_size, v_align,
						v_elem, v_type);
		  }

		// Embedded Coderの型情報を更新
		if (data_error == 0)
		  ec_sizealign.set (k, sai);
	      }
	  }

	if (data_error)
	  return -1;

	return 0;
      }

      /// \brief YAMLのSystemBuilderParametersを処理するコールバック関数
      ///
      /// 読み込むフォーマットは以下の通り
      /// - SystemBuilderParametersはハッシュ
      /// - キーは以下の通り
      ///   - SW
      ///     - core\n
      ///       SWのコア指定。省略時はNios
      ///   - HW
      ///     - core\n
      ///       HWのコア指定。省略時はFPGA
      ///   - HW_FREQ_MHZ\n
      ///     SystemBulderに出力する値。省略時は出力しない
      ///   - CyclicSync
      ///     - init_start\n
      ///       trueかfalse。省略時はtrueを出力
      ///     - hw_base_nsec\n
      ///       HWのベース周期(ns)。省略時は1000(1us)を出力
      ///     - sw_base_nsec\n
      ///       SWのベース周期(ns)。省略時はタスクの周期の最大公約数
      /// .
      ///
      /// 例えば以下のようになる
      ///
      /*! \verbatim
          SystemBuilderParameters:
            SW: { core: Nios }
            HW: { core: FPGA }

            HW_FREQ_MHZ: 100

            CyclicSync:
              init_start: true
              sw_base_nsec: 1000000
              hw_base_nsec: 1000
          \endverbatim */
      ///
      /// \param[in] obj YAMLObj のポインタ
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      ///
      /// \sa CSP_TRANSLATOR_MAIN_OPTIONS --config-file
      static int
      yaml_cb_SystemBuilderParametes (YAMLObj* obj)
      {
	int data_error = 0;

	if (obj != NULL && obj->type () == YAMLObj::MAP)
	  {
	    const YAMLMap* sbpm = *obj;

	    for (YAMLMap::const_iterator sbpi (sbpm->begin ());
		 data_error == 0 && sbpi != sbpm->end (); sbpi++)
	      {
		const std::string& k = sbpi->first;
		const YAMLObj* v = sbpi->second;

		// HW か SW を処理
		if (k == "SW" || k == "HW")
		  {
		    if (v->type () != YAMLObj::MAP)
		      {
			std::cerr << "SystemBuilderParameters: "
				  << k << ": not map"
				  << std::endl;
			data_error++;
			break;
		      }

		    // core のタイプ
		    const YAMLMap* vm = *v;
		    const YAMLObj* v_core = vm->get ("core");

		    if (v_core && v_core->type () == YAMLObj::SCALAR)
		      {
			const std::string& core = *v_core->toScalar ();
			if (k == "SW")
			  sw_core_type = core;
			else
			  hw_core_type = core;
		      }
		  }
		else if (k == "HW_FREQ_MHZ")
		  {
		    if (v->type () != YAMLObj::SCALAR)
		      {
			std::cerr << "SystemBuilderParameters: "
				  << k << ": not scalar"
				  << std::endl;
			data_error++;
			break;
		      }
		    hw_freq_mhz = *v->toScalar ();
		  }
		else if (k == "CyclicSync")
		  {
		    if (v->type () != YAMLObj::MAP)
		      {
			std::cerr << "SystemBuilderParameters: "
				  << k << ": not map"
				  << std::endl;
			data_error++;
			break;
		      }

		    const YAMLMap* csm = *v;

		    struct cyclic_sync_kv_type
		    {
		      const std::string& key;
		      std::string& val;
		    } kv[] = {
		      { "init_start", cyclic_sync_init_start, },
		      { "sw_base_nsec", cyclic_sync_sw_base_nsec, },
		      { "hw_base_nsec", cyclic_sync_hw_base_nsec, },
		    };

		    for (size_t i = 0; i < sizeof kv / sizeof kv[0]; i++)
		      {
			const YAMLObj* csv = csm->get (kv[i].key);
			if (csv && data_error == 0)
			  {
			    if (csv->type () == YAMLObj::SCALAR)
			      kv[i].val = *csv->toScalar ();
			    else
			      {
				std::cerr << "SystemBuilderParameters: "
					  << k << '/' << kv[i].key
					  << ": not scalar" << std::endl;
				data_error++;
				break;
			      }
			  }
		      }
		  }
	      }
	  }

	return data_error;
      }

      //
      // チャネル対応
      //

      /// \brief EmbeddedCoderで定義される型をCの型に戻す場合に1にする
#ifndef CHANGE_EC_TYPE
#define CHANGE_EC_TYPE	0
#endif

      /// \brief SMBP用ライタクラス
      ///
      /// var_writer を継承する。EmbeddedCoderの型とCの型を変換すること
      /// を目的としている
      class smbp_var_writer
	: public var_writer
      {
      public:
	/// \brief コンストラクタ
	///
	/// var_writer を継承している CspVar を保持し、その型に対する
	/// SizeAlignInfo を取得、保持する
	smbp_var_writer (const CspVar* cv)
	  : vw_cv (cv), vw_sai (NULL)
	{
	  EmbeddedCoderType* ect = ec_sizealign.get (cv->type ());
	  if (ect)
	    vw_sai = ect->sizealign ();
	}

	/// \brief SystemBuilder用のCの型を取得する
	///
	/// \return Cの型
	const std::string&
	sbtype (void) const
	{
	  return vw_sai ? *vw_sai->type () : vw_cv->type ();
	}

#if CHANGE_EC_TYPE
	/// \brief 変数の型を返す仮想関数の実体
	///
	/// SystemBuilder用のCの型を返す
	///
	/// \return 変数の型
	const std::string& type (void) const { return sbtype (); }
#else
	/// \brief 変数の型を返す仮想関数の実体
	///
	/// Embedded CoderのCの型を返す
	///
	/// \return 変数の型
	const std::string& type (void) const { return vw_cv->type (); }
#endif

	/// \brief 変数名を返す仮想関数の実体
	///
	/// \return 変数名
	const std::string& name (void) const { return vw_cv->name (); }

	/// \brief 配列サイズを返す仮想関数の実体
	///
	/// \return 配列サイズ
	size_t size (void) const { return vw_cv->size (); }

	/// \brief CspVar* へのキャスト
	///
	/// \return CspVar のポインタ
	operator const CspVar* () const { return vw_cv; }

	/// \brief SystemBuilder用のチャネルの型を持っているかどうかを返す
	///
	/// \retval true チャネルの型を持つ
	/// \retval false 持たない
	bool
	has_sb_type (void) const
	{
	  return vw_sai != NULL && vw_sai->has_sb_type ();
	}

	/// \brief 保存された SizeAlignInfo のポインタを返す
	///
	/// \return 保存された SizeAlignInfo のポインタ
	const SizeAlignInfo* sizealigninfo (void) const { return vw_sai; }

      public:
	/// \brief 保存された CspNode のポインタ
	const CspVar* vw_cv;

	/// \brief 保存された SizeAlignInfo のポインタ
	const SizeAlignInfo *vw_sai;
      };

      /// \brief メモリチャネルのメンバ
      ///
      /// 実装は構造体のメンバになる
      ///
      /// このクラスは smbp_var_writer を継承しない。
      /// (var_writer の純粋仮想関数が置き換わるので)
      class SBChanMember
      {
      public:
	/// \brief コンストラクタ
	///
	/// \param[in] v CspVar のポインタ。変数を示す
	/// \param[in] sid 構造体のメンバの場合、そのid (-1は構造体でない)
	/// \param[in] a 変数の型のアラインメント
	/// \param[in] s 変数のサイズ
	/// \param[in] o メンバのオフセット
	///
	/// サイズは変数のサイズで、ダブルバッファかどうかは考慮しない
	SBChanMember (const CspVar* v, int sid,
		       size_t a, size_t s, size_t o)
	  : sbcm_vw (v), sbcm_sid (sid),
	    sbcm_align (a), sbcm_size (s), sbcm_offset (o)
	{
	}

	/// \brief smbp_var_writer の参照を返す
	operator const smbp_var_writer& () const { return sbcm_vw; }

	/// \brief CspVar のポインタへのキャスト
	///
	/// \return CspVar のポインタ
	operator const CspVar* () const { return sbcm_vw; }

	/// \brief CspVar のポインタ取得
	///
	/// \return CspVar のポインタ
	const CspVar* var (void) const { return sbcm_vw; }

	/// \brief sidの取得
	///
	/// \return sid
	int sid (void) const { return sbcm_sid; }

	/// \brief メンバのアラインメントの取得
	///
	/// \return メンバのアラインメント
	size_t align (void) const { return sbcm_align; }

	/// \brief メンバのサイズの取得
	///
	/// \return メンバのサイズ
	size_t size (void) const { return sbcm_size; }

	/// \brief メンバのオフセット取得
	///
	/// \return メンバのオフセット
	size_t offset (void) const { return sbcm_offset; }

	/// \brief メンバのオフセット設定
	///
	/// \param[in] o オフセット
	///
	/// \return このインスタンスの参照
	SBChanMember& offset (size_t o) { sbcm_offset = o; return *this; }

	/// \brief lessthanオペレータ
	///
	/// SBChanInfo::reorder(void) のソートで用いる
	///
	/// \param[in] x 比較対象
	///
	/// \retval true アラインメントまたはサイズが小さい
	/// \retval false 同じかそれ以外
	bool
	operator< (const SBChanMember& x) const
	{
	  if (sbcm_sid < x.sbcm_sid)
	    return true;
	  if (sbcm_align < x.sbcm_align)
	    return true;
	  return sbcm_size < x.sbcm_size;
	}

	/// \brief equalオペレータ
	///
	/// \param[in] x 比較対象
	///
	/// \retval true アラインメントおよびサイズが同じ
	/// \retval false 異なる
	bool
	operator== (const SBChanMember& x)
	{
	  return sbcm_align == x.sbcm_align && sbcm_size == x.sbcm_size;
	}

	/// \brief 型の要素の取得(参照用)
	///
	/// SizeAlignInfo::elements(void) const へのショートカット
	///
	/// \return 要素のベクタ
	const ElemVec& elements (void) const
	{
	  return sbcm_vw.sizealigninfo ()->elements ();
	}

      private:
	/// \brief var_writer
	smbp_var_writer sbcm_vw;

	/// \brief 変数が構造体のメンバの場合そのID
	int sbcm_sid;

	/// \brief メンバのアラインメント
	size_t sbcm_align;

	/// \brief メンバのサイズ
	size_t sbcm_size;

	/// \brief メンバのオフセット
	size_t sbcm_offset;
      };

      /// \brief MemoryChannelの情報
      ///
      /// 実体は SBChanMember のベクタ
      class SBChanInfo
	: public std::vector < SBChanMember >
      {
      public:
	/// 最終的に SystemBuilder でマッピングされるチャネルの種別
	enum ChanType {
	  /// \brief NBCチャネル
	  NBC_CH = 0,

	  /// \brief BCチャネル
	  BC_CH = 1,

	  /// \brief MemoryChannel(NBC不使用)
	  MEM_CH = 2,

	  /// \brief MemoryChannel+BCチャネル
	  BC_MEM_CH = 3
	};

	/// \brief SBChanInfo のコンストラクタ
	///
	/// \param[in] cn_tail FROM側 CspNode のポインタ
	/// \param[in] cn_head TO側 CspNode のポインタ
	/// \param[in] ce CspEdge の参照
	/// \param[in] db ダブルバッファかどうか
	SBChanInfo (const CspNode* cn_tail, const CspNode* cn_head,
		    const CspEdge& ce, bool db = false)
	  : sbci_chtype (NBC_CH), sbci_tail (cn_tail), sbci_head (cn_head),
	    sbci_edge (ce), sbci_double_buffer (db),
	    sbci_check_output_time (false), sbci_check_input_time (false),
	    sbci_memsize (0)
	{
	  is_need_check_output_time ();
	  is_need_check_input_time ();
	}

	/// \brief SystemBuilderのチャネルのタイプを取得
	enum ChanType chtype (void) const { return sbci_chtype; }

	/// \brief SBChanInfo のFrom側の CspNode の取得
	///
	/// \return CspNode のポインタ(From)
	const CspNode* tail (void) const { return sbci_tail; }

	/// \brief SBChanInfo のTo側の CspNode の取得
	///
	/// \return CspNode のポインタ(To)
	const CspNode* head (void) const { return sbci_head; }

	/// \brief CspEdge の参照を返す
	///
	/// \return CspEdge の参照
	const CspEdge& edge (void) const { return sbci_edge; }

	/// \brief ダブルバッファが必要かどうか
	///
	/// \retval true ダブルバッファである
	/// \retval false ダブルバッファでない
	bool double_buffer (void) const { return sbci_double_buffer; }

	/// \brief 出力で時刻チェックが必要かどうか
	///
	/// \retval true 出力チャネルに時刻チェックが必要
	/// \retval false 不要
	bool check_output_time (void) const { return sbci_check_output_time; }

	/// \brief 入力で時刻チェックが必要かどうか
	///
	/// \retval true 入力チャネルに時刻チェックが必要
	/// \retval false 不要
	bool check_input_time (void) const { return sbci_check_input_time; }

	/// \brief SBChanInfo にメンバを追加
	///
	/// \param[in] v CspVar のポインタ
	/// \param[in] adjust サイズを整合させるかどうか
	///
	/// \return 追加したメンバの SBChanMember の参照
	SBChanMember& add_member (const CspVar* v, bool adjust = false);

	/// \brief メンバをソートする
	///
	/// アラインの小さい順、サイズの小さい順にソートし、オフセットと
	/// サイズを再計算する
	///
	/// \return このインスタンスの参照
	SBChanInfo& reorder (void);

	/// \brief MemoryChannel のサイズ
	size_t memsize (void) const { return sbci_memsize; }

	/// \brief CspNode の比較オペレータ
	bool operator== (const CspNode* x) const { return sbci_head == x; }

	/// \brief sidから構造体オブジェクト名を取得
	const std::string& sobject (int sid) const { return sbci_sobj[sid]; }

	// BC/NBCチャネルの名前を返す
	/// \brief チャネルの名前を返す
	///
	/// \return チャネル名
	std::string
	chan (void) const
	{
	  return std::string (mk_task_id (sbci_tail->id ())
			      + "_" + mk_task_id (sbci_head->id ()));
	}

      private:
	/// \brief 実際に MemoryChannel が必要かどうか
	///
	/// SystemBuilderのBC/NBCのレジスタにできるのは、単一の32bit以
	/// 下のものなので、それ以外はMemoryChannelにする。
	///
	/// \retval true 変数が二つ以上か32bit以上か要素数が2以上なので必要
	/// \retval false 不要
	bool
	is_need_memchan (void) const
	{
	  return (size () > 1
		  || (*this)[0].size () > 4
		  || (*this)[0].elements ().size () > 1);
	}

	/// \brief 出力で時刻チェックが必要か判定する
	///
	/// \retval true 出力で時刻チェックが必要
	/// \retval false 不要
	bool
	is_need_check_output_time (void)
	{
	  // 時刻チェックを要するケース
	  //
	  // ・ダブルバッファでなく(0次ホールド)
	  // 　・早いレートから遅いレートへの送信
	  // 　・または、後段のサンプル時間のオフセットが大きい
	  //   　-> 実行時間に後段が開始前
	  //
	  // ダブルバッファの場合、常に出力を行い、入力側で時刻に応じて
	  // 適切な面にアクセスするものとする
	  const rate_T& tr = sbci_tail->rate ();
	  const Rate_rate_T rate_x = tr.rate ();
	  const Rate_rate_T offset_x = tr.offset ();

	  const rate_T& hr = sbci_head->rate ();
	  const Rate_rate_T rate_y = hr.rate ();
	  const Rate_rate_T offset_y = hr.offset ();

	  bool f = false;
	  if (!sbci_double_buffer
	      && (rate_x < rate_y || offset_x < offset_y))
	    f = true;
	  sbci_check_output_time = f;
	  return f;
	}

	/// \brief 入力で時刻チェックが必要か判定する
	///
	/// \retval true 入力で時刻チェックが必要
	/// \retval false 不要
	bool
	is_need_check_input_time (void)
	{
	  // 時刻チェックを要するケース
	  //
	  // ・ダブルバッファ
	  // ・0次ホールドで受信側が遅く起動する
	  const Rate_rate_T offset_x = sbci_tail->rate ().offset ();
	  const Rate_rate_T offset_y = sbci_head->rate ().offset ();

	  bool f = false;
	  if (sbci_double_buffer || offset_y > offset_x)
	    f = true;
	  sbci_check_input_time = f;
	  return f;
	}

	/// \brief チャネルのタイプ
	enum ChanType sbci_chtype;

	/// \brief From側 CspNode;
	const CspNode* sbci_tail;

	/// \brief To側 CspNode
	const CspNode* sbci_head;

	/// \brief CspEdge の参照
	const CspEdge& sbci_edge;

	/// \brief ダブルバッファかどうか
	bool sbci_double_buffer;

	/// \brief 出力で時刻チェックを要するかどうか
	bool sbci_check_output_time;

	/// \brief 出力で時刻チェックを要するかどうか
	bool sbci_check_input_time;

	/// \brief MemoryChannelのメモリサイズ
	size_t sbci_memsize;

	/// \brief 構造体オブジェクトのリスト
	std::vector < std::string > sbci_sobj;
      };


      // SBChanInfo にメンバを追加
      SBChanMember&
      SBChanInfo::add_member (const CspVar* v, bool adjust)
      {
	int sid = -1;
	const optional_refstring& osobj = v->storage ();
	if (osobj)
	  {
	    const std::string& sobj = *osobj;
	    for (size_t n = 0; n < sbci_sobj.size (); n++)
	      {
		if (sbci_sobj[n] == sobj)
		  sid = n;
	      }

	    if (sid == -1)
	      {
		sid = sbci_sobj.size ();
		sbci_sobj.push_back (sobj);
	      }
	  }

	const EmbeddedCoderType* ec_type = ec_sizealign.get (v->type ());
	assert (ec_type);
	const SizeAlignInfo* sai = ec_type->sizealign ();

	size_t na = sai->align ();
	size_t ns = sai->size ();
	size_t ne = sai->elements ().size ();

	// 要素の数(構造体のメンバ数)
	if (ne > 1)
	  ns *= ne;

	// 配列のサイズ
	size_t array = v->size ();
	if (array)
	  ns *= array;

	// オフセット計算用
	size_t off = 0;

	if (adjust)
	  {
	    // 初期のオフセットは MemoryChannel のサイズ
	    off = sbci_memsize;

	    // 不整合ならアジャスト
	    size_t x = off % na;
	    if (x)
	      off += (na - x);

	    sbci_memsize = off + ns;

	    // ダブルバッファの場合はサイズを2倍
	    if (sbci_double_buffer)
	      sbci_memsize += ns;
	  }

	push_back (SBChanMember (v, sid, na, ns, off));
	return back ();
      }

      // メンバをソートする
      SBChanInfo&
      SBChanInfo::reorder (void)
      {
	std::sort (begin (), end ());

	size_t sz = 0;

	for (iterator i (begin ()); i != end (); i++)
	  {
	    SBChanMember& m = *i;

	    size_t a = m.align ();
	    size_t n = sz % a;
	    if (n)
	      sz += (a - n);

	    m.offset (sz);
	    sz += m.size ();

	    // ダブルバッファ時はサイズを2倍
	    if (sbci_double_buffer)
	      sz += m.size ();
	  }

	sbci_memsize = sz;

	// タイプを設定する
	if (is_need_memchan ())
	  {
	    if (sbci_double_buffer)
	      sbci_chtype = MEM_CH;
	    else
	      sbci_chtype = BC_MEM_CH;
	  }
	else if (sbci_double_buffer)
	  sbci_chtype = NBC_CH;
	else
	  sbci_chtype = BC_CH;

	return *this;
      }

      /// \brief CspNode毎の SBChanInfo のリスト
      ///
      /// 対向する CspNode に対する SBChanInfo を保持する
      class SBChanList
	: public std::list < SBChanInfo >
      {
      public:
	/// \brief 対抗する CspNode に対する SBChanInfo を作成する
	///
	/// \param[in] cn_tail CspNode のポインタ(tail)
	/// \param[in] cn_head 対向の CspNode のポインタ(head)
	/// \param[in] ce CspEdge の参照
	/// \param[in] double_buffer ダブルバッファが必要かどうか
	///
	/// \return SBChanInfo の参照
	SBChanInfo&
	add_memchan (const CspNode* cn_tail, const CspNode* cn_head,
		     const CspEdge& ce, bool double_buffer)
	{
	  push_back (SBChanInfo (cn_tail, cn_head, ce, double_buffer));
	  return back ();
	}
      };

      /// \brief CspNode と SBChanList のマップのクラス
      ///
      /// キーは CspNode のポインタ
      class SBChanMap
	: public std::map < const CspNode*, SBChanList >
      {
      };

      /// \brief CspNode と SBChanList のマップ
      ///
      /// 基本的にはこちらは、データのアロケーション用途であり、実際は
      /// csp_memchan_ptr_map を使用する。
      ///
      /// \sa csp_memchan_ptr_map
      static SBChanMap csp_memchan_map;

      /// \brief SBChanPtr のリスト
      ///
      /// CspNode に関係する情報を保持するために使用する
      class SBChanPtrList
	: public std::vector < SBChanInfo* >
      {
      public:
	/// \brief チャネルの比較を行う叙述関数用構造体(Tailを比較)
	struct CmpTail {
	  ///
	  /// タイプで比較し、一致する場合はtail側のrateで比較する。
	  ///
	  /// \param[in] a 比較の左辺
	  /// \param[in] b 比較の右辺
	  ///
	  /// \return true 前にする
	  /// \return false そうしない
	  bool operator () (const SBChanInfo* a, const SBChanInfo* b) const
	  {
	    if (a->chtype () < b->chtype ())
	      return true;
	    return a->tail ()->rate () < b->tail ()->rate ();
	  }
	};

	/// \brief チャネルの比較を行う叙述関数用構造体(Headを比較)
	struct CmpHead {
	  /// \brief 比較用の叙述関数
	  ///
	  /// タイプで比較し、一致する場合はhead側のrateで比較する。
	  ///
	  /// \param[in] a 比較の左辺
	  /// \param[in] b 比較の右辺
	  ///
	  /// \return true 前にする
	  /// \return false そうしない
	  bool operator () (const SBChanInfo* a, const SBChanInfo* b) const
	  {
	    if (a->chtype () < b->chtype ())
	      return true;
	    return a->head ()->rate () < b->head ()->rate ();
	  }
	};

	/// \brief CmpTail を用いたソート
	void
	sort_tail (void)
	{
	  std::sort (begin (), end (), CmpTail ());
	}

	/// \brief CmpHead を用いたソート
	void
	sort_head (void)
	{
	  std::sort (begin (), end (), CmpHead ());
	}

	/// \brief CspNode (tail側)に対する入出力チャネルの抽出
	///
	/// \param[in] cn_tail CspNode のポインタ(tail側)
	/// \param[in] in 抽出された入力チャネル
	/// \param[in] out 抽出された出力チャネル
	void
	tail_inout (const CspNode* cn_tail,
		    SBChanPtrList& in, SBChanPtrList& out) const
	{
	  for (const_iterator i (begin ()); i != end (); i++)
	    {
	      SBChanInfo* sbci = *i;
	      if (sbci->tail () == cn_tail)
		out.push_back (sbci);
	      else
		in.push_back (sbci);
	    }
	}

	/// \brief CspNode (head側)に対する入出力チャネルの抽出
	///
	/// \param[in] cn_head CspNode のポインタ(head側)
	/// \param[in] in 抽出された入力チャネル
	/// \param[in] out 抽出された出力チャネル
	void
	head_inout (const CspNode* cn_head,
		    SBChanPtrList& in, SBChanPtrList& out) const
	{
	  for (const_iterator i (begin ()); i != end (); i++)
	    {
	      SBChanInfo* sbci = *i;
	      if (sbci->head () == cn_head)
		in.push_back (sbci);
	      else
		out.push_back (sbci);
	    }
	}
      };

      /// \brief CspNode と SBChanPtrList のマップクラス
      ///
      /// MemoryChannel は、TaskSet が異なる場合に生成されるため、
      /// CspNode に対して、正逆方向のチャネルを全て保持しても、コード
      /// 生成の際には衝突することはない
      class SBChanPtrMap
	: public std::map < const CspNode*, SBChanPtrList >
      {
      };

      /// \brief CspNodeと SBChanPtrList のマップ
      ///
      /// 逆方向の情報を保持するために使用する
      static SBChanPtrMap csp_memchan_ptr_map;

      /// \brief CspNode に対する MemoryChannel の構築
      ///
      /// \param[in] cn_tail 送信元の CspNode のポインタ
      ///
      /// \return 作成した SBChanList の参照
      SBChanList&
      make_smbp_memchan (const CspNode* cn_tail)
      {
	SBChanList& sbcl = csp_memchan_map[cn_tail];

	for (CspEdgeList::const_iterator
	       ei (cn_tail->forw_edges ().begin ());
	     ei != cn_tail->forw_edges ().end (); ei++)
	  {
	    const CspEdge& fedge = *ei;

	    // S-MBPコード生成では、条件付きサブシステムの初期値送信を
	    // おこなわないので、チャネルは不要。
	    // 初期値は0と仮定する
	    if (fedge.info () & CSP_EDGE_INFO_INITIAL_VALUE)
	      continue;

	    CspNode* cn_head = fedge.node ();

	    // 同じ TaskSet に属す場合、チャネルは不要
	    if (cn_tail->taskset () == cn_head->taskset ())
	      continue;

	    // ダブルバッファの判定
	    bool doubel_buffer = ((fedge.info ()
				   & CSP_EDGE_INFO_DOUBLE_BUFFER) != 0);

	    // 対向ノードに対する MemoryChannel の情報を作成
	    SBChanInfo&
	      sbci = sbcl.add_memchan (cn_tail, cn_head,
				       fedge, doubel_buffer);

	    // 伝播する変数を登録する
	    const PropagateVarList povl = fedge.var ().order ();
	    for (PropagateVarList::const_iterator i (povl.begin ());
		 i != povl.end (); i++)
	      {
		const PropagateVar* pv = *i;
		const CspVar* cv = *pv;

		if (cv->mode () & VAR_MODE_NODEF_VAR)
		  continue;

		sbci.add_member (cv);
	      }

	    // 変数を適切に並び変える
	    sbci.reorder ();

	    // 関連する CspNode に SBChanInfo のポインタを登録する
	    csp_memchan_ptr_map[cn_tail].push_back (&sbci);
	    csp_memchan_ptr_map[cn_head].push_back (&sbci);
	  }

	return sbcl;
      }

      /// \brief MemoryChannel の構築
      void
      make_smbp_memchan (void)
      {
	for (CspNodeList::const_iterator
	       i (CspNode::csp_nodelist ().begin ());
	     i != CspNode::csp_nodelist ().end (); i++)
	  make_smbp_memchan (*i);
      }

      /// \brief MemoryChannel の構造体の1チャネル出力
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] sbci SBChanInfo の参照
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      static int
      generate_memchan_types (std::ostream& os, const SBChanInfo& sbci)
      {
	std::string t_tmp;
	std::string h_tmp;

	const CspNode* cn_tail = sbci.tail ();
	const std::string& t_name = cn_tail->get_block_name (t_tmp);

	const CspNode* cn_head = sbci.head ();
	const std::string& h_name = cn_head->get_block_name (h_tmp);

	const std::string chan (sbci.chan ());

	os << '\n'
	   << "/* From: " << t_name << '\n'
	   << "   To: " << h_name << '\n'
	   << "   ";
	if (cn_tail->coretype () == HARDWARE_CORE)
	  os << "HW";
	os << "Core" << cn_tail->core ();
	if (cn_tail->rate () != cn_head->rate ())
	  os << '[' << cn_tail->rate ().rate () << ','
	     << cn_tail->rate ().offset () << ']';
	os << " -> ";
	if (cn_head->coretype () == HARDWARE_CORE)
	  os << "HW";
	os << "Core" << cn_head->core ();
	if (cn_tail->rate () != cn_head->rate ())
	  os << '[' << cn_head->rate ().rate ()
	     << ',' << cn_head->rate ().offset () << ']';

	// BC/NBC通信の場合MemoryChannelは不要
	if (sbci.chtype () == SBChanInfo::BC_CH
	    || sbci.chtype () == SBChanInfo::NBC_CH)
	  {
	    os << "\n   (CH_" << block_name_prefix
	       << chan << " not need MemoryChannel) */\n";
	    return 0;
	  }

	os << " */\n"
	   << "typedef struct {\n";

	bool doubel_buffer = sbci.double_buffer ();

	int sid = -1;
	for (SBChanInfo::const_iterator sbcii (sbci.begin ());
	     sbcii != sbci.end (); sbcii++)
	  {
	    const SBChanMember& mm = *sbcii;

	    // 構造体の変わり目
	    if (sid != mm.sid ())
	      {
		if (sid != -1)
		  os << indent_writer::iw1
		     << "} " << sbci.sobject (sid) << ";\n";
		os << indent_writer::iw1
		   << "struct {\n";
		sid = mm.sid ();
	      };

	    // 変数を出力する
	    const smbp_var_writer& svw = mm;
	    const var_defn_writer& vdw = svw;

	    if (doubel_buffer)
	      os << make_doublebuffer (vdw, DB_PLANE).set_indent (2);
	    else
	      os << vdw.set_indent (2);
	    os << ";\n";
	  }

	// 開いている構造体定義を閉じる
	if (sid != -1)
	  os << "  } " << sbci.sobject (sid) << ";\n";

	os << "} TYPE_" << block_name_prefix << chan << ";\n"
	   << "TYPE_" << block_name_prefix << chan
	   << " CH_" << block_name_prefix << chan << ";\n";

	return 0;
      }

      /// \brief CspNode の全チャネルの MemoryChannel の構造体出力
      ///
      /// MemoryChannel が生成されている場合 TaskSet が異なる。
      // そのため、生成ファイルが異なるので、
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] cn 対象の CspNode のポインタ
      /// \param[in] tail_only 対象の CspNode が送信元の場合のみ出力
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      int
      generate_memchan_types (std::ostream& os, const CspNode* cn,
			      bool tail_only)
      {
	int error = 0;

	SBChanPtrMap::const_iterator sbcpi = csp_memchan_ptr_map.find (cn);

	// 非対象
	if (sbcpi == csp_memchan_ptr_map.end ())
	  return 0;

	const SBChanPtrList& sbcpl = sbcpi->second;
	for (SBChanPtrList::const_iterator sbcpli (sbcpl.begin ());
	     sbcpli != sbcpl.end (); sbcpli++)
	  {
	    const SBChanInfo* sbci = *sbcpli;

	    // 送信元だけ出す場合
	    if (tail_only && sbci->tail () != cn)
	      continue;

	    int error = generate_memchan_types (os, *sbci);
	    if (error)
	      break;
	  }

	return error;
      }

      /// \brief TaskSet の MemoryChannel の構造体の出力
      ///
      /// SystemBuilder用の TaskSet 毎の MemoryChannel の構造体出力
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] taskset TaskSetのポインタ
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      int
      generate_memchan_types (std::ostream& os, const TaskSet* taskset)
      {
	int error = 0;

	for (CspNodeList::const_iterator
	       cni (taskset->cspnodelist ().begin ());
	     cni != taskset->cspnodelist ().end (); cni++)
	  {
	    const CspNode* cn = *cni;
	    error = generate_memchan_types (os, cn, false);
	    if (error)
	      break;
	  }

	return error;
      }


      /// \brief 全 CspNode の MemoryChannel の構造体の出力
      ///
      /// 主に TOPPERS/ATK2 用の MemoryChannel の構造体出力
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] tail_only 対象の CspNode が送信元の場合のみ出力
      ///
      /// \retval 0 成功
      /// \retval それ以外 失敗
      int
      generate_memchan_types (std::ostream& os, bool tail_only)
      {
	for (CspNodeList::const_iterator
	       i (CspNode::csp_nodelist ().begin ());
	     i != CspNode::csp_nodelist ().end (); i++)
	  {
	    CspNode* cn_tail = *i;

	    int error = generate_memchan_types (os, cn_tail, tail_only);
	    if (error)
	      return error;
	  }

	return 0;
      }

      /// \brief 単一の変数をMemoryChannelへ出力する
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] sbcmb SBChanMember の参照
      /// \param[in] sbmem MemoryChannelのプレフィクス
      /// \param[in] api_post SystemBuilderのAPIのポストフィクス
      /// \param[in] amp アドレスを取る&オペレータ
      /// \param[in] ind インデント
      static void
      generate_memchan_values (std::ostream& os,
			       const SBChanMember& sbcmb,
			       const std::string& sbmem,
			       const std::string& api_post,
			       const std::string& amp,
			       int ind)
      {
	const indent_writer iw0 (ind);
	const indent_writer iw1 (ind + 1);

	const ElemVec& elem = sbcmb.elements ();
	size_t nelem = elem.size ();

	const smbp_var_writer& svw = sbcmb;
	const var_rhs_writer &vrw = svw;
	size_t s = vrw.size ();
	if (s)
	  {
	    const std::string vind ("i");
	    os << iw0 << "for (i = 0; i < " << s << "i++) {\n";
	    if (nelem)
	      {
		for (size_t ne = 0; ne < nelem; ne++)
		  {
		    os << iw1 << sbmem << api_post << "(i*" << nelem;
		    if (ne)
		      os << '+' << ne;
		    os << ", " << amp << vrw.set_index (vind)
		       << '.' << elem[ne] << ");\n";
		  }
	      }
	    else
	      os << iw1 << sbmem << api_post << "(i, " << amp
		 << vrw.set_index (vind) << ");\n";
	    os << iw0 << "}\n";
	  }
	else
	  {
	    if (nelem)
	      {
		for (size_t ne = 0; ne < nelem; ne++)
		  {
		    os << iw0 << sbmem << api_post << '(';
		    if (ne)
		      os << '+' << ne;
		    else
		      os << '0';
		    os << ", " << amp << vrw << '.' << elem[ne] << ");\n";
		  }
	      }
	    else
	      os << iw0 << sbmem << api_post << "(0, "
		 << amp << vrw << ");\n";
	  }
      }

      /// \brief Task の変数を出力するコードの出力
      ///
      /// \param[in] input 1の場合入力、0の場合出力
      /// \param[in] os 出力ストリーム
      /// \param[in] sbci SBChanInfo の参照
      /// \param[in] chan チャネルの名前
      /// \param[in] ind インデント指定
      void
      generate_task_chan_values (std::ostream& os, SBChanInfo& sbci,
				 const std::string& chan,
				 int input, int ind)
      {
	const indent_writer iw0 (ind);
	const indent_writer iw1 (ind + 1);
	const indent_writer iw2 (ind + 2);

	std::string api_post;
	std::string amp;

	if (input)
	  {
	    api_post = "_READ";
	    amp = "&";
	  }
	else
	  api_post = "_WRITE";

	SBChanInfo::ChanType chtype = sbci.chtype ();

	if (chtype == SBChanInfo::BC_CH)
	  {
	    std::string sbchan (bc_prefix);
	    std::transform (sbchan.begin (), sbchan.end (),
			    sbchan.begin (), toupper);
	    sbchan += chan;
	    SBChanMember& sbcmb = sbci[0];
	    const smbp_var_writer& svw = sbcmb;
	    const var_rhs_writer &vrw = svw;

	    os << iw0 << sbchan << api_post << "(" << amp << vrw << ");\n";
	  }
	else if (chtype == SBChanInfo::NBC_CH)
	  {
	    std::string sbchan (nbc_prefix);
	    std::transform (sbchan.begin (), sbchan.end (),
			    sbchan.begin (), toupper);
	    sbchan += chan;
	    SBChanMember& sbcmb = sbci[0];
	    const smbp_var_writer& svw = sbcmb;
	    const var_rhs_writer &vrw = svw;

	    os << iw0 << "if (ch__side == 0) {\n";
	    for (size_t b = 0; b < DB_PLANE; b++)
	      {
		if (b == DB_PLANE - 1)
		  os << iw0 << "} else {\n";
		else if (b)
		  os << iw0 << "} else if (ch__side == " << b << ") {\n";
		os << iw1 << sbchan << '_' << b << api_post
		   << '(' << amp << vrw << ");\n";
	      }
	    os << iw0 << "}\n";
	  }
	else if (chtype == SBChanInfo::BC_MEM_CH)
	  {
	    std::string sbchan (bc_prefix);
	    std::transform (sbchan.begin (), sbchan.end (),
			    sbchan.begin (), toupper);
	    sbchan += chan;
	    std::string sbmem (mem_prefix);
	    std::transform (sbmem.begin (), sbmem.end (),
			    sbmem.begin (), toupper);
	    sbmem += chan;

	    os << iw0 << "{\n"
	       << iw1 << "int i;\n";

	    if (input)
	      os << iw1 << sbchan << api_post << "(&i); /* wait event */\n";

	    for (size_t n = 0; n < sbci.size (); n++)
	      {
		const SBChanMember& sbcmb = sbci[n];
		generate_memchan_values (os, sbcmb, sbmem, api_post, amp,
					 ind + 1);
	      }

	    if (input == 0)
	      os << iw1 << sbchan << api_post << "(0); /* send event */\n";

	    os << iw0 << "}\n";
	  }
	else
	  {
	    std::string sbmem (mem_prefix);
	    std::transform (sbmem.begin (), sbmem.end (),
			    sbmem.begin (), toupper);
	    sbmem += chan;

	    os << iw0 << "if (ch__side == 0) {\n"
	       << iw1 << "int i;\n";
	    for (size_t b = 0; b < DB_PLANE; b++)
	      {
		if (b == DB_PLANE - 1)
		  os << iw0 << "} else {\n";
		else if (b)
		  os << iw0 << "} else if (ch__side == " << b << ") {\n";
		std::ostringstream oss;
		oss << '_' << b;
		std::string sbmem_db (sbmem + oss.str ());
		for (size_t n = 0; n < sbci.size (); n++)
		  {
		    const SBChanMember& sbcmb = sbci[n];
		    generate_memchan_values (os, sbcmb, sbmem_db,
					     api_post, amp, ind + 1);
		  }
	      }
	    os << iw0 << "}\n";
	  }
      }

      /// \brief Taskの変数を入力処理を出力する
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] ind インデント指定
      /// \param[in] input_mode 初期値、タスク、Updateの入力のいずれか
      void
      generate_task_smbp_input_channel (std::ostream& os, CspNode* node,
					int ind, channel_io_mode input_mode)
      {
	const indent_writer iw0 (ind);
	const indent_writer iw1 (ind + 1);
	const indent_writer iw2 (ind + 2);
	const indent_writer iw3 (ind + 3);

	const std::string my_task_id (mk_task_id (node->id ()));

	SBChanPtrList input_cpl;

	// S-MBPにおいては初期値入力は行わない
	if (input_mode != CH_IO_MODE_INIT)
	  {
	    // CspNode に対する SystemBuilderのチャネルを探索
	    SBChanPtrMap::iterator cpmi = csp_memchan_ptr_map.find (node);

	    // 見つからなければ対象外
	    if (cpmi != csp_memchan_ptr_map.end ())
	      {
		// 入力を集める
		SBChanPtrList& cl = cpmi->second;
		for (SBChanPtrList::iterator cli (cl.begin ());
		     cli != cl.end (); cli++)
		  {
		    SBChanInfo* cip = *cli;
		    const CspEdge& ce = cip->edge ();

		    // エッジに、Update部への入力がある場合でUpdate部の
		    // 入力を生成する指定(input_modeが
		    // CH_IO_MODE_UPDATA)でないならここで処理しない。
		    //
		    // エッジに、初期値の送受信がある場合で、初期値の
		    // S-MBPでは初期値送信を行わないので、処理しない
		    //
		    // エッジがUpdateでも初期値でもない場合で、タスクの
		    // 入力を生成する指定(input_modeがCH_IO_MODE_TASK)
		    // でないならここで処理しない
		    if ((ce.info () & CSP_EDGE_INFO_HAS_UPDATE)
			== CSP_EDGE_INFO_HAS_UPDATE)
		      {
			if (input_mode != CH_IO_MODE_UPDATE)
			  continue;
		      }
		    else if ((ce.info () & CSP_EDGE_INFO_INITIAL_VALUE)
			     == CSP_EDGE_INFO_INITIAL_VALUE)
		      continue;
		    else if (input_mode != CH_IO_MODE_TASK)
		      continue;

		    if (cip->head () == node)
		      input_cpl.push_back (cip);
		  }
	      }
	  }

	// 対象チャネルがない
	if (input_cpl.size () == 0)
	  goto end;

	// NBC->BC->MEM->BC_MEMの順にする
	input_cpl.sort_head ();

	for (SBChanPtrList::const_iterator cpli (input_cpl.begin ());
	     cpli != input_cpl.end (); cpli++)
	  {
	    SBChanInfo& sbci = **cpli;
	    const CspNode* cn_tail = sbci.tail ();
	    std::string t_tmp;
	    const std::string& t_name = cn_tail->get_block_name (t_tmp);
	    const std::string xx_task_id (mk_task_id (cn_tail->id ()));
	    const std::string chan (xx_task_id + "_" + my_task_id);
	    const std::string ch_plane (mk_in_side (chan));
	    const std::string ch_time (mk_chan_time (chan));

	    // 送信側のレート
	    Rate_rate_T rate_x = cn_tail->rate ().rate ();

	    os << '\n'
	       << iw0 << "/* input: " << t_name << " */\n";

	    int ind2 = ind;

	    if (sbci.double_buffer ())
	      os << iw0 << "ch__side = " << ch_plane << ";\n";

	    if (sbci.check_input_time ())
	      {
		os << iw0 << "if (!CH__TIME_LT(task__time,"
		   << ch_time << ")) {\n";
		if (sbci.double_buffer ())
		  os << iw1 << "/* change side */\n"
		     << iw1 << "if (++ch__side == " << DB_PLANE << ")\n"
		     << iw2 << "ch__side = 0;\n"
		     << iw1 << ch_plane << " = ch__side;\n"
		     << iw1 << ch_time << " += " << rate_x << ";\n"
		     << iw0 << "}\n";
		else
		  ind2++;
	      }

	    generate_task_chan_values (os, sbci, chan, 1, ind2);

	    if (ind2 != ind)
	      os << "}\n";
	  }

      end:

	// 入力が完了した事を通知するマクロを出力
	if (input_mode == CH_IO_MODE_UPDATE)
	  os << '\n'
	     << "#ifdef TASK_UPDATE_IN_" << my_task_id << "_COMPLETED\n"
	     << iw0 << "TASK_UPDATE_IN_" << my_task_id << "_COMPLETED();\n"
	     << "#endif\n";
	else
	  os << '\n'
	     << "#ifdef TASK_IN_" << my_task_id << "_COMPLETED\n"
	     << iw0 << "TASK_IN_" << my_task_id << "_COMPLETED();\n"
	     << "#endif\n";
      }


      /// \brief Taskの変数の出力処理を出力する
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] node 対象の CspNode のポインタ
      /// \param[in] ind インデント指定
      void
      generate_task_smbp_output_channel (std::ostream& os, CspNode* node,
					 int ind)
      {
	const indent_writer iw0 (ind);
	const std::string my_task_id (mk_task_id (node->id ()));

	// CspNode に対する SystemBuilderのチャネルを探索
	SBChanMap::iterator cpmi = csp_memchan_map.find (node);

	// 見つけたチャネルを処理
	if (cpmi != csp_memchan_map.end ())
	  {
	    SBChanList& cl = cpmi->second;
	    SBChanPtrList output_cpl;

	    for (SBChanList::iterator cli (cl.begin ());
		 cli != cl.end (); cli++)
	      {
		SBChanInfo& ci = *cli;
		output_cpl.push_back (&ci);
	      }

	    // NBC->BC->MEM->BC_MEMの順にする
	    output_cpl.sort_tail ();

	    const indent_writer iw1 (ind + 1);
	    const indent_writer iw2 (ind + 2);

	    for (SBChanPtrList::const_iterator cpli (output_cpl.begin ());
		 cpli != output_cpl.end (); cpli++)
	      {
		SBChanInfo& sbci = **cpli;
		const CspNode* cn_head = sbci.head ();
		std::string h_tmp;
		const std::string& h_name = cn_head->get_block_name (h_tmp);
		const std::string xx_task_id (mk_task_id (cn_head->id ()));
		const std::string chan (my_task_id + "_" + xx_task_id);
		const std::string ch_plane (mk_out_side (chan));
		const std::string ch_time (mk_chan_time (chan));

		Rate_rate_T rate_x = node->rate ().rate ();
		Rate_rate_T rate_y = cn_head->rate ().rate ();

		bool double_buffer = sbci.double_buffer ();

		// 各出力条件設定と判定
		bool check_time = false;
		if (enable_multirate)
		  {
		    check_time = sbci.check_output_time ();

		    // 出力側の時刻を入力側が追い越していないかを確認す
		    // るコード出力
		    if (check_time && assert_output_enable)
		      {
			os << iw0 << "assert(!CH__TIME_LT("
			   << ch_time << ", task__time));\n";
		      }
		  }

		os << '\n'
		   << iw0 << "/* output: " << h_name << " */\n";

		int ind2 = ind;

		if (check_time)
		  {
		    // マルチレートで高速レートから低速レートへの出力、
		    // またはサンプル時間のオフセットにより後段が実行前
		    // である場合、出力時刻になったか判定する条件を出力
		    //
		    // なお、ダブルバッファの場合、常に出力し、入力側で
		    // 面情報を選択する
		    os << iw0 << "if (CH__TIME_LE(task__time+" << rate_x
		       << ", " << ch_time << ")) {\n"
		       << iw1 << "; /* no send this period */\n"
		       << iw0 << "} else {\n";
		    ind2++;
		  }
		else if (double_buffer)
		  {
		    os << iw0 << "{\n"
		       << iw1 << "ch__side = " << ch_plane << ";\n";
		    ind2++;
		  }

		generate_task_chan_values (os, sbci, chan, 0, ind2);

		// 時刻チェックを要する場合、次の実行時刻を設定する
		if (check_time)
		  os << iw1 << ch_time << " += " << rate_y << ";\n";

		// 出力面を切り替える
		if (double_buffer)
		  {
		    os << iw1 << "/* change side */\n"
		       << iw1 << "if (++ch__side == " << DB_PLANE << ")\n"
		       << iw2 << "ch__side = 0;\n"
		       << iw1 << ch_plane << " = ch__side;\n";
		  }

		if (ind2 != ind)
		  os << iw0 << "}\n";
	      }
	  }

	// 出力が完了した事を通知するマクロを出力
	os << '\n'
	   << "#ifdef TASK_OUT_" << my_task_id << "_COMPLETED\n"
	   << iw0 << "TASK_OUT_" << my_task_id << "_COMPLETED();\n"
	   << "#endif\n";
      }

      //
      // コード生成用の定義
      //

      /// \brief タスク関数で使用するS-MBP用の定義を CspNode 単位に出力する
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] node CspNode のポインタ
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_task_definitions (std::ostream& os, const CspNode* node)
      {
	SBChanPtrMap::const_iterator cpmi = csp_memchan_ptr_map.find (node);

	// 対象外
	if (cpmi == csp_memchan_ptr_map.end ())
	  return 0;

	const indent_writer iw0 (1);

	// ダブルバッファの面切替用変数が必要かどうか
	bool need_ch__side = false;

	const SBChanPtrList& cpl = cpmi->second;
	for (SBChanPtrList::const_iterator cpli (cpl.begin ());
	     cpli != cpl.end (); cpli++)
	  {
	    const SBChanInfo* cip = *cpli;
	    const std::string chan (cip->chan ());

	    const CspNode* cn_tail = cip->tail ();
	    const CspNode* cn_head = cip->head ();

	    std::string h_tmp;
	    std::string t_tmp;
	    const std::string& h_name = cn_head->get_block_name (h_tmp);
	    const std::string& t_name = cn_tail->get_block_name (t_tmp);

	    const rate_T& tr = cn_tail->rate ();
	    const Rate_rate_T rate_x = tr.rate ();
	    const Rate_rate_T offset_x = tr.offset ();

	    const rate_T& hr = cn_head->rate ();
	    const Rate_rate_T offset_y = hr.offset ();

	    if (cip->double_buffer ())
	      need_ch__side = true;

	    if (cn_tail == node)
	      {
		//
		// ダブルバッファの場合、NBC通信なので、そのまま出力を
		// 開始する。出力側では時刻チェックを行わない
		//
		// BC通信の場合は、受信側の起床時刻とする。また、送信側
		// の起床時刻までに受信側が起床しない場合は、送信を行わ
		// ない。
		//
		if (cip->double_buffer ())
		  os << '\n'
		     << iw0 << "/* double buffer output side:\n"
		     << iw0 << "   " << h_name << " */\n"
		     << iw0 << "static unsigned int " << mk_out_side (chan)
		     << " = 1;\n";
		else if (cip->check_output_time ())
		  os << '\n'
		     << iw0 << "/* output channel time:\n"
		     << iw0 << "     " << h_name << " */\n"
		     << iw0 << "static unsigned int " << mk_chan_time (chan)
		     << " = " << offset_y << ";\n";
	      }
	    else
	      {
		//
		// 入力で、遅いレートからは時刻及びダブルバッファの面情
		// 報が必要
		//
		// 同じレートまたは速いレートの場合で、送信側の起床が遅
		// い場合、受信を行わないための時刻情報が必要
		//
		if (cip->double_buffer ())
		  {
		    // 入力側が起動する前に、出力側が何周期動作するか算
		    // 出する。
		    size_t n = 0;
		    if (offset_y > offset_x)
		      n = (offset_y - offset_x) / rate_x;

		    // 直前の周期での面情報と出力側の次の周期の時刻を算
		    // 出し出力
		    os << '\n'
		       << iw0 << "/* double buffer input side:\n"
		       << iw0 << "   " << t_name << " */\n"
		       << iw0 << "static unsigned int " << mk_in_side (chan)
		       << " = " << (n % DB_PLANE) << ";\n"
		       << '\n'
		       << iw0 << "/* input channel time:\n"
		       << iw0 << "   " << t_name << " */\n"
		       << iw0 << "static unsigned int " << mk_chan_time (chan)
		       << " = " << offset_x + (n + 1) * rate_x << ";\n";
		  }
		else if (cip->check_input_time ())
		  os << '\n'
		     << iw0 << "/* input channel time:\n"
		     << iw0 << "   " << h_name << " */\n"
		     << iw0 << "static unsigned int " << mk_chan_time (chan)
		     << " = " << offset_x << ";\n";
	      }
	  }

	if (need_ch__side)
	  os << '\n'
	     << iw0 << "/* side index for double buffer */\n"
	     << iw0 << "int ch__side;\n";

	return 0;
      }

      /// \brief タスク関数で使用するS-MBP用の定義を出力する
      ///
      /// この関数は ATK2 のコード生成部から呼ばれる想定
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] taskset TaskSetのポインタ
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_task_definitions (std::ostream& os, const TaskSet* taskset)
      {
	int error = 0;

	for (CspNodeList::const_iterator
	       cni (taskset->cspnodelist ().begin ());
	     cni != taskset->cspnodelist ().end (); cni++)
	  {
	    const CspNode* cn = *cni;
	    error = generate_task_definitions (os, cn);
	    if (error)
	      break;
	  }

	os << '\n'
	   << indent_writer::iw1 << "/* initialized */\n"
	   << indent_writer::iw1
	   << "static unsigned char task__initialized = 0;\n";

	return error;
      }

      /// \brief 同一周期・オフセットを持つブロックのタスクを生成する
      ///
      /// \param[in] os 出力ストリーム
      /// \param[in] taskset TaskSetのポインタ
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      ///
      int
      generate_task (std::ostream& os, const TaskSet* taskset)
      {
	assert (!taskset->has_variable_rate ());

	// do_staticalize と no_gcc を上書きし、関数終了時に回復するクラス
	struct BackupOptions {
	  BackupOptions(int ds, int ng)
	    : backup_do_staticalize (do_staticalize),
	      backup_no_gcc (no_gcc)
	  {
	    do_staticalize = ds;
	    no_gcc = ng;
	  }

	  ~BackupOptions()
	  {
	    do_staticalize = backup_do_staticalize;
	    no_gcc = backup_no_gcc;
	  }

	  int backup_do_staticalize;
	  int backup_no_gcc;
	};

#if 0
	static const char* os_includes[] = {
	};
#endif

	int error = 0;
	int mark_index = -1;

	os << "/*\n"
	   << " * generate \""
	   << ATK2::model_name
	   << "\" code for SystemBuilder\n"
	   << " */\n\n"
	   << "#include \"systembuilder.h\"\n"
	   << "#include <string.h>\n";

#if 0
	// SystemBuilder用ヘッダファイル出力
	for (size_t i = 0;
	     i < sizeof os_includes / sizeof os_includes[0]; i++)
	  os << "#include \"" << os_includes[i] << "\"\n";
#endif

	// HWコアに対する #include <math.h> の置換テキストの設定
	c_include_list_T::text_map&
	  textmap = c_codewriter.include_file ().get_text_map ();
	if (taskset->coretype () == HARDWARE_CORE)
	  textmap["<math.h>"] = ("#ifdef __SYSB_HW_EXCITE__\n"
				 "#include \"ipt_math.h\"\n"
				 "#else\n"
				 "#include <math.h>\n"
				 "#endif\n");

	// Simulinkモデル用のヘッダファイルの前までinclude文を出力。モ
	// デル用ヘッダファイルはBLXMLに取り込まれているので、それは出
	// 力しない
	generate_includes (os, c_codewriter.include_file (), mark_index);

	// ヘッダファイルの二重インクルード抑止の記述があればそれを出力
	if (c_codewriter.header_preamble ().size ())
	  os << '\n' << c_codewriter.header_preamble ();

	// 共通のマクロ, Real-Time Model Objectのマクロ, Real-Time Model
	// Objectの型を出力
	if (taskset->need_rtm ())
	  {
	    if (c_codewriter.c_macro ().size ())
	      os << '\n' << c_codewriter.c_macro ();
	    if (c_codewriter.rt_macro ().size ())
	      os << '\n' << c_codewriter.rt_macro ();
	    if (c_codewriter.rt_type ().size ())
	      os << '\n' << c_codewriter.rt_type ();
	  }

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

	// 置換テキストのクリア
	if (taskset->coretype () == HARDWARE_CORE)
	  textmap.clear ();

	// 利用者指定のinclude文
	generate_user_includes (os);

	// チャネルの時刻判定マクロ
	if (enable_multirate)
	  os << "\n#define CH__TIME_LT(L,R)\t((long)((L)-(R)) < (long)0)\n"
	     << "#define CH__TIME_LE(L,R)\t((long)((L)-(R)) <= (long)0)\n";

#if 0
	// MemoryChannelで使用する型
	error = generate_memchan_types (os, taskset);
	if (error)
	  return error;
#endif

	//
	// タスク関数生成
	//

	def_list_T define_list;

	// SystemBuilderのHW側は、関数が繰り返し呼ばれるため、変数を
	// static とし、gcc用のコードとしない。
	// なお、関数終了時には(デストラクタで)自動回復する
	BackupOptions backopts (1, 1);

	const int num = taskset->core ();
	const CoreType type = taskset->coretype ();
	const rate_T& task_rate = taskset->rate ();
	const std::string
	  task_name (ATK2::mk_task_name (type, num, task_rate));

	// タスクの変数の定義
	if (ATK2::generate_definitions (os, num, taskset, true,
					task_name.c_str (), &define_list))
	  return -1;

	os << '\n';

	if (enable_multirate)
	  os << "/* rate: " << task_rate << " */\n"; // レートの表示

	os << "/* Core#" << num << ": Rate: " << task_rate << " */\n";
	os << "void " << task_name << "(void)\n" << "{";

	// マルチレートの場合は、自分の実行時刻、相手への送信タイミング
	// を知る必要があるのでその対応を行う
	if (enable_multirate)
	  os << '\n'		// 自分の時刻の変数出力
	     << indent_writer::iw1 << "/* task time */\n"
	     << indent_writer::iw1 << "static unsigned int task__time = "
	     << task_rate.offset ()
	     << ";\n";

	// S-MBP用のタスクの定義を出力
	generate_task_definitions (os, taskset);

	// タスク関数に、元コード(step関数)の先頭部分を出力
	if (c_codewriter.preamble ().size ())
	  os << '\n'
	     << indent_writer::iw1 << "/* definitions of step function */\n"
	     << c_codewriter.preamble ()
	     << indent_writer::iw1 << "/* end of definitions */\n";

	// タスク関数内に変数や関数の定義を行う
	if (ATK2::generate_definitions (os, num, taskset, false, ""))
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

	int out_initialized = 0;

	// 共通の初期化コードを出力
	if (c_codewriter.common_init ().size ())
	  {
	    os << '\n';
	    if (out_initialized++ == 0)
	      os << indent_writer::iw1 << "if (task__initialized == 0) {\n";
	    os << indent_writer::iw2 << "/* Common Initialization */\n"
	       << c_codewriter.common_init ();
	  }

	// Real-Time Model Objectが必要な場合は、初期化コードを出力
	if (taskset->need_rtm ())
	  {
	    os << '\n';
	    if (out_initialized++ == 0)
	      os << indent_writer::iw1 << "if (task__initialized == 0) {\n";
	    generate_task_rtw_initializer (os, 2);
	  }

	// 状態変数を持つ場合は、状態変数をゼロクリア
	if (taskset->has_state ())
	  {
	    const optional_refstring&
	      state_storage = c_codewriter.state_storage ();
	    os << '\n';
	    if (out_initialized++ == 0)
	      os << indent_writer::iw1 << "if (task__initialized == 0) {\n";
	    os << indent_writer::iw2 << "/* initialize state structure */\n"
	       << indent_writer::iw2 << "memset(&" << state_storage
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

	    os << '\n';
	    if (out_initialized++ == 0)
	      os << indent_writer::iw1 << "if (task__initialized == 0) {\n";
	    os << indent_writer::iw2 << "/* Block: " << n << " */\n";

	    generate_task_context_initializer (os, node, 2, &cnmap);
	  }

	// 本来、ここで初期値を受信するが、S-MBPでは初期値は受信しない

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

	if (out_initialized)
	  os << '\n'
	     << indent_writer::iw2 << "task__initialized = 1;\n"
	     << indent_writer::iw1 << "}\n";

	//
	// 本来、ここでタスクの一斉起動等の対応を行うべきだが、S-MBPで
	// は行わない
	//

	// イテレーションの先頭で何かするためのマクロ
	os << '\n'
	   << "#ifdef TASK_" << task_name << "_ITERATION_TOP\n"
	   << indent_writer::iw1
	   << "TASK_" << task_name << "_ITERATION_TOP\n"
	   << "#endif\n";

	// RateTransition の CspNode を除くタスクのコードを出力
	ATK2::generate_task_code_in_loop (os, taskset, ATK2::GENCODE_TASK);

	// RateTransition の CspNode のタスクのコードを出力
	ATK2::generate_task_code_in_loop (os, taskset,
					  ATK2::GENCODE_RATETRANSITION);

	// RateTransition の CspNode を除くUpdateのコードを出力
	ATK2::generate_task_code_in_loop (os, taskset, ATK2::GENCODE_UPDATE);

	// Real-Time Model Objectの更新が必要であれば出力
	if (taskset->need_rtm ())
	  generate_update_rtw (os, 1);

	// タスクの時刻更新
	if (enable_multirate)
	  os << '\n'
	     << indent_writer::iw1
	     << "task__time += " << task_rate.rate () << ";\n";

	// イテレーションの終了で何かするためのマクロ
	os << '\n'
	   << "#ifdef TASK_" << task_name << "_ITERATION_END\n"
	   << indent_writer::iw1
	   << "TASK_" << task_name << "_ITERATION_END\n"
	   << "#endif\n";

	// タスク関数の終了
	os << "}\n\n";

	// タスク関数外に出力済みのマクロをundefineする
	if (!define_list.empty ())
	  {
	    os << '\n';
	    define_list.undefine (os);
	  }

	return error;
      }

      /// \brief 同一周期・オフセットを持つブロックのタスクを生成する
      ///
      /// \param[in] file ファイル名
      /// \param[in] taskset TaskSetのポインタ
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      ///
      int
      generate_task (const std::string& file, const TaskSet* taskset)
      {
	std::ofstream ofs (file.c_str ());

	if (!ofs)
	  {
	    std::cerr << file << ": Couldn't write: "
		      << strerror (errno) << std::endl;
	    return -1;
	  }

	ofs.exceptions (std::ostream::failbit | std::ostream::badbit);

	int error = -1;

	try
	  {
	    error = generate_task (ofs, taskset);
	  }

	catch (std::ofstream::failure e)
	  {
	    std::cerr << file << ": " << e.what () << std::endl;
	  }

	if (ofs)
	  ofs.close ();

	return error;
      }

      /// \brief SystemBuilderコード生成関数の本体
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_task (void)
      {
	int error = 0;

	make_smbp_memchan ();

	std::string prefix (std::string (smbp_prefix) + "_");

	for (size_t i = 0; i < ATK2::pcore_tslist.size (); i++)
	  {
	    const baseTaskSetList& btsl = ATK2::pcore_tslist[i];
	    for (baseTaskSetList::const_iterator tsi (btsl.begin ());
		 tsi != btsl.end (); tsi++)
	      {
		const TaskSet* ts = *tsi;

		std::string out = ATK2::mk_task_name (ts, &prefix) + ".c";
		error = generate_task (out, ts);
		if (error)
		  break;
	      }
	  }

	return error;
      }

      /// \brief SystemBuilder用YAMLファイルの生成本体
      ///
      ///
      /// \param[in] os 出力ストリーム
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_yaml (std::ostream& os)
      {
	int error = 0;
	unsigned long sw_base_rate = ULONG_MAX;

	os << "SysName: " << ATK2::model_name << "\n";

	//
	// CPU
	//

	int cpu_state = 0;

	unsigned int sw_cpu_num = 1;
	for (size_t i = 0; i < ATK2::pcore_tslist.size (); i++)
	  {
	    const baseTaskSetList& btsl = ATK2::pcore_tslist[i];

	    if (btsl[0]->coretype () == SOFTWARE_CORE)
	      {
		if (cpu_state != 1)
		  {
		    os << "\nSW:\n";
		    cpu_state = 1;
		  }

		os << "  - {core: " << sw_core_type << ", "
		   << "id: " << sw_cpu_num++ << ", " << "process: [";

		unsigned int pnum = 0;
		for (baseTaskSetList::const_iterator tsi (btsl.begin ());
		     tsi != btsl.end (); tsi++)
		  {
		    if (pnum++)
		      os << ", ";
		    os << ATK2::mk_task_name (*tsi);

		    const rate_T& ts_rate = (*tsi)->rate ();
		    if (sw_base_rate == ULONG_MAX)
		      sw_base_rate = ts_rate.rate ();
		    else
		      sw_base_rate = SampleTime::gcd (sw_base_rate,
						      ts_rate.rate ());
		  }
		os << "]}\n";
	      }
	    else
	      {
		if (cpu_state != 2)
		  {
		    os << "\nHW:\n";
		    cpu_state = 2;
		  }

		os << "  - {core: " << hw_core_type << ", " << "process: [";

		unsigned int pnum = 0;
		for (baseTaskSetList::const_iterator tsi (btsl.begin ());
		     tsi != btsl.end (); tsi++)
		  {
		    if (pnum++)
		      os << ", ";
		    os << ATK2::mk_task_name (*tsi);
		  }
		os << "]}\n";
	      }
	  }

	//
	// 同期関連
	//

	// タスクのベースレートを ns 単位にする
	unsigned int ns_baserate = round (SampleTime::get_tick () / 1e-9);

	char* p;
	unsigned long
	  hw_base_ns = strtoul (cyclic_sync_hw_base_nsec.c_str (), &p, 0);
	unsigned long sw_base_ns;

	bool cyclic_sync_error = (*p || hw_base_ns == 0);

	if (cyclic_sync_sw_base_nsec.length ())
	  {
	    sw_base_ns = strtoul (cyclic_sync_sw_base_nsec.c_str (), &p, 0);
	    if (*p || sw_base_ns)
	      cyclic_sync_error = true;
	  }
	else
	  sw_base_ns = sw_base_rate * ns_baserate;

	if (!cyclic_sync_error && sw_base_ns % hw_base_ns)
	  {
	    std::cout << "Warning: S/W base rate[" << sw_base_ns
		      << "] not divide H/W base rate[" << hw_base_ns
		      << "]\n"
		      << "         S/W base rate => 1000000"
		      << std::endl;
	    sw_base_ns = 1000000; // 1sec
	  }

	if (hw_freq_mhz.length ())
	  os << "\nHW_FREQ_MHZ: " << hw_freq_mhz << '\n';

	os << "\nCyclicSync:\n"
	   << "  init_start: " << cyclic_sync_init_start << '\n'
	   << "  sw_base_nsec: " << sw_base_ns << '\n'
	   << "  hw_base_nsec: " << hw_base_ns << '\n';

	//
	// チャネル
	//

	SBChanPtrList bc_ch;	// BCチャネル
	SBChanPtrList nbc_ch;	// NBCチャネル
	SBChanPtrList mem_ch;	// MEMチャネル

	for (CspNodeList::const_iterator
	       i (CspNode::csp_nodelist ().begin ());
	     i != CspNode::csp_nodelist ().end (); i++)
	  {
	    const CspNode* cn_tail = *i;

	    // CspNode に対するSystemBuilderのチャネルを探す
	    SBChanMap::iterator msbci = csp_memchan_map.find (cn_tail);

	    // SystemBuilderのチャネルがない
	    if (msbci == csp_memchan_map.end ())
	      continue;

	    // チャネルのタイプ毎に振り分ける
	    SBChanList& ml = msbci->second;
	    for (SBChanList::iterator mli (ml.begin ());
		 mli != ml.end (); mli++)
	      {
		SBChanInfo& sbci = *mli;

		switch (sbci.chtype ())
		  {
		  case SBChanInfo::NBC_CH:
		    nbc_ch.push_back (&sbci);
		    break;
		  case SBChanInfo::BC_CH:
		    bc_ch.push_back (&sbci);
		    break;
		  case SBChanInfo::MEM_CH:
		    mem_ch.push_back (&sbci);
		    break;
		  case SBChanInfo::BC_MEM_CH:
		    mem_ch.push_back (&sbci);
		    bc_ch.push_back (&sbci);
		  };
	      }
	  }

	// BCチャネル
	if (bc_ch.size ())
	  {
	    os << "\nBlockingChannel:\n";
	    for (SBChanPtrList::iterator i (bc_ch.begin ());
		 i != bc_ch.end (); i++)
	      {
		SBChanInfo& sbci = **i;
		const std::string chan (sbci.chan ());

		os << "  - {name: " << bc_prefix << chan << ", size: ";

		if (sbci.chtype () == SBChanInfo::BC_CH)
		  {
		    os << sbci[0].size () * 8; // 3byteはない

		    const smbp_var_writer& vw = sbci[0];
		    if (vw.has_sb_type ())
		      os << ", " << vw.sbtype ();
		  }
		else
		  os << "32";
		os << "}\n";
	      }
	  }

	// NBCチャネル
	if (nbc_ch.size ())
	  {
	    os << "\nNonBlockingChannel:\n";
	    for (SBChanPtrList::iterator i (nbc_ch.begin ());
		 i != nbc_ch.end (); i++)
	      {
		SBChanInfo& sbci = **i;
		const std::string chan (sbci.chan ());

		// ダブルバッファである
		for (size_t i = 0; i < DB_PLANE; i++)
		  {
		    os << "  - {name: " << nbc_prefix << chan << '_' << i
		       << ", size: "
		       << sbci[0].size () * 8; // 3byteはない
		    // NBCの初期値は0とする
		    const smbp_var_writer& vw = sbci[0];
		    SizeAlignInfo::TypeInfo
		      t = vw.sizealigninfo ()->typeinfo ();
		    if (t == SizeAlignInfo::TYPE_INT)
		      os << ", init_val: 0";
		    if (vw.has_sb_type ())
		      os << ", " << vw.sbtype ();
		    os << "}\n";
		  }
	      }
	  }

	// MEMチャネル
	if (mem_ch.size ())
	  {
	    os << "\nMemoryChannel:\n";
	    for (SBChanPtrList::iterator i (mem_ch.begin ());
		 i != mem_ch.end (); i++)
	      {
		SBChanInfo& sbci = **i;
		const std::string chan (sbci.chan ());

		if (sbci.double_buffer ())
		  {
		    for (size_t b = 0; b < DB_PLANE; b++)
		      {
			for (size_t n = 0; n < sbci.size (); n++)
			  {
			    const SBChanMember& sbcm = sbci[n];
			    const smbp_var_writer& vw = sbcm;
			    os << "  - {name: "
			       << mem_prefix << chan << "_" << n << "_" << b
			       << ", size: "
			       << sbcm.size () * 8 // 3byteはない
			       << ", depth: ";

			    // depth の算出
			    // 構造体の要素数と配列サイズの積
			    size_t a = vw.size ();
			    size_t e = sbcm.elements ().size ();
			    size_t d = 1;
			    if (e)
			      {
				if (a)
				  d = e * a;
				else
				  d = e;
			      }
			    else if (a)
			      d = a;
			    os << d;

			    if (vw.has_sb_type ())
			      os << ", " << vw.sbtype ();
			    os << "}\n";
			  }
		      }
		  }
		else
		  {
		    for (size_t n = 0; n < sbci.size (); n++)
		      {
			const SBChanMember& sbcm = sbci[n];
			const smbp_var_writer& vw = sbcm;
			os << "  - {name: " << mem_prefix << chan << "_" << n
			   << ", size: " << sbcm.size () * 8 // 3byteはない
			   << ", depth: ";

			// depth の算出
			// 構造体の要素数と配列サイズの積
			size_t a = vw.size ();
			size_t e = sbcm.elements ().size ();
			size_t d = 1;
			if (e)
			  {
			    if (a)
			      d = e * a;
			    else
			      d = e;
			  }
			else if (a)
			  d = a;
			os << d;

			if (vw.has_sb_type ())
			  os << ", " << vw.sbtype ();
			os << "}\n";
		      }
		  }
	      }
	  }

	//
	// プロセス
	//

	os << "\nStandardProcess:";
	for (size_t i = 0; i < ATK2::pcore_tslist.size (); i++)
	  {
	    const baseTaskSetList& btsl = ATK2::pcore_tslist[i];
	    for (baseTaskSetList::const_iterator tsi (btsl.begin ());
		 tsi != btsl.end (); tsi++)
	      {
		const TaskSet* ts = *tsi;

		//
		// プロセスの入力・出力チャネルの振り分け
		//

		SBChanPtrList inch;
		SBChanPtrList outch;

		for (CspNodeList::const_iterator
		       cni (ts->cspnodelist ().begin ());
		     cni != ts->cspnodelist ().end (); cni++)
		  {
		    const CspNode* cn = *cni;

		    // CspNode に対するチャネルの探索
		    const SBChanPtrMap::iterator
		      sbcpi = csp_memchan_ptr_map.find (cn);

		    // SystemBuilderのチャネルを持たない
		    if (sbcpi == csp_memchan_ptr_map.end ())
		      continue;

		    // CspNode のチャネルを入出力に分けて登録する
		    const SBChanPtrList& sbcpl = sbcpi->second;
		    for (SBChanPtrList::const_iterator sbcpli (sbcpl.begin ());
			 sbcpli != sbcpl.end (); sbcpli++)
		      {
			SBChanInfo* sbci = *sbcpli;

			// ダブルバッファのMEMチャネルは除外
			if (sbci->chtype () == SBChanInfo::MEM_CH)
			  continue;

			// CspNode が tail なら出力。そうでなければ入力
			if (sbci->tail () == cn)
			  outch.push_back (sbci);
			else
			  inch.push_back (sbci);
		      }
		  }

		//
		// プロセス毎の出力
		//

		std::string prefix (std::string (smbp_prefix) + "_");
		std::string task_name (ATK2::mk_task_name (ts, &prefix));

		os << "\n  - name: " << task_name << '\n';

		os << "    file: [" << task_name << ".c]\n";
		if (inch.size ())
		  {
		    os << "    ch(in): [";
		    unsigned int ch_num = 0;
		    for (SBChanPtrList::iterator sbcpli (inch.begin ());
			 sbcpli != inch.end (); sbcpli++)
		      {
			SBChanInfo* sbci = *sbcpli;
			const std::string chan (sbci->chan ());

			// NBCチャネルはダブルバッファで、
			// BCチャネルは単一
			if (sbci->chtype () == SBChanInfo::NBC_CH)
			  {
			    for (size_t n = 0; n < DB_PLANE; n++)
			      {
				if (ch_num++)
				  os << ", ";
				os << nbc_prefix << chan << '_' << n;
			      }
			  }
			else
			  {
			    if (ch_num++)
			      os << ", ";
			    os << bc_prefix << chan;
			  }
		      }
		    os << "]\n";
		  }
		if (outch.size ())
		  {
		    os << "    ch(out): [";
		    unsigned int ch_num = 0;
		    for (SBChanPtrList::iterator sbcpli (outch.begin ());
			 sbcpli != outch.end (); sbcpli++)
		      {
			SBChanInfo* sbci = *sbcpli;
			const std::string chan (sbci->chan ());

			// NBCチャネルはダブルバッファで、
			// BCチャネルは単一
			if (sbci->chtype () == SBChanInfo::NBC_CH)
			  {
			    for (size_t n = 0; n < DB_PLANE; n++)
			      {
				if (ch_num++)
				  os << ", ";
				os << nbc_prefix << chan << '_' << n;
			      }
			  }
			else
			  {
			    if (ch_num++)
			      os << ", ";
			    os << bc_prefix << chan;
			  }
		      }
		    os << "]\n";
		  }
		os << "    cycle_nsec: "
		   << ts->rate ().rate () * ns_baserate << "\n";
		os << "    start_offset: "
		   << ts->rate ().offset () * ns_baserate << "\n";
	      }
	  }

	return error;
      }

      /// \brief SystemBuilder用YAMLファイルの生成
      ///
      /// \retval 0 成功
      /// \retval -1 エラー
      int
      generate_yaml (void)
      {
	std::ofstream ofs (smbp_yaml);

	if (!ofs)
	  {
	    std::cerr << smbp_yaml << ": Couldn't write: "
		      << strerror (errno) << std::endl;
	    return -1;
	  }

	ofs.exceptions ( std::ostream::failbit | std::ostream::badbit );

	int error = -1;

	try
	  {
	    error = generate_yaml (ofs);
	  }

	catch (std::ofstream::failure e)
	  {
	    std::cerr << smbp_yaml << ": " << e.what () << std::endl;
	  }

	if (ofs)
	  ofs.close ();

	return error;
      }


      /// @}
    }

    /// @}

    using namespace SMBP;

    /// \addtogroup SMBP_GENERATOR_C_CODE
    ///
    /// @{

    /// \brief SystemBuilder用コード生成機能の初期化
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    int
    smbp_generator_init (void)
    {
      SampleTime::set_tick (0.001); // 1ms
      assert_output_enable = 0;	    // assert()を出さない
      smbp_mode = true;		    // S-MBPモード有効

      sizealign_default_init ();
      YAMLCallBack::add_callback ("SystemBuilderParameters",
				  yaml_cb_SystemBuilderParametes);
      YAMLCallBack::add_callback ("SystemBuilderTypes",
				  yaml_cb_SystemBuilderTypes);
      YAMLCallBack::add_callback ("EmbeddedCoderTypes",
				  yaml_cb_EmbeddedCoderTypes);

      return 0;
    }

    /// \brief SystemBuilder用コード生成
    ///
    /// \retval 0 成功
    /// \retval -1 エラー
    int
    smbp_generate_code (void)
    {
      std::string output_yaml;
      int error;

      error = ATK2::prepare_generate_code ();
      if (error)
	return error;

      if (smbp_prefix == NULL)
	smbp_prefix = ATK2::model_name.c_str ();

      if (smbp_yaml == NULL)
	{
	  output_yaml = std::string (smbp_prefix) + "-smbp.yaml";
	  smbp_yaml = output_yaml.c_str ();
	}

      error = generate_task ();
      if (error)
	return error;

      error = generate_yaml ();
      if (error)
	return error;

      return 0;
    }

    /// @}
  }
}
