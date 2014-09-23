#include "Packet_PAT.h"
#include <string>
using namespace std;

Packet_PAT::Packet_PAT(){reset();}
Packet_PAT::Packet_PAT(const uchar* data,unsigned len){parseData(data,len);}

unsigned Packet_PAT::parseData(const uchar* data,unsigned len)
{
	DataBlock::parseData();

	if(len < 12)return 0;
	if(table_id() != 0x00)return 0;
	if(len < 3 + section_length())return 0;
	if(section_length() > 1021)return 0;

	rawDataLen=3+section_length();
	crc32=&rawDataPtr[rawDataLen-4];
	return rawDataLen;
}

uint8 Packet_PAT::table_id()const{return rawDataPtr[0];}
bool Packet_PAT::section_syntax_indicator()const{return rawDataPtr[1]&0x80;}
uint16 Packet_PAT::section_length()const{return (((uint16)rawDataPtr[1]&0x0F)<<8)|rawDataPtr[2];}
uint16 Packet_PAT::transport_stream_id()const{return ((uint16)rawDataPtr[3]<<8)|rawDataPtr[4];}
uint8 Packet_PAT::version_number()const{return (rawDataPtr[5]&0x3E)>>1;}
bool Packet_PAT::current_next_indicator()const{return rawDataPtr[5]&0x01;}
uint8 Packet_PAT::section_number()const{return rawDataPtr[6];}
uint8 Packet_PAT::last_section_number()const{return rawDataPtr[7];}

Packet_PAT::ProgramAssociation::ProgramAssociation(const uchar *data):pointer(data){}
uint16 Packet_PAT::ProgramAssociation::program_number()const{return ((uint16)pointer[0]<<8)|pointer[1];}
uint16 Packet_PAT::ProgramAssociation::pid()const{return (((uint16)pointer[2]&0x1F)<<8)|pointer[3];}

uint8 Packet_PAT::programAssociationCount()const{return (rawDataLen-12)/4;}
Packet_PAT::ProgramAssociation Packet_PAT::programAssociation(uint8 index)const{return ProgramAssociation(&rawDataPtr[8+index*4]);}

uint32 Packet_PAT::CRC_32()const
{
	return ((uint32)crc32[0]<<24)|
		((uint32)crc32[1]<<16)|
		((uint16)crc32[2]<<8)|
		crc32[3];
}


bool Packet_PAT::getAssociationNumber(uint16 pid,uint16 *number)const
{
	int count=programAssociationCount();
	for(int i=0;i<count;++i)
	{
		if(programAssociation(i).pid()==pid)
		{
			if(number)*number=programAssociation(i).program_number();
			return true;
		}
	}
	return false;
}
bool Packet_PAT::getAssociationPID(uint16 number,uint16 *pid)const
{
	int count=programAssociationCount();
	for(int i=0;i<count;++i)
	{
		if(programAssociation(i).program_number()==number)
		{
			if(pid)*pid=programAssociation(i).pid();
			return true;
		}
	}
	return false;
}

bool Packet_PAT::getNITpid(uint16 *value)const
{
	return getAssociationPID(0,value);
}
bool Packet_PAT::getPMTpid(uint16 number,uint16 *pid)const
{
	if(number==0)return false;
	return getAssociationPID(number,pid);
}
bool Packet_PAT::getPMTnumber(uint16 pid,uint16 *number)const
{
	uint16 num;
	if(getAssociationNumber(pid,&num) && num!=0)
	{
		if(number)*number=num;
		return true;
	}
	else return false;
}

unsigned Packet_PAT::reset()
{
	crc32=NULL;
	return Packet::reset();
}
//
unsigned PacketPAT::parseData(const uchar *data, unsigned len)
{
	memcpy(&rawData[rawDataLen],data,len);
	return Packet_PAT::parseData(&rawData[1+rawData[0]],rawDataLen+len-rawData[0]-1);
}