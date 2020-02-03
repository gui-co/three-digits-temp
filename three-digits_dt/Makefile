DTC = dtc
DTC_FLAGS = -@ -W no-unit_address_vs_reg

%.dtbo: %.dts
	$(DTC) $(DTC_FLAGS) -I dts -O dtb -o $@ $<

