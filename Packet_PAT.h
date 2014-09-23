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
		 * @brief 构造函数
		 * @param data 原始数据地址
		 */
		ProgramAssociation(const uchar *data=0);//构造函数
		inline uint16 program_number()const;	//节目号,0表示对应的PID应该是NIT的PID
		inline uint16 pid()const;				//若节目号为0,此PID表示NIT的PID,否则表示PMT的PID
		const uchar *pointer;					//指向记录的指针
	};

	uint8 programAssociationCount()const;					//表中的关联数量
	ProgramAssociation programAssociation(uint8 index)const;//通过index来获取某个记录

	bool getAssociationNumber(uint16 pid,uint16 *number=NULL)const;			//根据pid反查节目号
	bool getAssociationPID(uint16 number,uint16 *pid=NULL)const;			//根据节目号反查pid

	uint32 CRC_32()const;

	//辅助函数
	/**
	 * @brief 判断NIT的PID是否存在表里
	 * @param value 如果NIT的PID存在,且value不为NULL,则NIT的PID会写入value所指向的地址
	 * @return 是否存在NIT的PID
	 */
	bool getNITpid(uint16 *value=NULL)const;
	/**
	 * @brief getPMTpid 根据节目号判断表里是否存在对应的包ID
	 * @param number 节目号,为0时返回false
	 * @param pid 输出指针,当非NULL时会把包ID输出到pid所指向的地方
	 * @return 是否存在指定的pid
	 */
	bool getPMTpid(uint16 number,uint16 *pid=NULL)const;
	/**
	 * @brief getPMTnumber 根据包ID判断表里是否存在对应的节目号
	 * @param pid 包ID
	 * @param number 输出指针,当非NULL时会把节目号输出到number所指向的地方
	 * @return 是否存在指定的number
	 */
	bool getPMTnumber(uint16 pid,uint16 *number=NULL)const;
private:
	uchar *crc32;
};

//自带有缓存的PAT类
class PacketPAT:public Packet_PAT
{
public:
	unsigned parseData(const uchar* data,unsigned len);//将数据拷入后再分析
private:
	uchar rawData[1024];
};

#endif // PACKET_PAT_H