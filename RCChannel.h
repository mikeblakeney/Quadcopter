#ifndef RCCHANNEL_H
#define RCCHANNEL_H

class RCChannel
{
public:
	RCChannel();
	long read();

	void risingInterrupt();
	void fallingInterrupt();

private:
	long high_time;
	long low_time;
	long pulse;
};

RCChannel::RCChannel()
{
	high_time = 0;
	low_time = 0;
	pulse = 0;
}


void RCChannel::fallingInterrupt()
{
	high_time = micros();
	
	long newPulse = high_time - low_time;
	if(newPulse >= 0)
	{
		pulse = newPulse;
	}
}

void RCChannel::risingInterrupt()
{
	low_time = micros();
	
}

long RCChannel::read()
{
	/*
	Serial.print(high_time);
	Serial.print('\t');
	Serial.print(low_time);
	Serial.print('\t');
	Serial.println(pulse);
*/
	return pulse;
}

#endif