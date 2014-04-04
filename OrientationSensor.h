
#ifndef ORIENTATIONSENSOR_H
#define ORIENTATIONSENSOR_H

#include "defines.h"
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

class OrientationSensor
{
public:
	OrientationSensor();

	float* getOrientation(bool mpuInterrupt);
	

private:
	MPU6050 mpu;
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

	float pitchAngle;


	float getAccelPitch();
	float getGyroPitch();
	void filterOrientation(float rawYaw, float rawPitch, float rawRoll);
};

#endif