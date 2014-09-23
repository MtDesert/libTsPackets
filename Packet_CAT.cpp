#include "Packet_CAT.h"
#include<string.h>

Packet_CAT::Packet_CAT(){}

unsigned Packet_CAT::parseData(const uchar *data, unsigned len)
{
	reset();
	dataPointer=(uchar*)data;
	dataLength=len;

	if(len < 12)return 0;
	if(table_id() != 0x01)return 0;
	if(len < 3 + section_length())return 0;
	if(section_length() > 1021)return 0;

	//
	dataLength=3+section_length();
	crc32=&dataPointer[dataLength-4];
	//
	return dataLength;
}
unsigned Packet_CAT::reset()
{
	crc32=NULL;
	return 0;
}

uint8 Packet_CAT::table_id()const{return dataPointer[0];}
bool Packet_CAT::section_syntax_indicator()const{return dataPointer[1]&0x80;}
uint16 Packet_CAT::section_length()const{return (((uint16)dataPointer[1]&0x0F)<<8)|dataPointer[2];}
uint8 Packet_CAT::version_number()const{return (dataPointer[5]&0x3E)>>1;}
bool Packet_CAT::current_next_indicator()const{return dataPointer[5]&0x01;}
uint8 Packet_CAT::section_number()const{return dataPointer[6];}
uint8 Packet_CAT::last_section_number()const{return dataPointer[7];}

uint32 Packet_CAT::CRC_32()const
{
	return ((uint32)crc32[0]<<24)|
		((uint32)crc32[1]<<16)|
		((uint16)crc32[2]<<8)|
		crc32[3];
}

unsigned PacketCAT::parseData(const uchar *data, unsigned len)
{
	memcpy(&rawData[dataLength],data,len);
	return Packet_CAT::parseData(&rawData[1+rawData[0]],dataLength+len-rawData[0]-1);
}