/*
 * Jack Gentsch
 * UWHPS Electronics Core Code '16-'17
 * 2/11/17
 * This Arduino sketch acts as the master of the Sub controls network
 * Sets PWM timers for 4 rear control fins, requests data from slave joystick via I2C
 */

#include <Servo.h>

Servo x_servo;
Servo y_servo;

int val;

void setup() {
  // put your setup code here, to run once:
  x_servo.attach(9); //analog pin 1
  y_servo.attach(10);
  val = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  val = (val + 1) % 180;
  x_servo.write(val);
  y_servo.write(val);
  delay(100);
}
