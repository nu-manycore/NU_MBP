#!/bin/sh

: ${prefix:=}

log=${prefix}log
log_c_gmres_Selector_1=${prefix}c_gmres_Selector_1
log_c_gmres_plant_Add_1=${prefix}c_gmres_plant_Add_1

c_gmres_Selector_1=../../Simulink_XSD/20150313/20150313_mpc2/log/c_gmres_Selector_1.csv
c_gmres_plant_Add_1=../../Simulink_XSD/20150313/20150313_mpc2/log/c_gmres_plant_Add_1.csv

grep c_gmres_Selector_1 $log | sed 's/.*://' > $log_c_gmres_Selector_1
grep c_gmres_plant_Add_1 $log | sed 's/.*://' > $log_c_gmres_plant_Add_1

echo c_gmres_Selector_1
perl ./d2f.pl -n 10 $c_gmres_Selector_1 $log_c_gmres_Selector_1
echo c_gmres_plant_Add_1
perl ./d2f.pl -n 10 $c_gmres_plant_Add_1 $log_c_gmres_plant_Add_1
