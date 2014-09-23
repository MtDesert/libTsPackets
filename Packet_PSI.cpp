#include "Packet_PSI.h"

Packet_PSI::Packet_PSI(){}

unsigned Packet_PSI::parseData(const uchar *data, unsigned len)
{
	reset();
	dataPointer=(uchar*)data;
	dataLength=len;
	//check
	if(len<1+pointer_field())return 0;
	//
	dataLength=1+pointer_field();
	return dataLength;
}
uint8 Packet_PSI::pointer_field()const{return dataPointer[0];}