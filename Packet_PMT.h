#ifndef PACKET_PMT_H
#define PACKET_PMT_H


#include <list>
using namespace std;

#include "Descriptor.h"

class Packet_PMT:public DataBlock
{
public:
	Packet_PMT();

	unsigned parseData(const uchar *data, unsigned len);
//private:
	inline uint8 table_id()const;
	inline bool section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint16 program_number()const;
	inline uint8 version_number()const;
	inline bool current_next_indicator()const;
	inline uint8 section_number()const;
	inline uint8 last_section_number()const;
	inline uint16 PCR_PID()const;
	inline uint16 program_info_length()const;

	DescriptorsList programInfoDescriptorsList()const;

	struct Stream
	{
		Stream(const uchar *data=0);
		inline uint8 stream_type()const;
		inline uint16 elementary_PID()const;
		inline uint16 ES_info_length()const;
		DescriptorsList esInfoDescriptorList()const;
		//
		bool isVideoStreamType()const;
		bool isAudioStreamType()const;

		const uchar *pointer;
		DescriptorsList m_descriptorsList;
	};
	list<Stream> pmList;
	bool containElementary_pid(uint16 pid)const;
protected:
	void resetPointer();
private:
	DescriptorsList m_descriptorsList;
	uchar* streams;
	uchar* ES_info_descriptors;
	uchar* crc32;
};

//自带有缓存的PMT类
class TSPACKETS_EXPORT PacketPMT:public Packet_PMT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//将数据拷入后再分析
private:
	uchar rawData[1024];
};

#endif // PACKET_PMT_H