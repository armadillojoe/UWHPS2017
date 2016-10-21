#include <Wire.h>
#include <QueueList.h>

#define SLAVE_ADDRESS 0x06

QueueList<byte> byteQue;
int stateX = 0;
int stateY = 0;
float currPosX = 0;
float currPosY = 0;
int testInt = 0;
int x = 0;
int y = 0;
byte newByte = 0;
boolean recievingX = true;
boolean settingX = true;
boolean const printValues = true;

#define inA1_1 0 // input 1 of the 1st stepper
#define inA2_1 1 // input 2 of the 1st stepper
#define inB1_1 2 // input 3 of the 1st stepper
#define inB2_1 3 // input 4 of the 1st stepper

#define inA1_2 4 // input 1 of the 2nd stepper
#define inA2_2 5 // input 2 of the 2nd stepper
#define inB1_2 6 // input 3 of the 2nd stepper
#define inB2_2 7 // input 4 of the 2nd stepper

#define inA1_3 8 // input 1 of the 3rd stepper
#define inA2_3 9 // input 2 of the 3rd stepper
#define inB1_3 10 // input 3 of the 3rd stepper
#define inB2_3 11 // input 4 of the 3rd stepper

#define inA1_4 A3 // input 1 of the 4th stepper
#define inA2_4 A2 // input 2 of the 4th stepper
#define inB1_4 A1 // input 3 of the 4th stepper
#define inB2_4 A0 // input 4 of the 4th stepper

int stepDelay = 10; // Delay between steps in milliseconds

void setup() {
  if (printValues) {
    Serial.begin(115200);
  }
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  if (!printValues) {
    pinMode(inA1_1, OUTPUT);
    pinMode(inA2_1, OUTPUT);
    pinMode(inB1_1, OUTPUT);
    pinMode(inB2_1, OUTPUT);
  }
  pinMode(inA1_2, OUTPUT);
  pinMode(inA2_2, OUTPUT);
  pinMode(inB1_2, OUTPUT);
  pinMode(inB2_2, OUTPUT);
  pinMode(inA1_3, OUTPUT);
  pinMode(inA2_3, OUTPUT);
  pinMode(inB1_3, OUTPUT);
  pinMode(inB2_3, OUTPUT);
  pinMode(inA1_4, OUTPUT);
  pinMode(inA2_4, OUTPUT);
  pinMode(inB1_4, OUTPUT);
  pinMode(inB2_4, OUTPUT);
}

int degreeToStep(int degree) {
  return (int)(degree * 27 * 2 / 1.8);
}

void moveToDegree(int degree, float *currPos, int *state) {
  float displacement = (float)degree - *currPos;
  if (abs(displacement) < 1) {
    stopMotor();
  } else if (displacement > 0) {
    stepFun(state, 1);
    *currPos += 1.8 / (27 * 2);
  } else {
    stepFun(state, 0);
    *currPos -= 1.8 / (27 * 2);
  }
  delay(100);
}

void receiveData(int byteCount) {
  int reg = Wire.read();

  switch (reg) {
    case 0x01: readInt(); break;
  }
}

void readInt() {
  while (byteQue.count() > 0) {
    byteQue.pop();
  }
  while (Wire.available()) {
    newByte = Wire.read();
    //Serial.println(newByte);
    byteQue.push(newByte);
  }
  if (byteQue.count() == 2) {
    int num = (int)byteQue.pop();
    int num2 = ((int)byteQue.pop()) << 8;
    num += num2;
    testInt = num;
    if (recievingX) {
      x = testInt;
    } else {
      y = testInt;
    }
    recievingX = !recievingX;
    Serial.print("data received: ");
    Serial.println(testInt);
    //moveToDegree(testInt);
  }
}

