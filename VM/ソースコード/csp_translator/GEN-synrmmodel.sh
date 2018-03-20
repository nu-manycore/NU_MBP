#!/bin/sh

exec > testout-synrmmodel.log
exec 2>&1

no_gcc=--no-gcc
#no_gcc=

./csp_translator ${no_gcc} \
    --verbose \
    --target=mcos \
    -i testout-synrmmodel.h \
    -s pe_test/synrmmodel.xml \
    --output-code testout-synrmmodel.c \
    --output-csp testout-synrmmodel.dot \
    --output-var testout-synrmmodel.csv \
    --label-only-task-id \
    --create-t0=n \
    --ignore-to-sink=y \
    --ignore-isolated=y \
    --ignore-leaves=n \
    --concat-block=n \
    --add-timer=0 \
    --input-multirate=n \
    --incorporate-io=y \
    --subsystem-multirate=n \
    --unshare-state
