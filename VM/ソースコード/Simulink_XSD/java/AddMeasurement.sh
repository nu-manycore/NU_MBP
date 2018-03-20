#!/bin/sh

# usage: AddMeasurement [-v] [-o output.c] [-m output.h] [-p prefix]
#         [-i include] [-b blocks.csv] -h header c_source
#
# -v (optional)
#     verbose
#
# -o code.c (optional, using stdout unless defined this option)
#     output .c file (add measurement code)
#
# -m macro.h (optional, using "macro.h" unless defined this option)
#     output .h file (included measurement information)
#
# -p prefix (optional, no prefix unless defined this option)
#     macro name prefix for output.h
#
# -i include (optional, multiple, no "#include" unless defined this option)
#     add "#include" directive
#
# -b blocks.csv (optional, no output unless defined this option)
#     output information of Blocks to CSV file
#
# -h header (required)
#     input RTW .h file
#
# c_source (required)
#     input RTW .c file
#

name=`basename $0 .sh`
dir=`dirname $0`

# truss, strace, ...
: ${PRECMD:=}

# java, jdb
: ${JAVA:=java}

classpath="${dir}/build"

exec ${PRECMD} ${JAVA} -classpath "${classpath}" $name "$@"
