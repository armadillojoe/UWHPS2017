#include <Wire.h>
#include <QueueList.h>

#define SLAVE_ADDRESS 0x06

QueueList<byte> byteQue;
int state = 0;
float currPos = 0;
int testInt = 0;
byte newByte = 0;

#define inA1_1 0 // input 1 of the 1st stepper
#define inA2_1 1 // input 2 of the 1st stepper
#define inB1_1 2 // input 3 of the 1st stepper
#define inB2_1 3 // input 4 of the 1st stepper
#define inA1_2 4 // input 1 of the 2nd stepper
#define inA2_2 5 // input 2 of the 2nd stepper
#define inB1_2 6 // input 3 of the 2nd stepper
#define inB2_2 7 // input 4 of the 2nd stepper

int stepDelay = 25; // Delay between steps in milliseconds

void setup() {
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);                
  pinMode(inA1_1, OUTPUT);     
  pinMode(inA2_1, OUTPUT);     
  pinMode(inB1_1, OUTPUT);     
  pinMode(inB2_1, OUTPUT);     
  pinMode(inA1_2, OUTPUT);     
  pinMode(inA2_2, OUTPUT);     
  pinMode(inB1_2, OUTPUT);     
  pinMode(inB2_2, OUTPUT);     
}

int degreeToStep(int degree) {
  return (int)(degree * 27 * 2 / 1.8);
}

void moveToDegree(int degree) {
  float displacement = (float)degree - currPos;
  if (displacement > 0) {
    stepFun(&state, 1);
    currPos += 1.8 / (27*2);
  } else {
    stepFun(&state, 0);
    currPos -= 1.8 / (27*2);
  }
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
    moveToDegree(testInt);
  }
}

void step1() {
  digitalWrite(inA1_1, LOW);   
  digitalWrite(inA2_1, HIGH);   
  digitalWrite(inB1_1, HIGH);   
  digitalWrite(inB2_1, LOW);   
  digitalWrite(inA1_2, LOW);   
  digitalWrite(inA2_2, HIGH);   
  digitalWrite(inB1_2, HIGH);   
  digitalWrite(inB2_2, LOW);   
  delay(stepDelay);
  
}
void step2() {
  digitalWrite(inA1_1, LOW);   
  digitalWrite(inA2_1, HIGH);   
  digitalWrite(inB1_1, LOW);   
  digitalWrite(inB2_1, HIGH);   
  digitalWrite(inA1_2, LOW);   
  digitalWrite(inA2_2, HIGH);   
  digitalWrite(inB1_2, LOW);   
  digitalWrite(inB2_2, HIGH);   
  delay(stepDelay);
}
void step3() {
  digitalWrite(inA1_1, HIGH);   
  digitalWrite(inA2_1, LOW);   
  digitalWrite(inB1_1, LOW);   
  digitalWrite(inB2_1, HIGH);   
  digitalWrite(inA1_2, HIGH);   
  digitalWrite(inA2_2, LOW);   
  digitalWrite(inB1_2, LOW);   
  digitalWrite(inB2_2, HIGH);   
  delay(stepDelay);
}
void step4() {
  digitalWrite(inA1_1, HIGH);   
  digitalWrite(inA2_1, LOW);   
  digitalWrite(inB1_1, HIGH);   
  digitalWrite(inB2_1, LOW);   
  digitalWrite(inA1_2, HIGH);   
  digitalWrite(inA2_2, LOW);   
  digitalWrite(inB1_2, HIGH);   
  digitalWrite(inB2_2, LOW);   
  delay(stepDelay);
}

void stepFun(int *state, int mode) {
  switch (*state) {
    case 0: step1(); 
    if (mode) {
      (*state)++ % 4; 
    } else {
      (*state)--;
      if (*state == -1) {
        *state = 3;
      }
    }
    break;
    case 1: step2(); 
    if (mode) {
      (*state)++ % 4; 
    } else {
      (*state)--;
      if (*state == -1) {
        *state = 3;
      }
    } 
    break;
    case 2: step3();
    if (mode) {
      (*state)++ % 4; 
    } else {
      (*state)--;
      if (*state == -1) {
        *state = 3;
      }
    }
    break;
    case 3: step4();
    if (mode) {
      (*state)++ % 4; 
    } else {
      (*state)--;
      if (*state == -1) {
        *state = 3;
      }
    }
    break;
  }
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
}

void rest() {
  stopMotor();
  delay(1000);
}

// the loop rout1ine runs over and over aga1in forever:
void loop() {
  moveToDegree(testInt);
//  for (int i=0; i<=11; i++){ 
//    step1();
//    rest(); 
//    step2();
//    rest();
//    step3();
//    rest();
//    step4();
//    rest();
//  }
//  stopMotor();
//  delay(2000);
//  
//  for (int i=0; i<=11; i++){
//    step3();
//    step2();
//    step1();
//    step4(); 
//  }
//  
//  stopMotor();
//  delay(2000);
}


