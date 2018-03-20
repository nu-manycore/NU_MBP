#ifndef MCOS_H
#define MCOS_H

/*
 * $(CC) -DUSE_PTHREAD ... -lpthread
 *
 * -DMAIN_WAIT_TIME=N (default 1H)
 *     main()を終了するまでの時間
 *     単位は秒で、デフォルトは3600秒(1時間)
 *
 * -DCHECK_USER_CYCLE
 *     カウンタ mcos__cycle_step が MCOS_THREAD_MAX_CYCLE を越えるまで
 *     実行を続ける。なお、1秒間隔のポーリングで判定するため、必ずしも
 *     MCOS_THREAD_MAX_CYCLEでは終わらない。
 *     mcos__cycle_stepは、利用者が適当な箇所でカウントアップする必要が
 *     ある。
 *
 * -DMCOS_THREAD_MAX_CYCLE=N
 *     CHECK_USE_CYCLEを有効にした際に使用される。デフォルトは10000
 *
 * -D_MCOS_VERSION=<eMCOSのAPI仕様のバージョン番号>
 *     eMCOSのAPI仕様を選択する。
 *     -D_MCOS_VERSION=0x00000800 は eMCOS version 0.8.0 対応。
 *     (旧来のcsp_translatorを使用する場合はこちら)
 *     本マクロを指定しない場合は -D_MCOS_VERSION=0x00010000 と同じで、
 *     eMCOS version 1.0.0のAPI仕様となる
 *
 */

#ifdef USE_PTHREAD
#include <pthread.h>
#if defined(__MINGW32__) || defined(__MINGW64__)
#define __struct_timespec_defined	1	/* conflict <time.h> ... */
#endif
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#if defined(__MINGW32__) || defined(__MINGW64__)
#define alloca	__builtin_alloca
#else
#include <alloca.h>
#endif
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__ (X)
#endif
#endif

#ifndef _MCOS_VERSION
#define _MCOS_VERSION	0x00010000
#endif

typedef int16_t mcos_erid_t;
typedef int16_t mcos_id_t;
typedef int16_t mcos_er_t;

#if _MCOS_VERSION == 0x00000800

/*
 * eMCOS v0.8.0
 */

#define MC_EOK		0
#define MC_EPAR		(-1)
#define MC_ENOMEM	(-2)

#define LCID_SELF	(0)
#define LCID_ANY	(-1)

#define TPRI_INHERIT	(0)

/* mcos_message_send */
#define MESSAGE_ASYNC	(0)
#define MESSAGE_SYNC	(1)

/* mcos_message_recieve */
#define MESSAGE_RECV_ASYNC		(0)
#define MESSAGE_RECV_SYNC		(1)
#define MESSAGE_RECV_FROM_THREAD	(2)
#define MESSAGE_RECV_FROM_ISR		(4)

/* msg_t */
#define MPRI_INHERIT	(0)

typedef struct { uint32_t attrs[2]; } mcos_threadattr_t;

typedef struct {
  uint8_t priority;
  uint32_t size;
  uint8_t *data;
} msg_t;

typedef uint32_t reltime_t;

/* cyclic_create_t */
typedef struct {
  void (*entry) (mcos_id_t, uintptr_t);
  reltime_t cycle;
  mcos_id_t lcid;
  size_t stacksize;
  uintptr_t arg;
} cyclic_create_t;

#define _MCOS__EOK		MC_EOK
#define _MCOS__THR_ATTR_T	mcos_threadattr_t
#define _MCOS__CYC_CRE_T	cyclic_create_t

#elif _MCOS_VERSION == 0x00010000
/*
 * eMCOS v1.0.0
 */

#define MCOS_EOK			0
#define MCOS_EPAR			(-1)
#define MCOS_ENOMEM			(-2)

#define MCOS_LCID_SELF			(0)
#define MCOS_LCID_ANY			(-1)
#define MCOS_LCID_INHERIT		(-2)

/* mcos_message_send */
#define MCOS_MSG_SEND_ASYNC		(0)
#define MCOS_MSG_SEND_SYNC		(1)

/* mcos_message_recieve */
#define MCOS_MSG_RECV_ASYNC		(0)
#define MCOS_MSG_RECV_SYNC		(1)

typedef struct { uint32_t attrs[2]; } mcos_thr_attr_t;
typedef uint32_t mcos_reltime_t;

/* mcos_cyc_cre_t */
typedef struct {
  void (*entry) (mcos_id_t, uintptr_t);
  mcos_reltime_t cycle;
  mcos_id_t lcid;
  size_t stacksize;
  uintptr_t arg;
} mcos_cyc_cre_t;

