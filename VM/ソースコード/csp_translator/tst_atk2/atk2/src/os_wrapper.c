#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "Os.h"

/* Start time */
struct timespec OS_WRAPPER(_start_time);

StatusType
OS_WRAPPER(SetEvent)(TASK__DECL, TaskType tid, EventMaskType bits)
{
  OS_WRAPPER(_TASK_TYPE) *task = &OS_WRAPPER(_task_resource)[tid];

  (void)TASK__CTX;		/* unused */

  pthread_mutex_lock (&task->mutex);
  task->event |= (task->mask & bits);
  pthread_cond_signal (&task->cond);
  pthread_mutex_unlock (&task->mutex);

  return E_OK;
}

StatusType
OS_WRAPPER(WaitEvent)(TASK__DECL, EventMaskType bits)
{
  OS_WRAPPER(_TASK_TYPE) *task = (OS_WRAPPER(_TASK_TYPE) *)TASK__CTX;

  assert (task->sid == OS_WRAPPER(_STRUCTID_TASK));

  pthread_mutex_lock (&task->mutex);
  bits &= task->mask;
  while ((task->event & bits) != bits)
    pthread_cond_wait (&task->cond, &task->mutex);
  pthread_mutex_unlock (&task->mutex);

  return E_OK;
}

StatusType
OS_WRAPPER(ClearEvent)(TASK__DECL, EventMaskType bits)
{
  OS_WRAPPER(_TASK_TYPE) *task = (OS_WRAPPER(_TASK_TYPE) *)TASK__CTX;

  assert (task->sid == OS_WRAPPER(_STRUCTID_TASK));

  pthread_mutex_lock (&task->mutex);
  task->event &= ~(task->mask & bits);
  pthread_mutex_unlock (&task->mutex);

  return E_OK;
}

StatusType
OS_WRAPPER(SetRelAlarm)(TASK__DECL,
			AlarmType aid, TickType inc, TickType cycle)
{
  OS_WRAPPER(_ALARM_TYPE) *alarm = &OS_WRAPPER(_alarm_list)[aid];
  OS_WRAPPER(_COUNTER_TYPE) *counter = alarm->counter;

  size_t clock_hz = counter->timer_clock;
  size_t nsec_mul = counter->nsec_mul;
  size_t nsec_div = counter->nsec_div;

  (void)TASK__CTX;		/* unused */

  time_t sec = inc / clock_hz;
  int64_t nsec = inc % clock_hz;
  if (nsec_mul)
    nsec *= nsec_mul;
  if (nsec_div)
    nsec /= nsec_div;

  alarm->timer_value.it_value.tv_sec = sec;
  alarm->timer_value.it_value.tv_nsec = (long)nsec;

  sec = cycle / clock_hz;
  nsec = cycle % clock_hz;
  if (nsec_mul)
    nsec *= nsec_mul;
  if (nsec_div)
    nsec /= nsec_div;

  alarm->timer_value.it_interval.tv_sec = sec;
  alarm->timer_value.it_interval.tv_nsec = (long)nsec;

  timer_settime (alarm->timer_id, 0, &alarm->timer_value, NULL);

  return E_OK;
}

StatusType
OS_WRAPPER(GetCounterValue)(TASK__DECL, CounterType cid, TickRefType value)
{
  struct timespec current;
  struct timespec *start = &OS_WRAPPER(_start_time);
  OS_WRAPPER(_COUNTER_TYPE) *counter = &OS_WRAPPER(_counter_list)[cid];

  uint64_t elapsed;

  size_t nsec_div = counter->nsec_div;
  size_t nsec_mul = counter->nsec_mul;

  (void)TASK__CTX;		/* unused */

  clock_gettime (CLOCK_REALTIME, &current);
  current.tv_sec -= start->tv_sec;
  if (current.tv_nsec < start->tv_nsec)
    {
      current.tv_sec--;
      current.tv_nsec += 1000000000 - start->tv_nsec;
    }
  else
    current.tv_nsec -= start->tv_nsec;

  elapsed = (uint64_t)current.tv_sec * 1000000000 + current.tv_nsec;

  if (nsec_div)
    elapsed *= nsec_div;
  if (nsec_mul)
    elapsed /= nsec_mul;
  elapsed %= (counter->max_load + 1);
  *value = (TickType)elapsed;

  return E_OK;
}

