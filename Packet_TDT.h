#ifndef PACKET_TDT_H
#define PACKET_TDT_H

#include"DataBlock.h"

class Packet_TDT:public DataBlock
{
public:
	Packet_TDT();

	unsigned parseData(const uchar* data,unsigned len);
//private:
	inline uint8 table_id()const;
	inline uint8 section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint64 UTC_time()const;
};

class PacketTDT:public Packet_TDT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//将数据拷入后再分析
private:
	uchar rawData[1024];
};

#endif // PACKET_TDT_H