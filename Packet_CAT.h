#ifndef PACKET_CAT_H
#define PACKET_CAT_H

#include"Descriptor.h"

class Packet_CAT:public DataBlock
{
public:
	Packet_CAT();

	unsigned parseData(const uchar *data, unsigned len);
	unsigned reset();

	inline uint8 table_id()const;
	inline bool section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint8 version_number()const;
	inline bool current_next_indicator()const;
	inline uint8 section_number()const;
	inline uint8 last_section_number()const;

	inline uint32 CRC_32()const;
private:
	uchar *crc32;
};

//自带有缓存的CAT类
class PacketCAT:public Packet_CAT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//将数据拷入后再分析
private:
	uchar rawData[1024];
};

#endif // PACKET_CAT_H