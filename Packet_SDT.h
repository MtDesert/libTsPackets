#ifndef PACKET_SDT_H
#define PACKET_SDT_H

#include <string>
#include <list>
using namespace std;

#include "Packet_PSI.h"
#include "Descriptor.h"

class Packet_SDT:public DataBlock
{
public:
	Packet_SDT();

	unsigned parseData(const uchar* data,unsigned len);
	unsigned reset();
	
	inline uint8 table_id()const;
	inline bool section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint16 transport_stream_id()const;
	inline uint8 version_number()const;
	inline bool current_next_indicator()const;
	inline uint8 section_number()const;
	inline uint8 last_section_number()const;
	inline uint16 original_network_id()const;

	struct TSPACKETS_EXPORT ServiceDescriptor:public Packet//����һ������������
	{
		ServiceDescriptor();
		unsigned parseData(const uchar *data, unsigned len);

		uint16 service_id()const;
		bool EIT_echedule_flag()const;
		bool EIT_present_following_flag()const;
		uint8 running_status()const;
		bool free_CA_mode()const;
		uint16 descriptors_loop_length()const;
		
		DescriptorsList descriptors;
	};
	uint8 serviceDescriptorsCount()const;
	ServiceDescriptor serviceDescriptor(uint8 index)const;

	inline uint32 CRC_32()const;//CRCУ�����β��
private:
	uint8 m_serviceDescriptorsCount;
	ServiceDescriptor m_serviceDescriptors[64];
	uchar* crc32;
};

//�Դ��л����SDT��
class TSPACKETS_EXPORT PacketSDT:public Packet_SDT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//�����ݿ�����ٷ���
	Packet_PSI packetPSI;
private:
	uchar rawData[1280];
};
#endif // PACKET_SDT_H