CoreIdType
OS_WRAPPER(GetCoreID)(TASK__DECL)
{
  if (TASK__CTX == NULL)
    return 0;

  if (TASK__CTX->sid == OS_WRAPPER(_STRUCTID_TASK))
    {
      OS_WRAPPER(_TASK_TYPE) *task = (OS_WRAPPER(_TASK_TYPE) *)TASK__CTX;
      return (CoreIdType)task->coreid;
    }
  else
    {
      assert (TASK__CTX->sid == OS_WRAPPER(_STRUCTID_CORE));
      OS_WRAPPER(_CORE_TASK_TYPE)
	*core = (OS_WRAPPER(_CORE_TASK_TYPE) *)TASK__CTX;
      return (CoreIdType)(core - OS_WRAPPER(_core_assign));
    }
}


void
OS_WRAPPER(StartCore)(TASK__DECL, CoreIdType coreid, StatusType* er)
{
  OS_WRAPPER(_CORE_TASK_TYPE) *core = &OS_WRAPPER(_core_assign)[coreid];

  (void)TASK__CTX;		/* unused */

  core->enable = 1;

  if (core->stup && OS_WRAPPER(StartupHook))
    OS_WRAPPER(StartupHook) ((OS_WRAPPER(_ANY_TASK_TYPE) *)core);

  OS_WRAPPER(main)((OS_WRAPPER(_ANY_TASK_TYPE) *)core);

  if (er)
    *er = 0;
}


static void*
thread_wrapper (void* arg)
{
  OS_WRAPPER(_TASK_TYPE)* task = (OS_WRAPPER(_TASK_TYPE)*)arg;
  task->running = 1;
  task->func ((OS_WRAPPER(_ANY_TASK_TYPE) *)task);
  task->running = 0;
  return NULL;
}


void
OS_WRAPPER(StartOS)(TASK__DECL, AppModeType apmode)
{
  size_t t;
  OS_WRAPPER(_TASK_TYPE) **tl;
  OS_WRAPPER(_CORE_TASK_TYPE) *core;

  assert (TASK__CTX);

  if (TASK__CTX->sid == OS_WRAPPER(_STRUCTID_TASK))
    {
      OS_WRAPPER(_TASK_TYPE) *task = (OS_WRAPPER(_TASK_TYPE) *)TASK__CTX;

      OS_WRAPPER(_CORE_TASK_TYPE)
	*core = &OS_WRAPPER(_core_assign)[task->coreid];

      OS_WRAPPER(StartOS) ((OS_WRAPPER(_ANY_TASK_TYPE) *)core, apmode);
      return;
    }

  assert (TASK__CTX->sid == OS_WRAPPER(_STRUCTID_CORE));

  core = (OS_WRAPPER(_CORE_TASK_TYPE) *)TASK__CTX;

  if (core->enable == 0)
    return;

  if (core->nosapp)
    {
      OS_WRAPPER(_OSAPP_TYPE) *oa = core->osapplist;
      size_t n = core->nosapp;
      size_t i;

      for (i = 0; i < n; i++)
	{
	  const OS_WRAPPER(_APPMODE_TYPE) *a = oa->appmode;
	  if (oa->startup && a->startuphook)
	    (*a->startuphook) ((OS_WRAPPER(_ANY_TASK_TYPE) *)core);
	  oa++;
	}
    }

  tl = core->tasklist;
  for (t = 0; t < core->ntask; t++)
    {
      size_t i;
      OS_WRAPPER(_TASK_TYPE)* task = *tl++;

      if (task->running)
	continue;

      for (i = 0; i < task->napp; i++)
	{
	  if (task->appmode[i] == apmode)
	    {
	      pthread_attr_t attr;
	      struct sched_param param;

	      pthread_attr_init (&attr);
	      memset (&param, 0, sizeof param);
	      param.sched_priority = task->pri;
	      pthread_attr_setschedparam(&attr, &param);
	      if (pthread_create (&task->thread, &attr, thread_wrapper, task))
		{
		  fprintf (stderr, "pthread_create: %s\n", strerror (errno));
		  exit (1);
		}
	      break;
	    }
	}
    }
}

/* syslog */
void
OS_WRAPPER(syslog_initialize)(TASK__DECL)
{
  (void)TASK__CTX;
  /* none */
}

StatusType
OS_WRAPPER(syslog_msk_log)(TASK__DECL, unsigned int mask)
{
  (void)TASK__CTX;
  (void)mask;
  /* none */
  return E_OK;
}

static pthread_mutex_t syslog_mutex = PTHREAD_MUTEX_INITIALIZER;

void
OS_WRAPPER(syslog)(TASK__DECL, uint32 pri, const char8* fmt, ...)
{
  va_list va;
  (void)TASK__CTX;			/* unused */

  va_start (va, fmt);
  pthread_mutex_lock (&syslog_mutex);
  vfprintf (stdout, fmt, va);
  putchar ('\n');
  fflush (stdout);
  pthread_mutex_unlock(&syslog_mutex);
  va_end (va);
}

