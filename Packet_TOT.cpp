#include "Packet_TOT.h"

Packet_TOT::Packet_TOT(){}

/*bool Packet_TOT::fromRawData(const uint8 *data,unsigned len)
{
	int pos=1+data[0];//pointer_field
	if(len<pos+8)return false;
	//
	table_id = data[pos];
	section_syntax_indicator = data[pos+1]>>7;
	section_length = ((uint16)data[pos+1]&0xF)<<8|data[pos+2];
	UTC_time=
		((uint64)data[pos+3]<<32)|
		((uint64)data[pos+4]<<24)|
		((uint64)data[pos+5]<<16)|
		((uint64)data[pos+6]<<8)|
		(uint64)data[pos+7];
	descriptors_loop_length = ((uint16)data[8]&0xF)<<8|data[9];
	
	int validLen=1+data[0]+3+section_length;
	if(validLen>len)return false;
	//中间有一堆字段
	CRC_32=
		((uint16)data[validLen-4]<<24)|
		((uint16)data[validLen-3]<<16)|
		((uint16)data[validLen-2]<<8)|
		((uint16)data[validLen-1]);
}*/

unsigned Packet_TOT::parseData(const uchar* data,unsigned len)
{
	rawDataPtr=(uchar*)data;
	rawDataLen=len;

	//检查
	if(len < 14)return 0;//基本长度不足
	if(table_id() != 0x73)return 0;//ID不对

	rawDataLen=3+section_length();
	crc32=&rawDataPtr[rawDataLen-4];
	return 8;
}

uint8 Packet_TOT::table_id()const{return rawDataPtr[0];}
uint8 Packet_TOT::section_syntax_indicator()const{return rawDataPtr[1]&0x80;}
uint16 Packet_TOT::section_length()const{return (((uint16)rawDataPtr[1]&0x0F)<<8)|rawDataPtr[2];}
uint64 Packet_TOT::UTC_time()const
{
	return
		((uint64)rawDataPtr[3]<<32)|
		((uint64)rawDataPtr[4]<<24)|
		((uint64)rawDataPtr[5]<<16)|
		((uint64)rawDataPtr[6]<<8)|
		(uint64)rawDataPtr[7];
}
uint16 Packet_TOT::descriptors_loop_length()const{return (((uint16)rawDataPtr[8]&0x0F)<<8)|rawDataPtr[9];}

/*uint8 Packet_TOT::descriptorsLoopCount()const
{
	uint8 ret=0;
	unsigned pos=0;
	while(pos<descriptors_loop_length())
	{
		Descriptor dscptr(&descriptors[pos]);
		++ret;
		pos+=2+dscptr.descriptor_length();
	}
	return ret;
}*/
/*Descriptor Packet_TOT::descriptor(uint8 index)const
{
	uint8 count=descriptorsLoopCount();
	if(index>=count)return Descriptor(NULL);
	//scan
	int idx=0;
	unsigned pos=0;
	while(pos<descriptors_loop_length())
	{
		Descriptor dscptr(&descriptors[pos]);
		if(idx>=index)return dscptr;
		++idx;
		pos+=2+dscptr.descriptor_length();
	}
	return Descriptor(NULL);
}*/

void Packet_TOT::resetPointer()
{
	descriptors=NULL;
	crc32=NULL;
}

uint32 Packet_TOT::CRC_32()const
{
	return ((uint32)crc32[0]<<24)|
		((uint32)crc32[1]<<16)|
		((uint16)crc32[2]<<8)|
		crc32[3];
}

unsigned PacketTOT::parseData(const uchar* data,unsigned len)
{
	memcpy(&rawData[rawDataLen],data,len);
	return Packet_TOT::parseData(&rawData[1+rawData[0]],rawDataLen+len-rawData[0]-1);
}