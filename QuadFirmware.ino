#include <Wire.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "PIDController.h"

#include "Servo.h"
#include "MotorController.h"
#include "OrientationSensor.h"

#include "defines.h"

OrientationSensor *orientation;
MotorController *motorControl;
PIDController *pitchControl;
PIDController *rollControl;

double pitch;
double roll;

int throttle_sig = 0;

int roll_sig = 0;
int pitch_sig = 0;

long lastTime = millis();

bool mpuInterrupt = false;

void setup()
{
	Serial.begin(115200);
	
	Serial.println("Setting up sensors");
	orientation = new OrientationSensor();

	motorControl = new MotorController(MOTOR_A, MOTOR_B, MOTOR_C, MOTOR_D);

	pitchControl = new PIDController();
	rollControl = new PIDController();

	pitchControl->setTunings(100.0, 0.01, 0.01);
	
	Serial.println("Arming motors");
	motorControl->arm_esc();
	
	Serial.println("Setting up interrups");
	init_interrupts();

	Serial.println("Ready.");
}

void loop()
{
	while(!mpuInterrupt && !orientation->extraPackets())
	{
		long now = millis();
		if( (now - lastTime) >= CYCLE_TIME)
		{
			int newThrottleSig = pulseIn(THROTTLE_PIN, HIGH, 11000);
			if(newThrottleSig > 0)
			{
				throttle_sig = newThrottleSig;
			}

			int newRollSig = pulseIn(ROLL_PIN, HIGH, 11000);
			if(newRollSig > 0)
			{
				roll_sig = newRollSig;
			}

			int newPitchSig = pulseIn(PITCH_PIN, HIGH, 11000);
			if(newPitchSig > 0)
			{
				pitch_sig = newPitchSig;
			}
			lastTime = now;
		}
		
		float pitch_g = pitchSignalToAngle(pitch_sig);
		double pitch_d = 0;
		
		
		pitchControl->compute(pitch, pitch_g, pitch_d);
		
		if(throttle_sig >= THROTTLE_SIG_MIN)
		{
			motorControl->setThrottle(throttle_sig);
			motorControl->setPitch(pitch_d);
		}
		
		float roll_g = rollSignalToAngle(roll_sig);
		
		/*
		Serial.print(pitch_g);
		Serial.print('\t');
		Serial.println(pitch);
		*/

		
		//Serial.println(throttle_sig);
		//Serial.println(roll_sig);
		//Serial.println(pitch_sig);
	}

	float *ypr = orientation->getOrientation(mpuInterrupt);
	pitch = ypr[1];
	mpuInterrupt = false;
}


float rollSignalToAngle(float aSignal)
{
	float angle = map(aSignal, ROLL_SIG_MIN, ROLL_SIG_MAX, ROLL_MAX, ROLL_MIN);
	return angle;
}

float pitchSignalToAngle(float aSignal)
{
	float angle = map(aSignal, PITCH_SIG_MIN, PITCH_SIG_MAX, PITCH_MAX, PITCH_MIN);
	return angle;
}

void init_interrupts()
{
	
    Serial.println(F("Waiting for first MPU interrupt..."));
	pinMode(MPU_INT_PIN, INPUT);
	attachInterrupt(0, &updateMPUInterrupt, RISING);

	throttle_sig = 0;
	pitch_sig = (PITCH_SIG_MAX + PITCH_SIG_MIN) / 2;
	roll_sig = (ROLL_SIG_MAX + ROLL_SIG_MIN) / 2;
}

void updateMPUInterrupt()
{
	mpuInterrupt = true;
}


