/*
 * MFRC.c
 *
 *  Created on: Jan 30, 2024
 *      Author: rhett
 */


#include "MFRC.h"
#include "stm32f4xx.h"
#include "usbd_cdc_if.h"
#include "OLED.h"

extern I2C_HandleTypeDef hi2c1;



/* Write single byte MFRC register
 *
 * @param addr: register address
 *
 * @param data: value to write to register
 */

PCD_StatusTypeDef MFRC_REGW(uint8_t addr,uint8_t data){
	uint8_t value=data;
	if(HAL_I2C_Mem_Write(&hi2c1, MFRC_ADDR<<1, addr, 1, &value, 1, 100)!=HAL_OK){
		return(PCD_I2C_ERR);
	}
	else{
		return(PCD_OK);
	}
	HAL_Delay(1);
}

/* Read single byte from MFRC register
 *
 * @param addr: register address
 *
 * @param data: pointer to store read value
 */

PCD_StatusTypeDef MFRC_REGR(uint8_t addr,uint8_t* data){
	if(HAL_I2C_Mem_Read(&hi2c1, MFRC_ADDR<<1, addr, 1, data, 1, 100)!=HAL_OK){
		return(PCD_I2C_ERR);
	}
	else{
		return(PCD_OK);
	}
	HAL_Delay(1);
}

/* Write to MFRC FIFO buffer
 *
 * @param data: Array of data to write to FIFO
 *
 * @param size: Size of array (bytes)
 */

PCD_StatusTypeDef MFRC_FIFOW(uint8_t* data,uint8_t size){
	for(int i=0;i<size;i++){
		if(HAL_I2C_Mem_Write(&hi2c1, MFRC_ADDR<<1, 0x09, 1, &data[i], 1, 100)!=HAL_OK){
			return(PCD_I2C_ERR);

		}
		HAL_Delay(1);

	}
	return(PCD_OK);
}

/* Read from MFRC FIFO buffer
 *
 * @param data: Array to store read values (Ensure array is large enough)
 *
 * @param size: Number of bytes to read from FIFO
 */

PCD_StatusTypeDef MFRC_FIFOR(uint8_t* data,uint8_t size){
	for(int i=0;i<size;i++){
		if(HAL_I2C_Mem_Read(&hi2c1, MFRC_ADDR<<1, 0x09, 1, &data[i], 1, 100)!=HAL_OK){
			return(PCD_I2C_ERR);
		}
		HAL_Delay(1);

	}
	return(PCD_OK);
}

void MFRC_Test(void){
	uint8_t value;
	MFRC_REGR(0x37,&value); //Expect 0x91 or 0x92
	CDC_Transmit_FS(&value,1);
}

void MFRC_ANTON(void){
	uint8_t value;
	MFRC_REGR(TX_CONT, &value);
	if ((value & 0x03) != 0x03) {
		MFRC_REGW(TX_CONT, value | 0x03);
	}
}

void SET_ANTGAIN(void){
	uint8_t value;
	MFRC_REGR(RFCfgReg,&value);
	MFRC_REGW(RFCfgReg,value|0x70); //Set receiver gain to 48dB
}

void ClearBitMask(uint8_t addr,uint8_t mask){
	uint8_t tmp;
	MFRC_REGR(addr,&tmp);
	MFRC_REGW(addr,tmp&(~mask));
}

void MFRC_ANTOFF(void){
	ClearBitMask(TX_CONT,0x03);
}

/*
 * Function to calculate CRC on whatever data is in FIFO
 * !!CRCIRQ bit in DivIrqReg register can be used to detect when CRC has been calculated!!
 *
 * @param result: Array to store the two bytes of CRC
 *
 * */

PCD_StatusTypeDef CALC_CRC(uint8_t* data,uint8_t size,uint8_t* result){
	uint8_t CRCIRQ;
	MFRC_REGW(CMD_REG,IDLE); //clear command register
	MFRC_REGW(DIVIRQ,0x04); //Clear interrupt bits so we can detect when CRCIRQ is set
	MFRC_REGW(FIFO_LEV,0x80);
	MFRC_FIFOW(data, size); //Write data to FIFO ready for CRC calculation
	MFRC_REGW(CMD_REG,CALCCRC); //Execute CRC calculation command
	HAL_Delay(100);
	MFRC_REGR(DIVIRQ,&CRCIRQ);
	if((CRCIRQ&0x04)!=0x04){
		return(CRC_ERR); //CRC calculation took too long
	}
	MFRC_REGW(CMD_REG,IDLE);
	MFRC_REGR(CRCL, &result[0]);
	MFRC_REGR(CRCH, &result[1]); //Store the 16-bit CRC in result
	return(PCD_OK);

}

