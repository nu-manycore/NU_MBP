SIMULINK_XSD = ../../Simulink_XSD
ADD_BLOCK_INFO = $(SIMULINK_XSD)/add_block_info

#TARGET=synrmmodel
TARGET=c_gmres

#
# You should change them
#
ifeq ($(TARGET),synrmmodel)
INPUT_XML = $(SIMULINK_XSD)/0925/yy.xml
else
ifeq ($(TARGET),c_gmres)
INPUT_XML = $(SIMULINK_XSD)/testout.xml
endif
endif

CSV = $(TARGET).csv
OUT_XML = $(TARGET).xml

$(OUT_XML) : $(CSV) $(INPUT_XML) $(ADD_BLOCK_INFO)
	$(ADD_BLOCK_INFO) -o $@ -i $(INPUT_XML) $(CSV)
