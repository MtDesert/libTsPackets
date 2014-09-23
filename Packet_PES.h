#ifndef PACKET_PES_H
#define PACKET_PES_H

#include"DataBlock.h"

class Packet_PES:public DataBlock
{
public:
	Packet_PES();
	//��������
	unsigned parseData(const uchar *data,unsigned len);
	//��������
	inline uint32 packet_start_code_prefix()const;
	inline uint8 stream_id()const;
	inline uint16 PES_packet_length()const;

	//��IDö��,�����ж�
	enum StreamId
	{
		program_stream_map=0xBC,
		private_stream_1=0xBD,
		padding_stream=0xBE,
		private_stream_2=0xBF,
		//audio_stream,110x xxxx
		//video_stream,1110 xxxx
		ECM_stream=0xF0,
		EMM_stream=0xF1,
		DSMCC_stream=0xF2,
		ISO_IEC_13522_stream=0xF3,
		typeA_stream=0xF4,
		typeB_stream=0xF5,
		typeC_stream=0xF6,
		typeD_stream=0xF7,
		typeE_stream=0xF8,
		ancillary_stream=0xF9,
		SL_packetized_stream=0xFA,
		FlexMux_stream=0xFB,
		metadata_stream=0xFC,
		extended_stream_id=0xFD,
		reserved_data_stream=0xFE,
		program_stream_directory=0xFF
	};

	bool containExtention()const;//�ж��Ƿ������չͷ��
	//��չ����
	uint8 PES_scrambling_control()const;
	bool PES_priority()const;
	bool data_alignment_indicator()const;
	bool copyright()const;
	bool original_or_copy()const;
	//flag
	uint8 PTS_DTS_flags()const;
	bool ESCR_flag()const;
	bool ES_rate_flag()const;
	bool DSM_trick_mode_flag()const;
	bool additional_copy_info_flag()const;
	bool PES_CRC_flag()const;
	bool PES_extension_flag()const;
	//length
	uint8 PES_header_data_length()const;

	uint64 getPTSvalue()const;
	uint64 getDTSvalue()const;
private:
	void resetPointer();
	//������ָ��ĳЩ���ݵ�ָ��,Ϊ��ָ��ʱ��ʾ������
	uchar *extention;
	uchar *ptsValue;
	uchar *dtsValue;
};
#endif // PACKET_PES_H