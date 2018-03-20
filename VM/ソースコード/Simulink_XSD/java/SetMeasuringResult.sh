#!/bin/sh

# usage: SetMeasuringResult.sh -i inputXML [-o outputXML] [-v] [CSV]
#
# -i inputXML
#     input XML(BLXML)
# -o outputXML
#     output XML(BLXML), modified "measuringresult" in <block>
# -v
#     verbose
# CSV
#     measuring result file
#

name=`basename $0 .sh`
dir=`dirname $0`

# truss, strace, ...
: ${PRECMD:=}

# java, jdb
: ${JAVA:=java}

classpath="${dir}/build"

exec ${PRECMD} ${JAVA} -classpath "${classpath}" $name "$@"
