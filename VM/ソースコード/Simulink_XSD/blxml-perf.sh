#!/bin/sh

# -v                  : (optional) verbose option
# -o output           : (optional) Output BLXML
# -shim=SHIM          : (optional) SHIM File included `CommonInstructionSet'
# -shim-schema=SCHEMA : (optional) SHIM Schema for validation
# -C clang-option     : (optional) add CLANG options as `-O', `-I', etc
# -O pass-option      : (optional) add LLVM pass options, see below
# -S pass-module      : (optional) use LLVM Pass Module
# -N                  : (optional) blxml2code not generate function params
# -H user-header      : (optional) insert header into generated file
# -n                  : (optional) not validation for blxml XML Scema

# -O -MasterComponent=name      : Name of SHIM MasterComponent
# -O -CommonInstructionSet=name : Name of SHIM CommonInstructionSet
# -O -no-ret                    : Not count LLVM ReturnInstruction
# -O -no-br                     : Not count LLVM BranchInstruction
# -O -out-dot                   : basename of output GraphViz DOT file
# -O <other `opt' options>      : Other `opt' options...

os=`uname -s`

so=so
case $os in
    MSYS* | MINGW* | CYGWIN*)
	so=dll
	;;
esac

programdir=`dirname $0`
perflibdir="${programdir}"

case "x${MBP_ROOT}" in
x)
	if test -d "${programdir}/../libexec"; then
	    perflibdir="${programdir}/../libexec"
	fi
	;;
*)
	programdir="${MBP_ROOT}/bin"
	perflibdir="${MBP_ROOT}/libexec"

	if ! test -d $programdir -o -d $perflibdir; then
	    echo "${programdir} or/and ${perflibdir} not found" 1>&2
	    exit 1
	fi
	;;
esac

: ${BLXML2CODE:=${programdir}/blxml2code}
: ${CLANG:=clang}
: ${LLVM_OPT:=opt}
: ${LLVM_BLXMLPERF:=${perflibdir}/llvm-BLXMLPerf.${so}}

for i in BLXML2CODE CLANG LLVM_OPT
do
    eval 'x=${'"${i}"'}'

    if ! test -x "${x}" && ! type "${x}" >/dev/null 2>&1; then
	echo "${x}"'(as ${'"${i}"'})' not found 1>&2
	exit 1
    fi
done

if ! test -r "${LLVM_BLXMLPERF}"; then
    echo "${LLVM_BLXMLPERF}" not found 1>&2
    exit 1
fi

infile=
outfile=

opt_opts=
clang_opts=
shim_file=-no
shim_schema=
verbose=
b2c_opts=

# check memory defects by valgrind
opt_valcmd=
b2c_valcmd=
if test -n "${VALGRIND}" && type "${VALGRIND}" >/dev/null 2>&1; then
    opt_valcmd="${VALGRIND} ${VALGRIND_OPTION} --log-file=blxml-perf-opt.vlog"
    b2c_valcmd="${VALGRIND} ${VALGRIND_OPTION} --log-file=blxml-perf-b2c.vlog"
fi

usage() {
    echo usage: $0 \
	"[-v]" \
	"[-o output]" \
	"[-shim=SHIM]" \
	"[-shim-schema=schema]" \
	"[-C clang-option]" \
	"[-S pass-module]" \
	"[-O opt-option]" \
	"[-N]" \
	"[-H user-header]" \
	"[-n]" \
	"blxml-input" 1>&2
    exit 1
}

while getopts 'vo:s:C:S:O:NH:n' c
do
    case ${c} in
	v)
	    verbose=y
	    ;;
	o)
	    outfile="${OPTARG}"
	    ;;
	s)
	    case "${OPTARG}" in
		him=*)
		    shim_file=`expr ${OPTARG} : 'him=\(.*\)'`
		    ;;
		him-schema=*)
		    shim_schema=`expr ${OPTARG} : 'him-schema=\(.*\)'`
		    ;;
		*)
		    usage
		    ;;
	    esac
	    ;;
	C)
	    clang_opts="${clang_opts}${clang_opts:+ }${OPTARG}"
	    ;;
	S)
	    LLVM_BLXMLPERF="${OPTARG}"
	    ;;
	O)
	    opt_opts="${opt_opts}${opt_opts:+ }${OPTARG}"
	    ;;
        N)
	    b2c_opts="${b2c_opts}${b2c_opts:+ }-N"
	    ;;
	H)
	    b2c_opts="${b2c_opts}${b2c_opts:+ }-H ${OPTARG}"
	    ;;
	n)
	    b2c_opts="${b2c_opts}${b2c_opts:+ }-n"
	    opt_opts="${opt_opts}${opt_opts:+ }-no-blxml-validation"
	    ;;
	*)
	    usage
	    ;;
    esac
done

shift `expr $OPTIND - 1`

case $# in
    0)
	usage
	;;
    *)
	infile="$1"
	;;
esac

tmp=`basename $1 .xml`.$$

out_c=${tmp}.c
out_i=${tmp}.i.ll
out_o=${tmp}.o.ll

trap "rm -f ${out_c} ${out_i} ${out_o}" 0 1 2 3 9 15

case ${verbose} in y) set -x ;; esac

${b2c_valcmd} ${BLXML2CODE} ${b2c_opts} -o ${out_c} $infile || exit 1

${CLANG} -emit-llvm -S ${clang_opts} -o ${out_i} ${out_c} || exit 1

${opt_valcmd} ${LLVM_OPT} \
    -load=${LLVM_BLXMLPERF} \
    -blxml-perf \
    ${opt_opts} \
    ${shim_file:+"-shim=${shim_file}"} \
    ${shim_schema:+"-shim-schema=${shim_schema}"} \
    -in-xml=${infile} ${outfile:+"-out-xml=${outfile}"} \
    -S -o ${out_o} ${out_i} || exit 1
