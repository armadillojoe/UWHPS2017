#include <Servo.h>

const int servoPin = 9;
const int buttonPin = 2;

int buttonState = 0;
int is_Pressed = 0;

int currentState = -1;

Servo spaceDicks42069;

void setup() {
  // put your setup code here, to run once:

  //Button to move servo motor
  pinMode(buttonPin, INPUT);
  //Verify frequency of servo is 50 Hz
  spaceDicks42069.attach(servoPin, 860, 2330);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);

  if (is_Pressed == 0 && buttonState == 1) {
    is_Pressed = 1;
    currentState = (currentState + 1) % 2;
  } else if (is_Pressed == 1 && buttonState == 0) {
    is_Pressed = 0;
  }
  if (currentState == 0) {
    //Verify given PWM widths map to 0 and 180 degrees
    spaceDicks42069.write(0);
  } else if (currentState == 1) {
    spaceDicks42069.write(180);
  }
  Serial.println(spaceDicks42069.read());
}
