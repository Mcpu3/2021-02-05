#include "Servo.h"

Servo servo;
const int MOTOR_LEFT_IN1 = 2;
const int MOTOR_LEFT_IN2 = 4;
const int MOTOR_RIGHT_IN1 = 7;
const int MOTOR_RIGHT_IN2 = 8;
const int MOTOR_LEFT_OUT = 3;
const int MOTOR_RIGHT_OUT = 5;
const int PHOTO_LEFT = A0;
const int PHOTO_LEFT = A1;
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
	left();
	set_servo_left();
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

void set_servo_front() {
	servo.write(90);
}

void set_servo_left() {
	servo.write(135);
}

void set_servo_right() {
	servo.write(45);
}

bool is_wall() {
	const double dist = get_dist();

	if (dist < 0.0) {
		return is_wall();
	}

	if (dist < 13.0) {
		return true;
	}

	return false;
}

bool is_left = true;

void loop() {
	Serial.print("Dist: ");
	Serial.print(get_dist());
	Serial.print("cm");
	Serial.println();

	if (is_wall()) {
		reverse();
		delay(5);
		brake();

		if (is_left) {
			set_servo_right();
		}
		else {
			set_servo_left();
		}

		is_left = !is_left;

		delay(500);
	}
	else {
		if (is_left) {
			left();
		}
		else {
			right();
		}

		delay(5);
	}
}