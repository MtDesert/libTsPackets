#ifndef PACKET_PSI_H
#define PACKET_PSI_H

#include"DataBlock.h"

class Packet_PSI:public DataBlock
{
public:
	Packet_PSI();

	unsigned parseData(const uchar *data, unsigned len);
	uint8 pointer_field()const;
};

#endif // PACKET_PSI_H
