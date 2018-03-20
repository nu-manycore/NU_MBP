#ifndef CSP_TRANSLATOR_H
#define CSP_TRANSLATOR_H

/// \file csp_translator.h
///
/// \brief CSPトランスレータのヘッダーファイル

#include <string>
#include <vector>

#ifndef WEAK
/// \brief コンパイラに対してweakシンボルを定義するためのマクロ
#define WEAK __attribute__ ((weak))
#endif

//#define USE_DMALLOC
//#define USE_IDENT

#ifdef USE_DMALLOC
#include "dmalloc.h"
#endif

/// \defgroup CSP_TRANSLATOR CSPトランスレータ
///
/// \brief CSPトランスレータ
///
/// @{

/// \defgroup COMMON_MACRO CSPトランスレータ共通マクロ
///
/// \brief CSPトランスレータの共通マクロ
///
/// @{

/// \brief 最大コア数
#define MAX_CORE_NUM	256

/// \brief デフォルトのコア数
#define DEFAULT_CORE_NUM	8

/// @}

/// @}

// 以下は定義側でdoxygenに含める

namespace Simulink
{
  /// \addtogroup CSP_TRANSLATOR
  ///
  /// @{

  namespace CspTrans
  {
    extern std::string block_name_prefix;
    extern long max_task_id;

    /* options */
    extern int verbose;		/* -v */
    extern int core_num;	/* -N */
    extern int code_target;	/* -T */

    /* CSP */
    extern int low_rate_pulsegenerator; /* --low-rate-pulsegenerator=[yn] */
    extern int ignore_to_sink;		/* --ignore-to-sink=[yn] */
    extern int ignore_isolated_block;	/* --ignore-isolated=[yn] */
    extern int ignore_leaf_block;	/* --ignore-leaves=[ny] */
    extern int label_only_task_id;	/* --label-only-task-id */
    extern int enable_multirate;	/* --enable-multirate */

    /* code generation */
    extern int do_create_t0;	/* --create-t0=[ny] */

    extern int do_staticalize;	/* --staticalize */
    extern int no_gcc;		/* --no-gcc */

    /* for debug */
    extern int gen_c_test_code;	/* -y */
  }

  /// @}
}

#include "c_codewriter.h"
#include "xml_csp.h"
#include "arch_memchan.h"
#include "mcos_generator.h"
#include "atk2_generator.h"
#include "smbp_generator.h"
#include "support_yaml.h"

#endif	/* CSP_TRANSLATOR_H */
