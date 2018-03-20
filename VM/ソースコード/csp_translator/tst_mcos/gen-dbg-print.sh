#!/bin/sh

: ${mcos_dump_dbg_print:=mcos_dump_dbg_print}

function usage {
	echo "$0 -t target -f func_prefix xml" >&2
	exit 1
}

: ${target:=}
: ${func_prefix:=}
: ${xml:=}

ids=

while getopts 't:f:b:' c
do
	case $c in
	t)
		target=$OPTARG
		;;
	f)
		func_prefix=$OPTARG
		;;
	b)
		ids="$ids -b${OPTARG}"
	esac
done

shift `expr $OPTIND - 1`

case $# in
0)
	;;
*)
	xml=$1
	;;
esac

case "x${target}" in x) usage;; esac
case "x${func_prefix}" in x) usage;; esac
case "x${xml}" in x) usage;; esac

(
cat << EOF
#ifndef SEQUENCIAL_PROGRAM
#include "testout-${target}_res.c"
#define INC_MCOS__CYCLE_STEP()	do { mcos__cycle_step++; } while (0)
#else /* SEQUENCIAL_PROGRAM */
#define INC_MCOS__CYCLE_STEP()	do { ; } while (0)
#endif /* SEQUENCIAL_PROGRAM */
EOF

${mcos_dump_dbg_print} $ids ${xml}
) > testout-${target}.h

(
cat << EOF
#ifdef SEQUENCIAL_PROGRAM
#include <mcos/mcos_ds.h>
#ifndef MCOS_THREAD_MAX_CYCLE
#define MCOS_THREAD_MAX_CYCLE	10000
#endif /* MCOS_THREAD_MAX_CYCLE */
void ${func_prefix}_step(void);
void ${func_prefix}_initialize(void);
void ${func_prefix}_terminate(void);
static unsigned int mcos__cycle_step;
int main (void)
{
  ${func_prefix}_initialize();
  for (mcos__cycle_step = 0; mcos__cycle_step < MCOS_THREAD_MAX_CYCLE; mcos__cycle_step++) {
    ${func_prefix}_step();
EOF

${mcos_dump_dbg_print} -c4 ${xml}

cat << EOF
  }
  ${func_prefix}_terminate();
  return 0;
}
#endif /* SEQUENCIAL_PROGRAM */
EOF
) > testout-${target}_seqpost.h
