#TEST_TARGET = pid
#TEST_TARGET = JIKKEN
TEST_TARGET = synrmmodel_mt_RTW

#
# Test Data
#
ifeq ($(TEST_TARGET),pid)
TEST_RTW_TARGET_XML = models/pid/xx.xml
TEST_RTW_TARGET_HDR = models/pid/pid_controller1_ert_rtw/pid_controller1.h
TEST_RTW_TARGET_SRC = models/pid/pid_controller1_ert_rtw/pid_controller1.c
TEST_RTW_TARGET_DAT = models/pid/pid_controller1_ert_rtw/pid_controller1_data.c
TEST_RTW_TARGET_OUT = models/pid/yy.xml
TEST_BLXML_TARGET_OPTION_ADD += -H pid_controller1_types.h
else	# !pid
ifeq ($(TEST_TARGET),test1)
TEST_RTW_TARGET_XML = models/test1/xx.xml
TEST_RTW_TARGET_HDR = models/test1/testsys_ert_rtw/testsys.h
TEST_RTW_TARGET_SRC = models/test1/testsys_ert_rtw/testsys.c
TEST_RTW_TARGET_DAT = models/test1/testsys_ert_rtw/testsys_data.c
TEST_RTW_TARGET_OUT = models/test1/yy.xml
TEST_BLXML_TARGET_OPTION_ADD += -H testsys_types.h
else	# !test1
ifeq ($(TEST_TARGET),testmux)
TEST_RTW_TARGET_XML = models/testmux/xx.xml
TEST_RTW_TARGET_HDR = models/testmux/testmux3_ert_rtw/testmux3.h
TEST_RTW_TARGET_SRC = models/testmux/testmux3_ert_rtw/testmux3.c
TEST_RTW_TARGET_DAT = models/testmux/testmux3_ert_rtw/testmux3_data.c
TEST_RTW_TARGET_OUT = models/testmux/yy.xml
TEST_BLXML_TARGET_OPTION_ADD += -H testmux3_types.h
else	# !testmux
ifeq ($(TEST_TARGET),Mfunc)
TEST_RTW_TARGET_XML = models2/Mfunc/mfunction2.xml
TEST_RTW_TARGET_HDR = models2/Mfunc/mfunction2_ert_rtw/mfunction2.h
TEST_RTW_TARGET_SRC = models2/Mfunc/mfunction2_ert_rtw/mfunction2.c
TEST_RTW_TARGET_DAT = models2/Mfunc/mfunction2_ert_rtw/mfunction2_data.c
TEST_RTW_TARGET_OUT = models2/Mfunc/yy.xml
TEST_BLXML_TARGET_OPTION_ADD += -H mfunction2_types.h
else	# !Mfunc
ifeq ($(TEST_TARGET),cell)
#TEST_RTW_TARGET_XML = models/cell/xx.xml
#TEST_RTW_TARGET_HDR = models/cell/vipcellcounting_win_ert_rtw/vipcellcounting_win.h
#TEST_RTW_TARGET_SRC = models/cell/vipcellcounting_win_ert_rtw/vipcellcounting_win.c
#TEST_RTW_TARGET_DAT = models/cell/vipcellcounting_win_ert_rtw/vipcellcounting_win_data.c
#TEST_RTW_TARGET_OUT = models/cell/yy.xml
TEST_RTW_TARGET_XML = models2/cell/vipcellcounting_win.xml
TEST_RTW_TARGET_HDR = models2/cell/vipcellcounting_win_ert_rtw/vipcellcounting_win.h
TEST_RTW_TARGET_SRC = models2/cell/vipcellcounting_win_ert_rtw/vipcellcounting_win.c
TEST_RTW_TARGET_DAT = models2/cell/vipcellcounting_win_ert_rtw/vipcellcounting_win_data.c
TEST_RTW_TARGET_OUT = models2/cell/yy.xml
TEST_BLXML_TARGET_OPTION_ADD += -H vipcellcounting_win_types.h -H math.h \
	-C -I. -C -Imodels2/cell/vipcellcounting_win_ert_rtw \
	-C -ferror-limit=1000