void step1(boolean x) {
  if (x) {
    Serial.println("Step 1");
    digitalWrite(inA1_1, LOW);
    digitalWrite(inA2_1, HIGH);
    digitalWrite(inB1_1, HIGH);
    digitalWrite(inB2_1, LOW);
    digitalWrite(inA1_2, LOW);  //Correct
    digitalWrite(inA2_2, HIGH); //Correct
    digitalWrite(inB1_2, HIGH); //Correct
    digitalWrite(inB2_2, LOW);  //Correct
  } else {
    digitalWrite(inA1_3, LOW);
    digitalWrite(inA2_3, HIGH);
    digitalWrite(inB1_3, HIGH);
    digitalWrite(inB2_3, LOW);
    digitalWrite(inA1_4, LOW);
    digitalWrite(inA2_4, HIGH);
    digitalWrite(inB1_4, HIGH);
    digitalWrite(inB2_4, LOW);
  }
  delay(stepDelay);
}
void step2(boolean x) {
  if (x) {
    Serial.println("Step 2");
    digitalWrite(inA1_1, LOW);
    digitalWrite(inA2_1, HIGH);
    digitalWrite(inB1_1, LOW);
    digitalWrite(inB2_1, HIGH);
    digitalWrite(inA1_2, LOW);  //Correct
    digitalWrite(inA2_2, HIGH); //Correct
    digitalWrite(inB1_2, LOW);  //Correct
    digitalWrite(inB2_2, HIGH); //Correct
  } else {
    digitalWrite(inA1_3, LOW);
    digitalWrite(inA2_3, HIGH);
    digitalWrite(inB1_3, LOW);
    digitalWrite(inB2_3, HIGH);
    digitalWrite(inA1_4, LOW);
    digitalWrite(inA2_4, HIGH);
    digitalWrite(inB1_4, LOW);
    digitalWrite(inB2_4, HIGH);
  }
  delay(stepDelay);
}
void step3(boolean x) {
  if (x) {
    Serial.println("Step 3");
    digitalWrite(inA1_1, HIGH);
    digitalWrite(inA2_1, LOW);
    digitalWrite(inB1_1, LOW);
    digitalWrite(inB2_1, HIGH);
    digitalWrite(inA1_2, HIGH); //Correct
    digitalWrite(inA2_2, LOW);  //Correct
    digitalWrite(inB1_2, LOW);  //Correct
    digitalWrite(inB2_2, HIGH); //Correct
  } else {
    digitalWrite(inA1_3, HIGH);
    digitalWrite(inA2_3, LOW);
    digitalWrite(inB1_3, LOW);
    digitalWrite(inB2_3, HIGH);
    digitalWrite(inA1_4, HIGH);
    digitalWrite(inA2_4, LOW);
    digitalWrite(inB1_4, LOW);
    digitalWrite(inB2_4, HIGH);
  }
  delay(stepDelay);
}
void step4(boolean x) {
  if (x) {
    Serial.println("Step 4");
    digitalWrite(inA1_1, HIGH);
    digitalWrite(inA2_1, LOW);
    digitalWrite(inB1_1, HIGH);
    digitalWrite(inB2_1, LOW);
    digitalWrite(inA1_2, HIGH); //Correct
    digitalWrite(inA2_2, LOW);  //Correct
    digitalWrite(inB1_2, HIGH); //Correct
    digitalWrite(inB2_2, LOW);  //Correct
  } else {
    digitalWrite(inA1_3, HIGH);
    digitalWrite(inA2_3, LOW);
    digitalWrite(inB1_3, HIGH);
    digitalWrite(inB2_3, LOW);
    digitalWrite(inA1_4, HIGH);
    digitalWrite(inA2_4, LOW);
    digitalWrite(inB1_4, HIGH);
    digitalWrite(inB2_4, LOW);
  }
  delay(stepDelay);
}

void stepFun(int *state, int mode) {
  Serial.print("Entering stepFun with state");
  Serial.println(*state);
  if (*state < 0) {
    Serial.println("State was ");
    Serial.println(*state);
    *state = 3;
  } else if (*state > 3) {
    Serial.print("State was ");
    Serial.println(*state);
    *state = 0;
  }
  switch (*state) {
    case 0: step1(settingX);
      if (mode) {
        (*state)++ % 4;
      } else {
        (*state)--;
        if (*state == -1) {
          *state = 3;
        }
      }
      break;
    case 1: step2(settingX);
      if (mode) {
        (*state)++ % 4;
      } else {
        (*state)--;
        if (*state == -1) {
          *state = 3;
        }
      }
      break;
    case 2: step3(settingX);
      if (mode) {
        (*state)++ % 4;
      } else {
        (*state)--;
        if (*state == -1) {
          *state = 3;
        }
      }
      break;
    case 3: step4(settingX);
      if (mode) {
        *state = ((*state) + 1) % 4;
      } else {
        (*state)--;
        if (*state == -1) {
          *state = 3;
        }
      }
      break;
  }
  smallRest();
}

void stopMotor() {
  digitalWrite(inA1_1, LOW);
  digitalWrite(inA2_1, LOW);
  digitalWrite(inB1_1, LOW);
  digitalWrite(inB2_1, LOW);
  digitalWrite(inA1_2, LOW);
  digitalWrite(inA2_2, LOW);
  digitalWrite(inB1_2, LOW);
  digitalWrite(inB2_2, LOW);
  digitalWrite(inA1_3, LOW);
  digitalWrite(inA2_3, LOW);
  digitalWrite(inB1_3, LOW);
  digitalWrite(inB2_3, LOW);
  digitalWrite(inA1_4, LOW);
  digitalWrite(inA2_4, LOW);
  digitalWrite(inB1_4, LOW);
  digitalWrite(inB2_4, LOW);
}

//Stop the motor for a set time between the 10 ms steps
void smallRest() {
  stopMotor();
  delay(5);
}

void rest() {
  stopMotor();
  delay(500);
}

// the loop routine runs over and over again forever:
void loop() {
  if (settingX) {
    moveToDegree(x, &currPosX, &stateX);
  } else {
    moveToDegree(y, &currPosY, &stateY);
  }
  settingX = !settingX;
  if (printValues) {
    Serial.print("Current test angle to turn is ");
    Serial.println(testInt);
  }
  //rest();
//  for (int i=0; i<=11; i++){ //1234, 1324, 1342 (fast), 1432 (slow jitter), 1243 (fast),
//    step1(settingX);
//    rest();
//    step2(settingX);
//    rest();
//    step3(settingX);
//    rest();
//    step4(settingX);
//    rest();
//  }
//  settingX = !settingX;
//  stopMotor();
//  delay(2000);
}


