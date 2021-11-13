#include "crc.hpp"

void CRC16Init()
{
	uint32_t cnt = 0;
	static const uint16_t polynomial = 0x1021;

	for ( ; cnt < CRC16_TABLE_LENGTH; cnt++) {
		uint16_t crcItemValue = 0;
		int shifted = cnt << 8;
		int bitCounter ;
		for ( bitCounter = 0; bitCounter < 8; bitCounter++) {
			if (((crcItemValue  ^ shifted) & 0x8000) != 0 ){
				crcItemValue = ((crcItemValue << 1) ^ polynomial);
			} else {
				crcItemValue = crcItemValue << 1;
			}
			shifted <<= 1;
			crcItemValue &= 0xffff;
		}
		crc16Table[cnt] = crcItemValue;
	}
}

uint16_t CRC16(uint8_t* ptr, uint32_t length)
{
	uint32_t cnt;
	uint16_t crc16 = 0x0000; //zero init! uber important!
	
	for ( cnt = 0; cnt < length; cnt++)	{
		uint8_t item = ptr[cnt];
		// xor the next data byte with the high byte of what we have so far to
		// look up the scrambler.
		// xor that with the low byte of what we have so far.
		// Mask back to 16 bits.
		crc16 = ((crc16Table[(item ^ (crc16 >> 8)) & 0xff] ^ (crc16 << 8)) & 0xffff);
	}
	return crc16;
}