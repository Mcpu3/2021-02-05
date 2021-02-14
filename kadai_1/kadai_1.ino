#include <limits.h>
#include "Servo.h"

Servo servo;

class Buzzer
{
private:
	enum PIN
	{
		BUZZER = 6
	};

public:
	Buzzer()
	{
		pinMode(BUZZER, OUTPUT);
	}

	void play_melody1()
	{
		tone(PIN::BUZZER, 220, 100);
		delay(100);
		tone(PIN::BUZZER, 440, 100);
		delay(100);
		tone(PIN::BUZZER, 880, 300);
		delay(300);
	}

	void play_melody2()
	{
		tone(PIN::BUZZER, 440, 100);
		delay(100);
		tone(PIN::BUZZER, 880, 400);
		delay(400);
	}

	void play_melody3()
	{
		tone(PIN::BUZZER, 440, 400);
		delay(400);
		tone(PIN::BUZZER, 880, 100);
		delay(100);
	}

	void play_melody4()
	{
		tone(PIN::BUZZER, 880, 100);
		delay(100);
		tone(PIN::BUZZER, 440, 400);
		delay(400);
	}

	void play_melody5()
	{
		tone(PIN::BUZZER, 880, 400);
		delay(400);
		tone(PIN::BUZZER, 440, 100);
		delay(100);
	}

	void play_melody6()
	{
		tone(PIN::BUZZER, 880, 100);
		delay(100);
		tone(PIN::BUZZER, 440, 100);
		delay(100);
		tone(PIN::BUZZER, 200, 300);
		delay(300);
	}

	void play_melody7()
	{
		tone(PIN::BUZZER, 440, 1000);
		delay(1000);
	}
};

class LED
{
private:
	enum PIN
	{
		_LED = 10
	};

public:
	LED()
	{
		pinMode(PIN::_LED, OUTPUT);
	}

	void set_high()
	{
		digitalWrite(PIN::_LED, HIGH);
	}

	void set_low()
	{
		digitalWrite(PIN::_LED, LOW);
	}
};

class Motor
{
private:
	enum PIN
	{
		IN1_LEFT = 2,
		IN1_RIGHT = 7,
		IN2_LEFT = 4,
		IN2_RIGHT = 8,
		OUT_LEFT = 3,
		OUT_RIGHT = 5
	};

	enum STATE
	{
		BRAKE,
		DRIVE,
		LEFT,
		TURN_LEFT,
		RIGHT,
		TURN_RIGHT,
		REVERSE
	};

	STATE state;
	int speed_left, speed_right;

	void change_speed(const PIN pin, const int new_speed)
	{
		switch (pin)
		{
		case PIN::OUT_LEFT:
			speed_left = new_speed;
			analogWrite(PIN::OUT_LEFT, speed_left);
			break;
		case PIN::OUT_RIGHT:
			speed_right = new_speed;
			analogWrite(PIN::OUT_RIGHT, speed_right);
			break;
		}
	}

	void change_speed(const int new_speed)
	{
		speed_left = speed_right = new_speed;
		analogWrite(OUT_LEFT, new_speed);
		analogWrite(OUT_RIGHT, new_speed);
	}

public:
	Motor() : state(STATE::BRAKE), speed_left(0), speed_right(0)
	{
		pinMode(PIN::IN1_LEFT, OUTPUT);
		pinMode(PIN::IN1_RIGHT, OUTPUT);
		pinMode(PIN::IN2_LEFT, OUTPUT);
		pinMode(PIN::IN2_RIGHT, OUTPUT);
		pinMode(PIN::OUT_LEFT, OUTPUT);
		pinMode(PIN::OUT_RIGHT, OUTPUT);
	}

	void brake()
	{
		state = STATE::BRAKE;

		digitalWrite(PIN::IN1_LEFT, LOW);
		digitalWrite(PIN::IN1_RIGHT, LOW);
		digitalWrite(PIN::IN2_LEFT, LOW);
		digitalWrite(PIN::IN2_RIGHT, LOW);
	}

	void drive(const int new_speed)
	{
		state = STATE::DRIVE;

		speed_left = speed_right = new_speed;
		change_speed(new_speed);

		digitalWrite(PIN::IN1_LEFT, HIGH);
		digitalWrite(PIN::IN1_RIGHT, HIGH);
		digitalWrite(PIN::IN2_LEFT, LOW);
		digitalWrite(PIN::IN2_RIGHT, LOW);
	}

	void left(const int new_speed)
	{
		state = STATE::LEFT;

		speed_left = new_speed;
		change_speed(PIN::OUT_LEFT, speed_left);

		digitalWrite(PIN::IN1_LEFT, LOW);
		digitalWrite(PIN::IN1_RIGHT, HIGH);
		digitalWrite(PIN::IN2_LEFT, LOW);
		digitalWrite(PIN::IN2_RIGHT, LOW);
	}

	void turn_left(const int new_speed)
	{
		state = STATE::TURN_LEFT;

		speed_left = speed_right = new_speed;
		change_speed(new_speed);

		digitalWrite(PIN::IN1_LEFT, LOW);
		digitalWrite(PIN::IN1_RIGHT, HIGH);
		digitalWrite(PIN::IN2_LEFT, HIGH);
		digitalWrite(PIN::IN2_RIGHT, LOW);
	}

	void right(const int new_speed)
	{
		state = STATE::RIGHT;

		speed_right = new_speed;
		change_speed(PIN::OUT_RIGHT, speed_right);

		digitalWrite(PIN::IN1_LEFT, HIGH);
		digitalWrite(PIN::IN1_RIGHT, LOW);
		digitalWrite(PIN::IN2_LEFT, LOW);
		digitalWrite(PIN::IN2_RIGHT, LOW);
	}

