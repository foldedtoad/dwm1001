/*! --------------------------------------------------------------------------
 *  @file    ex_11a_main.c
 *  @brief   Example of button usage. Simple callback on button press.
 * Copyright 2019 (c) Frederic Mes, RTLOC.
 *
 * All rights reserved.
 *
 */
#include <zephyr.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#define LOG_LEVEL 3
#include <logging/log.h>
LOG_MODULE_REGISTER(main);

/* Defines */
#define APP_HEADER  "\nDWM1001 & Zephyr\n"
#define APP_NAME    "Example 11a - BUTTON\n"
#define APP_VERSION "Version - 1.3\n"
#define APP_LINE    "=================\n"

/* 
 * See ./build/zephyr/include/generated/generated_dts_board.conf for details
 */
#define BUTTON_GPIO_CONTROLLER_NAME		DT_ALIAS_SW0_GPIOS_CONTROLLER
#define BUTTON_0_PIN					DT_GPIO_KEYS_BUTTON_0_GPIOS_PIN

static struct gpio_callback gpio_cb;
static struct device * gpiob;

/* Button event callback */
static void button_event(struct device * gpiob, 
	                     struct gpio_callback * cb, 
	                     u32_t pins)
{
	int button_state;

	gpio_pin_read(gpiob, BUTTON_0_PIN, &button_state);

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
	gpiob = device_get_binding(BUTTON_GPIO_CONTROLLER_NAME);
	if (!gpiob) {
		printk("error\n");
		return -1;
	}

	/* Init Button Interrupt */
	gpio_pin_configure(gpiob, BUTTON_0_PIN,
			            GPIO_DIR_IN | 
			            GPIO_INT |  
			            GPIO_PUD_PULL_UP | 
			            GPIO_INT_EDGE | 
			            GPIO_INT_DOUBLE_EDGE );

	gpio_init_callback(&gpio_cb, button_event, BIT(BUTTON_0_PIN));

	gpio_add_callback(gpiob, &gpio_cb);

	gpio_pin_enable_callback(gpiob, BUTTON_0_PIN);

	return 0;
}