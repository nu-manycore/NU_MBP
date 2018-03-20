#!/bin/sh

dir=`dirname $0`
. ${dir}/MyEnv.sh

: ${MEASUREMENT_TOOL_DIR:=${dir}}

$TOOL_PATH/v850-renesas-elf-gdb -x ${MEASUREMENT_TOOL_DIR}/sub/gdb-command.x
