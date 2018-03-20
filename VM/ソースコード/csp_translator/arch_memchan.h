// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef ARCH_MEMCHAN_H
#define ARCH_MEMCHAN_H

/// \file arch_memchan.h
///
/// \brief Cコードライタのヘッダファイル

#include <iostream>

/// \addtogroup CSP_TRANSLATOR
///
/// @{

/// \defgroup ARCH_MEMCHAN メモリ上のチャネル用ライタ
///
/// \brief チャネルをメモリに配置するアーキテクチャ用ライタ
///
/// @{

/// \brief チャネルのメンバのプレフィクス
///
/// 変数または構造体のプレフィクスである
#define ARCH_MEMCH_MEMBER_PREFIX	"m__"

/// @}

/// @}

namespace Simulink
{
  namespace CspTrans
  {
    /// \addtogroup ARCH_MEMCHAN
    ///
    /// @{

    /// \brief チャネルの入出力タイミングのモード
    enum channel_io_mode
      {
	CH_IO_MODE_INIT,	///!< 初期値の送受信
	CH_IO_MODE_TASK,	///!< タスクの入出力
	CH_IO_MODE_UPDATE	///!< Updateの入力
      };


    /// \brief assertを出してよいかどうか
    extern int assert_output_enable;

    /// \brief S-MBPモード
    extern bool smbp_mode;

    /// \brief 変数または構造体のプレフィクスの const な string
    extern const std::string ch_member_prefix;

    /// \brief チャネル操作用マクロ生成
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] var_prefix チャネルの変数のプレフィクス
    void generate_channel_macros (std::ostream& os,
				  const std::string& var_prefix);


    /// \brief チャネル操作用inline関数生成
    ///
    /// \param[in] os 出力ストリーム
    void generate_channel_functions (std::ostream& os);


