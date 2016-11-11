//Class that controls position of server

#include<iostream>
using namespace std;

class Servo {
	public:
		Servo(); //Constructor
		~Servo(); //Destructor
		void turnTo (int);
		int currentAngle();
		void autoAdjust();
		
};

//Returns current angle of the servo
int Servo::currentAngle () {
	return 0;
}

//Turns servo motor to specified angle
void Servo::turnTo (int angle) {

}


void Servo::autoAdjust() {

}

int main() {
	return 0;
}