#ifndef COUNTER_HANDLER_H
#define COUNTER_HANDLER_H

#define SwCnt0_TIMER_CLOCK_HZ	1000

void swcnt_timer_init(void);
ISR(SwCntTimerHdr);

#endif
