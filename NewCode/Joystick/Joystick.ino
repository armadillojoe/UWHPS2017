// Joystick Controller Slave
// by Tremaine Ng
// based on code from Nicholas Zambetti at https://www.arduino.cc/en/Tutorial/MasterReader
// Collects, processes, then sends data on request to master servo Arduino
// x position, y position, z twist, b button
// reading rate limited by analog read rate ~10,000 times per second

//NOTE: Black wire is center of twist potentiometer, red and white are ends (z-axis).
#include <Wire.h>

#define POT_RESISTOR    4500 // resistance values of resistors connected to 4.5k potentiometer
#define VOLTAGE_BATTERY 12 // voltage level of battery connected to joystick
#define SLAVE_ADRX 8
#define DEBUG 0
#define DEBUG_JOYSTICK 1

int Ax, Ay, Az, Db;

void setup() {
  if (DEBUG || DEBUG_JOYSTICK) {
    Serial.begin(9600);
  }
  Wire.begin(SLAVE_ADRX); // join i2c bus with slave address
  Wire.onRequest(requestEvent); // register event
  // initialize x center, y center, z center on startup
  Ax = 0;
  Ay = 0;
  Az = 0;
  Db = 0;
}

void loop() {
  //
  // Ensure that analog pins are NOT utilized by the Serial port or I2C!!!
  // Should be good with definition that they are not digital 0, 1
  //
  Ax = analogRead(A0);   // read x value from analog pin 0
  Ay = analogRead(A1);   // read y value from analog pin 1
  Az = analogRead(A2);   // read z value from analog pin 2
  Db = digitalRead(2);  // read b value from digital pin 2
  if (DEBUG_JOYSTICK) {
    printJoystickStatus(Ax, Ay, Az, Db);
  }
}

// Send data when requested by master
void requestEvent() {
  // send 4 byte signal in order: 1 byte x, 1 byte y, 1 byte z, 1 byte b
  //Analog read int is 10 bits in length
  //Bitshift right by 2, i2c data transmitted is 8 bits
  unsigned char sendX = Ax >> 2;
  unsigned char sendY = Ay >> 2;
  unsigned char sendZ = Az >> 2;
  unsigned char sendB = Db >> 2;
  byte toSend[] = {sendX, sendY, sendZ, sendB};
  Wire.write(toSend, 4);
  //Print status of the transmitted bytes after the I2C transmission has been completed to prevent timeout
  //Only print during debug status during actual run, reduced computation time per transmission -> faster motor response
  if (DEBUG) {
    printTransmission(toSend);
  }
}

void printTransmission(byte toSend[]) {
  Serial.println("I2C transmission completed with following results:");
  for (int i = 0; i < 4; i++) {
    Serial.print("Value num. ");
    Serial.print(i + 1, DEC);
    Serial.print(" has int, char values of: ");
    Serial.print((unsigned int) toSend[i]);
    Serial.print(", ");
    Serial.println((unsigned char) toSend[i]);
  }
}

void printJoystickStatus(int Ax, int Ay, int Az, int Db) {
  Serial.print("Status of joystick - X: ");
  Serial.print(Ax);
  Serial.print(" Y: ");
  Serial.print(Ay);
  Serial.print(" Z: ");
  Serial.print(Az);
  Serial.print(" Button: ");
  Serial.println(Db);
}

