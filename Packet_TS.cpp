#include "Packet_TS.h"

#include <stddef.h>
#include <string.h>

Packet_TS::Packet_TS(){reset();}

unsigned Packet_TS::parseData(uchar *pointer, unsigned length, unsigned offset)
{
	DataBlock::parseData(pointer,length,offset);
	unsigned pos=header.parseData(pointer,length,offset);
	if(pos==0)return 0;
	pointer += pos;
	length -= pos;
	offset += pos;

	pos=adaptionField.parseData(pointer,length,offset);
}

unsigned DataBlock_TsHeader::parseData(uchar *pointer, unsigned length, unsigned offset)
{
	DataBlock::parseData(pointer,length,offset);
	if(dataLength<4)return 0;
	if(getSyncByte()!=0x47)return 0;
	dataLength=4;
	return dataLength;
}

/*uint8 DataBlock_TsHeader::sync_byte()const{return dataPointer[0];}
void DataBlock_TsHeader::setSync_byte(uint8 value){dataPointer[0]=value;}*/

//uint16 DataBlock_TsHeader::pid()const{return getUInt16(2,false)&0x1FFF;}
//void DataBlock_TsHeader::setPid(uint16 value){setUInt16(2,value&0x1FFF,false);}

/*uint8 DataBlock_TsHeader::transport_scrambling_control()const{return (dataPointer[3]&0xC0)>>6;}
void DataBlock_TsHeader::setTransport_scrambling_control(uint8 value){dataPointer[3]=(value<<6);}

uint8 DataBlock_TsHeader::adaptation_field_control()const{return (dataPointer[3]&0x30)>>4;}*/

/*uint8 DataBlock_TsHeader::continuity_counter()const{return dataPointer[3]&0x0F;}
void DataBlock_TsHeader::setContinuity_counter(uint8 value){dataPointer[3]=value&0x0F;}*/

uint8 Packet_TS::adaptation_field_length()const{return adaptation_field[0];}
//void Packet_TS::setAdaptation_field_length(uint8 value){adaptation_field[0]}
//flag
bool Packet_TS::discontinuity_indicator()const{return adaptation_field[1]&0x80;}
bool Packet_TS::random_access_indicator()const{return adaptation_field[1]&0x40;}
bool Packet_TS::elementary_stream_priority_indicator()const{return adaptation_field[1]&0x20;}
bool Packet_TS::PCR_flag()const{return adaptation_field[1]&0x10;}
bool Packet_TS::OPCR_flag()const{return adaptation_field[1]&0x08;}
bool Packet_TS::splicing_point_flag()const{return adaptation_field[1]&0x04;}
bool Packet_TS::transport_private_data_flag()const{return adaptation_field[1]&0x02;}
bool Packet_TS::adaptation_field_extension_flag()const{return adaptation_field[1]&0x01;}

//pcr_flag
uint64 Packet_TS::program_clock_reference_base()const
{
	return ((uint64)pcrVal[0]<<25)|
		((uint64)pcrVal[1]<<17)|
		((uint64)pcrVal[2]<<9)|
		((uint64)pcrVal[3]<<1)|
		((uint64)pcrVal[4]>>7);
}
uint16 Packet_TS::program_clock_reference_extension()const
{
	return (((uint16)pcrVal[4]&0x1)<<8)|pcrVal[5];
}
uint64 Packet_TS::pcrValue()const
{
	return program_clock_reference_base()*300+program_clock_reference_extension();
}

//opcr_flag
uint64 Packet_TS::original_program_clock_reference_base()const
{
	return ((uint64)opcrVal[0]<<25)|
		((uint64)opcrVal[1]<<17)|
		((uint64)opcrVal[2]<<9)|
		((uint64)opcrVal[3]<<1)|
		((uint64)opcrVal[4]>>7);
}
uint16 Packet_TS::original_program_clock_reference_extension()const
{
	return (((uint16)opcrVal[4]&0x1)<<8)|opcrVal[5];
}
uint64 Packet_TS::opcrValue()const
{
	return original_program_clock_reference_base()*300+original_program_clock_reference_extension();
}

//splicing_point_flag
uint8 Packet_TS::splice_countdown()const{return splice[0];}

//transport_private_data_flag
uint8 Packet_TS::transport_private_data_length()const{return transport_private_data_len[0];}

//adaptation_field_extension_flag
uint8 Packet_TS::adaptation_field_extension_length()const{return adaptation_field_extension[0];}
uint8 Packet_TS::ltw_flag()const{return adaptation_field_extension[1]&0x80;}
uint8 Packet_TS::piecewise_rate_flag()const{return adaptation_field_extension[1]&0x40;}
uint8 Packet_TS::seamless_splice_flag()const{return adaptation_field_extension[0]&0x20;}

//ltw_flag
uint8 Packet_TS::ltw_valid_flag()const{return ltw[0]&0x80;}
uint16 Packet_TS::ltw_offset()const{return (((uint16)ltw[0]&0x7F)<<8)|ltw[1];}

//piecewise_rate_flag;
uint32 Packet_TS::piecewise_rate()const
{
	return (((uint32)piecewiseRate[0]&0x3F)<<16)|
		((uint16)piecewiseRate[1]<<8)|
		piecewiseRate[2];
}

//seamless_splice_flag
uint8 Packet_TS::splice_type()const{return (seamless_splice[0]&0xF0)>>4;}
uint64 Packet_TS::DTS_next_AU()const
{
	return (((uint64)seamless_splice[0]&0x0E)<<29)|
		((uint64)seamless_splice[1]<<22)|
		(((uint64)seamless_splice[2]&0xFE)<<14)|
		((uint64)seamless_splice[3]<<7)|
		(((uint64)seamless_splice[4]&0xFE)>>1);
}

/*unsigned Packet_TS::reset()
{
	adaptation_field=NULL;
	pcrVal=NULL;
	opcrVal=NULL;
	splice=NULL;
	transport_private_data_len=NULL;
	adaptation_field_extension=NULL;
	ltw=NULL;
	piecewiseRate=NULL;
	seamless_splice=NULL;
	stuffing_byte=NULL;
	payload=NULL;
	return 0;
}*/

unsigned PacketTS::parseData(const uchar *data,unsigned len)
{
	memcpy(rawData,data,len);
	return Packet_TS::parseData(rawData,len);
}