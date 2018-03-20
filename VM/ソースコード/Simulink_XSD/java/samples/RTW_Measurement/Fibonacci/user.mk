# vpath and -I
AUX_SRC_DIR = /home/kohei-y/src/github/NIS/Simulink_XSD/models/Fibonacci \

# auxiliary C source files
AUX_C_SRCS = Fibonacci_data.c rtGetInf.c  rtGetNaN.c  rt_nonfinite.c


# auxiliary asm source files
AUX_S_SRCS =

# generated file for spec measurement
TARGET_SRC = measure.c

# include and depend header (generated header and your write header)
TARGET_HDRS = measure.h measure_cforest.h
