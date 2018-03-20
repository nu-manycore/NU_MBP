#ifndef MCOS_DS_H
#define MCOS_DS_H

#ifdef NO_DEBUG_PRINTF
#define mcos_debug_printf(X,...)	do { ; } while (0)
#else
#include <stdio.h>
#include <stdarg.h>

#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__ (X)
#endif
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#include "pthread.h"
#endif

static void
__attribute__ ((unused))
mcos_debug_printf(const char *fmt, ...)
{
#if defined(__MINGW32__) || defined(__MINGW64__)
  static pthread_mutex_t mcos_debug_printf_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
  va_list ap;

  va_start (ap, fmt);

#if defined(__MINGW32__) || defined(__MINGW64__)
  (void)pthread_mutex_lock (&mcos_debug_printf_mutex);
#endif

  vprintf (fmt, ap);
  fflush (stdout);

#if defined(__MINGW32__) || defined(__MINGW64__)
  (void)pthread_mutex_unlock (&mcos_debug_printf_mutex);
#endif
  va_end (ap);
}

#endif
#endif
