// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef SMBP_GENERATOR_H
#define SMBP_GENERATOR_H

/// \file smbp_generator.h
///
/// \brief SystemBuilder向けコード生成のCSPトランスレータ用インタフェース

#ifndef SMBP_WEAK
#ifdef __GNUC__
/// \brief smbp_generator を取り外し可能にするためのマクロ
///
/// smbp_generator.c では SMBP_WEAK を空にしてweakシンボルではない実体を
/// 宣言する。csp_translator側からはweakシンボルにする。
/// これにより smbp_generator が取り外し可能になる
#define SMBP_WEAK	__attribute__ ((weak))
#else	// !__GNUC__
#error "unknown compiler"
#endif	// !__GNUC__
#endif	// !SMBP_WEAK

namespace Simulink
{
  namespace CspTrans
  {
    // このモデルのSystemBuilder用YAMLファイル
    extern const char* smbp_yaml SMBP_WEAK;

    // このモデルの出力コードのファイル名のベース名
    extern const char* smbp_prefix SMBP_WEAK;

    // longは64bit
    extern bool smbp_long_is_64bit SMBP_WEAK;

    // realは64bit
    extern bool smbp_real_is_64bit SMBP_WEAK;

    // 64bit longのアラインメントの指定。デフォルトでは4バイト
    extern size_t smbp_long64_align SMBP_WEAK;

    // doubleのアラインメントの指定。デフォルトでは4バイト
    extern size_t smbp_real64_align SMBP_WEAK;

    // 構造体のアラインメントの指定。デフォルトでは4バイト
    extern size_t smbp_struct_align SMBP_WEAK;

    // SytemBuilder用のメモリチャネルのアラインメント指定。
    // デフォルトでは64バイト
    extern size_t smbp_mem_align SMBP_WEAK;

    // SystemBuilder用コード生成機能の初期化
    int smbp_generator_init (void) SMBP_WEAK;

    // SystemBuilder用コード生成
    int smbp_generate_code (void) SMBP_WEAK;
  }
}

#endif	// SMBP_GENERATOR_H
