#!/bin/sh

# usage: SetRate.sh -i inputXML [-o outputXML] [-v] [CSV]
#
# -i inputXML
#     input XML(BLXML)
# -o outputXML
#     output XML(BLXML), modified "rate" in <block>
# -v
#     verbose
# CSV
#     CSV File created by GetRate
#

name=`basename $0 .sh`
dir=`dirname $0`

# truss, strace, ...
: ${PRECMD:=}

# java, jdb
: ${JAVA:=java}

classpath="${dir}/build"

exec ${PRECMD} ${JAVA} -classpath "${classpath}" $name "$@"
