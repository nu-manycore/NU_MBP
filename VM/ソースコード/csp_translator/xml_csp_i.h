/* -*- mode: c++; coding: utf-8-unix; -*- */
#ifndef XML_CSP_I_H
#define XML_CSP_I_H

/// \file xml_csp_i.h
///
/// \brief xml_csp.h, c_codewriter.h が相互参照する際に、
/// 外出しする必要がある型や暮らすの不完全型の定義と宣言

//
// Boostのthrow_exceptionの対応
// (boostのヘッダファイルはこれが定義された後にincludeされねばならない)
//
#include <boost/config.hpp>

#ifdef BOOST_NO_EXCEPTIONS
#include <boost/assert.hpp>
namespace boost
{
  /// \brief boostでexceptionなしでビルドされている場合に必要な
  /// throw_exception()
  ///
  /// \param[in] e エラー
  template <class E>
  void
  throw_exception (const E& e)
  {
    std::cerr << e.what () << std::endl;
    BOOST_ASSERT(false);
  }
}
#endif

#include <vector>

#include "Simulink_XSD/SimulinkXML.h"

#ifdef __GNUC__
// TR1 using BOOST Project
// libstdc++-V3 doesn't work
#include <boost/tr1/regex.hpp>
#else
#include <tr1/regex>
#endif

//
// xml_csp.hと相互参照を解決するための宣言と定義
//
namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup XMLCSP_MAIN
    ///
    /// @{

    /// \brief CspNode のコード配置の対象種別
    enum CoreType {
      /// \brief SW(CPU)への配置
      SOFTWARE_CORE,

      /// \brief HW(FPGA等の機能合成等)への配置
      HARDWARE_CORE
    };

    // 以下はxml_csp.hで定義されるクラスの不完全型
    class CspNode;
    class CspEdge;
    class base_CspBlockInfo;
    class base_CspBlockConnect;
    class base_CspVar;
    class param_ent_T;
    class ParamList;

    /// \brief レートの型
    ///
    /// サンプル時間のベースレートに対する整数比の型
    typedef long Rate_rate_T;

    /// \brief オフセットの型
    ///
    /// サンプル時間のオフセットのベースレートに対する整数比の型
    typedef unsigned long Rate_offset_T;

    /// \brief 変数の属性の型
    ///
    /// \sa VAR_MODE
    typedef unsigned int var_mode_T;

    /// \brief CspBlockInfo の型
    ///
    /// CspBlockInfo は base_CspBlockInfo を継承したクラス
    ///
    /// \sa base_CspBlockInfo
    /// \sa base_CspBlockConnect
    /// \sa base_CspVar
    typedef Simulink::XML::Tmpl_BlockInfo< base_CspBlockInfo,
					   base_CspBlockConnect,
					   base_CspVar
					   > CspBlockInfo;

    /// \brief CspBlockInfo のリストの型
    ///
    /// \sa CspNode::contain_blocks()
    /// \sa base_CspBlockInfo::merge_to()
    typedef Simulink::XML::Tmpl_block_list_T<CspBlockInfo> csp_block_list_T;

    /// \brief CspBlockInfo のリストの反復子の型
    typedef csp_block_list_T::iterator csp_block_list_iterator;

    /// \brief CspBlockInfo のリストの反復子(const)の型
    typedef csp_block_list_T::const_iterator csp_block_list_const_iterator;

    /// \brief CspVar の型
    ///
    /// \sa base_CspVar
    typedef Simulink::XML::Tmpl_var_T_wrapper< base_CspVar > CspVar;

    /// \brief CspNode のポインタのベクタ
    ///
    /// \sa CspNode
    class CspNodeList
      : public std::vector< CspNode* >
    {
      /// \brief クラス内部で用いる継承している CspNode
      /// のポインタのベクタの型
      typedef std::vector< CspNode* > this_vec;

    public:
      /// \brief CspNodeList のデフォルトコンストラクタ
      CspNodeList() {}

      /// \brief CspNodeList のサイズ指定のコンストラクタ
      ///
      /// \param[in] n ベクタのサイズ
      CspNodeList(size_t n) : this_vec (n) {}

      /// \brief CspNodeList のサイズと初期値指定のコンストラクタ
      ///
      /// \param[in] n ベクタのサイズ
      /// \param[in] v CspNode のポインタの初期値。通常NULL
      CspNodeList(size_t n, CspNode* v) : this_vec (n,v) {}

      // TODO: 有用なメンバ関数の追加
    };

    // 以下はc_code_writer.hで定義されるクラスの不完全型
    class function_ent_T;

    /// \brief 関数エントリのポインタの配列
    ///
    /// \sa function_ent_T
    class function_ptr_list_T
      : public std::vector < function_ent_T* >
    {
      /// \brief クラス内部で用いる継承している関数エントリのポインタの
      /// 配列の型
      typedef std::vector < function_ent_T* > this_vec;
    public:
      /// \brief function_ptr_list_T のデフォルトコンストラクタ
      function_ptr_list_T() {}

      /// \brief function_ptr_list_T のサイズ指定のコンストラクタ
      ///
      /// \param[in] n ベクタのサイズ
      function_ptr_list_T(size_t n) : this_vec (n) {}

      /// \brief function_ptr_list_T のサイズと初期値指定のコンストラクタ
      ///
      /// \param[in] n ベクタのサイズ
      /// \param[in] v finction_ent_T のポインタの初期値。通常NULL
      function_ptr_list_T(size_t n, function_ent_T* v) : this_vec (n, v) {}

      // TODO: 有用なメンバ関数の追加
    };

    /// @}
  }
}

#endif	/* XML_CSP_I_H */
