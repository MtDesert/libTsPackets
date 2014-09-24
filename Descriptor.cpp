#include "Descriptor.h"

Descriptor::Descriptor(){reset();}
//Descriptor::Descriptor(const uchar *data){dataPointer=(uchar*)data;}

unsigned Descriptor::parseData(uchar *pointer, unsigned length, unsigned offset)
{
	DataBlock::parseData(pointer,length,offset);
	//check
	if(length<2)return 0;
	if(length<2+descriptor_length())return 0;

	dataLength=2+descriptor_length();
	return dataLength;
}

uint8 Descriptor::descriptor_tag()const{return dataPointer[0];}
uint8 Descriptor::descriptor_length()const{return dataPointer[1];}

//0x40
string Descriptor::networkName()const{return string((char*)&dataPointer[2],descriptor_length());}

//0x43
uint32 Descriptor::frequency()const
{
	return (uint32)dataPointer[2]<<24|
		(uint32)dataPointer[3]<<16|
		(uint32)dataPointer[4]<<8|
		(uint32)dataPointer[5];
}
uint16 Descriptor::orbital_position()const{return (uint16)dataPointer[6]<<8 | (uint16)dataPointer[7];}
uint8 Descriptor::west_east_flag()const{return dataPointer[8]>>7;}
uint8 Descriptor::polarization()const{return (dataPointer[8]>>5)&0x03;}
uint8 Descriptor::roll_off()const{return (dataPointer[8]>>3)&0x03;}
uint8 Descriptor::modulation_system()const{return (dataPointer[8]>>2)&0x01;}
uint8 Descriptor::modulation_type()const{return dataPointer[8]&0x03;}
uint32 Descriptor::symbol_rate()const
{
	return (uint32)dataPointer[9]<<20|
		(uint32)dataPointer[10]<<12|
		(uint32)dataPointer[11]<<4|
		(uint32)dataPointer[12]>>4;
}
uint8 Descriptor::FEC_inner()const{return dataPointer[12]&0xF;}

//0x44
uint8 Descriptor::FEC_outer()const{return dataPointer[7]&0xF;}
uint16 Descriptor::modulation()const{return dataPointer[8];}

//0x48
uint8 Descriptor::service_type()const{return dataPointer[2];}
uint8 Descriptor::service_provider_name_length()const{return dataPointer[3];}
string Descriptor::service_provider_name()const{return string((char*)&dataPointer[4],service_provider_name_length());}
uint8 Descriptor::service_name_length()const{return dataPointer[4+service_provider_name_length()];}
string Descriptor::service_name()const{return string((char*)&dataPointer[4+service_provider_name_length()+1],service_name_length());}
//0x4A
uint16 Descriptor::transport_stream_id()const{return (uint16)dataPointer[2]<<8 | (uint16)dataPointer[3];}
uint16 Descriptor::original_network_id()const{return (uint16)dataPointer[4]<<8 | (uint16)dataPointer[5];}
uint16 Descriptor::service_id()const{return (uint16)dataPointer[6]<<8 | (uint16)dataPointer[7];}
uint8 Descriptor::linkage_type()const{return dataPointer[8];}

//0x5A
uint32 Descriptor::centre_frequency()const
{
	return (uint32)dataPointer[2]<<24|
		(uint32)dataPointer[3]<<16|
		(uint32)dataPointer[4]<<8|
		(uint32)dataPointer[5];
}
uint8 Descriptor::bandwidth()const{return (dataPointer[6]&0xE0)>>5;}
bool Descriptor::priority()const{return dataPointer[6]&0x10;}
bool Descriptor::Time_Slicing_indicator()const{return dataPointer[6]&0x08;}
bool Descriptor::MPE_FEC_indicator()const{return dataPointer[6]&0x04;}
uint8 Descriptor::constellation()const{return (dataPointer[7]&0xC0)>>6;}
uint8 Descriptor::hierarchy_information()const{return (dataPointer[7]&0x38)>>3;}
uint8 Descriptor::code_rate_HP_stream()const{return dataPointer[7]&0x07;}
uint8 Descriptor::code_rate_LP_stream()const{return (dataPointer[8]&0xE0)>>5;}
uint8 Descriptor::guard_interval()const{return (dataPointer[8]&0x18)>>3;}
uint8 Descriptor::transmission_mode()const{return (dataPointer[8]&0x06)>>1;}
bool Descriptor::other_frequency_flag()const{return dataPointer[8]&0x01;}

//0x79
bool Descriptor::scrambling_sequence_selector()const{return dataPointer[2]&0x80;}
bool Descriptor::multiple_input_stream_flag()const{return dataPointer[2]&0x40;}
bool Descriptor::backwards_compatibility_indicator()const{return dataPointer[2]&0x20;}
uint32 Descriptor::scrambling_sequence_index()const
{
	return ((uint32)dataPointer[3]&0x03)<<16|
			(uint32)dataPointer[4]<<8|
			(uint32)dataPointer[5];
}
uint8 Descriptor::input_stream_identifier()const
{
	return scrambling_sequence_selector()?dataPointer[6]:dataPointer[3];
}

unsigned DescriptorsList::parseData(uchar *pointer, unsigned length, unsigned offset)
{
	DataBlock::parseData(pointer,length,offset);

	//parse
	unsigned pos=0;
	m_descriptorsCount=0;
	while(pos<length)
	{
		Descriptor dsc;
		if(dsc.parseData(&dataPointer[pos],length-pos)==0)return 0;
		m_descriptorData[m_descriptorsCount]=dsc;
		++m_descriptorsCount;
		pos += dsc.dataLength;
	}

	//check(pos==len is correct)
	if(pos>length)return 0;
	else return dataLength;
}
uint8 DescriptorsList::descriptorsCount()const{return m_descriptorsCount;}
Descriptor DescriptorsList::descriptor(uint8 index)const{return Descriptor(m_descriptorData[index]);}