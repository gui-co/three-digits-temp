#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

#define N_DIGITS 3

struct three_digits_data {
    char characters[N_DIGITS];           // character for earch digit
    bool dots[N_DIGITS];                 // dot for each digit
    struct gpio_desc *powers[N_DIGITS];  // gpios that control each digit power line
    struct gpio_descs *segments;         // gpios of the differents segments
};

static int three_digits_start(struct platform_device *pdev) {
    struct device *dev;
    struct three_digits_data *s;
    int d;

    dev = &pdev->dev;
    s = devm_kzalloc(dev, sizeof(struct three_digits_data), GFP_KERNEL);

    for (d = 0 ; d < N_DIGITS ; d++)
        s->powers[d] = devm_gpiod_get_index(dev, "power", d, GPIOD_OUT_LOW);

    s->segments = devm_gpiod_get_array(dev, "leds", GPIOD_OUT_LOW);

    platform_set_drvdata(pdev, s);

    return 0;
}

static int three_digits_stop(struct platform_device *dev) {
    printk(KERN_INFO "three-digits driver stops\n");
    return 0;
}

static const struct of_device_id of_three_digits_match[] = {
    { .compatible = "three-digits", },
    {},
};

MODULE_DEVICE_TABLE(of, of_three_digits_match);

static struct platform_driver three_digits_driver = {
    .probe = three_digits_start,
    .remove = three_digits_stop,
    .driver = {
        .name = "three-digits",
        .of_match_table = of_three_digits_match,
    },
};

module_platform_driver(three_digits_driver);
MODULE_LICENSE("GPL");