/* Initialise MFRC to begin transceiving
 *
 * Code is written to interface with MIFARE Ultralight PICC. Different PICCs will need different baud rate.
 */

PCD_StatusTypeDef MFRC_INIT(void){
	HAL_GPIO_WritePin(GPIOB, NRST, 1); //Make sure MFRC is not reset
	HAL_GPIO_WritePin(GPIOB, NRST, 0);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, NRST, 1);
	HAL_Delay(50);
	MFRC_REGW(TX_REG,0x00);
	MFRC_REGW(RX_REG,0x00);
	MFRC_REGW(MODWIDTH,0x26);
	MFRC_REGW(TXASK,0x40); //Force 100% ASK modulation regardless of ModGsPrereg
	MFRC_REGW(MODE_REG,0x3D);
	MFRC_ANTON();
	return(PCD_OK);



}


/* Transceive data between MFRC and PICC
 *
 * @param sendData: Array of data to send
 *
 * @param sendsize: Number of bytes to send
 *
 * @param recdata: Array to store received data
 *
 * @param recsize: Number of bytes to receive
 *
 * @param validbits: Number of valid bits within given byte in sendData
 *
 * !!Check PICC datasheet to see what data is needed in sendData i.e. Read/Write command, response bytes etc!!
 * !!When writing ensure correct address, most MIFARE PICCs contain OTP (one-time-programmable) sections as well as
 * lock bytes to remove write permissions from certain pages. These operations are NOT reversable!!
 */

PCD_StatusTypeDef MFRC_TRANSCEIVE(uint8_t* sendData,uint8_t sendsize,uint8_t* recdata,uint8_t recsize,uint8_t validbits){
	uint8_t IRQval=0;
	uint8_t BIT_val=0;

	MFRC_REGW(CMD_REG,IDLE); //Clear command register
	MFRC_REGW(IRQ_REG,0x7F);
	MFRC_REGW(FIFO_LEV,0x80); //Clear FIFO buffer
	MFRC_FIFOW(sendData,sendsize); //Write data to FIFO ready for transmission
	MFRC_REGW(BITFRAME,validbits);
	MFRC_REGW(CMD_REG,TRANSCEIVE); //Send FIFO data and receive PICC response
	MFRC_REGR(BITFRAME,&BIT_val);
	MFRC_REGW(BITFRAME,(BIT_val|0x80)); //Start send bit
	while((IRQval&0x30)!=0x30){ //Hang here until RXIRQ and IDLEIRQ bits are set
		MFRC_REGR(IRQ_REG,&IRQval);


	}
	HAL_Delay(1);
	MFRC_FIFOR(recdata,recsize); //Read and store received data


	return(PCD_OK);
}

/*
 * Function to send a REQA to detect if a PICC is present
 *
 * @param response: Array to store response from PICC
 *
 * */

PCD_StatusTypeDef MFRC_REQA(uint8_t* response){
	uint8_t REQ=0x26;
	ClearBitMask(COLLREG, 0x80);
	if(MFRC_TRANSCEIVE(&REQ,1, response, 2, 7)!=PCD_OK){//REQA is a 7-bit command
		return(PCD_COMM_ERR);
	}

	else{
		return(PCD_OK);
	}
}

/*
 * Function to issue the WUPA command to PICC (We use this to go from HALT to READY state check ISO standard)
 *
 * @param reponse: PICC reponse to WUPA (Expect 0x44, 0x00 for ULTRALIGHT
 * */

PCD_StatusTypeDef MFRC_WUPA(uint8_t* response){
	uint8_t WUPA=0x52;
	ClearBitMask(COLLREG, 0x80);
	if(MFRC_TRANSCEIVE(&WUPA,1, response, 2, 7)!=PCD_OK){//WUPA is a 7-bit command
		return(PCD_COMM_ERR);
	}

	else{
		return(PCD_OK);
	}
}

