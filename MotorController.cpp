#include "MotorController.h"
#include "defines.h"
MotorController::MotorController(int pinA, int pinB, int pinC, int pinD)
{
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(pinC, OUTPUT);
	pinMode(pinD, OUTPUT);

	motorA = Servo();
	motorB = Servo();
	motorC = Servo();
	motorD = Servo();


	motorA.attach(pinA);
	motorB.attach(pinB);
	motorC.attach(pinC);
	motorD.attach(pinD);

	motorA_speed = 0;
	motorB_speed = 0;
	motorC_speed = 0;
	motorD_speed = 0;
}


void MotorController::arm_esc()
{ 
	 motorA.writeMicroseconds(THROTTLE_MIN);
	 motorB.writeMicroseconds(THROTTLE_MIN);
	 motorC.writeMicroseconds(THROTTLE_MIN);
	 motorD.writeMicroseconds(THROTTLE_MIN); 
	 delay(1000);
}

void MotorController::stop_motors()
{
	this->setThrottle(0);
}


void MotorController::setThrottle(int throttle_sig)
{
	int throttle = map(throttle_sig, THROTTLE_SIG_MIN, THROTTLE_SIG_MAX, THROTTLE_MIN, THROTTLE_MAX);
	
	motorA_speed = throttle;
	motorB_speed = throttle;
	motorC_speed = throttle;
	motorD_speed = throttle;



	this->updateMotorSpeeds();
}

void MotorController::setPitch(float pitch_d)
{
	pitch_d = (pitch_d / ( PITCH_MAX - PITCH_MIN) ) * (PITCH_SIG_MAX - PITCH_SIG_MIN);
	motorA_speed = motorA_speed + pitch_d;
	motorC_speed = motorC_speed - pitch_d;
	
	motorA_speed = this->ensureSpeed(motorA_speed);
	motorC_speed = this->ensureSpeed(motorC_speed);

/*
	Serial.print('\t');
	 Serial.print(pitch_d);
	
		Serial.print('\t');
		Serial.print(motorA_speed);
		Serial.print('\t');
		Serial.println(motorC_speed);
	*/
	this->updateMotorSpeeds();
}

int MotorController::ensureSpeed(int speed)
{
	if(speed >= THROTTLE_MAX)
		return THROTTLE_MAX;
	else if(speed <= THROTTLE_MIN)
		return THROTTLE_MIN;
	else
		return speed;
}

void MotorController::updateMotorSpeeds()
{
	motorA.writeMicroseconds(motorA_speed);
	motorB.writeMicroseconds(motorB_speed);
	motorC.writeMicroseconds(motorC_speed);
	motorD.writeMicroseconds(motorD_speed);
}