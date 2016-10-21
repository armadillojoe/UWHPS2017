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

#include "StepperDriver.h"
#include "Arduino.h"
#include "SPI.h"
#include "AMIS30543.h"

StepperDriver::StepperDriver() {
  //do nothing.  No guaruntee intitializer is executed on microcontroller
}

boolean StepperDriver::checkDriverError() {

  uint16_t latchedFlags = stepper.readLatchedStatusFlagsAndClear();
  uint8_t nonLatchedFlags = stepper.readNonLatchedStatusFlags();

  if (!stepper.verifySettings()) {
    stepper.applySettings();
    if (!stepper.verifySettings()) {
      Serial.println("Settings reprogramming failed: board may be experiencing power issues");
      return 1;
    }
  }

  if (latchedFlags || nonLatchedFlags) {
    if (stepper.OPENY && nonLatchedFlags)   {
      Serial.println("OPENY - Open Y coil: Motor's Y coil is not attached");
    }
    if (stepper.OPENX && nonLatchedFlags)   {
      Serial.println("OPENX - Open X coil: Motor's X coil is not attached");
    }
    if (stepper.WD && nonLatchedFlags)      {
      Serial.println("WD - Watchdog Reset: Internal reset event triggered, AMIS Driver reset");
    }
    if (stepper.CPFAIL && nonLatchedFlags)  {
      Serial.println("CPFAIL - Charge Pump Failure: Charge pump could not reach voltage required by motor");
    }
    if (stepper.TW && nonLatchedFlags)      {
      Serial.println("TW - Thermal Warning: Inadequate heat dissapation");
    }
    if (stepper.TSD && latchedFlags)         {
      Serial.println("TSD - Thermal Shutdown: Inadequate heat dissapation.  AMIS shut down triggered to prevent damage");
    }
    if (stepper.OVCXNB && latchedFlags)     {
      Serial.println("OVCXNB - Overcurrent on H-Bridge: X coil negative pole grounding transistor");
    }
    if (stepper.OVCXNT && latchedFlags)      {
      Serial.println("OVCXNT - Overcurrent on H-Bridge: X coil negative pole power rail transistor");
    }
    if (stepper.OVCXPB && latchedFlags)      {
      Serial.println("OVCXPB - Overcurrent on H-Bridge: X coil positive pole grounding transistor");
    }
    if (stepper.OVCXPT && latchedFlags)      {
      Serial.println("OVCXPT - Overcurrent on H-Bridge: X coil positive pole power rail transistor");
    }
    if (stepper.OVCYNB && latchedFlags)      {
      Serial.println("OVCYNB - Overcurrent on H-Bridge: Y coil negative pole grounding transistor");
    }
    if (stepper.OVCYNT && latchedFlags)      {
      Serial.println("OVCYNT - Overcurrent on H-Bridge: Y coil negative pole power rail transistor");
    }
    if (stepper.OVCYPB && latchedFlags)      {
      Serial.println("OVCYPB - Overcurrent on H-Bridge: Y coil positive pole grounding transistor");
    }
    if (stepper.OVCYPT && latchedFlags)      {
      Serial.println("OVCYPT - Overcurrent on H-Bridge: Y coil positive pole power rail transistor");
    }
    return 1;
  }
  return 0;
}

void StepperDriver::step() {
  digitalWrite(this->NXT, HIGH);
  delayMicroseconds(this->STEP_DELAY);
  digitalWrite(this->NXT, LOW);
  delayMicroseconds(this->STEP_DELAY);
}

void StepperDriver::direction(boolean dir) {
  if (dir != this->lastDirection) {
    delayMicroseconds(this->DIR_DELAY);
    digitalWrite(this->DIR, dir);
    delayMicroseconds(this->DIR_DELAY);
    this->lastDirection = dir;
  }
}

boolean StepperDriver::prepare() {
  noInterrupts();           //disable all interrupts
  TCCR1B = 0;               // same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 5000; // = (16*10^6) / (1*64) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 64 prescaler
  TCCR1B |= (1 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts();             // enable all interrupts

  SPI.begin();
  this->stepper.init(this->SLAVE_SELECT);

  pinMode(this->DIR, OUTPUT);
  pinMode(this->NXT, OUTPUT);
  pinMode(this->SLAVE_SELECT, OUTPUT);
  pinMode(this->CLR, OUTPUT);

  digitalWrite(this->DIR, LOW);
  digitalWrite(this->NXT, LOW);
  digitalWrite(this->SLAVE_SELECT, LOW);

  digitalWrite(this->CLR, HIGH);
  delay(1);
  digitalWrite(this->CLR, LOW);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings.
  this->stepper.resetSettings();

  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  this->stepper.setCurrentMilliamps(this->MAX_MILLIAMP);

  // Set the number of microsteps that correspond to one full step.
  this->stepper.setStepMode(this->uStep);
  this->direction(0);

  return (this->checkDriverError());
}

void StepperDriver::update() {
  if (this->stepBuffer != 0) {
    if (this->stepBuffer > 0) {
      this->stepBuffer--;
      this->stepDisplacement++;
    } else {
      this->stepBuffer++;
      this->stepDisplacement--;
    }
    step();
  }
}

void StepperDriver::move(float goTo) {
  //put here and not in update for speed considerations
  this->stepBuffer += degreeToStep(goTo) - this->stepDisplacement;

  if (this->stepBuffer >= 0) {
    direction(0);
  } else {
    direction(1);
  }
}

int StepperDriver::degreeToStep(float degree) {
  return (degree * this->GEAR_RATIO * this->uStep / this->STEP_ANGLE);
}

float StepperDriver::stepToDegree(int step) {
  return ((float)step) * this->STEP_ANGLE / (((float)this->GEAR_RATIO) * ((float)this->uStep));
}

float StepperDriver::getBuffer() {
  return this->stepToDegree(this->stepBuffer);
}

float StepperDriver::getDisplacement() {
  return this->stepToDegree(this->stepDisplacement);
}

void StepperDriver::activate() {
  stepper.enableDriver();
}

void StepperDriver::deactivate() {
  stepper.disableDriver();
}

