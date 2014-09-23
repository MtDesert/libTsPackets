#include "Packet_TDT.h"
#include <string>
using namespace std;

Packet_TDT::Packet_TDT(){}

unsigned Packet_TDT::parseData(const uchar* data,unsigned len)
{
	rawDataPtr=(uchar*)data;
	rawDataLen=len;

	//检查
	if(len < 8)return 0;//基本长度不足
	if(table_id() != 0x70)return 0;//ID不对
	if(section_length()!=5)return 0;//总长度不对

	return 8;
}

uint8 Packet_TDT::table_id()const{return rawDataPtr[0];}
uint8 Packet_TDT::section_syntax_indicator()const{return rawDataPtr[1]&0x80;}
uint16 Packet_TDT::section_length()const{return (((uint16)rawDataPtr[1]&0x0F)<<8)|rawDataPtr[2];}
uint64 Packet_TDT::UTC_time()const
{
	return
	((uint64)rawDataPtr[3]<<32)|
	((uint64)rawDataPtr[4]<<24)|
	((uint64)rawDataPtr[5]<<16)|
	((uint64)rawDataPtr[6]<<8)|
	(uint64)rawDataPtr[7];
}

unsigned PacketTDT::parseData(const uchar* data,unsigned len)
{
	memcpy(&rawData[rawDataLen],data,len);
	return Packet_TDT::parseData(&rawData[1+rawData[0]],rawDataLen+len-rawData[0]-1);
}