// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef ATK2_GENERATOR_H
#define ATK2_GENERATOR_H

/// \file atk2_generator.h
///
/// \brief Toppers/ATK2用コード生成のCSPトランスレータ用インタフェース

#ifndef ATK2_WEAK
#ifdef __GNUC__
#define ATK2_WEAK	__attribute__ ((weak))
#else  // !__GNUC__
#error "unknown compiler"
#endif	// !__GNUC__
#endif	// !ATK2_WEAK

namespace Simulink
{
  namespace CspTrans
  {
    // このモデルのAPPMODEのポストフィクス
    extern std::string atk2_appmode ATK2_WEAK;

    // このモデル用の出力コードファイル名
    extern const char* atk2_output_code ATK2_WEAK;

    // このモデルのABREX用YAMLファイル名
    extern const char* atk2_abrex_yaml ATK2_WEAK;

    // このモデル用のヘッダファイル名
    extern const char* atk2_app_header ATK2_WEAK;

    // このモデルで使用するタイマのISR
    extern const char* atk2_timer_isrref ATK2_WEAK;

    // このモデルではタイマのISRを使用しない
    extern bool atk2_no_timer_isrref ATK2_WEAK;

    // このモデルで使用するタイマのカウンタ
    extern const char* atk2_timer_counterref ATK2_WEAK;

    // このモデルで使用するタイマの周波数
    extern const char* atk2_timer_hz ATK2_WEAK;

    // mainとStartupHookを生成するかどうか
    extern bool atk2_gen_main ATK2_WEAK;

    // TOPPERS/ATK2用コード生成機能の初期化
    int atk2_generator_init (void) ATK2_WEAK;

    // TOPPERS/ATK2用コード生成
    int atk2_generate_code (void) ATK2_WEAK;
  }
}

#endif	// ATK2_GENERATOR_H