else	# !cell
ifeq ($(TEST_TARGET),JIKKEN)
TEST_RTW_TARGET_XML = temp/xx.xml
TEST_RTW_TARGET_HDR = temp/JIKKEN_trigger_7_ert_rtw/JIKKEN_trigger_7.h
TEST_RTW_TARGET_SRC = temp/JIKKEN_trigger_7_ert_rtw/JIKKEN_trigger_7.c
TEST_RTW_TARGET_DAT = none
TEST_RTW_TARGET_OUT = temp/yy.xml
TEST_BLXML_TARGET_OPTION_ADD += -C -I. -C -DMAJOR_TIME_STEP=1 \
	-C -ferror-limit=1000
else	# !JIKKEN
ifeq ($(TEST_TARGET),synrmmodel_mt_RTW)
#TEST_RTW_TARGET_XML = 0910/synrmmodel_mt_RTW.xml
#TEST_RTW_TARGET_HDR = 0910/synrmmodel_mt_RTW_ert_rtw/synrmmodel_mt_RTW.h
#TEST_RTW_TARGET_SRC = 0910/synrmmodel_mt_RTW_ert_rtw/synrmmodel_mt_RTW.c
#TEST_RTW_TARGET_DAT = 0910/synrmmodel_mt_RTW_ert_rtw/synrmmodel_mt_RTW_data.c
#TEST_RTW_TARGET_OUT = 0910/yy.xml
#TEST_RTW_TARGET_XML = 0911/synrmmodel_mt_RTW_ert_rtw_140911/xx.xml
#TEST_RTW_TARGET_OUT = 0911/synrmmodel_mt_RTW_ert_rtw_140911/yy.xml
TEST_RTW_TARGET_XML = 0925/synrmmodel_mt_RTW.xml
TEST_RTW_TARGET_OUT = 0925/yy.xml
TEST_RTW_TARGET_HDR = 0911/synrmmodel_mt_RTW_ert_rtw_140911/synrmmodel_mt_RTW_ert_rtw/synrmmodel_mt_RTW.h
TEST_RTW_TARGET_SRC = 0911/synrmmodel_mt_RTW_ert_rtw_140911/synrmmodel_mt_RTW_ert_rtw/synrmmodel_mt_RTW.c
TEST_RTW_TARGET_DAT = 0911/synrmmodel_mt_RTW_ert_rtw_140911/synrmmodel_mt_RTW_ert_rtw/synrmmodel_mt_RTW_data.c
TEST_BLXML_TARGET_OPTION_ADD += -C -I. -C -DMAJOR_TIME_STEP=1 \
	-C -ferror-limit=1000
else	# !synrmmodel_mt_RTW
ifeq ($(TEST_TARGET),c_gmres)
TEST_RTW_TARGET_XML = 20150313/20150313_mpc2/xml/c_gmres.xml
TEST_RTW_TARGET_OUT = 20150313/20150313_mpc2/xml/yy.xml
TEST_RTW_TARGET_HDR = 20150313/20150313_mpc2/c_gmres_ert_rtw/c_gmres.h
TEST_RTW_TARGET_SRC = 20150313/20150313_mpc2/c_gmres_ert_rtw/c_gmres.c
TEST_RTW_TARGET_DAT = 20150313/20150313_mpc2/c_gmres_ert_rtw/c_gmres_data.c
TEST_BLXML_TARGET_OPTION_ADD += -C -I. -C -DMAJOR_TIME_STEP=1 \
	-C -ferror-limit=1000
else	# !c_gmres
TEST_RTW_TARGET_XML = none
TEST_RTW_TARGET_HDR = none
TEST_RTW_TARGET_SRC = none
TEST_RTW_TARGET_OUT = none
endif	# !c_gmres
endif	# !synrmmodel_mt_RTW
endif	# !JIKKEN
endif	# !cell
endif	# !Mfunc
endif	# !testmux
endif	# !test1
endif	# !pid
