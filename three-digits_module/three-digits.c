#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/kthread.h>
#include <linux/delay.h>

// number of 7 segments digits
#define N_DIGITS 3

// duration of the on state for each digit in us
#define ON_DURATION 500

// to be applied on power gpios
static int DIGITS_OFF[N_DIGITS] = {0, 0, 0};

// to be applied on segments gpios
//
//       __a__
//    f /     / b
//     /__g__/
//  e /     / c
//   /_____/
//      d
//                          a  b  c  d  e  f  g
//                          |  |  |  |  |  |  |
static int CHAR_0[7]     = {1, 1, 1, 1, 1, 1, 0};  // display a '0'
static int CHAR_1[7]     = {0, 1, 1, 0, 0, 0, 0};  // displat a '1'
static int CHAR_2[7]     = {1, 1, 0, 1, 1, 0, 1};  // display a '2'
static int CHAR_3[7]     = {1, 1, 1, 1, 0, 0, 1};  // display a '3'
static int CHAR_4[7]     = {0, 1, 1, 0, 0, 1, 1};  // display a '4'
static int CHAR_5[7]     = {1, 0, 1, 1, 0, 1, 1};  // display a '5'
static int CHAR_6[7]     = {1, 0, 1, 1, 1, 1, 1};  // display a '6'
static int CHAR_7[7]     = {1, 1, 1, 0, 0, 0, 0};  // display a '7'
static int CHAR_8[7]     = {1, 1, 1, 1, 1, 1, 1};  // display a '8'
static int CHAR_9[7]     = {1, 1, 1, 1, 0, 1, 1};  // display a '9'
static int CHAR_SPACE[7] = {0, 0, 0, 0, 0, 0, 0};  // displat a ' '

struct three_digits_data {
    char characters[N_DIGITS];           // character for earch digit
    bool dots[N_DIGITS];                 // dot for each digit
    struct gpio_desc *powers[N_DIGITS];  // gpios that control each digit power line
    struct gpio_descs *segments;         // gpios of the differents segments
};

/*****************************************************************************
 * digits loop task                                                          *
 *****************************************************************************/

static struct task_struct *three_digits_task;

static void three_digits_display_char(struct gpio_descs *s, char c) {
    switch (c) {
        case '0':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_0);
            break;
        case '1':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_1);
            break;
        case '2':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_2);
            break;
        case '3':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_3);
            break;
        case '4':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_4);
            break;
        case '5':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_5);
            break;
        case '6':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_6);
            break;
        case '7':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_7);
            break;
        case '8':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_8);
            break;
        case '9':
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_9);
            break;
        default:
            gpiod_set_array_value(s->ndescs, s->desc, CHAR_SPACE);
            break;
    }
}

static int three_digits_loop(void *data) {
    int current_digit = 0;
    struct three_digits_data *digits;

    digits = (struct three_digits_data*) data;

    while (!kthread_should_stop()) {
        // all digits off
        gpiod_set_array_value(N_DIGITS, digits->powers, DIGITS_OFF);

        // draw character
        three_digits_display_char(digits->segments,
                                  digits->characters[current_digit]);
        gpiod_set_value(digits->powers[current_digit], 1);

        // got to next digit
        current_digit++;
        if (current_digit > 2)
            current_digit = 0;

        usleep_range(ON_DURATION, ON_DURATION + 10);
    }

    return 0;
}

static int three_digits_start(struct platform_device *pdev) {
    struct device *dev;
    struct three_digits_data *s;
    int d;

    dev = &pdev->dev;
    s = devm_kzalloc(dev, sizeof(struct three_digits_data), GFP_KERNEL);
    if (!s)
        return -ENOMEM;

    for (d = 0 ; d < N_DIGITS ; d++) {
        s->powers[d] = devm_gpiod_get_index(dev, "power", d, GPIOD_OUT_LOW);
        if (IS_ERR(s->powers[d]))
            return PTR_ERR(s->powers[d]);
    }

    s->segments = devm_gpiod_get_array(dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(s->segments))
        return PTR_ERR(s->segments);

    platform_set_drvdata(pdev, s);

    // start main loop
    three_digits_task = kthread_run(three_digits_loop, (void*) s,
                                    "three-digits-loop");

    return 0;
}

static int three_digits_stop(struct platform_device *pdev) {
    struct three_digits_data *data;

    data = platform_get_drvdata(pdev);
    kthread_stop(three_digits_task);

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
MODULE_AUTHOR("Guillaume Communie <guillaume.communie@gmail.com>");

