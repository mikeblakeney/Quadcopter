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
}

ypr_t OrientationSensor::getOrientation(bool mpuInterrupt)
{
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	ypr_t curYPR;

	if(!mpuInterrupt && fifoCount < packetSize)
	{
		curYPR.error = 1;
		return curYPR;
	} else {
		mpuIntStatus = mpu.getIntStatus();

    	// get current FIFO count
    	fifoCount = mpu.getFIFOCount();

    	// check for overflow (this should never happen unless our code is too inefficient)
	    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
	        // reset so we can continue cleanly
	        mpu.resetFIFO();
	        curYPR.error = 2;
	        return curYPR;

	    // otherwise, check for DMP data ready interrupt (this should happen frequently)
	    } else if (mpuIntStatus & 0x02) {
	        // wait for correct available data length, should be a VERY short wait
	        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

	        // read a packet from FIFO
	        mpu.getFIFOBytes(fifoBuffer, packetSize);
	        
	        // track FIFO count here in case there is > 1 packet available
	        // (this lets us immediately read more without waiting for an interrupt)
	        fifoCount -= packetSize;
	        float ypr[3];
	        
	        
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
           
           /*
	        Serial.print("ypr\t");
	        Serial.print(ypr[0] * 180/M_PI);
	        Serial.print("\t");
	        Serial.print(ypr[1] * 180/M_PI);
	        Serial.print("\t");
	        Serial.println(ypr[2] * 180/M_PI);
			*/

	        
	        curYPR.yaw = ypr[0] * 180/M_PI;
	        curYPR.pitch = ypr[1] * 180/M_PI;
	        curYPR.roll = ypr[2] * 180/M_PI;	        
	        curYPR.error = 0;

	        return curYPR;
	     }
        
	}
}

bool OrientationSensor::extraPackets()
{
	return (fifoCount >= packetSize);
}