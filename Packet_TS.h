#ifndef PACKET_TS_H
#define PACKET_TS_H

#include"DataBlock.h"

class DataBlock_TsHeader:public DataBlock
{
public:
	unsigned parseData(uchar *pointer, unsigned length, unsigned offset=0);

	inline uint8 sync_byte()const;
	inline void setSync_byte(uint8 value=0x47);

	DATABLOCK_BOOL_GET_SET_BIGENDIAN(TransportErrorIndicator,1,0)
	DATABLOCK_BOOL_GET_SET_BIGENDIAN(PayloadUnitStartIndicator,1,1)
	DATABLOCK_BOOL_GET_SET_BIGENDIAN(TransportPriority,1,2)

	inline uint16 pid()const;
	inline void setPid(uint16 value);

	DATABLOCK_BOOL_GET_SET_BIGENDIAN(TransportScramblingControl_0,3,0)
	DATABLOCK_BOOL_GET_SET_BIGENDIAN(TransportScramblingControl_1,3,1)

	DATABLOCK_BOOL_GET_SET_BIGENDIAN(AdaptationFieldControl_adaptationField,3,2)
	DATABLOCK_BOOL_GET_SET_BIGENDIAN(AdaptationFieldControl_payload,3,3)

	inline uint8 transport_scrambling_control()const;
	inline void setTransport_scrambling_control(uint8 value);

	inline uint8 adaptation_field_control()const;
	inline void setAdaptation_field_control(uint8 value);

	inline uint8 continuity_counter()const;
	inline void setContinuity_counter(uint8 value);
};
class DataBlock_AdaptionField:public DataBlock
{
public:
	unsigned parseData(uchar *pointer, unsigned length, unsigned offset);

	uint8 adaptation_field_length()const;
	void setAdaptation_field_length(uint8 value);

	//flag
	bool discontinuity_indicator()const;
	void setDiscontinuity_indicator(bool value);

	bool random_access_indicator()const;
	void setRandom_access_indicator(bool value);

	bool elementary_stream_priority_indicator()const;
	void setElementary_stream_priority_indicator(bool value);

	bool PCR_flag()const;
	void setPCR_flag(bool value);

	bool OPCR_flag()const;
	void setOPCR_flag(bool value);

	bool splicing_point_flag()const;
	void splicing_point_flag(bool value);

	bool transport_private_data_flag()const;
	void setTransport_private_data_flag(bool value);

	bool adaptation_field_extension_flag()const;
	void setAdaptation_field_extension_flag(bool value);
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