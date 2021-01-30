#include "Servo.h"

Servo servo;
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
}

void drive() {
	digitalWrite(MOTOR_LEFT_IN1, HIGH);
	digitalWrite(MOTOR_LEFT_IN2, LOW);
	digitalWrite(MOTOR_RIGHT_IN1, HIGH);
	digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

void left() {
	digitalWrite(MOTOR_LEFT_IN1, LOW);
	digitalWrite(MOTOR_LEFT_IN2, LOW);
	digitalWrite(MOTOR_RIGHT_IN1, HIGH);
	digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

void right() {
	digitalWrite(MOTOR_LEFT_IN1, HIGH);
	digitalWrite(MOTOR_LEFT_IN2, LOW);
	digitalWrite(MOTOR_RIGHT_IN1, LOW);
	digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

void reverse() {
	digitalWrite(MOTOR_LEFT_IN1, LOW);
	digitalWrite(MOTOR_LEFT_IN2, HIGH);
	digitalWrite(MOTOR_RIGHT_IN1, LOW);
	digitalWrite(MOTOR_RIGHT_IN2, HIGH);
}

void brake() {
	digitalWrite(MOTOR_LEFT_IN1, LOW);
	digitalWrite(MOTOR_LEFT_IN2, LOW);
	digitalWrite(MOTOR_RIGHT_IN1, LOW);
	digitalWrite(MOTOR_RIGHT_IN2, LOW);
}

bool is_black(const int PHOTO) {
	if (analogRead(PHOTO) > 500) {
		return true;
	}

	return false;
}

void loop() {
	Serial.print("Left: ");
	Serial.print(analogRead(PHOTO_LEFT));
	Serial.print(", ");
	Serial.print("Right: ");
	Serial.print(analogRead(PHOTO_RIGHT));
	Serial.println();

	if (is_black(PHOTO_LEFT) && !is_black(PHOTO_RIGHT)) {
		left();
	}

	else if (!is_black(PHOTO_LEFT) && is_black(PHOTO_RIGHT)) {
		right();
	}

	else {
		drive();
	}

	delay(5);
}