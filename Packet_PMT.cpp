#include "Packet_PMT.h"
#include<stdint.h>

Packet_PMT::Packet_PMT(){resetPointer();}

unsigned Packet_PMT::parseData(const uchar *data, unsigned len)
{
	resetPointer();
	rawDataPtr=(uchar*)data;
	rawDataLen=len;
	//check
	if(len < 16)return 0;//基本长度不足
	if(table_id() != 0x02)return 0;//ID不对
	if(len < 3 + section_length())return 0;//总长度过短
	if(section_length() > 1021)return 0;//总长度过长

	//确定正确的包长
	rawDataLen=3+section_length();
	crc32=&rawDataPtr[rawDataLen-4];

	//确定program info
	int pos=12;
	//if(m_descriptorsList.parseData(&rawDataPtr[12],program_info_length())==0)return 0;
	pos+=program_info_length();
	if(pos>=rawDataLen)return 0;

	//确定Streams
	streams=&rawDataPtr[pos];
	unsigned stopPos=rawDataLen-4;
	while(pos<stopPos)
	{
		Stream stream(&rawDataPtr[pos]);
		pos+=5+stream.ES_info_length();
		pmList.push_back(stream);
	}
	//OK
	return rawDataLen;
}

uint8 Packet_PMT::table_id()const{return rawDataPtr[0];}
bool Packet_PMT::section_syntax_indicator()const{return rawDataPtr[1]&0x80;}
uint16 Packet_PMT::section_length()const{return (((uint16)rawDataPtr[1]&0x0F)<<8)|rawDataPtr[2];}
uint16 Packet_PMT::program_number()const{return ((uint16)rawDataPtr[3]<<8)|rawDataPtr[4];}
uint8 Packet_PMT::version_number()const{return (rawDataPtr[5]&0x3E)>>1;}
bool Packet_PMT::current_next_indicator()const{return rawDataPtr[5]&0x01;}
uint8 Packet_PMT::section_number()const{return rawDataPtr[6];}
uint8 Packet_PMT::last_section_number()const{return rawDataPtr[7];}
uint16 Packet_PMT::PCR_PID()const{return (((uint16)rawDataPtr[8]&0x1F)<<8)|rawDataPtr[9];}
uint16 Packet_PMT::program_info_length()const{return (((uint16)rawDataPtr[10]&0x0F)<<8)|rawDataPtr[11];}

DescriptorsList Packet_PMT::programInfoDescriptorsList()const{return m_descriptorsList;}

Packet_PMT::Stream::Stream(const uchar *data):pointer(data){}
uint8 Packet_PMT::Stream::stream_type()const{return pointer[0];}
uint16 Packet_PMT::Stream::elementary_PID()const{return (((uint16)pointer[1]&0x1F)<<8)|pointer[2];}
uint16 Packet_PMT::Stream::ES_info_length()const{return (((uint16)pointer[3]&0x0F)<<8)|pointer[4];}
DescriptorsList Packet_PMT::Stream::esInfoDescriptorList()const{return DescriptorsList();}

bool Packet_PMT::Stream::isVideoStreamType()const
{
	switch(stream_type())
	{
	case 0x01://ISO/IEC 11172-2 Video
	case 0x02://ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream
	//case 0x10://ISO/IEC 14496-2 Visual
	case 0x1B://AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video(H264_VIDEO)
	case 0x1E://Auxiliary video stream as defined in ISO/IEC 23002-3
	case 0x42://(AVS_VIDEO)
		return true;
	default:return false;
	}
}
bool Packet_PMT::Stream::isAudioStreamType()const
{
	switch(stream_type())
	{
	case 0x03://ISO/IEC 11172-3 Audio
	case 0x04://ISO/IEC 13818-3 Audio
	case 0x0F://ISO/IEC 13818-7 Audio with ADTS transport syntax(AAC_AUDIO)
	case 0x11://ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3(MPEG4_AUDIO)
	case 0x1C://ISO/IEC 14496-3 Audio, without using any additional transport syntax, such as DST, ALS and SLS
	case 0x81://(AC3_AUDIO)
	case 0x82://(DTS_AUDIO)
		return true;
	default:return false;
	}
}

bool Packet_PMT::containElementary_pid(uint16 pid)const
{
	for(list<Stream>::const_iterator itr=pmList.begin();itr!=pmList.end();++itr)
	{
		if(itr->elementary_PID()==pid)return true;
	}
	return false;
}

void Packet_PMT::resetPointer()
{
	ES_info_descriptors=NULL;
	crc32=NULL;
}

unsigned PacketPMT::parseData(const uchar *data, unsigned len)
{
	memcpy(&rawData[rawDataLen],data,len);
	return Packet_PMT::parseData(&rawData[1+rawData[0]],rawDataLen+len-rawData[0]-1);
}