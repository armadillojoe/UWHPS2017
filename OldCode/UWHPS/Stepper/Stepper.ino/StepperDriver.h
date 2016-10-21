/*
 * Stepper Controller
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

#ifndef StepperDriver_H
#define StepperDriver_H
#include <Arduino.h>
#include <SPI.h>
#include <AMIS30543.h>

class StepperDriver {
  private:
  //internal variables
  int ustepState;                   //microstepper state
  int stepBuffer;                   //integer storing number of steps to go before move instruction completed
  int stepDisplacement;             //integer storing displaced steps since power on
  const int STEP_DELAY = 2;         //required delay in microseconds for stepper driver NXT
  const int DIR_DELAY = 1;          //required delay in microseconds for stepper driver DIR
  boolean lastDirection;
  unsigned long lastStep;           //time of last step in microseconds
  AMIS30543 stepper;

  //pins
  const uint8_t DIR = 6;            //Direction pin
  const uint8_t NXT = 7;            //Next pin
  const uint8_t SLAVE_SELECT = 10;  //SPI Slave Select pin
  const uint8_t CLR = 5;            //Clear pin

  //changable presets
  const int GEAR_RATIO = 27;        //gear ratio between sensor shaft and drive shaft
  const float STEP_ANGLE = 1.8;     //number of steps per degree
  int MAX = 30;                     //maximum number of steps that can be displaced
  int MIN = -30;                    //maximum number of steps that can be displaced in the negative direction
  unsigned long speedPeriod = 500;  //period for delaying between steps.  Is divided by microsteps
  int uStep = 2;                    //uStep setting
  int MAX_MILLIAMP = 1650;          //max stepper current

  //functions
  void step();                           //sends step signal
  void direction(bool);                  //sends direction signal
  int degreeToStep(float);          //converts degrees to steps
  float stepToDegree(int);          //converts steps to degrees
  
  public:
  StepperDriver();                  //initializer
  boolean prepare();                //sets pins and prepares the stepper driver
  void update();                    //must be called continuously if interrupts not enabled.  For stepping purposes.
  float getBuffer();                //gets degrees to still move.
  float getDisplacement();          //gets degrees moved since power on
  boolean checkDriverError();              //gets flags sent by driver

  void move(float);
  void activate();
  void deactivate();
  
};

#endif
