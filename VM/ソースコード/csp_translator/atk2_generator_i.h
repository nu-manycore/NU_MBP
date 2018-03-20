// -*- mode: c++; coding: utf-8-unix; -*-
#ifndef ATK2_GENERATOR_I_H
#define ATK2_GENERATOR_I_H

/// \file atk2_generator_i.h
///
/// \brief TOPPERS/ATK2向けコード生成のローカルな宣言・定義

#include <vector>
#include <string>

namespace Simulink
{
  namespace CspTrans
  {
    namespace ATK2
    {
      /// \addtogroup ATK2_GENERATOR_C_CODE
      ///
      /// @{

      /// \brief タスクのコード生成種別
      enum GenCodeType {
	/// \brief タスクのコード
	GENCODE_TASK,
	/// \brief Updateのコード
	GENCODE_UPDATE,
	/// \brief RateTransitionのコード
	GENCODE_RATETRANSITION
      };

      /// \brief 状態変数のオブジェクトが未定義の場合の代替名
      extern const std::string unknown_state;

      /// \brief DataStoreMemory用状態変数のオブジェクトのプレフィクス
      extern const std::string dsm_prefix;

      /// \brief モデル名
      extern std::string model_name;

      // 処理済み関数リスト(マーク用ベクタ)
      extern std::vector< bool > output_function_mark;

      // 処理済み共通関数リスト
      extern std::vector< bool > output_common_function_mark;

      // 物理コアのタスク割り当て
      extern std::vector < baseTaskSetList > pcore_tslist;

      // ATK2のコア数
      extern size_t number_of_cores;

      /// \brief Tpooers/ATK2用タスクベース名生成
      ///
      /// \param[in] type コアタイプ
      /// \param[in] num コア番号
      /// \param[in] rate レート
      /// \param[in] prefix 出力するプレフィクスの指定
      ///
      /// \return タスクベース名
      inline std::string
      mk_base_name (CoreType type, int num, const rate_T& rate,
		    const std::string* prefix = NULL)
      {
	std::ostringstream oss;
	oss << (prefix  ? *prefix : block_name_prefix);
	if (type == HARDWARE_CORE)
	  oss << "HW";
	oss << "Core" << num << '_' << rate.rate () << '_' << rate.offset ();
	return oss.str ();
      }

      /// \brief Tpooers/ATK2用タスクベース名生成
      ///
      /// \param[in] node CspNode のポインタ
      /// \param[in] prefix 出力するプレフィクスの指定
      ///
      /// \return タスク名
      inline std::string
      mk_base_name (const CspNode* node,
		    const std::string* prefix = NULL)
      {
	return mk_base_name (node->coretype (), node->core (), node->rate (),
			     prefix);
      }

      /// \brief Tpooers/ATK2用タスク名生成
      ///
      /// \param[in] type コアタイプ
      /// \param[in] num コア番号
      /// \param[in] rate レート
      /// \param[in] prefix 出力するプレフィクスの指定
      ///
      /// \return タスク名
      inline std::string
      mk_task_name (CoreType type, int num, const rate_T& rate,
		    const std::string* prefix = NULL)
      {
	return mk_base_name (type, num, rate, prefix) + "_Task";
      }

      /// \brief Tpooers/ATK2用タスク名生成
      ///
      /// \param[in] node CspNode のポインタ
      /// \param[in] prefix 出力するプレフィクスの指定
      ///
      /// \return タスク名
      inline std::string
      mk_task_name (const CspNode* node,
		    const std::string* prefix = NULL)
      {
	return mk_task_name (node->coretype (), node->core (), node->rate (),
			     prefix);
      }

      /// \brief Tpooers/ATK2用タスク名生成
      ///
      /// \param[in] taskset TaskSet のポインタ
      /// \param[in] prefix 出力するプレフィクスの指定
      ///
      /// \return タスク名
      inline std::string
      mk_task_name (const TaskSet* taskset,
		    const std::string* prefix = NULL)
      {
	return mk_task_name (taskset->cspnodelist ()[0], prefix);
      }

      /// \brief TOPPERS/ATK2用タスクのアラーム名生成
      ///
      /// \param[in] taskset TaskSet のポインタ
      ///
      /// \return アラーム名
      inline std::string
      mk_alarm_name (const TaskSet* taskset)
      {
	return mk_base_name (taskset->cspnodelist ()[0]) + "_Alarm";
      }

      // TOPPERS/ATK2用タスク関連情報のセットアップ
      int correct_task (void);

      // 共通関数の出力処理
      void generate_common_functions (std::ostream& os);

      // タスクループ内のタスクのコード生成
      void generate_task_code_in_loop (std::ostream& os,
				       const TaskSet* taskset,
				       GenCodeType gentype);

      // タスクの定義・宣言の出力
      int
      generate_definitions (std::ostream& os, int num,
			    const TaskSet* taskset,
			    bool outer,
			    const std::string& postfix,
			    def_list_T* define_list = NULL);

      // TOPPERS/ATK2コードの生成
      int generate_code (void);

      // TOPPERS/ATK2のABREX用YAMLファイルの生成
      int generate_yaml (void);

      // TOPPERS/ATK2のYAML用ヘッダファイルの生成
      int generate_app_header (void);

      // TOPPERS/ATK2コード生成のための準備用関数
      int prepare_generate_code (void);

      /// @}
    }
  }
}


#endif	// ATK2_GENERATOR_I_H
