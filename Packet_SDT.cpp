#include "Packet_SDT.h"

Packet_SDT::Packet_SDT(){}

unsigned Packet_SDT::parseData(const uchar* data,unsigned len)
{
	reset();
	rawDataPtr=(uchar*)data;
	rawDataLen=len;

	//检查
	if(len < 15)return 0;//基本长度不足
	if(table_id() != 0x42 && table_id() != 0x46)return 0;//ID不对
	if(len < 3 + section_length())return 0;//总长度过短
	if(section_length() > 1021)return 0;//总长度过长

	//确定正确的大小
	rawDataLen=3+section_length();
	crc32=&rawDataPtr[rawDataLen-4];

	unsigned pos=11;
	unsigned stopPos=rawDataLen-4;

	m_serviceDescriptorsCount=0;
	while(pos<stopPos)
	{
		ServiceDescriptor sd;
		if(sd.parseData(&rawDataPtr[pos],stopPos-pos)==0)return 0;
		m_serviceDescriptors[m_serviceDescriptorsCount]=sd;
		++m_serviceDescriptorsCount;
		pos+=sd.dataLength();
	}
	if(pos>stopPos)return 0;
	return rawDataLen;
}

uint8 Packet_SDT::table_id()const{return rawDataPtr[0];}
bool Packet_SDT::section_syntax_indicator()const{return rawDataPtr[1]&0x80;}
uint16 Packet_SDT::section_length()const{return (((uint16)rawDataPtr[1]&0x0F)<<8)|rawDataPtr[2];}
uint16 Packet_SDT::transport_stream_id()const{return ((uint16)rawDataPtr[3]<<8)|rawDataPtr[4];}
uint8 Packet_SDT::version_number()const{return (rawDataPtr[5]&0x3E)>>1;}
bool Packet_SDT::current_next_indicator()const{return rawDataPtr[5]&0x01;}
uint8 Packet_SDT::section_number()const{return rawDataPtr[6];}
uint8 Packet_SDT::last_section_number()const{return rawDataPtr[7];}
uint16 Packet_SDT::original_network_id()const{return ((uint16)rawDataPtr[8]<<8)|rawDataPtr[9];}

uint32 Packet_SDT::CRC_32()const
{
	return ((uint32)crc32[0]<<24)|
		((uint32)crc32[1]<<16)|
		((uint16)crc32[2]<<8)|
		crc32[3];
}

Packet_SDT::ServiceDescriptor::ServiceDescriptor(){}
unsigned Packet_SDT::ServiceDescriptor::parseData(const uchar *data, unsigned len)
{
	reset();
	rawDataPtr=(uchar*)data;
	rawDataLen=len;
	//check
	if(len<5)return 0;
	if(descriptors_loop_length()>1024-11-5)return 0;
	if(descriptors_loop_length()>len-5)return 0;

	if(descriptors.parseData(&rawDataPtr[5],descriptors_loop_length())==0)return 0;

	rawDataLen=descriptors.dataLength()+5;
	return rawDataLen;
}

uint16 Packet_SDT::ServiceDescriptor::service_id()const{return ((uint8)rawDataPtr[0]<<8)|rawDataPtr[1];}
bool Packet_SDT::ServiceDescriptor::EIT_echedule_flag()const{return rawDataPtr[2]&0x02;}
bool Packet_SDT::ServiceDescriptor::EIT_present_following_flag()const{return rawDataPtr[2]&0x01;}
uint8 Packet_SDT::ServiceDescriptor::running_status()const{return (rawDataPtr[3]&0xE0)>>5;}
bool Packet_SDT::ServiceDescriptor::free_CA_mode()const{return rawDataPtr[3]&0x10;}
uint16 Packet_SDT::ServiceDescriptor::descriptors_loop_length()const{return (((uint16)rawDataPtr[3]&0x0F)<<8)|rawDataPtr[4];}

uint8 Packet_SDT::serviceDescriptorsCount()const{return m_serviceDescriptorsCount;}
Packet_SDT::ServiceDescriptor Packet_SDT::serviceDescriptor(uint8 index)const{return m_serviceDescriptors[index];}

unsigned Packet_SDT::reset()
{
	crc32=NULL;
	return Packet::reset();
}

unsigned PacketSDT::parseData(const uchar *data, unsigned len)
{
	memcpy(&rawData[rawDataLen],data,len);
	rawDataLen+=len;
	if(packetPSI.parseData(&rawData[0],rawDataLen)==0)return 0;
	return Packet_SDT::parseData(&rawData[packetPSI.dataLength()],rawDataLen-packetPSI.dataLength());
	//return Packet_SDT::parseData(&rawData[1+rawData[0]],rawDataLen+len-rawData[0]-1);
}