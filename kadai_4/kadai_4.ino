#include "Servo.h"

const int MOTOR_LEFT_IN1 = 2;
const int MOTOR_LEFT_IN2 = 4;
const int MOTOR_RIGHT_IN1 = 7;
const int MOTOR_RIGHT_IN2 = 8;
const int MOTOR_LEFT_OUT = 3;
const int MOTOR_RIGHT_OUT = 5;
const int PHOTO_LEFT = A0;
const int PHOTO_RIGHT = A1;
const int PHOTO = A2;
const int SERVO = 11;
const int TRIG = 12;
const int ECHO = 13;
const int LED = 10;

void setup() {
	delay(5000);
	Serial.begin(9600);
	pinMode(MOTOR_LEFT_IN1, OUTPUT);
	pinMode(MOTOR_LEFT_IN2, OUTPUT);
	pinMode(MOTOR_RIGHT_IN1, OUTPUT);
	pinMode(MOTOR_RIGHT_IN2, OUTPUT);
	pinMode(MOTOR_LEFT_OUT, OUTPUT);
	pinMode(MOTOR_RIGHT_OUT, OUTPUT);
	pinMode(SERVO, OUTPUT);
	servo.attach(SERVO, 500, 2400);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);

	analogWrite(MOTOR_LEFT_OUT, 96);
	analogWrite(MOTOR_RIGHT_OUT, 96);
	drive();
	digitalWrite(LED, HIGH);
}

bool is_high() {
	if (analogRead(PHOTO) > x) {
		return true;
	}

	return false;
}

void loop() {
	Serial.println(analogRead(PHOTO));

	if (is_high()) {
		digitalWrite(LED, LOW);
	}
}
