#include "Packet_NIT.h"
#include<string.h>

Packet_NIT::Packet_NIT(){}

unsigned Packet_NIT::parseData(uchar *pointer, unsigned length, unsigned offset)
{
	DataBlock::parseData(pointer,length,offset);

	if(length < 14)return 0;
	if(table_id() != 0x40 && table_id() != 0x41)return 0;
	if(length < 3 + section_length())return 0;
	if(section_length() > 1021)return 0;


	dataLength=3+section_length();
	crc32=&dataPointer[dataLength-4];

	//net work descriptor
	unsigned pos=10;
	if(m_networkDescriptorsList.parseData(&dataPointer[pos],network_descriptors_length())==0)return 0;
	pos+=network_descriptors_length();
	if(pos>=dataLength)return 0;

	//transport stream loop
	m_transportStreamLoops=&dataPointer[pos];
	pos+=2;
	int stopPos = pos + transport_stream_loop_length();
	if(stopPos > dataLength-4)return 0;

	/*m_transportStreamsCount=0;
	while(pos<stopPos)
	{
		TransportStreamLoop tsl;
		if(tsl.parseData(&dataPointer[pos])==0)return 0;
		m_transportStreams[m_transportStreamsCount]=tsl;
		pos += 6+tsl.transport_descriptors_length();
	}*/
	if(pos>=dataLength)return 0;

	return dataLength;
}

uint8 Packet_NIT::table_id()const{return dataPointer[0];}
bool Packet_NIT::section_syntax_indicator()const{return dataPointer[1]&0x80;}
uint16 Packet_NIT::section_length()const{return (((uint16)dataPointer[1]&0x0F)<<8)|dataPointer[2];}
uint16 Packet_NIT::network_id()const{return ((uint16)dataPointer[3]<<8)|dataPointer[4];}
uint8 Packet_NIT::version_number()const{return (dataPointer[5]&0x3E)>>1;}
bool Packet_NIT::current_next_indicator()const{return dataPointer[5]&0x01;}
uint8 Packet_NIT::section_number()const{return dataPointer[6];}
uint8 Packet_NIT::last_section_number()const{return dataPointer[7];}

uint16 Packet_NIT::network_descriptors_length()const{return (((uint16)dataPointer[8]&0x0F)<<8)|dataPointer[9];}

DescriptorsList Packet_NIT::networkDescriptorsList()const{return m_networkDescriptorsList;}

uint16 Packet_NIT::transport_stream_loop_length()const{return (((uint16)m_transportStreamLoops[0]&0x0F)<<8)|m_transportStreamLoops[1];}

Packet_NIT::TransportStreamLoop::TransportStreamLoop(){}

unsigned Packet_NIT::TransportStreamLoop::parseData(const uchar *data)
{
	dataPointer=(uchar*)data;
	return parseData(data,6+transport_descriptors_length());
}
unsigned Packet_NIT::TransportStreamLoop::parseData(const uchar *data, unsigned len)
{
	reset();
	dataPointer=(uchar*)data;
	dataLength=len;
	//check
	if(len<6)return 0;
	if(len<6+transport_descriptors_length())return 0;
	//parse
	if(m_descriptorsList.parseData(&dataPointer[6],dataLength)==0)return 0;
}

uint16 Packet_NIT::TransportStreamLoop::transport_stream_id()const{return (uint16)dataPointer[0]|dataPointer[1];}
uint16 Packet_NIT::TransportStreamLoop::original_network_id()const{return (uint16)dataPointer[2]|dataPointer[3];}
uint16 Packet_NIT::TransportStreamLoop::transport_descriptors_length()const{return (((uint16)dataPointer[4]&0x0F)<<8)|dataPointer[5];}

DescriptorsList Packet_NIT::TransportStreamLoop::transportDescriptorsList()const{return m_descriptorsList;}

uint8 Packet_NIT::transportStreamLoopCount()const{return m_transportStreamsCount;}
Packet_NIT::TransportStreamLoop Packet_NIT::transportStreamLoop(uint8 index)const{return m_transportStreams[index];}

unsigned Packet_NIT::reset()
{
	m_transportStreamLoops=NULL;
	crc32=NULL;
	return DataBlock::reset();
}

uint32 Packet_NIT::CRC_32()const
{
	return ((uint32)crc32[0]<<24)|
		((uint32)crc32[1]<<16)|
		((uint16)crc32[2]<<8)|
		crc32[3];
}

unsigned PacketNIT::parseData(const uchar *data, unsigned len)
{
	/*memcpy(&rawData[dataLength],data,len);
	return Packet_NIT::parseData(&rawData[1+rawData[0]],dataLength+len-rawData[0]-1);*/
	memcpy(&rawData[dataLength],data,len);
	dataLength+=len;
	if(packetPSI.parseData(&rawData[0],dataLength)==0)return 0;
	//return Packet_NIT::parseData(&rawData[packetPSI.dataLength()],dataLength-packetPSI.dataLength());
}