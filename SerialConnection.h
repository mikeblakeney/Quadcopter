//
//  SerialConnection.h
//  QuadFirmware
//
//  Created by Michael J Blakeney on 2/17/14.
//  Copyright (c) 2014 Michael Blakeney. All rights reserved.
//

#ifndef QuadFirmware_SerialConnection_h
#define QuadFirmware_SerialConnection_h

#include "QuadLink.h"


char* readMessage()
{
	char* out;
	
	if(Serial.available() > 0)
	{	
		uint8_t firstByte = Serial.read();
		
		if( firstByte != QL_PACKET_START)
		{
			return out;
		}
		
		
		if(Serial.available())
		{
			uint8_t size = QL_PACKET_SIZE;
			
			uint8_t payload_size = Serial.read();
			size += payload_size;
			
			char buff[size];
			
			buff[0] = QL_PACKET_START;
			buff[1] = payload_size;
			
			int remaining = size - 2;
			
			while(remaining > 0)
			{
				if(Serial.available() > 0)
				{
					int offset = size - remaining;
					int result = Serial.readBytes(&buff[offset], remaining);
					if(result > 0){
						remaining -= result;
					}
				}
				
			}
			out = buff;
		}
	}
	return out;
}

void sendQuadLinkMessage(const uint8_t* msg, uint8_t size)
{
	Serial.write(msg, size);
}

void sendStatusUpdate()
{
	ql_message_t status;
	
	status.header.start = QL_PACKET_START;
	status.header.payloadsize = 0;
	status.header.sequence = status_sequence;
	status.header.ql_message = STATUS_MESSAGE;
	
	status.checksum = 11;
	
	sendQuadLinkMessage((const uint8_t*)&status, sizeof(ql_message_t));
	last_status = millis();
	status_sequence++;
}

void status_update()
{
	// Send link status msg (1Hz)
	
	if(connected)
	{
		//digitalWrite(13, HIGH);
		if( (millis()-last_status) >= 1000)
		{
			sendStatusUpdate();
		}
	}else
	{
		if(Serial.available() > 0)
		{
			char* buff = readMessage();
			if(buff > 0)
			{
				ql_message_t msg;
				memcpy(&msg, buff, QL_PACKET_SIZE);
				Serial.println(msg.header.ql_message);
				if(msg.header.ql_message == CONNECT_SYNC)
				{
					connected = true;
				}
				
			}
		}
	}

}


#endif

