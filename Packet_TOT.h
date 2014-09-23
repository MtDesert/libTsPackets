#ifndef PACKET_TOT_H
#define PACKET_TOT_H

#include "Descriptor.h"

class Packet_TOT:public DataBlock
{
public:
	Packet_TOT();

	unsigned parseData(const uchar* data,unsigned len);
	//bool fromRawData(const uint8 *data,unsigned len);
//private:
	inline uint8 table_id()const;
	inline uint8 section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint64 UTC_time()const;
	uint16 descriptors_loop_length()const;

	//inline uint8 descriptorsLoopCount()const;
	//Descriptor descriptor(uint8 index)const;

	inline uint32 CRC_32()const;
protected:
	void resetPointer();
private:
	uchar *descriptors;
	uchar *crc32;
};

//自带有缓存的TOT类
class PacketTOT:public Packet_TOT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//将数据拷入后再分析
private:
	uchar rawData[1024];
};

#endif // PACKET_TOT_H