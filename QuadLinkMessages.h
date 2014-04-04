//
//  QuadLinkMessages.h
//  QuadBase
//
//  Created by Michael J Blakeney on 2/15/14.
//
//

#ifndef QuadBase_QuadLinkMessages_h
#define QuadBase_QuadLinkMessages_h

#define QL_PACKET_START 0xFE

#define QL_PACKET_SIZE 5

enum QL_MESSAGE {
	CONNECT_SYNC,
	CONNECY_ACK,
	STATUS_MESSAGE,
	STATUS_MOTOR,
	STATUS_ARM_ESC,
	TELEMETRY_ACCELEROMETER,
	TELEMETRY_GYROSCOPE,
	TELEMETRY_COMPASS,
	TELEMETRY_BAROMETER,
	COMMAND_PWM,
	COMMAND_BEHAVIOR
};


struct ql_message_header
{
	uint8_t start;
	uint8_t payloadsize;
	uint8_t sequence;
	uint8_t ql_message;
}__attribute__((__packed__));

struct ql_message_t
{
	ql_message_header header;
	uint8_t checksum;
	
}__attribute__((__packed__));



struct ql_pwm_status_t
{
	ql_message_header header;
	pwm_t pwm;
	uint8_t checksum;
	
}__attribute__((__packed__));

#endif
