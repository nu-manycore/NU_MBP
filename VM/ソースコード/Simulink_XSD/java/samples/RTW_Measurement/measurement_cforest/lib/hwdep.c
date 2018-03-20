//************************************************************
// Hardware dependency code for Rapier16 and CForest
//
//************************************************************

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdarg.h>

#if 0
#include    <mcos/mcos.h>
#include    <mcos/mcos_ds.h>
#endif
#include    "hwdep.h"
#if 0
#include    "led.h"
#include    "ostm.h"
#endif

#if 0
//======================================================================
// Initial Display
//======================================================================
// display_execmode()
//      Initialize 7SEG-LED, and
//      Display execution mode on 7SEG-LED
//        P = Parallel mode
//        S = Sequential mode
//        0 = Illegal
//========================================
void display_execmode( void )
{
#if ((defined PAR)&&(SEQ))
    mcos_debug_printf ("# Sequential/Parallel execution mode.\n");
    #ifndef CFOREST
        mcg4p_7seg_set ( 7, 0, MODE_DEC );
    #endif
#else
    #ifdef  PAR
        mcos_debug_printf ("# Parallel execution mode.\n");
        #ifndef CFOREST
            mcg4p_7seg_ctl ( 7, 0xf3 );
        #endif
    #endif
    #ifdef  SEQ
        mcos_debug_printf ("# Sequential execution mode.\n");
        #ifndef CFOREST
            mcg4p_7seg_ctl ( 7, 0xed );
        #endif
    #endif
#endif 

    return;
}

//========================================
// wait_button()
//      It wait to push a button.
//========================================
void wait_button()
{
#ifdef CFOREST

#else
    while ( HWDEP_STAT_SW2 != 1 ){
        //mcos_debug_printf( "SW1 : %0x\n", HWDEP_STAT_SW );
        hwdep_snooze();
    }
#endif
  return;
}


//========================================
// Timer functions
//========================================

void hwdep_init_timer(void)
{
#ifdef USE_HW_TIMER
//    mcg4p_ostm_intmask( OSTM_EI1 );
    mcg4p_ostm_stop ( OSTM_EI1 );
    mcg4p_ostm_init ( OSTM_EI1, FREERUN_WO_INT );
    mcg4p_ostm_set_count ( OSTM_EI1, 0 );
    mcg4p_ostm_start( OSTM_EI1 );

#else   // USE_HW_TIMER
#endif  // USE_HW_TIMER
}

uint32_t hwdep_get_timer(void)
{
#ifdef USE_HW_TIMER

    uint32_t    t;
    mcg4p_ostm_get_count( OSTM_EI1, &t );

    return  (uint32_t)(t/HWDEP_CPU_FREQ_MHZ);

#else   // USE_HW_TIMER

#ifdef CFOREST
    return  (uint32_t)( (mcos_debug_time_get()/HWDEP_CPU_FREQ_MHZ) & 0x00000000ffffffffULL );
#else   // CFOREST
    return  (uint32_t)(mcos_debug_time_get() & 0x00000000ffffffffULL );
#endif  // CFOREST

#endif  // USE_HW_TIMER
}

void hwdep_wait (unsigned int n)
{
    int i = 0;

    for (i = 0; i < n; i++) {
	hwdep_snooze ();
    }
}
#endif

//========================================
// Cforest Trace cotnroll
//========================================

void hwdep_start_trace ( uint32_t pe )
{
#ifdef CFOREST
    *((uint8_t *)CFLOG_ADRS(pe)) = CFLOG_ON;
#endif
    return;
}

void hwdep_stop_trace ( uint32_t pe )
{
#ifdef CFOREST
    *((uint8_t *)CFLOG_ADRS(pe)) = CFLOG_OFF;
#endif
    return;
}

//========================================
// Printf
//========================================
void hwdep_printf(char *fmt, ...)
{
#ifdef CFOREST
    char buf[HWDEP_PBUF];
    va_list args;
    int len;
    int i;

    // formating
    va_start(args, fmt);
    len = vsnprintf(buf, HWDEP_PBUF, fmt, args);
    va_end(args);

    // put to UART-TX
    for (i=0; i<len; i++ ){
        *HWDEP_SIMIO_BUF = buf[i];
    }
#endif
    return;
}



