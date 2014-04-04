#include "OrientationSensor.h"

#include "defines.h"

OrientationSensor::OrientationSensor()
{
	/*
	dof   = Adafruit_10DOF();
	accel = Adafruit_LSM303_Accel_Unified(30301);
	mag = Adafruit_LSM303_Mag_Unified(30302);
	gyro = Adafruit_L3GD20_Unified(20);

	if(!accel.begin())
	{
		Serial.println(F("Ooops, no LSM303 Accel detected ... Check your wiring!"));
	}

	if(!mag.begin())
	{
		Serial.println(F("Ooops, no LSM303 Mag detected ... Check your wiring!"));		
	}

	if(!gyro.begin())
	{
		Serial.println(F("Ooops, no L3GD20 Gyro detected ... Check your wiring!"));
	}

	*/

	Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

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

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        //attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
		packetSize = mpu.dmpGetFIFOPacketSize();
    }

	pitchAngle = 0;
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

	        ypr[0] = ypr[0] * 180/M_PI;
	        ypr[1] = ypr[1] * 180/M_PI;
	        ypr[2] = ypr[2] * 180/M_PI;


	       
	        return ypr;
	     }
        
	}
}


void OrientationSensor::filterOrientation(float rawYaw, float rawPitch, float rawRoll)
{

}

