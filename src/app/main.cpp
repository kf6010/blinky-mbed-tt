/*
 * Simple program for the application board
 *
 * Implemented using a time-triggered scheduler.
 *
 * DK - 08-June-2018
 */


#include <stdbool.h>
#include <stdint.h>
#include <scheduler.h>
#include <mbed.h>
#include "C12832.h"
#include "MMA7660.h"
#include "LM75B.h"
#include <stdio.h>

static DigitalOut red(LED_RED);
static DigitalOut green(LED_GREEN);
static DigitalOut led_app_red(D5);
static DigitalOut led_app_green(D9);
static C12832 lcd(D11, D13, D12, D7, D10);
static AnalogIn pot1(A0); // Pot 1 - Left
static AnalogIn pot2(A1); // Pot 2 - Right
static float pot1Val;
static float pot2Val;
static DigitalIn joystick[5] = {D4, A2, A3, A4, A5};
static char symbols[] = {'C', 'U', 'D', 'L', 'R'};
static char joystickVal;
static MMA7660 accel(D14, D15);
static float accelVal[3];
static LM75B temp(D14, D15);
static float tempVal;
static DigitalIn sw3(PTA4);
static bool sw3Pressed = false;
static PwmOut speaker(D6);

static void samplePot(void);
static void sampleJoystick(void);
static void sampleSW3(void);
static void sampleAccel(void);
static void sampleTemp(void);
static void led1ToggleTask(void);
static void led2ToggleTask(void);
static void updatePot(void);
static void updateJoystick(void);
static void updateAccel(void);
static void updateTemp(void);
static void updateSpeaker(void);

int main () {
  red = 0;
  green = 1;
  led_app_red = 1;
  lcd.cls();
  speaker.period_us(2272);
  speaker.pulsewidth_us(0);

  schInit();
  schAddTask(sampleJoystick, 0, 10);
  schAddTask(sampleSW3, 0, 10);
  schAddTask(samplePot, 0, 20);
  schAddTask(sampleAccel, 0, 20);
  schAddTask(sampleTemp, 0, 120);
  schAddTask(led1ToggleTask, 11, 60);
  schAddTask(led2ToggleTask, 61, 60);
  schAddTask(updatePot, 3, 20);
  schAddTask(updateJoystick, 7, 10);
  schAddTask(updateAccel, 13, 20);
  schAddTask(updateTemp, 17, 120);
  schAddTask(updateSpeaker, 0, 10);

  schStart();
  
  while (true) {
    schDispatch();
  }
}

static void samplePot(void) {
  pot1Val = pot1.read();
  pot2Val = pot2.read();
}

static void sampleJoystick(void) {
  int i = 0;

  joystickVal = '-';
  for (i = 0; i < 5; i+=1) {
      if (joystick[i] == 1) {
          joystickVal = symbols[i];
          break;
      }
   }
}

static void sampleSW3(void) {
  sw3Pressed = (sw3 == 0) ? true : false;
}

static void sampleAccel(void) {
  accelVal[0] = accel.x();
  accelVal[1] = accel.y();
  accelVal[2] = accel.z();
}

static void sampleTemp(void) {
  tempVal = temp.read();
}

static void led1ToggleTask(void) {
  red = 1 - red;
}

static void led2ToggleTask(void) {
  led_app_green = 1 - led_app_green;
}

static void updatePot(void) {
  lcd.locate(0, 0);
  lcd.printf("L: %0.2f", pot1Val);
  lcd.locate(0, 8);
  lcd.printf("R: %0.2f", pot2Val);
}

static void updateJoystick(void) {
  lcd.locate(0, 16);
   lcd.printf("J: %c", joystickVal);
}

static void updateAccel(void) {
  lcd.locate(43, 0);
  lcd.printf("X: %0.2f", accelVal[0]);
  lcd.locate(43, 8);
  lcd.printf("Y: %0.2f", accelVal[1]);
  lcd.locate(43, 16);
  lcd.printf("Z: %0.2f", accelVal[2]);
}

static void updateTemp(void) {
  lcd.locate(86, 0);
  lcd.printf("T: %02.2f", tempVal);
}

static void updateSpeaker(void) {
  if (sw3Pressed) {
    speaker.pulsewidth_us(1136);
  } 
  else {
    speaker.pulsewidth_us(0);
  }
}

