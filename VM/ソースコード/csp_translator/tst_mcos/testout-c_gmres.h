#include "testout-c_gmres_res.c"

#define TASK_EXECUTE_2101_COMPLETED()					\
  mcos_debug_printf ("[%u]c_gmres_Selector_1:%.7f,%.7f\n",			\
		     mcos__cycle_step, c_gmres_Selector_1[0], c_gmres_Selector_1[1])

#define TASK_EXECUTE_2118_COMPLETED()					\
  do {									\
    mcos_debug_printf ("[%u]c_gmres_plant_Add_1:%.7f,%.7f\n",		\
		       mcos__cycle_step, c_gmres_plant_Add_1[0],c_gmres_plant_Add_1[1]);	\
    mcos__cycle_step++;							\
  } while (0)
