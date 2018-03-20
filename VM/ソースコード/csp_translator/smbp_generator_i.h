// -*- mode: c++; coding: utf-8-unix; -*-

#ifndef SMBP_GENERATOR_I_H
#define SMBP_GENERATOR_I_H

/// \file smbp_generator_i.h
///
/// \brief SystemBuilder向けコード生成のローカルな宣言・定義

namespace Simulink
{
  namespace CspTrans
  {
    namespace SMBP
    {
      // 全 CspNode の MemoryChannel の構造体の出力
      int generate_memchan_types (std::ostream& os, bool tail_only);

      void generate_task_smbp_input_channel (std::ostream& os, CspNode* node,
					     int ind,
					     channel_io_mode input_mode);

      void generate_task_smbp_output_channel (std::ostream& os, CspNode* node,
					      int ind);

      int generate_task_definitions (std::ostream& os, const TaskSet* taskset);
    }
  }
}

#endif // SMBP_GENERATOR_I_H
