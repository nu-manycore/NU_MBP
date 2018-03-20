#include "aos.h"

#include "prc_sil.h"

/*
 *  タイマのカウント周期
 */
#define TIMER_CLOCK	UINT_C(60000)  //1msec

/*
 *  ベースアドレス
 */
#define TIMER_BASE	0x0c000240

/*
 *  Avalon Timer ハードウェア定義
 */

/*
 *  レジスタ
 */
#define AVALON_TIM_STATUS		0x00U
#define AVALON_TIM_CONTROL		0x04U
#define AVALON_TIM_PERIODL		0x08U
#define AVALON_TIM_PERIODH		0x0CU
#define AVALON_TIM_SNAPL		0x10U
#define AVALON_TIM_SNAPH		0x14U

/*
 *  ビット定義
 */
#define AVALON_TIM_STATUS_RUN		UINT_C(0x02)
#define AVALON_TIM_STATUS_TO		UINT_C(0x01)

#define AVALON_TIM_CONTROL_STOP		UINT_C(0x08)
#define AVALON_TIM_CONTROL_START	UINT_C(0x04)
#define AVALON_TIM_CONTROL_CONT		UINT_C(0x02)
#define AVALON_TIM_CONTROL_ITO		UINT_C(0x01)

void
swcnt_timer_init (void)
{

  /* タイマストップ */
  sil_wrw_iop((void *) (TIMER_BASE + AVALON_TIM_CONTROL),
	      AVALON_TIM_CONTROL_STOP);
  /* タイムアウトステータスクリア */
  sil_wrw_iop((void *) (TIMER_BASE + AVALON_TIM_STATUS), 0x00U);

 /* カウンタセット 下位16bit */
  sil_wrw_iop((void *) (TIMER_BASE + AVALON_TIM_PERIODL),
	      ((TIMER_CLOCK - 1U) & 0xffffU));

  /* カウンタセット 上位16bit */
  sil_wrw_iop((void *) (TIMER_BASE + AVALON_TIM_PERIODH),
	      ((uint32) (TIMER_CLOCK - 1U) >> 16));

  /*
   * タイマスタート，オートリロード，割込み許可
   */
  sil_wrw_iop((void *) (TIMER_BASE + AVALON_TIM_CONTROL),
	      AVALON_TIM_CONTROL_START
	      | AVALON_TIM_CONTROL_CONT
	      | AVALON_TIM_CONTROL_ITO);
}

ISR (SwCntTimerHdr)
{
  StatusType  ercd;

  /* TOビットのクリア */
  sil_wrw_iop((void *) (TIMER_BASE + AVALON_TIM_STATUS), 0x00U);

  /*
   *  カウンタ加算通知処理実行
   */
  ercd = IncrementCounter(SwCnt0);

  /* エラーリターンの場合はシャットダウン */
  if (ercd != E_OK)
    ShutdownAllCores(ercd);
}
