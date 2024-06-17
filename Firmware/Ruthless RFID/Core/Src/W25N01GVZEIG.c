/*
 * W25N01GVZEIG.c
 *
 *  Created on: Feb 1, 2024
 *      Author: rhett
 *
 *  This driver will be used in combination with FATFS to create a FAT filesystem on the SPI memory.
 *  Writing to the W25N01GVZEIG is done 2KiB at a time, so the memory should be formatted to FAT32 with
 *  sector size of 2KiB so that 1 sector in the filesystem is equal to 1 page in memory
 */


#include "W25N01GVZEIG.h"
#include <stdlib.h>
#include <string.h>

extern SPI_HandleTypeDef hspi2;
/*
 * Function to read manufacturer and device ID's
 *
 * @param rec: Array to store received values (Note first two elements will be useless)
 *
 * */
void READ_ID(uint8_t* rec){
	uint8_t transaction[]={ID, DUMMY, DUMMY, DUMMY, DUMMY}; //ID opcode and four dummy bytes
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	while(HAL_SPI_TransmitReceive(&hspi2, transaction,rec, 5, 100)!=HAL_OK); //Here the third to last byte will be MFR ID and last two will be Device ID
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
}

/*
 * Function to read one of three status registers. Registers in order are (Protection, config and status)
 *
 * @param addr: Address of status register we wish to read from
 * */

uint8_t STAT_READ(uint8_t addr){
	uint8_t transaction[]={READ_STAT, addr, DUMMY};
	uint8_t rec[3];
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	while(HAL_SPI_TransmitReceive(&hspi2, transaction,rec, 3, 100)!=HAL_OK);
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	return(rec[2]); //Status register value
}

/*
 * Function to write to one of three status registers.
 *
 * @param addr: Address of status register
 *
 * @param value: Value to write to register
 * */

HAL_StatusTypeDef STAT_WRITE(uint8_t addr,uint8_t value){
	uint8_t transaction[]={WRIT_STAT,addr,value};
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
		HAL_SPI_Transmit(&hspi2, transaction, 3, 100);
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	return(HAL_OK);
}

/*
 * Function to enable write permissions.
 *
 * !!Write enable instruction must preceed the following instructions:!!
 * 		Page program
 * 		Block erase
 * 		Bad block manage
 *
 * The Write Enable bit in status register 3 will be reset to 0 after any of these instructions
 * */

void WRIT_EN(void){
	uint8_t write_en=W_EN;
	WRITE_DIS();

	while((STAT_READ(STAT_REG3)&0x02)!=0x02){ //Check that second to last bit (Write Enable bit) of status register is 1
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
		HAL_SPI_Transmit(&hspi2, &write_en, 1, 100);
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	}


}

/**
 * Disable write permissions (Sets WEL bit in STAT_REG3 to 0)
 * */
void WRITE_DIS(void) {
	uint8_t write_dis = W_DIS;

	while((STAT_READ(STAT_REG3)&0x02)==0x02){
			HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
			HAL_SPI_Transmit(&hspi2, &write_dis, 1, 100);
			HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		}
}

/**
 * Erase a 128kB (64 pages) block to 0xFF
 *
 * @param page_addr - Page address to begin erasing from
 * */
void block_erase(uint16_t page_addr) {
	uint8_t transaction [] = {BLOCK_ERS, DUMMY, page_addr>>8, page_addr};
	WRIT_EN();
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	HAL_SPI_Transmit(&hspi2, transaction, 4, 100);
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	while((STAT_READ(STAT_REG3)&0x01) == 0x01);
}

/*
 * Function to initialise memory
 *
 * All we are doing here is removing write protection from entire array by setting BP bits
 * in status register 1 to zero
 *
 * */

HAL_StatusTypeDef MEM_INIT(void){
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	STAT_WRITE(STAT_REG1, 0x00); //remove protection of entire memory array
	if (STAT_READ(STAT_REG1) != 0x00) {
		return HAL_ERROR;
	}

	WRITE_DIS();
	return HAL_OK;
}

/*
 * Function to write to 2KiB page in memory (Can program from 1 up to 2112 bytes)
 *
 * @param col_addr: Index of page to begin writing to (See datasheet)
 *
 * @param page_addr: Address of page to write to
 *
 * @param data: data to write
 *
 * @param bytes: Number of bytes to write to page
 * */