void
OS_WRAPPER(InitSerial)(TASK__DECL)
{
  /* none */
  (void)TASK__CTX;			/* unused */
}


void
OS_WRAPPER(print_banner)(TASK__DECL)
{
  /* none */
  (void)TASK__CTX;			/* unused */
}

/* constructors */
static void /* __attribute__((constructor)) */
OS_WRAPPER(_init)(void)
{
  int i;
  for (i = 0; i < TNUM_TASK; i++) {
    if (pthread_mutex_init(&OS_WRAPPER(_task_resource)[i].mutex, NULL))
      fprintf (stderr, "pthread_mutex_init: %s\n", strerror (errno));
    if (pthread_cond_init(&OS_WRAPPER(_task_resource)[i].cond, NULL))
      fprintf (stderr, "pthread_cond_init: %s\n", strerror (errno));
  }
  for (i = 0; i < TNUM_ALARM; i++) {
    if (timer_create(CLOCK_REALTIME, &OS_WRAPPER(_alarm_list)[i].timer_event,
                     &OS_WRAPPER(_alarm_list)[i].timer_id))
      fprintf (stderr, "timer_create: %s\n", strerror (errno));
  }
  clock_gettime (CLOCK_REALTIME, &OS_WRAPPER(_start_time));
}

/* destructors */
static void /* __attribute__((destructor)) */
OS_WRAPPER(_fini)(void)
{
  int i;
  for (i = 0; i < TNUM_TASK; i++) {
    pthread_mutex_destroy(&OS_WRAPPER(_task_resource)[i].mutex);
    pthread_cond_destroy(&OS_WRAPPER(_task_resource)[i].cond);
  }
  for (i = 0; i < TNUM_ALARM; i++) {
    timer_delete(OS_WRAPPER(_alarm_list)[i].timer_id);
  }
}

int __attribute__ ((weak))
OS_WRAPPER(_check_task_ticks_done) (void);

void __attribute__ ((weak))
OS_WRAPPER(_dump_task_ticks) (TASK__DECL);

void
OS_WRAPPER(_check_and_dump)(TASK__DECL)
{
  static int checkbit = 0;
  if ((checkbit & 1) == 0
      && OS_WRAPPER(_check_task_ticks_done)
      && OS_WRAPPER(_dump_task_ticks)
      && OS_WRAPPER(_check_task_ticks_done)() != 0) {
    checkbit |= 1;
    OS_WRAPPER(_dump_task_ticks) (TASK__CTX);
  }
}

void
OS_WRAPPER(_dump_task_info) (TASK__DECL)
{
  if (OS_WRAPPER(_dump_task_ticks))
    OS_WRAPPER(_dump_task_ticks) (TASK__CTX);
}

#undef main
int
main(int argc, char **argv)
{
  int c;
  struct timespec slpt;
  unsigned long s = 3600;
  char *p;
  OS_WRAPPER(_CORE_TASK_TYPE)
    *master = &OS_WRAPPER(_core_assign)[OS_CORE_ID_MASTER];
  OS_WRAPPER(_ANY_TASK_TYPE)
    *TASK__CTX = (OS_WRAPPER(_ANY_TASK_TYPE) *)master;
  while ((c = getopt (argc, argv, "s:")) != -1) {
    switch (c) {
    case 's':
      s = (unsigned int)strtoul(optarg, &p, 0);
      break;
    default:
      fprintf (stderr, "usage: %s [-s sleep]\n", argv[0]);
      return -1;
    }
  }
  OS_WRAPPER(_init)();
  OS_WRAPPER(StartCore)(TASK__CTX, 0, NULL);
  if (s == 0) {
    while (1) {
      sleep (1);
      OS_WRAPPER(_check_and_dump) (TASK__CTX);
    }
    return 0;
  }
  slpt.tv_sec = s;
  slpt.tv_nsec = 0;
  nanosleep (&slpt, NULL);
  for (c = 0; c < TNUM_TASK; c++)
    pthread_cancel (OS_WRAPPER(_task_resource)[c].thread);
  for (c = 0; c < TNUM_TASK; c++) {
    void *ptr;
    if (OS_WRAPPER(_task_resource)[c].running
	&& pthread_join (OS_WRAPPER(_task_resource)[c].thread, &ptr) != 0)
      fprintf (stderr, "pthread_join: %s\n", strerror (errno));
  }
  OS_WRAPPER(_dump_task_info) (TASK__CTX);
  OS_WRAPPER(_fini)();
  return 0;
}
