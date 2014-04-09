#ifndef VOLTAGESENSOR_H
#define VOLTAGESENSOR_H

class VoltageSensor
{
public:
	VoltageSensor(int sensorPin, int outPin);
	float getVoltage();
	void checkLevel();

private:
	int v_pin;
	int o_pin;
};

#endif
