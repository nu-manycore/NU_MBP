#!/bin/sh

: ${d2f_opts:=}

: ${prefix:=}

dir=`dirname $0`

log=${prefix}log
log_out1=${prefix}out1
log_out3=${prefix}out3
log_out9=${prefix}out9

DATA_DIR=${HOME}/proj/CSP/nu/20150304/data_01

out1=${DATA_DIR}/simlink_out1.csv
out3=${DATA_DIR}/simlink_out3.csv
out9=${DATA_DIR}/simlink_out9.csv

opt_n='-n 20'

# check multirate
#rate_out1=500
#rate_out3=10
#rate_out9=500

case x$rate_out1 in x) opt_rate1= ;; *) opt_rate1="-r $rate_out1" ;; esac
case x$rate_out3 in x) opt_rate3= ;; *) opt_rate3="-r $rate_out3" ;; esac
case x$rate_out9 in x) opt_rate9= ;; *) opt_rate9="-r $rate_out9" ;; esac

grep synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1 $log | sed 's/.*://' > $log_out1
grep synrmmodel_mt_RTW_Subsystem2_C2A_1 $log | sed 's/.*://' > $log_out3
grep synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1 $log | sed 's/.*://' > $log_out9

echo synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1
perl ${dir}/d2f.pl $opt_rate1 $opt_n $d2f_opts $out1 $log_out1
echo synrmmodel_mt_RTW_Subsystem2_C2A_1
perl ${dir}/d2f.pl $opt_rate3 $opt_n $d2f_opts $out3 $log_out3
echo synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1
perl ${dir}/d2f.pl $opt_rate9 $opt_n $d2f_opts $out9 $log_out9
