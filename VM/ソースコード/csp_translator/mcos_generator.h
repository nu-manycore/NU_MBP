#ifndef MCOS_GENERATOR_H
#define MCOS_GENERATOR_H

/// \file mcos_generator.h
///
/// \brief eMCOS用コード生成のCSPトランスレータ用インタフェース

#ifdef __GNUC__
#ifndef MCOS_WEAK
#define MCOS_WEAK	__attribute__ ((weak))
#endif	/* MCOS_WEAK */
#else	/* !__GNUC__ */
#error "unknown compiler"
#endif	/* !__GNUC__ */

namespace Simulink
{
  namespace CspTrans
  {

    // eMCOS用オプション
    extern int enable_csr MCOS_WEAK;	 // --enable-csr=[ny]

    // eMCOS用コード生成
    int mcos_generate_code (const char *mcos_out) MCOS_WEAK;
  }
}

#endif	/* MCOS_GENERATOR_H */
