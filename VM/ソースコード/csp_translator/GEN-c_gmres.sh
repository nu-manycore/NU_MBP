#!/bin/sh

exec > testout-c_gmres.log
exec 2>&1

no_gcc=--no-gcc
#no_gcc=

./csp_translator ${no_gcc} \
    --verbose \
    --target=mcos \
    -i testout-c_gmres.h \
    -s pe_test/c_gmres.xml \
    --output-code testout-c_gmres.c \
    --output-csp testout-c_gmres.dot \
    --output-var testout-c_gmres.csv \
    --label-only-task-id \
    --create-t0=n \
    --ignore-to-sink=y \
    --ignore-isolated=y \
    --ignore-leaves=n \
    --input-multirate=n \
    --incorporate-io=y \
    --subsystem-multirate=n \
    --unshare-state
