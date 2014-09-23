#ifndef PACKET_PAT_H
#define PACKET_PAT_H

#include "typedef.h"
#include"DataBlock.h"

/**
 * @brief This is Packet_PAT class. Its PID is 0
 */
class Packet_PAT:public DataBlock
{
public:
	Packet_PAT();
	Packet_PAT(const uchar* data,unsigned len);

	unsigned parseData(const uchar* data,unsigned len);
	unsigned reset();

	inline uint8 table_id()const;
	inline bool section_syntax_indicator()const;
	inline uint16 section_length()const;
	inline uint16 transport_stream_id()const;
	inline uint8 version_number()const;
	inline bool current_next_indicator()const;
	inline uint8 section_number()const;
	inline uint8 last_section_number()const;

	struct ProgramAssociation
	{
		/**
		 * @brief ���캯��
		 * @param data ԭʼ���ݵ�ַ
		 */
		ProgramAssociation(const uchar *data=0);//���캯��
		inline uint16 program_number()const;	//��Ŀ��,0��ʾ��Ӧ��PIDӦ����NIT��PID
		inline uint16 pid()const;				//����Ŀ��Ϊ0,��PID��ʾNIT��PID,�����ʾPMT��PID
		const uchar *pointer;					//ָ���¼��ָ��
	};

	uint8 programAssociationCount()const;					//���еĹ�������
	ProgramAssociation programAssociation(uint8 index)const;//ͨ��index����ȡĳ����¼

	bool getAssociationNumber(uint16 pid,uint16 *number=NULL)const;			//����pid�����Ŀ��
	bool getAssociationPID(uint16 number,uint16 *pid=NULL)const;			//���ݽ�Ŀ�ŷ���pid

	uint32 CRC_32()const;

	//��������
	/**
	 * @brief �ж�NIT��PID�Ƿ���ڱ���
	 * @param value ���NIT��PID����,��value��ΪNULL,��NIT��PID��д��value��ָ��ĵ�ַ
	 * @return �Ƿ����NIT��PID
	 */
	bool getNITpid(uint16 *value=NULL)const;
	/**
	 * @brief getPMTpid ���ݽ�Ŀ���жϱ����Ƿ���ڶ�Ӧ�İ�ID
	 * @param number ��Ŀ��,Ϊ0ʱ����false
	 * @param pid ���ָ��,����NULLʱ��Ѱ�ID�����pid��ָ��ĵط�
	 * @return �Ƿ����ָ����pid
	 */
	bool getPMTpid(uint16 number,uint16 *pid=NULL)const;
	/**
	 * @brief getPMTnumber ���ݰ�ID�жϱ����Ƿ���ڶ�Ӧ�Ľ�Ŀ��
	 * @param pid ��ID
	 * @param number ���ָ��,����NULLʱ��ѽ�Ŀ�������number��ָ��ĵط�
	 * @return �Ƿ����ָ����number
	 */
	bool getPMTnumber(uint16 pid,uint16 *number=NULL)const;
private:
	uchar *crc32;
};

//�Դ��л����PAT��
class PacketPAT:public Packet_PAT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//�����ݿ�����ٷ���
private:
	uchar rawData[1024];
};

#endif // PACKET_PAT_H