HAL_StatusTypeDef MEM_WRITE(uint16_t page_addr,uint16_t col_addr,uint8_t* data,uint32_t bytes){
	uint8_t* setup = malloc((bytes+3)*sizeof(uint8_t)); //Extra 3 bytes for write opcode and column address
	uint8_t execute[]={WRIT_EXE, DUMMY, page_addr>>8, page_addr};

	setup[0] = WRIT_LOAD;
	setup[1] = col_addr>>8;
	setup[2] = col_addr;
	memcpy(&setup[3], data, bytes);

	WRIT_EN();
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_Transmit(&hspi2, setup, bytes+3, 100)!=HAL_OK){ //load data to internal buffer
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	while((STAT_READ(STAT_REG3)&0x01) == 0x01); //Wait here until busy bit is cleared

	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_Transmit(&hspi2, execute, 4, 100)!=HAL_OK){ //Send command to dump internal buffer data to memory array
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	while((STAT_READ(STAT_REG3)&0x01) == 0x01);
	free(setup);
	return(HAL_OK);

}

/*
 * Function to read a single 2KiB page from memory (Note total size is 2KiB data + 64B ECC)
 *
 * @param page_addr: Page address
 *
 * @param col_addr: Index of page to begin reading (0x00 to start from beginning of page)
 *
 * @param data: Array to store data to
 *
 * @param bytes: Number of bytes to read from data buffer
 *
 * !!Note data must first be shifted from memory array into internal data buffer and then can be read!!
 * */

HAL_StatusTypeDef MEM_READPAGE(uint16_t page_addr,uint16_t col_addr,uint8_t* data,uint32_t bytes){ //Read one 2KiB page. Data will be put into internal buffer which can then be read. Wait at least tDR or until busy bit is clear
	uint8_t transaction[]={READ_PAGE, DUMMY, page_addr>>8, page_addr};
	uint8_t transaction_size = sizeof(transaction)/sizeof(transaction[0]);
	uint8_t* read_command = malloc(bytes+transaction_size); //Must allocate here since array may be too big for FreeRTOS task stack
	uint8_t* rec_data = malloc(bytes+transaction_size);

	memset(read_command, 0, bytes+transaction_size);
	read_command[0]=READ_BUF;
	read_command[1]= col_addr>>8;
	read_command[2] = col_addr;
	read_command[3] = DUMMY;

	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_Transmit(&hspi2, transaction, 4, 100)!=HAL_OK){ //load data to internal buffer
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	while((STAT_READ(STAT_REG3)&0x01) == 0x01); //Wait here until BUSY bit is cleared

	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_TransmitReceive(&hspi2, read_command, rec_data, bytes+transaction_size, 100)!=HAL_OK){
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	while((STAT_READ(STAT_REG3)&0x01) == 0x01);

	if (bytes == 1) {
		*data = rec_data[4];
	} else {
		for(int i = 0; i < bytes; i++){
				data[i] = rec_data[i+4]; //+4 as first four elements of rec_data are meaningless
		}
	}
	free(read_command);
	free(rec_data);
	return(HAL_OK);

}

/*
 * Function to find defective memory blocks (128kB) (There may be at most 20 defective blocks)
 *
 * Defective blocks are labeled by the manufacturer by the first page in the block beginning with
 * 0x00 instead of 0xFF.
 *
 * Defective blocks can be placed in the BBM look up table in order to avoid them whilst writing
 *
 * @param defect: Array to store addresses of defective blocks
 * */

HAL_StatusTypeDef MEM_SCAN(uint16_t* defect){
	int i = 0; //Keep track of index in defect array. +1 when defective address is found
	char msg[50];

	for(uint32_t addr = 0x0000; addr <= 0xFFFF; addr++) { //We have 65536 pages to check (0x0000 to 0xFFFF) but we only need to check the first byte
		uint8_t byte;
		if(MEM_READPAGE((uint16_t)addr, 0x0000, &byte, 1) != HAL_OK){ //Set col addr to 0x00 as we are only interested in first byte
			return(HAL_ERROR);

		} else if(byte != 0xFF){
				defect[i]=addr;
				sprintf(msg, "Error at page 0x%X, read 0x%X expected 0xFF\r\n", addr, byte);
				Print(msg);
				i++;
		}

	}
	return(HAL_OK);
}

int mem_find_free_block(void) {
	for (int i = 0; i < BLOCK_COUNT; i+=BLOCK_PAGECOUNT) {
		uint8_t first_byte;
		if (MEM_READPAGE(i * BLOCK_PAGECOUNT, 0x0000, &first_byte, 1) != HAL_OK) {
			return -1;
		}

		if (first_byte == 0xFF) {
			return i;
		}
	}
	return -1;
}




