//
//  MotorController.h
//  QuadFirmware
//
//  Created by Michael J Blakeney on 2/17/14.
//  Copyright (c) 2014 Michael Blakeney. All rights reserved.
//


#ifndef QuadFirmware_MotorController_h
#define QuadFirmware_MotorController_h

#include <arduino.h>
#include "Servo.h"



class MotorController
{
public:
	MotorController(int pinA, int pinB, int pinC, int pinD);

	void stop_motors();
	void arm_esc();

	void setThrottle(int throttle);

	void setPitch(float pitch_d);
	void setRoll(float roll_d);

private:
	void updateMotorSpeeds();
	int ensureSpeed(int speed);

	Servo motorA;
	Servo motorB;
	Servo motorC;
	Servo motorD;

	int motorA_speed;
	int motorB_speed;
	int motorC_speed;
	int motorD_speed;

};




#endif
