#include "VoltageSensor.h"
#include <arduino.h>
#include  "defines.h"
VoltageSensor::VoltageSensor(int sensorPin, int outPin)
{
	v_pin = sensorPin;
	o_pin = outPin;

	// setup low voltage alert
	pinMode(o_pin, OUTPUT);
	digitalWrite(o_pin, LOW);
}

float VoltageSensor::getVoltage()
{
	float raw_V = analogRead(v_pin);
	return raw_V/ANALOG_TO_V_SENSOR;
}

void VoltageSensor::checkLevel()
{
	float curV = 12; //voltageSensor->getVoltage();
	
	if(curV < LOW_LIPO_VOLTAGE)
	{
		digitalWrite(o_pin, HIGH);
		Serial.print("LOW VOLTAGE");
		Serial.print('\t');
		Serial.println(curV);
	}else{
		digitalWrite(o_pin, LOW);
	}

}