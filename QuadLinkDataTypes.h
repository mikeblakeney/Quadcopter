//
//  QuadLinkDataTypes.h
//  QuadBase
//
//  Created by Michael J Blakeney on 2/16/14.
//
//

#ifndef QuadBase_QuadLinkDataTypes_h
#define QuadBase_QuadLinkDataTypes_h


struct accelerometer_t
{
	float a_x;
	float a_y;
	float a_z;
};

struct gyroscope_t
{
	float roll;
	float pitch;
	float yaw;
};


struct compass_t
{
	float a_x;
	float a_y;
	float a_z;
};

struct barometer_t
{
	float pressure;
	float temperature;
	float altidude;
};

struct battery_t
{
	float voltage;
	float current;
};


struct pwm_t
{
	uint8_t motor;
	uint16_t speed;
}__attribute__((__packed__));
#endif
