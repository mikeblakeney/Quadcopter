#include "OrientationSensor.h"

#include "defines.h"

OrientationSensor::OrientationSensor()
{
	Wire.begin();
    TWBR = 24; 

 	Serial.println(F("Initializing MPU..."));
	mpu.initialize();

	Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

	// load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    uint8_t devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    if(devStatus == 0)
    {
    	Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
		packetSize = mpu.dmpGetFIFOPacketSize();
    }

	ypr[0] = 0;
	ypr[1] = 0;
	ypr[2] = 0;

	rawSum.yaw = 0;
	rawSum.pitch = 0;
	rawSum.roll = 0;

	rawIndex = 0;

	for(int i=0; i<MPU_ARRAY_SIZE; i++)
	{
		rawOrientation[i].yaw = 0;
		rawOrientation[i].pitch = 0;
		rawOrientation[i].roll = 0;
	}

}

float* OrientationSensor::getOrientation(bool mpuInterrupt)
{
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	
	if(!mpuInterrupt && fifoCount < packetSize)
	{
		return ypr;
	} else {
		mpuIntStatus = mpu.getIntStatus();

    	// get current FIFO count
    	fifoCount = mpu.getFIFOCount();

    	// check for overflow (this should never happen unless our code is too inefficient)
	    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
	        // reset so we can continue cleanly
	        mpu.resetFIFO();
	        Serial.println(F("FIFO overflow!"));

	        return ypr;

	    // otherwise, check for DMP data ready interrupt (this should happen frequently)
	    } else if (mpuIntStatus & 0x02) {
	        // wait for correct available data length, should be a VERY short wait
	        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

	        // read a packet from FIFO
	        mpu.getFIFOBytes(fifoBuffer, packetSize);
	        
	        // track FIFO count here in case there is > 1 packet available
	        // (this lets us immediately read more without waiting for an interrupt)
	        fifoCount -= packetSize;

	       float rawYPR[3]; 
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(rawYPR, &q, &gravity);
           
           /*
	            Serial.print("ypr\t");
	            Serial.print(ypr[0] * 180/M_PI);
	            Serial.print("\t");
	            Serial.print(ypr[1] * 180/M_PI);
	            Serial.print("\t");
	            Serial.println(ypr[2] * 180/M_PI);
			*/

	        /*
	        rawYPR[0] = rawYPR[0] * 180/M_PI;
	        rawYPR[1] = rawYPR[1] * 180/M_PI;
	        rawYPR[2] = rawYPR[2] * 180/M_PI;
			*/
	        ypr[0] = rawYPR[0] * 180/M_PI;
	        ypr[1] = rawYPR[1] * 180/M_PI;
	        ypr[2] = rawYPR[2] * 180/M_PI;

	       // filterOrientation(rawYPR[0], rawYPR[1], rawYPR[2]);
	       
	        return ypr;
	     }
        
	}
}

bool OrientationSensor::extraPackets()
{
	return (fifoCount >= packetSize);
}

void OrientationSensor::filterOrientation(float rawYaw, float rawPitch, float rawRoll)
{
	rawSum.pitch = rawSum.pitch - rawOrientation[rawIndex].pitch;
	rawOrientation[rawIndex].pitch = rawPitch;
	rawSum.pitch += rawOrientation[rawIndex].pitch;

	rawIndex++;
	if(rawIndex >= MPU_ARRAY_SIZE) rawIndex = 0;

	ypr[1] = rawSum.pitch/MPU_ARRAY_SIZE;
}

