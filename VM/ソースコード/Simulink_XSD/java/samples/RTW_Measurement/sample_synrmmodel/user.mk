SYNRMDIR = $(HOME)/proj/CSP/Simulink_XSD/0911/synrmmodel_mt_RTW_ert_rtw_140911

# vpath and -I
AUX_SRC_DIR = $(SYNRMDIR)/mtr $(SYNRMDIR)/synrmmodel_mt_RTW_ert_rtw

# auxiliary C source files
AUX_C_SRCS = conswitchy.c currentcond.c currentcony.c dIpmsmd.c dIpmsmy.c \
	degfiltery.c myconswitch_wrapper.c mycurrentcon_wrapper.c \
	mydIpmsm_wrapper.c mydegfilter_wrapper.c myobserverab_wrapper.c \
	mypositionestiab_wrapper.c myvelocitycon_wrapper.c \
	myvelocityesti_wrapper.c ovserverabd.c ovserveraby.c \
	positionestiaby.c rtGetInf.c rtGetNaN.c rt_nonfinite.c \
	rt_zcfcn.c velocitycond.c velocitycony.c velocityestid.c \
	synrmmodel_mt_RTW_data.c

# auxiliary asm source files
AUX_S_SRCS =

# generated file for spec measurement
TARGET_SRC = measure.c

# include and depend header (generated header and your write header)
TARGET_HDRS = measure.h measure_cforest.h

# CFLAGS = -O2
# CPPFLAGS = -DCOUNT=100
