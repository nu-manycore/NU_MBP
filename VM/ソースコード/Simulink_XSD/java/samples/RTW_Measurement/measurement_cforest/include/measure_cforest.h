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
#include <inttypes.h>

#ifndef COUNT
#define COUNT	10
#endif

static uint64_t volatile measure_cycles[MAX_BLOCK_ID + 1][2];
static uint8_t volatile measure_counted[MAX_BLOCK_ID];

static void __attribute__((noinline,noclone))
measurement_start(volatile uint64_t *save)
{
  *save = *HWDEP_SIMIO_CYC;
}

static void __attribute__((noinline,noclone))
measurement_stop(int id, void (*after)(int id, uint64_t cycle))
{
  after(id, *HWDEP_SIMIO_CYC);
}

static void __attribute__((noinline,noclone))
measurement_after_step(int id, uint64_t cycle)
{
  measure_cycles[id][0] = cycle - measure_cycles[id][1];
}

static void __attribute__((noinline,noclone))
measurement_after_stop(int id, uint64_t cycle)
{
  measure_cycles[id][0] += (cycle - measure_cycles[id][1]
			    - measure_cycles[MAX_BLOCK_ID][0]);
}

#define MEASUREMENT_STEP_BEGIN()				\
  do {								\
    measurement_start(&measure_cycles[MAX_BLOCK_ID][1]);	\
    measurement_stop(MAX_BLOCK_ID,measurement_after_step);	\
  } while(0)

#define MEASUREMENT_STEP_END()			\
  do {						\
  } while (0)

#define MEASUREMENT_START(ID,UPDATE,INDEX,PARAM)	\
  do {							\
    measure_counted[ID] = 1;				\
    measurement_start(&measure_cycles[ID][1]);		\
  } while(0)

#define MEASUREMENT_STOP(ID,UPDATE,INDEX,PARAM)		\
  do {							\
    measurement_stop(ID, measurement_after_stop);	\
  } while(0)

#ifndef MEASUREMENT_IF_REAL_CONDITION
#define MEASUREMENT_IF_CONDITION(X)	1	/* always true */
#define MEASUREMENT_TID_CONDITION(X)	1	/* always true */
#else
#define MEASUREMENT_IF_CONDITION(X)	X	/* real condition */
#define MEASUREMENT_TID_CONDITION(X)	X	/* real condition */
#endif

/* empty INT Hander */
void IntHandler(void) {}

int
main(void)
{
  int i;

#ifdef MEASUREMENT_INITIALIZE_FUNCTION
  MEASUREMENT_INITIALIZE_FUNCTION();
#endif

  for (i = 0; i < COUNT; i++)
    {
      MEASUREMENT_STEP_FUNCTION();
    }

  hwdep_printf("---- BEGIN MEASUREMENT ----\n");
  for (i = 0; i < MAX_BLOCK_ID; i++)
    {
      if (measure_counted[i])
	{
	  uint64_t cycles = measure_cycles[i][0];
	  if ((int64_t)cycles < 0)
	    {
	      hwdep_printf("%s,%" PRIi64 ",-1,\"ERROR\"\n",
			   BLOCK_NAMES[i], (int64_t)cycles / COUNT);
	    }
	  else
	    {
	      hwdep_printf("%s,%" PRIu64 ",-1\n",
			   BLOCK_NAMES[i], cycles / COUNT);
	    }
	}
      else
	hwdep_printf("%s,,-1\n", BLOCK_NAMES[i]); /* no measurement result */
    }

  /* last overhead */
  hwdep_printf(",%" PRIu64 ",-1\n",
	       measure_cycles[MAX_BLOCK_ID][0]);

  hwdep_printf("---- END MEASUREMENT ----\n");

  return 0;
}
