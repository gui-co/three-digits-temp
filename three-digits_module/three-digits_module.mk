THREE_DIGITS_MODULE_VERSION = 1.0
THREE_DIGITS_MODULE_SITE = $(BR2_EXTERNAL_THREE_DIGITS_PATH)/three-digits_module
THREE_DIGITS_MODULE_SITE_METHOD = local
THREE_DIGITS_MODULE_DEPENDENCIES = linux

LINUX_DIR=$(BUILD_DIR)/linux-$(call qstrip,$(BR2_LINUX_KERNEL_VERSION))
LINUX_VERSION_PROBED = `$(MAKE) $(LINUX_MAKE_FLAGS) -C $(LINUX_DIR) --no-print-directory -s kernelrelease 2>/dev/null`

define THREE_DIGITS_MODULE_BUILD_CMDS
	@echo $(LINUX_DIR)
	$(LINUX_MAKE_ENV) $(MAKE) -C $(@D) $(LINUX_MAKE_FLAGS) KERNELDIR=$(LINUX_DIR) PWD=$(@D)
	$(LINUX_MAKE_ENV) $(MAKE) -C $(@D)/dt KERNELDIR=$(LINUX_DIR)
endef

define THREE_DIGITS_MODULE_INSTALL_TARGET_CMDS
	mkdir -p $(TARGET_DIR)/lib/modules/$(LINUX_VERSION_PROBED)/extra
	$(INSTALL) -D -m 0644 $(@D)/*.ko $(TARGET_DIR)/lib/modules/$(LINUX_VERSION_PROBED)/extra
	$(INSTALL) -D -m 0644 $(@D)/dt/*.dtbo $(BINARIES_DIR)/rpi-firmware/overlays
endef

$(eval $(generic-package))
