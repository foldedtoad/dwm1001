/*! --------------------------------------------------------------------------
 *  @file    ex_11a_main.c
 *  @brief   Example of button usage. Simple callback on button press.
 * Copyright 2019 (c) Frederic Mes, RTLOC.
 *
 * All rights reserved.
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define LOG_LEVEL 3
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

/* Defines */
#define APP_HEADER  "\nDWM1001 & Zephyr\n"
#define APP_NAME    "Example 11a - BUTTON\n"
#define APP_VERSION "Version - 1.3\n"
#define APP_LINE    "=================\n"

/* 
 * See ./build/zephyr/include/generated/generated_dts_board.conf for details
 */
#define BUTTON_0_PIN  DT_GPIO_PIN(DT_ALIAS(sw0), gpios)

#define GPIO_NAME  DT_NODE_FULL_NAME(DT_NODELABEL(gpio0))


static struct gpio_callback gpio_cb;
static const struct device * gpiob;

/* Button event callback */
static void button_event(const struct device * gpiob, 
                         struct gpio_callback * cb, 
                         uint32_t pins)
{
    int button_state;

    button_state = gpio_pin_get(gpiob, BUTTON_0_PIN);

    printk("Button %s cycle count %u\n", 
           button_state ?  "released:" : "pressed: ", 
           k_cycle_get_32());
}

/**
 * Application entry point.
 */
int dw_main(void)
{
    /* Display application name on console. */
    printk(APP_HEADER);
    printk(APP_NAME);
    printk(APP_VERSION);
    printk(APP_LINE);
    
    /* Get GPIO device binding */
    gpiob = device_get_binding(GPIO_NAME);

    if (!gpiob) {
        printk("error\n");
        return -1;
    }

    /* Init Button Interrupt */
    int flags = (GPIO_INPUT      | 
                 GPIO_ACTIVE_LOW |  
                 GPIO_PULL_UP    | 
                 GPIO_INT_EDGE   | 
                 GPIO_INT_EDGE_BOTH);

    gpio_pin_configure(gpiob, BUTTON_0_PIN, flags);

    gpio_pin_interrupt_configure(gpiob, BUTTON_0_PIN, GPIO_INT_EDGE_TO_ACTIVE);

    gpio_init_callback(&gpio_cb, button_event, BIT(BUTTON_0_PIN));

    gpio_add_callback(gpiob, &gpio_cb);

    return 0;
}
