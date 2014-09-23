#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include"DataBlock.h"

#include <string>
#include <list>
using namespace std;

/**
 * @brief 描述子,用于携带描述信息的
 * 一个描述子都会带有描述类型,描述长度,描述正文三个部分,本类有很多方法
 */
class Descriptor:public DataBlock
{
public:
	Descriptor();

	unsigned parseData(uchar *pointer, unsigned length, unsigned offset=0);

	uint8 descriptor_tag()const;
	uint8 descriptor_length()const;

	//0x40
	string networkName()const;//网络名称

	//0x43
	uint32 frequency()const;//频率
	uint16 orbital_position()const;
	uint8 west_east_flag()const;//西东标记
	uint8 polarization()const;
	uint8 roll_off()const;
	uint8 modulation_system()const;
	uint8 modulation_type()const;
	uint32 symbol_rate()const;
	uint8 FEC_inner()const;
	
	//0x44
	//uint32 frequency()const; 0x43
	uint8 FEC_outer()const;
	uint16 modulation()const;
	//uint32 symbol_rate()const; 0x43
	//uint8 FEC_inner()const; 0x43

	//0x48
	uint8 service_type()const;
	uint8 service_provider_name_length()const;
	string service_provider_name()const;
	uint8 service_name_length()const;
	string service_name()const;

	//0x4A
	uint16 transport_stream_id()const;
	uint16 original_network_id()const;
	uint16 service_id()const;
	uint8 linkage_type()const;

	//0x5A
	uint32 centre_frequency()const;
	uint8 bandwidth()const;
	bool priority()const;
	bool Time_Slicing_indicator()const;
	bool MPE_FEC_indicator()const;
	uint8 constellation()const;
	uint8 hierarchy_information()const;
	uint8 code_rate_HP_stream()const;
	uint8 code_rate_LP_stream()const;
	uint8 guard_interval()const;
	uint8 transmission_mode()const;
	bool other_frequency_flag()const;

	//0x79
	bool scrambling_sequence_selector()const;
	bool multiple_input_stream_flag()const;
	bool backwards_compatibility_indicator()const;
	uint32 scrambling_sequence_index()const;
	uint8 input_stream_identifier()const;

	enum DescriptorType
	{
		network_name_descriptor=0x40,
		service_list_descriptor=0x41,
		stuffing_descriptor=0x42,
		satellite_delivery_system_descriptor=0x43,
		cable_delivery_system_descriptor=0x44,
		VBI_data_descriptor=0x45,
		VBI_teletext_descriptor=0x46,
		bouquet_name_descriptor=0x47,
		service_descriptor=0x48,
		country_availability_descriptor=0x49,
		linkage_descriptor=0x4A,
		NVOD_reference_descriptor=0x4B,
		time_shifted_service_descriptor=0x4C,
		short_event_descriptor=0x4D,
		extended_event_descriptor=0x4E,
		time_shifted_event_descriptor=0x4F,
		component_descriptor=0x50,
		mosaic_descriptor=0x51,
		stream_identifier_descriptor=0x52,
		CA_identifier_descriptor=0x53,
		content_descriptor=0x54,
		parental_rating_descriptor=0x55,
		teletext_descriptor=0x56,
		telephone_descriptor=0x57,
		local_time_offset_descriptor=0x58,
		subtitling_descriptor=0x59,
		terrestrial_delivery_system_descriptor=0x5A,
		multilingual_network_name_descriptor=0x5B,
		multilingual_bouquet_name_descriptor=0x5C,
		multilingual_service_name_descriptor=0x5D,
		multilingual_component_descriptor=0x5E,
		private_data_specifier_descriptor=0x5F,
		service_move_descriptor=0x60,
		short_smoothing_buffer_descriptor=0x61,
		frequency_list_descriptor=0x62,
		partial_transport_stream_descriptor=0x63,
		data_broadcast_descriptor=0x64,
		scrambling_descriptor=0x65,
		data_broadcast_id_descriptor=0x66,
		transport_stream_descriptor=0x67,
		DSNG_descriptor=0x68,
		PDC_descriptor=0x69,
		AC3_descriptor=0x6A,
		ancillary_data_descriptor=0x6B,
		cell_list_descriptor=0x6C,
		cell_frequency_link_descriptor=0x6D,
		announcement_support_descriptor=0x6E,
		application_signalling_descriptor=0x6F,
		adaptation_field_data_descriptor=0x70,
		service_identifier_descriptor=0x71,
		service_availability_descriptor=0x72,
		default_authority_descriptor=0x73,
		related_content_descriptor=0x74,
		TVA_id_descriptor=0x75,
		content_identifier_descriptor=0x76,
		time_slice_fec_identifier_descriptor=0x77,
		ECM_repetition_rate_descriptor=0x78,
		S2_satellite_delivery_system_descriptor=0x79,
		enhanced_AC_3_descriptor=0x7A,
		DTS_descriptor=0x7B,
		AAC_descriptor=0x7C,
		XAIT_location_descriptor=0x7D,
		FTA_content_management_descriptor=0x7E,
		extension_descriptor=0x7F,
		user_defined,//0x80 to 0xFE
		forbidden=0xFF
	};
};

class DescriptorsList:public DataBlock
{
public:
	unsigned parseData(uchar *pointer, unsigned length, unsigned offset=0);

	uint8 descriptorsCount()const;
	Descriptor descriptor(uint8 index)const;
private:
	Descriptor m_descriptorData[100];
	uint8 m_descriptorsCount;
};
#endif // DESCRIPTOR_H