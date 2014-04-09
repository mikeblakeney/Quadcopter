#include <Wire.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "PIDController.h"

#include "Servo.h"
#include "MotorController.h"
#include "OrientationSensor.h"
#include "VoltageSensor.h"

#include <PinChangeInt.h>

#include "RCChannel.h"

#include "defines.h"

OrientationSensor *orientation;
VoltageSensor *voltageSensor;
MotorController *motorControl;
PIDController *pitchControl;
PIDController *rollControl;

float pitch;
double pitch_d = 0;
int throttle_sig = 0;
int pitch_sig = 0;

bool mpuInterrupt = false;

RCChannel *throttle;

void setup()
{
	Serial.begin(115200);
	
	Serial.println("Setting up sensors");
	voltageSensor = new VoltageSensor(VOLTAGE_SENSOR_PIN, V_ALERT_PIN);
	orientation = new OrientationSensor();
	motorControl = new MotorController(MOTOR_A, MOTOR_B, MOTOR_C, MOTOR_D);
	
	Serial.println("Arming motors");
	motorControl->arm_esc();
	
	Serial.println("Settinp up stabilization and control");
	pitchControl = new PIDController(0.92, 0.0, 0.00);
	rollControl = new PIDController();

	pitchControl->setTiming(0);

	Serial.println("Setting up interrups");
	throttle = new RCChannel();
	init_interrupts();

	Serial.println("Ready.");
}

void loop()
{
	voltageSensor->checkLevel();
	
	throttle_sig = throttle->read();

	suspend_interrupts();
	
	while(!mpuInterrupt && !orientation->extraPackets())
	{}
	
	ypr_t ypr = orientation->getOrientation(mpuInterrupt);
	if(!ypr.error)
	{	
		pitch = ypr.pitch;
	}else if(ypr.error == 1) {
		Serial.println("MPU packet size error");
	}else if(ypr.error == 2) {
		Serial.println("FIFO overflow");
	}

	resume_interrupts();

	Serial.print(throttle_sig);
	Serial.print('\t');
	Serial.println(pitch);

	float pitch_g = pitchSignalToAngle(pitch_sig);
		
	pitchControl->compute(pitch, pitch_g, pitch_d);
	//Serial.println(pitch_d);

	motorControl->setThrottle(throttle_sig);
	motorControl->setPitch(pitch_d);
	//Serial.println(pitch);
	
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
	
    Serial.println(F("MPU interrupt initialized"));
	pinMode(MPU_INT_PIN, INPUT);
	attachInterrupt(0, &updateMPUInterrupt, RISING);


    Serial.println(F("Throttle interrupt initialized"));
	pinMode(THROTTLE_PIN, INPUT);
	digitalWrite(THROTTLE_PIN, LOW);
	PCintPort::attachInterrupt(THROTTLE_PIN, &throttleInt, CHANGE);


	throttle_sig = 0;
	pitch_sig = (PITCH_SIG_MAX + PITCH_SIG_MIN) / 2;
	//roll_sig = (ROLL_SIG_MAX + ROLL_SIG_MIN) / 2;
}

void suspend_interrupts()
{
	PCintPort::detachInterrupt(THROTTLE_PIN);
}

void resume_interrupts()
{
	PCintPort::attachInterrupt(THROTTLE_PIN, &throttleInt, CHANGE);
}

void updateMPUInterrupt()
{
	mpuInterrupt = true;
}

void throttleInt()
{
	if(PCintPort::pinState == HIGH)
	{
		throttle->risingInterrupt();
	}else {
		throttle->fallingInterrupt();
	}
	
}



