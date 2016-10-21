#include <QueueList.h>

#include <Wire.h>

#define SLAVE_ADDRESS 0x04
byte newByte = 0;
int testInt = 0;
float testFloat = 0.0;
boolean testBool = true;

QueueList<byte> byteQue;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

void loop() {
  delay(100);
}

// callback for received data
/*void receiveData(int byteCount){
  while(Wire.available()) {
    number = Wire.read();
    intQue.push(number);
  }
  if (intQue.count() == 2)
  {
    int num = intQue.pop();
    num = num << 8;
    num += intQue.pop();
    number = num;
    Serial.print("data received: ");
    Serial.println(number);
  }
}*/

void receiveData(int byteCount) {
  int reg = Wire.read();
  
  switch (reg) {
    case 0x01: readInt(); break;
    case 0x02: readFloat(); break;
    case 0x03: readBool(); break;
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
  }
}

void readFloat() {
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
  }
}

void readBool() {
}

// callback for sending data
void sendData(){
  if (byteQue.isEmpty())
  {
    int front = testInt >> 8;
    int back = testInt;
    byteQue.push(front);
    byteQue.push(back);
  }
  if (byteQue.count() > 0) 
  {
    Wire.write(byteQue.pop());
  }
}

/*void sendData() {
  int reg = Wire.read();
  Serial.println(reg);
  switch (reg) {
    case 0x01: Serial.println("int"); sendInt(); break;
    case 0x02: sendFloat(); break;
    case 0x03: sendBool(); break;
  }
}*/

void sendInt() {
  byte ints[2];
  byte* one = (byte*)&testInt;
  byte* two = one + 1;
  Serial.println(*one);
  Serial.println(*two);
  ints[1] = *one;
  ints[0] = *two;
  Wire.write(ints, 2);
  /*if (byteQue.isEmpty())
  {
    byte* front = ((byte*)&testInt) + 1;
    byte* back = (byte*)&testInt;
    Serial.println(*back);
    Serial.println(*front);
    byteQue.push(*front);
    byteQue.push(*back);
  }
  if (byteQue.count() > 0) 
  {
    Wire.write(byteQue.pop());
  }*/
}

void sendFloat() {
}

void sendBool() {
}
