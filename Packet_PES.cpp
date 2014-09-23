#include "Packet_PES.h"
#include <stdlib.h>

Packet_PES::Packet_PES(){resetPointer();}

unsigned Packet_PES::parseData(const uchar *data,unsigned len)
{
	rawDataPtr=(uchar*)data;rawDataLen=len;
	//基本检查
	if(len<6)return 0;//头部太短
	if(packet_start_code_prefix()!=0x000001)return 0;//头部不对
	unsigned totalLen=6+PES_packet_length();
	//if(len<totalLen)return 0;//总长度不足
	
	//扩展字段判定
	unsigned extentionLen=0;
	if(containExtention())
	{
		extention=(uchar*)&data[6];
		extentionLen=3;
		if(PTS_DTS_flags()==0x2)
		{
			ptsValue=(uchar*)&extention[3];
			extentionLen+=5;
		}
		if(PTS_DTS_flags()==0x3)
		{
			ptsValue=(uchar*)&extention[3];
			dtsValue=(uchar*)&extention[8];
			extentionLen+=10;
		}
		//后面的字段没有解析,我偷懒了...
	}
	return totalLen;
}

uint32 Packet_PES::packet_start_code_prefix()const{return ((uint32)rawDataPtr[0]<<16)|((uint16)rawDataPtr[1]<<8)|rawDataPtr[2];}
uint8 Packet_PES::stream_id()const{return rawDataPtr[3];}
uint16 Packet_PES::PES_packet_length()const{return ((uint16)rawDataPtr[4]<<8)|rawDataPtr[5];}

bool Packet_PES::containExtention()const
{
	uint8 id=stream_id();
	return id != program_stream_map
		&& id != padding_stream
		&& id != private_stream_2
		&& id != ECM_stream
		&& id != EMM_stream
		&& id != program_stream_directory
		&& id != DSMCC_stream
		&& id != typeE_stream;
}

uint8 Packet_PES::PES_scrambling_control()const{return (extention[0]&0x30)>>4;}
bool Packet_PES::PES_priority()const{return extention[0]&0x08;}
bool Packet_PES::data_alignment_indicator()const{return extention[0]&0x04;}
bool Packet_PES::copyright()const{return extention[0]&0x02;}
bool Packet_PES::original_or_copy()const{return extention[0]&0x01;}

uint8 Packet_PES::PTS_DTS_flags()const{return (extention[1]&0xC0)>>6;}
bool Packet_PES::ESCR_flag()const{return extention[1]&0x20;}
bool Packet_PES::ES_rate_flag()const{return extention[1]&0x10;}
bool Packet_PES::DSM_trick_mode_flag()const{return extention[1]&0x08;}
bool Packet_PES::additional_copy_info_flag()const{return extention[1]&0x04;}
bool Packet_PES::PES_CRC_flag()const{return extention[1]&0x02;}
bool Packet_PES::PES_extension_flag()const{return extention[1]&0x01;}

uint8 Packet_PES::PES_header_data_length()const{return extention[2];}

uint64 Packet_PES::getPTSvalue()const
{
	return (((uint64)ptsValue[0]&0x0E)<<29)|
		((uint64)ptsValue[1]<<22)|
		(((uint64)ptsValue[2]&0xFE)<<14)|
		((uint64)ptsValue[3]<<7)|
		(((uint64)ptsValue[4]&0xFE)>>1);
}
uint64 Packet_PES::getDTSvalue()const
{
	return (((uint64)dtsValue[0]&0x0E)<<29)|
		((uint64)dtsValue[1]<<22)|
		(((uint64)dtsValue[2]&0xFE)<<14)|
		((uint64)dtsValue[3]<<7)|
		(((uint64)dtsValue[4]&0xFE)>>1);
}
void Packet_PES::resetPointer()
{
	extention=NULL;
	ptsValue=NULL;
	dtsValue=NULL;
}