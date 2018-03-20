#!/bin/sh

# usage: GetRate.sh [-o outputCSV] [-v] filename.rtw
#
# -o outputCSV
#     output CSV, includes block-name and rate, extracted from RTW file.
# -v
#     verbose
# filename.rtw
#     rtw file
#

name=`basename $0 .sh`
dir=`dirname $0`

cup_runtime_path=${dir}/lib/java-cup-11b-runtime.jar

pathsep=':'

# truss, strace, ...
: ${PRECMD:=}

# java, jdb
: ${JAVA:=java}

os=`uname -s`
case "${os}" in
MINGW* | CYGWIN*)
        pathsep=';'
        ;;
esac

classpath="${dir}/build${pathsep}${cup_runtime_path}"

exec ${PRECMD} ${JAVA} -classpath "${classpath}" $name "$@"