/*
 * Function to issue HALTA command to PICC (Change from READY to HALT state) Send WUPA and select sequence to re-select PICC
 * */

PCD_StatusTypeDef MFRC_HALTA(void){
	uint8_t transaction[4]={ULTRA_HALTA,0x00};
	uint8_t CRC_val[2];
	uint8_t ack;

	if(CALC_CRC(transaction, 2, CRC_val)!=PCD_OK){
		return(CRC_ERR);
	}
	memcpy(transaction+2,CRC_val,2);

	if(MFRC_TRANSCEIVE(transaction, 4, &ack, 1, 0)!=PCD_OK){
		return(PCD_COMM_ERR);
	}

	else{
		return(PCD_OK);
	}
}

/*
 * Function to transceive anticollision cascade level 1 command (Second step after REQA to select PICC)
 *
 * @param response: Array to store PICC response to anticollision command
 * */

PCD_StatusTypeDef MFRC_ANTICOL1(uint8_t* reponse){
	uint8_t transaction[]={0x93,0x20};
	ClearBitMask(COLLREG, 0x80);
	if(MFRC_TRANSCEIVE(transaction, 2, reponse, 5, 0)!=PCD_OK){
		return(PCD_COMM_ERR);
	}
	else{
		return(PCD_OK);
	}
}

/*
 * Function to perform SELECT command (Cascade level 1)
 *
 * @param anticol: Data the is retrieved from MFRC_ANTICOL1
 *
 * @param response: Array to store PICC response to command (Expect SAK of 0x04 and two bytes CRC)
 *
 * */

PCD_StatusTypeDef MFRC_SEL1(uint8_t* anticol,uint8_t* response){
	uint8_t transaction[9]={0x93,0x70};
	uint8_t CRC_val[2];

	memcpy(transaction+2,anticol,5);
	CALC_CRC(transaction, 7, CRC_val);

	memcpy(transaction+7,CRC_val,2);

	if(MFRC_TRANSCEIVE(transaction, 9, response, 3, 0)!=PCD_OK){
		return(PCD_COMM_ERR);
	}

	else{
		return(PCD_OK);
	}

}

/*
 * Function to transceive anticollision cascade level 2 command (Cascade level 1 commands must be performed first)
 *
 * @param response: Array to store PICC response to anticollision command
 * */

PCD_StatusTypeDef MFRC_ANTICOL2(uint8_t* reponse){
	uint8_t transaction[]={0x95,0x20};
	ClearBitMask(COLLREG, 0x80);
	if(MFRC_TRANSCEIVE(transaction, 2, reponse, 5, 0)!=PCD_OK){
		return(PCD_COMM_ERR);
	}
	else{
		return(PCD_OK);
	}
}

/*
 * Function to perform SELECT command (Cascade level 2)
 *
 * @param anticol: Data the is retrieved from MFRC_ANTICOL2
 *
 * @param response: Array to store PICC response to command (Expect SAK of 0x00 and two bytes CRC)
 *
 * */

PCD_StatusTypeDef MFRC_SEL2(uint8_t* anticol,uint8_t* response){
	uint8_t transaction[9]={0x95,0x70};
	uint8_t CRC_val[2];

	memcpy(transaction+2,anticol,5);
	CALC_CRC(transaction, 7, CRC_val);

	memcpy(transaction+7,CRC_val,2);

	if(MFRC_TRANSCEIVE(transaction, 9, response, 3, 0)!=PCD_OK){
		return(PCD_COMM_ERR);
	}

	else{
		return(PCD_OK);
	}

}

/*
 * Function to select the MIFARE ULTRALIGHT PICC (Don't forget to call REQA on first power up or use PICC_CHECK below)
 * */

PCD_StatusTypeDef PICC_Select(void){
	  uint8_t ANTICOL1[5];
	  uint8_t SELECT1[3];
	  uint8_t ANTICOL2[5];
	  uint8_t SELECT2[3];

	  MFRC_ANTICOL1(ANTICOL1);
	  if(ANTICOL1[0]!=0x88){
		  return(PCD_COMM_ERR);
	  }
	  HAL_Delay(10);
	  MFRC_SEL1(ANTICOL1, SELECT1);
	  HAL_Delay(10);
	  MFRC_ANTICOL2(ANTICOL2);
	  HAL_Delay(10);
	  MFRC_SEL2(ANTICOL2, SELECT2);
	  HAL_Delay(10);
	  return(PCD_OK);
}

