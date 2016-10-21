/*
 * Project:          Stepper Controller
 * Target Board:     V2.0
 * Software:         V2.1
 * Author:           Ryan Pennell
 * 
 *         /\    /
 *        /  \  /
 *       /    \/
 *     ---------
 *     /\    /
 *    /  \  /
 *   /    \/
 * 
 */

#include <SPI.h>
#include <AMIS30543.h>
#include "StepperDriver.h"   
#include <Wire.h>
#include <QueueList.h>

StepperDriver stepper;

#define StepperDriverInterrupt
#define SLAVE_ADDRESS 0x06
QueueList<byte> byteQue;
int testInt = 0; //Stores value to be set to motor
byte newByte = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);  
  stepper.prepare();
  stepper.activate();
  //stepper.move(-15);
  Serial.println(stepper.getBuffer());
}

void loop() {
  stepper.update();
  
  /*if (stepper.getBuffer() != 0) {
    Serial.println(stepper.getBuffer());
  }
  
  if (millis() == 2000) {
    //stepper.move(15);
    Serial.println(stepper.getDisplacement());
  }
  
  if (millis() == 4000) {
    //stepper.move(10);
    Serial.println("return");
  }
  
  if (millis() == 6000) {
    stepper.move(0);
    Serial.println("return");
  }*/
  
}

void receiveData(int byteCount) {
  int reg = Wire.read();
  
  switch (reg) {
    case 0x01: readInt(); break;
  }
}

void readInt() {
  while(Wire.available()) {
    newByte = Wire.read();
    //Serial.println(newByte);
    byteQue.push(newByte);
  }
  if (byteQue.count() == 2)
  {
    int num = (int)byteQue.pop();
    int num2 = ((int)byteQue.pop()) << 8;
    num += num2;
    testInt = num;
    Serial.print("data received: ");
    Serial.println(testInt);
    stepper.move((float)testInt);
  }
}