	void turn_right(const int new_speed)
	{
		state = STATE::TURN_RIGHT;

		speed_left = speed_right = new_speed;
		change_speed(new_speed);
		digitalWrite(PIN::IN1_LEFT, HIGH);
		digitalWrite(PIN::IN1_RIGHT, LOW);
		digitalWrite(PIN::IN2_LEFT, LOW);
		digitalWrite(PIN::IN2_RIGHT, HIGH);
	}

	void reverse(const int new_speed)
	{
		state = STATE::REVERSE;

		speed_left = speed_right = new_speed;
		change_speed(new_speed);

		digitalWrite(PIN::IN1_LEFT, LOW);
		digitalWrite(PIN::IN1_RIGHT, LOW);
		digitalWrite(PIN::IN2_LEFT, HIGH);
		digitalWrite(PIN::IN2_RIGHT, HIGH);
	}

	void pause(const int pause_delay)
	{
		digitalWrite(PIN::IN1_LEFT, LOW);
		digitalWrite(PIN::IN1_RIGHT, LOW);
		digitalWrite(PIN::IN2_LEFT, LOW);
		digitalWrite(PIN::IN2_RIGHT, LOW);

		delay(pause_delay);

		switch (state)
		{
		case STATE::BRAKE:
			brake();
			break;
		case STATE::DRIVE:
			drive(min(speed_left, speed_right));
			break;
		case STATE::LEFT:
			left(speed_left);
			break;
		case STATE::TURN_LEFT:
			turn_left(speed_left);
			break;
		case STATE::RIGHT:
			right(speed_right);
			break;
		case STATE::TURN_RIGHT:
			turn_right(speed_right);
			break;
		case STATE::REVERSE:
			reverse(min(speed_left, speed_right));
			break;
		}
	}
};

class PhotoD
{
private:
	const int HIGH_VAL;

	enum PIN
	{
		_PHOTO_D = A2
	};

public:
	PhotoD() : HIGH_VAL(500) {}

	bool is_high()
	{
		if (analogRead(PIN::_PHOTO_D) > HIGH_VAL)
		{
			return true;
		}

		return false;
	}

	void print_debug()
	{
		Serial.println(analogRead(PIN::_PHOTO_D));
	}
};

class PhotoT
{
private:
	const int BLACK_VAL;

	enum PIN
	{
		LEFT = A0,
		RIGHT = A1
	};

public:
	PhotoT() : BLACK_VAL(500) {}

	bool is_black_left()
	{
		if (analogRead(PIN::LEFT) > 500)
		{
			return true;
		}

		return false;
	}

	bool is_black_right()
	{
		if (analogRead(PIN::RIGHT) > 500)
		{
			return true;
		}

		return false;
	}

	bool is_black()
	{
		return is_black_left() && is_black_right();
	}

	void print_debug()
	{
		Serial.print("Left: ");
		Serial.print(analogRead(PIN::LEFT));
		Serial.print(", ");
		Serial.print("RIGHT: ");
		Serial.println(analogRead(PIN::RIGHT));
	}
};

class Serv
{
public:
	enum PIN
	{
		_SERVO = 11,
		TRIG = 12,
		ECHO = 13
	};

	int current_servo;
	const double DIST_VAL_FRONT, DIST_VAL_SIDE;

public:
	Serv() : DIST_VAL_FRONT(6.0), DIST_VAL_SIDE(8.0)
	{
		pinMode(PIN::TRIG, OUTPUT);
		pinMode(PIN::ECHO, INPUT);
		set_servo_front();
	}

	int get_servo_pin()
	{
		return PIN::_SERVO;
	}

	double get_DIST_VAL_FRONT()
	{
		return DIST_VAL_FRONT;
	}

	double get_DIST_VAL_SIDE()
	{
		return DIST_VAL_SIDE;
	}

	void set_servo_front()
	{
		current_servo = 90;
		servo.write(current_servo);
	}

	void set_servo_left()
	{
		current_servo = 180;
		servo.write(current_servo);
	}

	void set_servo_right()
	{
		current_servo = 0;
		servo.write(current_servo);
	}

	double get_dist()
	{
		digitalWrite(PIN::TRIG, HIGH);
		delayMicroseconds(10);
		digitalWrite(PIN::TRIG, LOW);

		const unsigned long res = pulseIn(PIN::ECHO, HIGH);

		if (res < 0ul)
		{
			return get_dist();
		}

		return (double)res / 2.0 * 343.0 / 10000.0;
	}

	void print_debug()
	{
		Serial.print("Dist: ");
		Serial.print(get_dist());
		Serial.println("cm");
	}
};

Buzzer buzzer;
LED led;
Motor motor;
PhotoD photo_d;
PhotoT photo_t;
Serv serv;

void setup()
{
	pinMode(serv.get_servo_pin(), OUTPUT);
	servo.attach(serv.get_servo_pin(), 500, 2400);

	Serial.begin(9600);
	delay(5000);
}

void kadai_1()
{
	if (photo_t.is_black())
	{
		motor.drive(64);
	}
	else if (photo_t.is_black_left())
	{
		motor.left(92);
	}
	else if (photo_t.is_black_right())
	{
		motor.right(92);
	}
	else
	{
		motor.reverse(64);
	}
}

void kadai_2() {
	if (photo_d.is_high()) {
		led.set_high();
		buzzer.play_melody1();

		while (true) {
			delay(INT_MAX);
		}
	}
}

void loop()
{
	kadai_1();
	kadai_2();
}
