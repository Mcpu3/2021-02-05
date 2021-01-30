#include "Servo.h"
#include <limits.h>

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

// 失敗したときは負の数を返す
double get_dist() {
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);

	const unsigned long res = pulseIn(ECHO, HIGH);

	if (res < 0ul) {
		return -1.0;
	}

	return (double)res / 2.0 * 343.0 / 10000.0;
}

void loop() {
	drive();

	while (!is_black(PHOTO_LEFT) && !is_black(PHOTO_RIGHT)) {
		delay(1);
	}

	brake();

	const double dist_begin = get_dist();

	drive();

	while (is_black(PHOTO_LEFT) && is_black(PHOTO_RIGHT)) {
		delay(1);
	}

	brake();

	const double dist_end = get_dist();

	for (double i = dist_begin; i <= dist_end; i++) {
		digitalWrite(LED, HIGH);
		delay(500);
		digitalWrite(LED, LOW);
		delay(500);
	}

	delay(INT_MAX);
}