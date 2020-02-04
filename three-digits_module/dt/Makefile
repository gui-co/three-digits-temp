DTC = dtc
DTC_FLAGS = -@ -W no-unit_address_vs_reg -I dts -O dtb
CPP_FLAGS = -nostdinc -I $(KERNELDIR)/include -undef -x assembler-with-cpp

%.dtbo: %.dts
	$(CPP) $(CPP_FLAGS) $< > $<.tmp
	$(DTC) $(DTC_FLAGS) -o $@ $<.tmp

all: three-digits.dtbo

