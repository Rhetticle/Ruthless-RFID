/*
 * MFRC.h
 *
 *  Created on: Jan 30, 2024
 *      Author: rhett
 */

#ifndef INC_MFRC_H_
#define INC_MFRC_H_

#define MFRC_ADDR 0b0101000
#define NRST GPIO_PIN_5 //Port B

#include "stm32f4xx.h"
#include "ruthlessfs.h"

typedef enum{
	PCD_COMM_ERR= 0xAA,
	PCD_I2C_ERR = 0xBB,
	PCD_NO_PICC = 0x01,
	PCD_PROTECTED_ERR = 0x02,
	PCD_VERIFY_ERR = 0x03,
	PCD_OK      = 0xCC,
	TIMEOUT_ERR = 0xDD,
	CRC_ERR     = 0xEE,
	FIFO_ERR    = 0xFF,
	NOROOM_ERR  = 0x00,
}PCD_StatusTypeDef;

//OPCODES for MIFARE ULTRALIGHT

#define ULTRA_READ 0x30
#define ULTRA_WRITE 0xA2
#define ULTRA_ATQA0 0x44 //First byte of ATQA response should be 0x44
#define ULTRA_ATQA1 0x00 //Second byte of ATQA response should be 0x00
#define ULTRA_HALTA 0x50

//MIFARE ULTRALIGHT Card characteristics

#define UL_MEMSIZE 64 //16 pages each containing 4 bytes
#define UL_UIDSIZE 7 //7 Byte UID
#define UL_DATASIZE 48 //44 bytes of user data (Pages 4 to 15)
#define UL_READSIZE 16 //A UL read will give 16 bytes back (excluding 2 CRC bytes)
#define UL_WRITESIZE 4 //A UL write can only do 4 bytes at a time
#define UL_PAGESIZE 4 //4 bytes in a page
#define UL_DATASTART 0x04 //Start address of data memory
#define UL_DATAEND 0x0F //last page
//Register Addresses

#define CMD_REG 0x01
#define IRQ_REG 0x04
#define DIVIRQ 0x05
#define ERRORREG 0x06
#define STAT2 0x08
#define FIFO_DATA 0x09
#define FIFO_LEV 0x0A
#define BITFRAME 0x0D
#define COLLREG 0x0E
#define MODE_REG 0x11
#define TX_REG 0x12
#define RX_REG 0x13
#define TX_CONT 0x14
#define TXASK 0x15
#define CRCH 0x21
#define CRCL 0x22
#define MODWIDTH 0x24
#define RFCfgReg 0x26
#define MODGS 0x28
#define TModeReg 0x2A
#define TPrescalerRegLO 0x2B
#define TReloadHI 0x2C
#define TReloadLO 0x2D

//Commands for command register (Lower nibble) !!Commands are 4 bits long and must be OR'ed with other 4 bits before writing to register!!

#define IDLE 0x00
#define MEM 0x01 //store 25 bytes into internal buffer
#define RAND_ID 0x02 //Generates a random 10 byte number
#define CALCCRC 0x03 //Activates CRC coprocessor
#define TRANSMIT 0x04 //Transmit data from FIFO buffer
#define NOCMDCHNG 0x07 //No command change
#define RECEIVE 0x08 //Activates receiver circuitry
#define TRANSCEIVE 0x0C //Transmits from FIFO buffer and immediately activates receiver
#define MFAUTH 0x0E //Performs MIFARE reader authentication
#define SRST 0x0F //Soft reset

//Function Prototypes

PCD_StatusTypeDef MFRC_REGW(uint8_t addr,uint8_t data);
PCD_StatusTypeDef MFRC_REGR(uint8_t addr,uint8_t* data);
PCD_StatusTypeDef MFRC_FIFOW(uint8_t* data,uint8_t size);
PCD_StatusTypeDef MFRC_FIFOR(uint8_t* data,uint8_t size);
void MFRC_Test(void);
void MFRC_ANTON(void);
void MFRC_ANTOFF(void);
void Print(char* mess);
char* uid_tostring(uint8_t* uid, uint8_t size);
PCD_StatusTypeDef MFRC_INIT(void);
PCD_StatusTypeDef MFRC_TRANSCEIVE(uint8_t* sendData,uint8_t sendsize,uint8_t* recdata,uint8_t recsize,uint8_t validbits);
PCD_StatusTypeDef MFRC_REQA(uint8_t* response);
PCD_StatusTypeDef MFRC_WUPA(uint8_t* response);
PCD_StatusTypeDef MFRC_HALTA(void);
PCD_StatusTypeDef MFRC_ANTICOL1(uint8_t* reponse);
PCD_StatusTypeDef MFRC_SEL1(uint8_t* anticol,uint8_t* response);
PCD_StatusTypeDef MFRC_ANTICOL2(uint8_t* reponse);
PCD_StatusTypeDef MFRC_SEL2(uint8_t* anticol,uint8_t* response);
PCD_StatusTypeDef PICC_Select(void);
PCD_StatusTypeDef PICC_CHECK(void);
PCD_StatusTypeDef UL_READ(uint8_t addr,uint8_t* data);
PCD_StatusTypeDef UL_WRITE(uint8_t addr,uint8_t* data);
PCD_StatusTypeDef DumpINFO(uint8_t* data);
PCD_StatusTypeDef UL_getuid(uint8_t* uid);
PCD_StatusTypeDef UL_getuidstr(char* uid_str);
PCD_StatusTypeDef UL_getalldata(uint8_t* data);
PCD_StatusTypeDef UL_readcard(Card* result);
PCD_StatusTypeDef UL_writecard(Card* towrite);
PCD_StatusTypeDef UL_verify(Card* check);
#endif /* INC_MFRC_H_ */