    /// \brief イベントが必要なSubSystemが送信するイベントの型とコピー用マクロ
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] unknown_state 状態変数名が未定義の際に使用する名前
    void
    generate_sybsystem_state_and_macros (std::ostream& os,
					 const std::string& unknown_state);


    /// \brief CspNode 間の通信チャネルの型の生成
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] unknown_state 状態変数名が未定義の際に使用する名前
    void generate_channel_types (std::ostream& os,
				 const std::string& unknown_state);


    /// \brief 出力チャネルに変数を設定するコードの出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] ce 対象の CspEdge の参照
    /// \param[in] chan チャンネルを示す文字列
    /// \param[in] ind インデント
    ///
    /// 出力変数をチャネルに設定するコードを出力する
    void generate_task_output_values (std::ostream& os, const CspEdge& ce,
				      const std::string& chan, int ind);


    /// \brief 出力チャネル毎のサンプル時間のオフセット設定
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] ind インデント
    /// \param[in,out] first 最初の出力かどうかのbool型の参照
    ///
    /// CspNodeに対する出力チャネルに出力先のサンプル時間のオフセットを
    /// 設定する
    void generate_task_set_offset (std::ostream& os, CspNode* node, int ind,
				   bool& first);


    /// \brief 共有するDataStoreの出力
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] prefix データストアの状態変数のプレフィクス
    ///
    /// ブロックの状態変数とは同居できないので \a prefix
    /// で指定したプレフィクスを付けた独立な構造体名を変えて出力する
    void generate_datastore (std::ostream& os, const std::string& prefix);


    /// \brief 入出力の待ち合わせを行うためのチェック用ベクタを出力する
    ///
    /// \param[in] os 出力オペレータ
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] task_id タスクIDの4桁の文字列
    /// \param[in,out] first 最初かどうか
    ///
    /// チャネルの数に対応したビットベクタを出力する。0,1の場合は不要。
    void generate_task_channel_vector (std::ostream& os, CspNode* node,
				       const std::string& task_id,
				       bool& first);


    /// \brief CspNode のチャネルの入力処理を出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] core コア割り当てのスレッドかどうか
    /// \param[in] ind インデント
    /// \param[in] input_mode 初期値、タスク、Updateの入力のいずれか
    /// \param[in] unknown_state 状態変数名が未定義の際に使用する名前
    ///
    /// CspNode に対する入力チャネルの処理コードを出力する
    ///
    /// 入力側に関し、基本的なチャネルの要素は以下の二つである
    /// -# データがあるかないか示すフラグ(0は空、1以上はデータがある)
    /// -# チャネルで通信する変数
    ///
    /// 入力処理は、以下のように動く
    /// -# 入力のループで当該チャネルから入力済みなら何もしない。\n
    ///    これは generate_task_channel_vector()
    ///    で生成された、入力用ビットベクタの入力チャネルに相当するビット
    ///    チェックすることで行う。\n
    ///    ただし、チャネル数が1の場合はビットベクタは使わない
    /// -# フラグが0なら入力がないので当該チャネルの入力を行わない(入力待ち)
    /// -# フラグが1以上ならデータがあるので以下を行う
    ///    -# チャネルからタスク内の入力変数へコピーする
    ///    -# フラグから1を引く
    ///    -# 入力用ビットベクタを1にする
    ///
    /// フラグの有効な値は、送信側が低レート(遅い)、
    /// 受信側が高レート(速い)場合は、1以上であり、
    /// 同じか受信側のレートが低い(遅い)場合は1である。
    void generate_task_wait_input_channel (std::ostream& os, CspNode* node,
					   bool core, int ind,
					   channel_io_mode input_mode,
					   const std::string& unknown_state);


    /// \brief CspNode のチャネルの出力処理を出力する
    ///
    /// \param[in] os 出力ストリーム
    /// \param[in] node 対象の CspNode のポインタ
    /// \param[in] core コア割り当てのスレッドかどうか
    /// \param[in] ind インデント
    /// \param[in] unknown_state 状態変数名が未定義の際に使用する名前
    ///
    /// CspNode に対する出力チャネルの処理コードを出力する
    ///
    /// 出力側に関し、基本的なチャネルの要素は以下の三つである
    /// -# データがあるかないか示すフラグ(0は空、1以上はデータがある)
    /// -# 入力側の次の実行時刻
    /// -# チャネルで通信する変数
    ///
    /// 出力処理は、以下のように動く
    /// -# 出力のループで、当該チャネルへ出力済みなら何もしない。\n
    ///    これは generate_task_channel_vector()
    ///    で生成された、出力用ビットベクタの出力チャネルに相当するビット
    ///    チェックすることで行う。\n
    ///    ただし、チャネル数が1の場合はビットベクタは使わない
    /// -# 出力すべきタイミングかどうか判定する。\n
    ///    出力タイミングでなければこの出力ループ内では出力しない
    ///    - 同じレートまたは、遅いレートから速いレートへの出力は、常に行う
    ///    - 速いレートから遅いレートへの出力は、入力側の実行時刻が、
    ///      出力側の次の実行時刻と同じかその前である場合のみ行う。\n
    ///      出力タイミングでないなら出力用ビットベクタを1にして出力済みとする
    /// -# フラグが0以外なら出力できないので当該チャネルの出力を行わない
    ///    (出力待ち)
    /// -# フラグが0ならチャネルが空なので、以下の事を行う
    ///    -# チャネルへタスク内の出力変数をコピーする
    ///    -# 自タスクの次の実行時間までに、入力側が何回入力を行うかを算出。\n
    ///       同じレートまたは遅いレートから速いレートへの出力では1である。\n
    ///       そうでなければ1以上である
    ///    -# 入力回数をフラグに設定する
    ///    -# 入力側の時刻を合わせて設定する
    ///    -# 出力用ビットベクタを1にする
    ///
    /// \sa generate_task_set_offset()
    void generate_task_wait_output_channel (std::ostream& os, CspNode* node,
					    bool core, int ind,
					    const std::string& unknown_state);


    /// @}
  }
}

#endif // ARCH_MEMCHAN_H
