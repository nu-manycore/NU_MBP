#!/bin/sh
dir=`dirname $0`
. ${dir}/MyEnv.sh

LD_LIBRARY_PATH=$CFOREST_PATH
export LD_LIBRARY_PATH

: ${MEASUREMENT_TOOL_DIR:=${dir}}

# Run with GDB (Python)
$CFOREST_PATH/cforest_mp --scr ${MEASUREMENT_TOOL_DIR}/sub/gdb.py --msglvl ERR --halt-break off --cycle --use-stopio
