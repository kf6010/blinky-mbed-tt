/*
 * Simple program to flash the LEDs
 *
 * Implemented using a time-triggered scheduler.
 *
 * DK - 08-June-2018
 */


#include <stdbool.h>
#include <stdint.h>
#include <scheduler.h>
#include <mbed.h>

void led1ToggleTask(void);
void led2ToggleTask(void);


static DigitalOut red(LED_RED);
static DigitalOut green(LED_GREEN);
static DigitalOut led_app_red(D5);
static DigitalOut led_app_green(D9);

int main () {
    red = 1;
    green = 1;
    led_app_red = 1;
    led_app_green = 1;

    schInit();
    schAddTask(led1ToggleTask, 0, 500);
    schAddTask(led2ToggleTask, 500, 500);


    schStart();

    while (true) {
        schDispatch();
    }
}


void led1ToggleTask(void) {
    red = 1 - red;
}

void led2ToggleTask(void) {
    led_app_green = 1 - led_app_green;
}