#define _MCOS__EOK		MCOS_EOK
#define _MCOS__THR_ATTR_T	mcos_thr_attr_t
#define _MCOS__CYC_CRE_T	mcos_cyc_cre_t

#else
#error "unknown _MCOS_VERSION"
#endif

static mcos_er_t
__attribute__ ((unused))
mcos_threadattr_init(_MCOS__THR_ATTR_T *attr)
{
  (void)attr;
  return _MCOS__EOK;
}

static mcos_er_t
__attribute__ ((unused))
mcos_threadattr_setlcid(_MCOS__THR_ATTR_T *attr, mcos_id_t lcid)
{
  (void)attr;
  (void)lcid;
  return _MCOS__EOK;
}

static mcos_er_t
__attribute__ ((unused))
mcos_threadattr_setpriority(_MCOS__THR_ATTR_T *attr, uint8_t pri)
{
  (void)attr;
  (void)pri;
  return _MCOS__EOK;
}

static mcos_er_t
__attribute__ ((unused))
mcos_threadattr_setstacksize(_MCOS__THR_ATTR_T *attr, uint32_t stacksize)
{
  (void)attr;
  (void)stacksize;
  return _MCOS__EOK;
}

#if _MCOS_VERSION == 0x00000800
static mcos_er_t
__attribute__ ((unused))
mcos_message_send(const mcos_id_t tid, uint8_t command, const msg_t* msg)
{
  (void)tid;
  (void)command;
  (void)msg;
  return MC_EOK;
}

static mcos_er_t
__attribute__ ((unused))
mcos_message_recieve(mcos_id_t* tid, uint8_t command, msg_t* msg)
{
  (void)tid;
  (void)command;
  (void)msg;
  return MC_EOK;
}
#else
static mcos_er_t
__attribute__ ((unused))
mcos_message_send(const mcos_id_t tid, const void *message, size_t size,
		  uint32_t flags)
{
  (void)tid;
  (void)message;
  (void)size;
  (void)flags;
  return MCOS_EOK;
}

static mcos_er_t
__attribute__ ((unused))
mcos_message_recieve(void *data, size_t *size, uint32_t flags)
{
  (void)size;
  (void)flags;
  return _MCOS__EOK;
}
#endif

static mcos_erid_t
__attribute__ ((unused))
mcos_cyclic_create(_MCOS__CYC_CRE_T* ccp)
{
  (void)ccp;
  return _MCOS__EOK;
}


#ifdef USE_PTHREAD

#define CH___STR(X)	#X

#ifndef MCOS_THREAD_MAX_CYCLE
#define MCOS_THREAD_MAX_CYCLE	10000
#endif

#ifndef MCOS_THREAD_COND_TIMEOUT
#define MCOS_THREAD_COND_TIMEOUT	60
#endif

