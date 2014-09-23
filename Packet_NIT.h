#ifndef PACKET_NIT_H
#define PACKET_NIT_H

#include "Descriptor.h"
#include "Packet_PSI.h"

#include <list>
using namespace std;

class Packet_NIT:public DataBlock
{
public:
	Packet_NIT();

	unsigned parseData(uchar *pointer, unsigned length, unsigned offset=0);
	unsigned reset();

	inline uint8 table_id()const;
	inline bool section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint16 network_id()const;
	inline uint8 version_number()const;
	inline bool current_next_indicator()const;
	inline uint8 section_number()const;
	inline uint8 last_section_number()const;
	inline uint16 network_descriptors_length()const;

	inline DescriptorsList networkDescriptorsList()const;

	inline uint16 transport_stream_loop_length()const;
	struct TransportStreamLoop:public DataBlock
	{
		TransportStreamLoop();
		unsigned parseData(const uchar *data);
		unsigned parseData(const uchar *data,unsigned len);

		inline uint16 transport_stream_id()const;
		inline uint16 original_network_id()const;
		inline uint16 transport_descriptors_length()const;

		DescriptorsList transportDescriptorsList()const;
	private:
		DescriptorsList m_descriptorsList;
	};
	uint8 transportStreamLoopCount()const;
	TransportStreamLoop transportStreamLoop(uint8 index)const;

	inline uint32 CRC_32()const;
private:
	DescriptorsList m_networkDescriptorsList;

	uchar *m_transportStreamLoops;
	uint8 m_transportStreamsCount;
	TransportStreamLoop m_transportStreams[10];

	uchar *crc32;
};

//自带有缓存的NIT类
class PacketNIT:public Packet_NIT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//将数据拷入后再分析
	Packet_PSI packetPSI;
private:
	uchar rawData[1024];
};
#endif // PACKET_NIT_H