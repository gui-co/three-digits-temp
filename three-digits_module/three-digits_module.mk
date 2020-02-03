THREE_DIGITS_MODULE_VERSION = 1.0
THREE_DIGITS_MODULE_SITE = $(BR2_EXTERNAL_THREE_DIGITS_PATH)/three-digits_module
THREE_DIGITS_MODULE_SITE_METHOD = local
$(eval $(kernel-module))
$(eval $(generic-package))

