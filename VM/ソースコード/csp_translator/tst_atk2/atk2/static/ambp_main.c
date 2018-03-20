/*
 * TOPPERS/ATK2 main function
 */

#include "aos.h"
#include "counter_handler.h"

sint32
main (void)
{
  StatusType ercd;
  CoreIdType i;
  CoreIdType coreid = GetCoreID ();

  if (coreid == OS_CORE_ID_MASTER) {
    for (i = 0; i < TNUM_HWCORE; i++) {
      if (i != OS_CORE_ID_MASTER)
	StartCore (i, &ercd);
    }
    StartOS (THIS__APPMODE_0);
  } else {
    StartOS (THIS__APPMODE_0);
  }
  return 0;
}

/*
 * Startup Hook
 */
void StartupHook(void)
{
  CoreIdType coreid = GetCoreID ();

  if (coreid == OS_CORE_ID_MASTER) {
    syslog_initialize ();
    syslog_msk_log (LOG_UPTO(LOG_INFO));
    InitSerial ();
    print_banner ();
    swcnt_timer_init ();
  } else {
    InitSerial ();
  }

  syslog(LOG_EMERG, "StartupHook @ core%d", coreid);
}
