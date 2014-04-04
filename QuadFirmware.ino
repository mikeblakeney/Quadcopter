#include <Wire.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "PIDController.h"
#include "PinChangeInt.h"

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

	pitchControl->setTunings(0.10, 0.00, 0.0);
	
	Serial.println("Arming motors");
	motorControl->arm_esc();
	
	Serial.println("Setting up interrups");
	init_interrupts();

	Serial.println("Ready.");
}

void loop()
{

		motorControl->setThrottle(throttle_sig);
		if(throttle_sig >= THROTTLE_SIG_MIN)
		{
	
			float *ypr = orientation->getOrientation(mpuInterrupt);

			pitch = ypr[1];
			mpuInterrupt = false;
			float pitch_g = pitchSignalToAngle(pitch_sig);
			float pitch_d = 0;
			pitchControl->compute(pitch, pitch_g, pitch_d);
			motorControl->setPitch(pitch_d);
	
			Serial.print(pitch_g);
			Serial.print('\t');
			Serial.print(pitch);
			Serial.print('\t');
			Serial.println(pitch_d);

			float roll_g = rollSignalToAngle(roll_sig);
	}

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
	PCintPort::attachInterrupt(MPU_INT_PIN, &updateMPUInterrupt, RISING);

	pinMode(THROTTLE_PIN, INPUT);
	PCintPort::attachInterrupt(THROTTLE_PIN, &updateThrottle, RISING);
	pinMode(ROLL_PIN, INPUT);
	PCintPort::attachInterrupt(ROLL_PIN, &updateRoll, RISING);
	pinMode(PITCH_PIN, INPUT);
	PCintPort::attachInterrupt(PITCH_PIN, &updatePitch, RISING);

	throttle_sig = 0;
	pitch_sig = (PITCH_SIG_MAX + PITCH_SIG_MIN) / 2;
	roll_sig = (ROLL_SIG_MAX + ROLL_SIG_MIN) / 2;
}

void updateMPUInterrupt()
{
	mpuInterrupt = true;
}

void updateThrottle()
{
	throttle_sig = pulseIn(THROTTLE_PIN, HIGH, 25000);		
}

void updateRoll()
{
	if(throttle_sig >= THROTTLE_SIG_MIN)
		roll_sig = pulseIn(ROLL_PIN, HIGH, 25000);
	else
		roll_sig = (ROLL_SIG_MAX + ROLL_SIG_MIN) / 2;
}

void updatePitch()
{
	if(throttle_sig >= THROTTLE_SIG_MIN)
		pitch_sig = pulseIn(PITCH_PIN, HIGH, 25000);
	else
		pitch_sig = (PITCH_SIG_MAX + PITCH_SIG_MIN) / 2;
}
