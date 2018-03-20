//************************************************************
// Hardware dependency code for Rapier16 and CForest
//
//************************************************************


#ifndef HWDEP_H
#define HWDEP_H

#include <stdint.h>

extern  void display_execmode( void );
extern  void wait_button ( void );

//========================================
//  CPU Functions
//========================================
extern void     hwdep_snooze ( void );      // hwdep_asm.s
extern int32_t  hwdep_get_peid ( void );    // hwdep_asm.s
extern uint32_t hwdep_get_pc ( void );      // hwdep_asm.s
extern uint32_t hwdep_get_fepc ( void );    // hwdep_asm.s
extern uint32_t hwdep_get_sp ( void );      // hwdep_asm.s
extern int32_t  hwdep_disable_feint ( void ); // hwdep_asm.s
extern int32_t  hwdep_enable_feint ( void );  // hwdep_asm.s

//========================================
//  Switch ATTENTION: these define macro is bugged now.
//========================================
#define     HWDEP_DEVSTAT_ADDR ((volatile uint32_t *)0x48001258UL)
#define     HWDEP_STAT_SW      (uint8_t)(*((volatile uint8_t *)HWDEP_DEVSTAT_ADDR+0x02UL)&0xfUL)
#define     HWDEP_STAT_SW1     (uint8_t)((*((volatile uint8_t *)HWDEP_DEVSTAT_ADDR+0x02UL)>>0)&0x1UL)
#define     HWDEP_STAT_SW2     (uint8_t)((*((volatile uint8_t *)HWDEP_DEVSTAT_ADDR+0x02UL)>>1)&0x1UL)
#define     HWDEP_STAT_SW3     (uint8_t)((*((volatile uint8_t *)HWDEP_DEVSTAT_ADDR+0x02UL)>>2)&0x1UL)
#define     HWDEP_STAT_SW4     (uint8_t)((*((volatile uint8_t *)HWDEP_DEVSTAT_ADDR+0x02UL)>>3)&0x1UL)
#define     HWDEP_STAT_DIPSW5  ((volatile uint16_t *)HWDEP_DEVSTAT_ADDR+0x00UL)

//========================================
//  Timer
//========================================

// Half count of hardware timer
#define TIMER_HALF      (0x80000000UL)

// for Cycle to usec convert
#define HWDEP_CPU_FREQ_MHZ  (24)
#define HWDEP_WRAP_TIME     ((uint32_t)(0xffffffffUL/HWDEP_CPU_FREQ_MHZ))

extern void     hwdep_init_timer (void);
extern uint32_t hwdep_get_timer (void);
extern void     hwdep_wait (unsigned int);

//========================================
//  for SYSERR
//========================================
#define SEG_BASE    (0xfffee980UL)
#define SEG_FLAGCONT    *((volatile uint32_t *)(SEG_BASE+0x0))
#define SEG_CONT        *((volatile uint16_t *)(SEG_BASE+0x0))
#define SEG_FLAG        *((volatile uint16_t *)(SEG_BASE+0x2))
#define SEG_SIDETYPE    *((volatile uint32_t *)(SEG_BASE+0x4))
#define SEG_TYPE        *((volatile uint16_t *)(SEG_BASE+0x4))
#define SEG_SIDE        *((volatile uint16_t *)(SEG_BASE+0x6))
#define SEG_ADDR        *((volatile uint32_t *)(SEG_BASE+0x8))

//========================================
//  MISC_REG
//========================================
#define MISC_REG_BASE   (0x48012000UL)
#define MISC_REG(x)     *((volatile uint32_t *)(MISC_REG_BASE+(x*0x4)))

//========================================
//  INT_REG
//========================================
#define INTC1_BASE      (0xfffeea00UL)
#define EIC(x)          *((volatile uint16_t *)(INTC1_BASE+(x*0x02)))
#define EIBD(x)         *((volatile uint32_t *)(INTC1_BASE+0x100+(x*0x04)))

//========================================
//  for CForest
//========================================

#ifdef  CFOREST
    #define HWDEP_PBUF          (256)
    #define HWDEP_SIMIO_BASE    (0xffff7f00UL)
    #define HWDEP_SIMIO_BUF     ((volatile char *)(HWDEP_SIMIO_BASE + 0xE0UL))
    #define HWDEP_SIMIO_CYC_LO  ((volatile uint32_t *)(HWDEP_SIMIO_BASE + 0xF0UL))
    #define HWDEP_SIMIO_CYC_UP  ((volatile uint32_t *)(HWDEP_SIMIO_BASE + 0xF4UL))
    #define HWDEP_SIMIO_CYC     ((volatile uint64_t *)(HWDEP_SIMIO_BASE + 0xF0UL))
    #define HWDEP_SIMIO_STOP    ((volatile uint16_t *)(HWDEP_SIMIO_BASE + 0xFEUL))

    #define HWDEP_CYC_BASE    (0xF3008018UL)
    #define HWDEP_CYC         ((volatile uint64_t *)(HWDEP_CYC_BASE+0x0UL))
    #define HWDEP_CYC_LO      ((volatile uint32_t *)(HWDEP_CYC_BASE+0x0UL))
    #define HWDEP_CYC_HI      ((volatile uint32_t *)(HWDEP_CYC_BASE+0x4UL))
#endif

extern void hwdep_printf(char *fmt, ...);
    
#ifdef  CFOREST
    #define     CFLOG_BASE      (0xFE000000UL)
    #define     CFLOG_PE_STEP   (0x0100UL)
    #define     CFLOG_NC_OFF    (0xFFUL)

    #define     CFLOG_ADRS( pe )    (CFLOG_BASE+(CFLOG_PE_STEP*pe)+CFLOG_NC_OFF)

    #define     CFLOG_ON      (0x1)
    #define     CFLOG_OFF     (0x0)
#endif

extern void hwdep_start_trace ( uint32_t pe );
extern void hwdep_stop_trace ( uint32_t pe );

#endif  // HWDEP_H
