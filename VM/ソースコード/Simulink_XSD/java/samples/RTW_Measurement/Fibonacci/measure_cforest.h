/*
 * Sample measurement macros for CForest
 */

#ifndef CFOREST
#error "This header can used only for CForest"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "hwdep.h"

#define COUNT	10

static uint64_t volatile measure_cycles[MAX_BLOCK_ID + 1][2];
static uint8_t volatile measure_counted[MAX_BLOCK_ID];

#define MEASUREMENT_STEP_BEGIN()	\
  do {\
    measure_cycles[MAX_BLOCK_ID][1] = *HWDEP_SIMIO_CYC;\
    measure_cycles[MAX_BLOCK_ID][0] += *HWDEP_SIMIO_CYC - measure_cycles[MAX_BLOCK_ID][1];\
  } while(0)

#define MEASUREMENT_STEP_END()\
do {\
} while (0)

#define MEASUREMENT_START(ID,UPDATE,INDEX,PARAM)\
  do {\
    measure_counted[ID] = 1;\
    measure_cycles[ID][1] = *HWDEP_SIMIO_CYC;\
  } while(0)

#define MEASUREMENT_STOP(ID,UPDATE,INDEX,PARAM)\
  do {\
    measure_cycles[ID][0] += *HWDEP_SIMIO_CYC - measure_cycles[ID][1];\
  } while(0)


/* empty INT Hander */
void IntHandler(void) {}

int
main(void)
{
  int i;

  Fibonacci_initialize();

  for (i = 0; i < COUNT; i++) {
    Fibonacci_step();
  }

  hwdep_printf("---- BEGIN MEASUREMENT ----\n");
  for (i = 0; i < MAX_BLOCK_ID; i++) {
    if (measure_counted[i]) {
      hwdep_printf("%s,%lld,-1\n",
		   BLOCK_NAMES[i],
		   (measure_cycles[i][0]
		    - measure_cycles[MAX_BLOCK_ID][0]) / COUNT);
    } else {
      hwdep_printf("%s,,-1\n", BLOCK_NAMES[i]);
    }
  }
  hwdep_printf(",%lld,-1\n",
	       measure_cycles[MAX_BLOCK_ID][0] / COUNT);
  hwdep_printf("---- END MEASUREMENT ----\n");
  return 0;
}
