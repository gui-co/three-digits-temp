#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");

static const struct of_device_id of_three_digits_match[] = {
    { .compatible = "three-digits", },
    {},
};

MODULE_DEVICE_TABLE(of, of_three_digits_match);

static int three_digits_start(struct platform_device *dev) {
    printk(KERN_INFO "three-digits driver starts\n");
    return 0;
}

static int three_digits_stop(struct platform_device *dev) {
    printk(KERN_INFO "three-digits driver stops\n");
    return 0;
}

static void three_digits_shutdown(struct platform_device *dev) {
    three_digits_stop(dev);
}

static struct platform_driver three_digits_driver = {
    .probe = three_digits_start,
    .remove = three_digits_stop,
    .shutdown = three_digits_shutdown,
    .driver = {
        .name = "three-digits",
        .of_match_table = of_three_digits_match,
    },
};

module_platform_driver(three_digits_driver);

