/*
 * ruthlessfs.h
 *
 *  Created on: 17 Jun. 2024
 *      Author: rhett
 */

#ifndef INC_RUTHLESSFS_H_
#define INC_RUTHLESSFS_H_

#include <stdint.h>

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

#define WRITE_NUM_BLOCK 1022 //Stores total number of written cards (32-bit to allow measuring up to 4 billion cards)
#define READ_NUM_BLOCK 1023 //Stores total number of read cards (32-bit to allow measuring up to 4 billion cards)

//Functions
RFS_StatusTypeDef enter_card(Card* card, uint16_t entry, char* name);
RFS_StatusTypeDef enter_metadata(Card* card, uint16_t block_num);
Card* read_card_name (char* name);
Card* read_card_entry(uint16_t entry);
RFS_StatusTypeDef read_metadata(Card* result, uint16_t entry);
RFS_StatusTypeDef read_nameuid(Card* result, uint16_t entry);
RFS_StatusTypeDef read_cardcontents(Card* result, uint16_t entry);
uint16_t get_datasize(uint16_t entry, uint8_t page);
void free_card(Card* card);
void print_card_to_serial(uint16_t entry);
uint32_t get_number_files_section (uint16_t start, uint32_t count);
uint32_t get_number_files_all(void);
RFS_StatusTypeDef get_files_section (char** result, uint16_t start, uint32_t count);
RFS_StatusTypeDef get_all_files(char** result);
RFS_StatusTypeDef entry_present(uint16_t entry);
RFS_StatusTypeDef file_exists(char* name);
char* get_file_name(uint16_t entry);
int get_file_entry(char* name);
void free_filenames(char** file_names, int size);
void remove_card(uint16_t entry);
RFS_StatusTypeDef remove_card_byname(char* name);
RFS_StatusTypeDef modify_card(char* file_to_mod, int page, uint8_t* data, char* name);
uint32_t get_used_size(void);
uint32_t get_free_size(void);
void get_used_size_str(char* result);
void get_free_size_str(char* result);
RFS_StatusTypeDef inc_write_count(void);
RFS_StatusTypeDef inc_read_count(void);
uint32_t get_total_writes(void);
uint32_t get_total_reads(void);
char* get_total_writes_str(void);
char* get_total_reads_str(void);
#endif /* INC_RUTHLESSFS_H_ */
