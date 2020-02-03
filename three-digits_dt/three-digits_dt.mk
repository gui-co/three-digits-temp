THREE_DIGITS_DT_VERSION = 1.0
THREE_DIGITS_DT_SITE = $(BR2_EXTERNAL_THREE_DIGITS_PATH)/three-digits_dt
THREE_DIGITS_DT_SITE_METHOD = local

DTS = $(notdir $(basename $(wildcard $(BR2_EXTERNAL_THREE_DIGITS_PATH)/three-digits_dt/*.dts)))
DTBOS = $(addsuffix .dtbo, $(DTS))

define THREE_DIGITS_DT_BUILD_CMDS
	$(LINUX_MAKE_ENV) $(MAKE) $(LINUX_MAKE_FLAGS) -C $(@D) $(DTBOS)
endef

define THREE_DIGITS_DT_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0644 $(@D)/*.dtbo $(BINARIES_DIR)/rpi-firmware/overlays
endef

$(eval $(generic-package))

