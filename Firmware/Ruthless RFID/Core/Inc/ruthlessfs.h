/*
 * ruthlessfs.h
 *
 *  Created on: 17 Jun. 2024
 *      Author: rhett
 */

#ifndef INC_RUTHLESSFS_H_
#define INC_RUTHLESSFS_H_

typedef enum {
	RFS_OK = 0x00,
	RFS_NO_CARD = 0x01,
	RFS_CARD_OVERSIZE = 0x02,
	RFS_CARD_PROTECTED = 0x03,
	RFS_WRITE_ERROR = 0x04,
	RFS_READ_ERROR = 0x05,
	RFS_CARD_UNINTIALISED = 0x06
} RFS_StatusTypeDef;

typedef struct {
	uint8_t* uid; //Card UID
	uint8_t uidsize;
	char* name; //Name to put for card entry e.g. "Card1"
	char* type;
	uint8_t read_protected; //Option to make card entry read protected (contents cannot be read)
	uint8_t* contents;
	uint16_t contents_size;
}Card;

#define METAPAGE_OFFSET 0
#define NAMEPAGE_OFFSET 1
#define DATAPAGE_OFFSET 2
#define READ_PROTECTED  1

//Functions
RFS_StatusTypeDef enter_card(Card* card, uint16_t entry);
RFS_StatusTypeDef enter_metadata(Card* card, uint16_t block_num);
Card* read_card_name (char* name);
Card* read_card_entry(uint16_t entry);
RFS_StatusTypeDef read_metadata(Card* result, uint16_t entry);
RFS_StatusTypeDef read_nameuid(Card* result, uint16_t entry);
RFS_StatusTypeDef read_cardcontents(Card* result, uint16_t entry);
uint16_t get_datasize(uint16_t entry, uint8_t page);
void free_card(Card* card);
void print_card_to_serial(uint16_t entry);
int get_number_files(void);
RFS_StatusTypeDef get_all_files(char** result);
RFS_StatusTypeDef entry_present(uint16_t entry);
#endif /* INC_RUTHLESSFS_H_ */