static mcos_er_t
__attribute__ ((unused))
mcos__thread_create (void *(*func) (void *), void *arg,
		     int line, const char *name)
{
  pthread_t pth;
  pthread_attr_t attr;
  int r;
  static int thread_index = 0;

  pthread_attr_init (&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  r = pthread_create (&pth, NULL, func, arg);
  if (r != 0)
    {
      fprintf (stderr, "%d: %s: pthread_create r=%d: %s\n",
	       line, name, r, strerror (errno));
      exit (1);
    }
  return thread_index++;
}


#define mcos_thread_create(A,E,I)				\
  mcos__thread_create (CH__VEC_CONCAT(f__,E), (void *)I,	\
		       __LINE__, CH___STR(E))
#else  /* !USE_PTHREAD */
static mcos_erid_t
__attribute__ ((unused))
mcos_thread_create(const _MCOS__THR_ATTR_T *attr,
		   void (*entry)(uint32_t, uintptr_t), uintptr_t exinf)
{
  (void)attr;
  (void)entry;
  return _MCOS__EOK;
}
#endif	/* !USE_PTHREAD */

static mcos_erid_t
__attribute__ ((unused))
mcos_thread_delay(uint32_t m)
{
  (void)m;
#ifdef USE_PTHREAD
  sched_yield ();
#endif	/* USE_PTHREAD */
  return _MCOS__EOK;
}

static mcos_er_t
__attribute__ ((unused))
mcos_thread_start(mcos_id_t tid, uint32_t param)
{
  (void)tid;
  (void)param;
  return _MCOS__EOK;
}

#define CH__SYNCM()	__asm__ __volatile__ ("mfence" ::: "memory")

#if defined(USE_PTHREAD)

#ifndef PTHREAD_DEBUG
#define PTHREAD_DEBUG	0
#endif

static int ch__bitcheck (unsigned long *v, size_t b);
void create_init_threads (void);

static void
ch___event(pthread_mutex_t *mutex, pthread_cond_t *cond, int* flag,
	   int line, const char *ch_name, const char* task_name)
{
  (void)pthread_mutex_lock(mutex);
  (*flag)++;
  (void)pthread_cond_signal(cond);
  (void)pthread_mutex_unlock(mutex);
  if (PTHREAD_DEBUG) {
    printf("%d: %s: send event from %s\n", line, ch_name, task_name);
    fflush (stdout);
  }
}

#define CH__EVENT(T,X)				\
  ch___event(&CH__VEC_CONCAT(mutex__,X),	\
	     &CH__VEC_CONCAT(cond__,X),		\
	     &CH__VEC_CONCAT(flag__,X),		\
	     __LINE__, CH___STR(X), CH___STR(T))

static void
ch___task_sched(pthread_mutex_t *mutex, pthread_cond_t *cond, int* flag,
		unsigned long* v, size_t size,
		int line, const char* ch_name)
{
  char *bitbuf = alloca ((size + 1 + 15) / 16);
  char *s;
  size_t i;
  s = bitbuf + size;
  *s-- = 0;
  if (v == NULL)
    *s-- = '0';
  else
    {
      for (i = 0; i < size; i++)
	*s-- = ch__bitcheck(v, i) ? '1' : '0';
    }
  if (PTHREAD_DEBUG) {
    printf("%d: %s: wait event(%s)\n", line, ch_name, bitbuf);
    fflush(stdout);
  }
  (void)pthread_mutex_lock(mutex);
  while (*flag == 0) {
    int er;
#if MCOS_THREAD_COND_TIMEOUT == 0
    er = pthread_cond_wait(cond, mutex);
#else
    struct timespec val;
    val.tv_sec = time(NULL) + 60;
    val.tv_nsec = 0;
    er = pthread_cond_timedwait(cond, mutex, &val);
#endif
    if (er)
      {
#if defined(__MINGW32__) || defined(__MINGW64__)
	fprintf(stderr, "%d: %s: %s\n", line, ch_name, strerror (errno));
#else
	char errbuf[256];
	strerror_r (errno, errbuf, sizeof errbuf);
	fprintf(stderr, "%d: %s: %s\n", line, ch_name, errbuf);
#endif
	fflush (stderr);
	if (errno == 0)
	  continue;
	pthread_exit (NULL);
      }
    else
      {
	if (PTHREAD_DEBUG) {
	  printf("%d: %s: get event\n",line, ch_name);
	  fflush(stdout);
	}
	break;
      }
  }
  *flag = 0;
  pthread_mutex_unlock (mutex);
}

#define CH__TASK_SCHED(C,V,S)				\
  ch___task_sched(&CH__VEC_CONCAT(mutex__,C),		\
		  &CH__VEC_CONCAT(cond__,C),		\
		  &CH__VEC_CONCAT(flag__,C),		\
		  (V), (S), __LINE__, CH___STR(C))

#define CH__CORE_THREAD_SCHED(C,V,S)	CH__TASK_SCHED(C,V,S)

#define CH__END_LOG(C)					\
  do {							\
    if (PTHREAD_DEBUG) {				\
      printf ("%d: %s: end\n", __LINE__, CH___STR(C));	\
      fflush (stdout);					\
    }							\
  } while (0)

#endif	/* USE_PTHREAD */

#ifndef MAIN_WAIT_TIME
#define MAIN_WAIT_TIME	3600	/* 1h */
#endif

#ifndef mcos__cycle_step
unsigned int mcos__cycle_step = 0;
#endif

#ifdef CHECK_USER_CYCLE
int main (void)
{
  unsigned int laps = 0;
  unsigned int last;

  create_init_threads ();

  last = mcos__cycle_step;
  while (mcos__cycle_step < MCOS_THREAD_MAX_CYCLE) {
    if (laps++ >= 600) {
      printf ("not change 10min, mcos__cycle_step=%u\n", mcos__cycle_step);
      break;
    }
    if (last != mcos__cycle_step) {
      last = mcos__cycle_step;
      laps = 0;
    }
    sleep (1);
    laps++;
    __asm__ __volatile__ ("":::"memory");
  }
  return 0;
}
#else  /* !CHECK_USER_CYCLE */
int main (void)
{
  create_init_threads ();
  sleep (MAIN_WAIT_TIME);
  return 0;
}
#endif	/* !CHECK_USER_CYCLE */

#endif	/* MCOS_H */
