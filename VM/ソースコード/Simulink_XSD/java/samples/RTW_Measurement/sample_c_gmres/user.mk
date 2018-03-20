# vpath and -I
AUX_SRC_DIR = $(PWD)/../../../../../yamasefu_tmp/cgmres/c_gmres_ert_rtw

# auxiliary C source files
AUX_C_SRCS = c_gmres_data.c rtGetInf.c rt_nonfinite.c rtGetNaN.c

# auxiliary asm source files
AUX_S_SRCS =

# generated file for spec measurement
TARGET_SRC = c_gmres_measure.c

# include and depend header (generated header and your write header)
TARGET_HDRS = c_gmres_measure.h measure_cforest.h

# CFLAGS = -O2
# CPPFLAGS = -DCOUNT=10
