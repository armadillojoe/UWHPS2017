/*
 * Jack Gentsch
 * UWHPS Electronics Core Code '16-'17
 * 2/11/17
 * This Arduino sketch acts as the master of the Sub controls network
 * Sets PWM timers for 4 rear control fins, requests data from slave joystick via I2C
 */

#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADRX 8
#define DATA_BYTES 4
#define DEBUG 0 //Prevent printing when in operation, serial printing wastes computation time

Servo x_servo;
Servo y_servo;

int x_val;
int y_val;
int z_val;
int b_val;
int byte_num;
int receiveData[5];

void setup() {
  // put your setup code here, to run once:
  x_servo.attach(9); //analog pin 1
  y_servo.attach(10);
  x_val = 0; //Initialize potentiometer values and button val to zero
  y_val = 0;
  z_val = 0;
  b_val = 0;
  byte_num = 1;
  receiveData;
  Wire.begin();
  if (DEBUG) {
    Serial.begin(9600);
  }
}

void loop() {
  int byte_num = 0;
  char i2c_received = 'F';
  Wire.requestFrom(SLAVE_ADRX, DATA_BYTES);
  while (Wire.available()) {
    receiveData[byte_num] = Wire.read();
    byte_num++;
  }
  if (DEBUG) {
    printReceive(receiveData);
    Serial.print("Received ");
    Serial.print(byte_num + 1);
    Serial.print(" of requested ");
    Serial.print(DATA_BYTES);
    Serial.println(" bytes");
  }
  //Map incoming joystick potentiometer values to 180 degrees
  x_val = map(receiveData[0], 0, 255, 0, 180);
  y_val = map(receiveData[1], 0, 255, 0, 180);
  z_val = map(receiveData[2], 0, 255, 0, 180);
  //Drive servos across 180 degree range
  x_servo.write(x_val);
  y_servo.write(y_val);
  if (DEBUG) {
    delay(100);
  }
}

void printReceive(int data[]) {
  for (int i = 0; i < 4; i++) {
    Serial.println("Received byte num ");
    Serial.print(i);
    Serial.print(", is equal to int, char");
    Serial.print((unsigned int) data[i]);
    Serial.print(", ");
    Serial.println((unsigned char) data[i]);
  }
}
