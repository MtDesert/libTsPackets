#ifndef PACKET_TS_H
#define PACKET_TS_H

#include"DataBlock.h"

class DataBlock_TsHeader:public DataBlock
{
public:
	unsigned parseData(uchar *pointer, unsigned length, unsigned offset=0);

	DATABLOCK_CUSTOM_ATTRIBUTE_BIGENDIAN(SyncByte,uint8,0)
	DATABLOCK_BOOL_BIGENDIAN(TransportErrorIndicator,1,0)
	DATABLOCK_BOOL_BIGENDIAN(PayloadUnitStartIndicator,1,1)
	DATABLOCK_BOOL_BIGENDIAN(TransportPriority,1,2)
	DATABLOCK_BITS_VALUE_BIGENDIAN(Pid,1,3,13)
	DATABLOCK_BOOL_BIGENDIAN(TransportScramblingControl_0,3,0)
	DATABLOCK_BOOL_BIGENDIAN(TransportScramblingControl_1,3,1)
	DATABLOCK_BOOL_BIGENDIAN(AdaptationFieldControl_adaptationField,3,2)
	DATABLOCK_BOOL_BIGENDIAN(AdaptationFieldControl_payload,3,3)
	DATABLOCK_BITS_VALUE_BIGENDIAN(ContinuityCounter,3,4,4)
};
class DataBlock_AdaptionField:public DataBlock
{
public:
	unsigned parseData(uchar *pointer, unsigned length, unsigned offset);

	DATABLOCK_BITS_VALUE_BIGENDIAN(AdaptationFieldLength,0,0,8)

	DATABLOCK_BOOL_BIGENDIAN(DiscontinuityIndicator,1,0)
	DATABLOCK_BOOL_BIGENDIAN(RandomAccessIndicator,1,1)
	DATABLOCK_BOOL_BIGENDIAN(ElementaryStreamPriorityIndicator,1,2)
	DATABLOCK_BOOL_BIGENDIAN(PcrFlag,1,3)
	DATABLOCK_BOOL_BIGENDIAN(OpcrFlag,1,4)
	DATABLOCK_BOOL_BIGENDIAN(SplicingPointFlag,1,5)
	DATABLOCK_BOOL_BIGENDIAN(TransportPrivateDataFlag,1,6)
	DATABLOCK_BOOL_BIGENDIAN(AdaptationFieldExtentionFlag,1,7)
};

class Packet_TS:public DataBlock
{
public:
	Packet_TS();
	unsigned parseData(uchar *pointer, unsigned length, unsigned offset=0);

	uint8 adaptation_field_length()const;
	//flag
	bool discontinuity_indicator()const;
	bool random_access_indicator()const;
	bool elementary_stream_priority_indicator()const;
	bool PCR_flag()const;
	bool OPCR_flag()const;
	bool splicing_point_flag()const;
	bool transport_private_data_flag()const;
	bool adaptation_field_extension_flag()const;

	//pcr_flag
	uint64 program_clock_reference_base()const;
	uint16 program_clock_reference_extension()const;
	uint64 pcrValue()const;

	//opcr_flag
	uint64 original_program_clock_reference_base()const;
	uint16 original_program_clock_reference_extension()const;
	uint64 opcrValue()const;

	//splicing_point_flag
	uint8 splice_countdown()const;

	//transport_private_data_flag
	uint8 transport_private_data_length()const;
	uchar *private_data_byte;

	//adaptation_field_extension_flag
	uint8 adaptation_field_extension_length()const;
	uint8 ltw_flag()const;
	uint8 piecewise_rate_flag()const;
	uint8 seamless_splice_flag()const;

	//ltw_flag
	uint8 ltw_valid_flag()const;
	uint16 ltw_offset()const;

	//piecewise_rate_flag;
	uint32 piecewise_rate()const;

	//seamless_splice_flag
	uint8 splice_type()const;
	uint64 DTS_next_AU()const;
private:
	DataBlock_TsHeader header;
	DataBlock_AdaptionField adaptionField;
	DataBlock payload;//Maybe it will be stuff byte

	uchar *adaptation_field;//xxxOffset()
	uchar *pcrVal;
	uchar *opcrVal;
	uchar *splice;
	uchar *transport_private_data_len;
	uchar *adaptation_field_extension;
	uchar *ltw;
	uchar *piecewiseRate;
	uchar *seamless_splice;
	uchar *stuffing_byte;
	//uchar *payload;
};

class PacketTS:public Packet_TS
{
public:
	unsigned parseData(const uchar* data,unsigned len);
private:
	uchar rawData[1024];
};

#endif // PACKET_TS_H