/*
 * Function to check for PICC in field
 * */

PCD_StatusTypeDef PICC_CHECK(void){
	uint8_t ATQA[2];
	if(MFRC_REQA(ATQA)!=PCD_OK){
		return(PCD_COMM_ERR);
	}

	else{
		if(ATQA[0]!=ULTRA_ATQA){
			return(PCD_COMM_ERR);
		}
		else{
			return(PCD_OK);
		}
	}
}
/*
 * Function to read data from MIFARE ULTRALIGHT PICC
 *
 * @param addr: Start address (PICC will return data from first four pages)
 *
 * @param data: Array to store read data
 *
 * */

PCD_StatusTypeDef UL_READ(uint8_t addr,uint8_t* data){

		uint8_t transaction[4]={ULTRA_READ,addr};
		uint8_t CRC_val[2];

		CALC_CRC(transaction, 2, CRC_val);


		memcpy(transaction+2,CRC_val,2);

		if(MFRC_TRANSCEIVE(transaction, 4, data, 18, 0)!=PCD_OK){
			return(PCD_COMM_ERR);
		}

		else{
			return(PCD_OK);
		}
}

/*
 * Function to write 4 bytes (1 page) to MIFARE Ultralight PICC
 *
 * @param addr: Page start address
 *
 * @param data: Array of 4 bytes to write
 *
 * */

PCD_StatusTypeDef UL_WRITE(uint8_t addr,uint8_t* data){
	uint8_t transaction[4]={ULTRA_WRITE,addr};
	uint8_t ack;
	uint8_t CRC_val[2];
	memcpy(transaction+2,data,4);

	CALC_CRC(transaction, 6, CRC_val);

	memcpy(transaction+6,CRC_val,2);

	if(MFRC_TRANSCEIVE(transaction, 8, &ack, 1, 0)!=PCD_OK){
		return(PCD_COMM_ERR);
	}

	else{
		return(PCD_OK);
	}
}

void Print(char* mess){
	char send[strlen(mess)];
	memcpy(send,mess,strlen(mess));
	CDC_Transmit_FS((uint8_t*)send, strlen(mess));
	HAL_Delay(10);
}

/*
 * Function to dump data to serial terminal
 *
 * @param data: Destination array for card data
 * */
uint8_t WUPA=0;
PCD_StatusTypeDef DumpINFO(uint8_t* data){
	  uint8_t ATQA[2];
	  if(WUPA==1){
		 if(PICC_Select()!=PCD_OK){
			 Print("ERROR No PICC Found\r\n");
			 WUPA=0;
			 return(PCD_COMM_ERR);
		 }
		 else{
			 HAL_Delay(10);
				Print("    BYTE\r\n");
				Print("0 1 2 3\r\n");
				Print("        \r\n");
				for(int i=0;i<13;i+=4){
					UL_READ(i, data);
					for(int j=0;j<13;j+=4){
						char mess[20];
						sprintf(mess,"%X, %X, %X, %X\r\n",data[j],data[j+1],data[j+2],data[j+3]);
						Print(mess);
						return(PCD_OK);
					}
				}

			 MFRC_HALTA();
			 MFRC_WUPA(ATQA);
			 WUPA=1;
		 }

	  }
	  else{
		  if(PICC_CHECK()!=PCD_OK){
		  		  Print("ERROR No PICC Found\r\n");
		  		  WUPA=0;
		  		return(PCD_COMM_ERR);
		  }

		  else{
			  HAL_Delay(10);
			  PICC_Select();
			  HAL_Delay(10);
				Print("    BYTE\r\n");
				Print("0 1 2 3\r\n");
				Print("        \r\n");
				for(int i=0;i<13;i+=4){
					UL_READ(i, data);
					for(int j=0;j<13;j+=4){
						char mess[20];
						sprintf(mess,"%X, %X, %X, %X\r\n",data[j],data[j+1],data[j+2],data[j+3]);
						Print(mess);
						return(PCD_OK);
					}
				}

			  MFRC_HALTA();
			  MFRC_WUPA(ATQA);
			  WUPA=1;
		  }
	  }
	  return (PCD_COMM_ERR);



}


