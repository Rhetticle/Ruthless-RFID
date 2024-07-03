/*
 * ruthlessfs.c
 *
 *  Created on: 17 Jun. 2024
 *      Author: rhett
 */

/**
 * ******************************Ruthless File System***********************************
 *
 * File system used by Ruthless RFID to store data of read cards. Each entry occupies a 128kB block (smallest erasable block size).
 *
 * Page 1 - Metadata (Stored as: Type, card memory size, uid size, read protected,)
 * Page 2 - Card name + uid
 * Page 3 - Contents
 * */
#include "stm32f4xx.h"
#include "W25N01GVZEIG.h"
#include "ruthlessfs.h"
#include "MFRC.h"
#include "OLED.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Enter a card into the file system
 *
 * @param card - Card to store
 * @param entry - Entry (Block number) to store card
 * @return RFS_OK if card was successfully stored
 * */
RFS_StatusTypeDef enter_card(Card* card, uint16_t entry, char* name) {
	uint16_t block_startaddr = entry * BLOCK_PAGECOUNT;
	block_erase(entry); //Erase entire block ready for new data
	enter_metadata(card, entry);
	card->name = name;

	if (MEM_WRITE(block_startaddr + NAMEPAGE_OFFSET, 0x0000, (uint8_t*)card->name, strlen(card->name)) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}
	if (MEM_WRITE(block_startaddr + NAMEPAGE_OFFSET, 0x0000 + strlen(card->name),card->uid ,card->uidsize) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}
	if (MEM_WRITE(block_startaddr + DATAPAGE_OFFSET, 0x0000, card->contents, card->contents_size) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}

	return RFS_OK;

}

/**
 * Write the card metadata into a block
 *
 * @param card - Card to write
 * @param block_num - Block number
 * */
RFS_StatusTypeDef enter_metadata(Card* card, uint16_t block_num) {
	uint8_t card_size = card->contents_size; //Card contents is uint8_t
	uint8_t read_protected = card->read_protected;
	uint8_t uid_size = card->uidsize;
	uint8_t metasize = sizeof(card_size) + sizeof(uid_size) + sizeof(read_protected) + strlen(card->type);
	uint8_t* metadata = malloc(metasize);

	memcpy(metadata, (uint8_t*) card->type, strlen(card->type));
	metadata[strlen(card->type) + 0] = card_size;
	metadata[strlen(card->type) + 1] = uid_size;
	metadata[strlen(card->type) + 2] = read_protected;

	if (MEM_WRITE(block_num * BLOCK_PAGECOUNT, 0x0000, metadata, metasize) != HAL_OK) {
		free(metadata);
		return RFS_WRITE_ERROR;
	}
	free(metadata);
	return RFS_OK;
}

/**
 * Enter the name and UID of a card into entry
 * @param card - Card to enter
 * @return RFS_OK if data was entered successfully
 * */


/**
 * Read a card from memory
 *
 * @param Name - Card entry name
 * @return a new Card instance with read data. NULL if name is not in memory
 * */
Card* read_card_name (char* name) {
	uint16_t block_addr = 0;
	char* read_name = malloc(strlen(name) + 1);

	while ((strcmp(name, read_name) != 0) && (block_addr < BLOCK_COUNT)) {
		MEM_READPAGE(block_addr * BLOCK_PAGECOUNT, 0x0000, (uint8_t*)read_name, strlen(name));
		read_name[strlen(name)] = '\0';
		block_addr++;
	}
	if (block_addr == BLOCK_COUNT) {
		return NULL; //We couldn't find the name
	}
	return read_card_entry(block_addr);

}

/**
 * Get card from entry number
 *
 * @param entry - Entry number of card
 * @return a new Card instance with read data
 * */
Card* read_card_entry(uint16_t entry) {
	Card* result = malloc(sizeof(Card));

	if (read_metadata(result, entry) != RFS_OK) {
		return NULL;
	}

	if (read_nameuid(result, entry) != RFS_OK) {
		return NULL;
	}

	if(read_cardcontents(result, entry) != RFS_OK) {
		return NULL;
	}

	return result;
}

/**
 * Read metadata of entry
 *
 * @param result - Card to store data to
 * @param entry - entry to read from
 * @return RFS_OK if data was successfully read
 * */
RFS_StatusTypeDef read_metadata(Card* result, uint16_t entry) {
	uint16_t metadata_size = get_datasize(entry, METAPAGE_OFFSET);
	uint8_t* metadata = malloc(metadata_size * sizeof(uint8_t));
	char* type = malloc(((metadata_size - 3) + 1) * sizeof(char)); //+1 for null

	if (MEM_READPAGE(entry * BLOCK_PAGECOUNT, 0x0000, metadata, metadata_size) != HAL_OK) {
		free(metadata);
		return RFS_READ_ERROR;
	}

	if ((uint8_t)type[0] == 0xFF) { //Simple check to see if we read an empty entry
		free(metadata);
		return RFS_NO_CARD;
	}

	memcpy(type, metadata, metadata_size - 3);
	type[metadata_size - 3] = '\0';
	result->type = type;
	result->contents_size = metadata[metadata_size - 3];
	result->uidsize = metadata[metadata_size - 2];
	if (metadata[metadata_size - 1] == READ_PROTECTED) { //Card is read protected
		free(metadata);
		return RFS_CARD_PROTECTED;
	}
	result->read_protected = metadata[metadata_size - 1];
	free(metadata);

	return RFS_OK;
}

/**
 * Read the name and uid of card entry. Note that metadata must be read before name and uid can be read
 * @param result - Result to store data to
 * @param entry - Entry to read from
 * @return RFS_OK if name and uid was successfully read
 * */
RFS_StatusTypeDef read_nameuid(Card* result, uint16_t entry) {
	uint16_t datasize = get_datasize(entry, NAMEPAGE_OFFSET);
	uint8_t* raw_data = malloc(datasize*sizeof(uint8_t));
	char* name = malloc((datasize - result->uidsize + 1) * sizeof(char)); //+1 for null
	uint8_t* uid = malloc((result->uidsize) * sizeof(uint8_t));

	if (MEM_READPAGE((entry * BLOCK_PAGECOUNT) + NAMEPAGE_OFFSET, 0x0000, raw_data, datasize) != HAL_OK) {
		free(raw_data);
		return RFS_READ_ERROR;
	}

	if ((uint8_t)name[0] == 0xFF) { //Simple check to see if we read an empty entry
		free(raw_data);
		return RFS_NO_CARD;
	}

	memcpy(name, raw_data, datasize - result->uidsize);
	name[datasize - result->uidsize] = '\0';
	result->name = name;

	memcpy(uid, raw_data + strlen(name), result->uidsize);
	result->uid = uid;
	free(raw_data);

	return RFS_OK;
}

/**
 * Read the contents of a card entry
 * @param entry - Entry to read contents of
 * @return RFS_OK if data was successfully read
 * */
RFS_StatusTypeDef read_cardcontents(Card* result, uint16_t entry) {
	uint8_t* contents = malloc(result->contents_size * sizeof(uint8_t));

	if (MEM_READPAGE((entry * BLOCK_PAGECOUNT) + DATAPAGE_OFFSET, 0x0000, contents, result->contents_size) != HAL_OK) {
		free(contents);
		return RFS_READ_ERROR;
	}

	result->contents = contents;
	return RFS_OK;
}

/**
 * Get the size of a chunk of data from specific block and page
 * @param entry - Entry to read from
 * @param page - Page to begin reading from
 * @return size of data in bytes
 * */
uint16_t get_datasize(uint16_t entry, uint8_t page) {
	uint16_t size = 0;
	uint8_t byte_read = 0x00;

	while(byte_read != 0xFF) {
		if (MEM_READPAGE((entry * BLOCK_PAGECOUNT) + page, size, &byte_read, 1) != HAL_OK) {
			return 0; //Error occured whilst reading
		}
		size++;
	}

	return size - 1; //Last iteration will add 1 to true size so -1
}

/**
 * Free a card instance
 * @param card - Card to free
 * */
void free_card(Card* card) {
	free(card->contents);
	free(card->name);
	free(card->type);
	free(card->uid);
	free(card);
}

/**
 * Print card details to terminal (used for checking correct functionality)
 *
 * @param entry - Entry to read card from
 * */
void print_card_to_serial(uint16_t entry) {
	Card* card = read_card_entry(0);
	char* msg = malloc(200 * sizeof(char));

	sprintf(msg,"Card entry 0 is a %s with uid size %i and called %s\r\n", card->type, card->uidsize, card->name);

	Print(msg);
	free(msg);
	free_card(card);
}


/**
 * Get number of files in given section of memory
 * @param start - Start block
 * @param count - Number of block to check ahead of start
 * @return number of files present in section
 * */
uint32_t get_number_files_section (uint16_t start, uint32_t count) {
	int file_count = 0;

	for (int i = start; i < start + count; i++) {
		if (entry_present(i) == RFS_OK) {
			file_count++;
		}
	}

	return file_count;
}

/**
 * Get number of files currently stored
 * @return number of files stored
 * */
uint32_t get_number_files_all(void) {
	return get_number_files_section(0, BLOCK_COUNT - 2); //-2 since last two blocks are for read and write count stats
}

/**
 * Get the names of currently stored cards for given section of memory
 *
 * @param start - Start block number
 * @param count - Number of files to read
 * @param result - Array to store file names
 * @return RFS_OK if files were successfully read
 * */
RFS_StatusTypeDef get_files_section (char** result, uint16_t start, uint32_t count) {
	Card* work;
	uint32_t file_index = 0;

	for (int i = start; i < start + count; i++) {
		if (entry_present(i) == RFS_OK) {
			work = read_card_entry(i);
			result[file_index] = malloc(strlen(work->name) + 1);
			memcpy(result[file_index], work->name, strlen(work->name));
			result[file_index][strlen(work->name)] = '\0';
			file_index++;
		}
	}

	free_card(work);
	return RFS_OK;
}

/**
 * Get the names of all currently stored cards
 *
 * @param result - Array to store file names
 * @return RFS_OK if all file names were read correctly
 * */
RFS_StatusTypeDef get_all_files(char** result) {
	return get_files_section(result, 0, BLOCK_COUNT - 2); //-2 since last two blocks are for read and write count stats
}

/**
 * Check if entry is present
 *
 * @param entry - Entry to check
 * @return RFS_OK if entry is present
 * */
RFS_StatusTypeDef entry_present(uint16_t entry) {
	uint8_t byte_read;

	if (MEM_READPAGE(entry * BLOCK_PAGECOUNT, 0x0000, &byte_read, 1) != HAL_OK) {
		return RFS_READ_ERROR;
	}

	if (byte_read == 0xFF) {
		return RFS_NO_CARD;
	}

	return RFS_OK;
}

/**
 * Check if file exists by name
 * @param name - File name
 * @return RFS_OK if file exists
 * */
RFS_StatusTypeDef file_exists(char* name) {
	Card* work;
	uint16_t block = 0;

	while(1) {
		if (entry_present(block) == RFS_OK) {
			work = read_card_entry(block);
			if (strcmp(name, work->name) == 0) {
				free_card(work);
				return RFS_OK;
			}
		}
		if (block == BLOCK_COUNT - 1) {
			break;
		}
		block++;
		free_card(work);
	}
	return RFS_NO_CARD;
}

/**
 * Get the file name of a given entry
 * @param entry - Entry to get name of
 * @return pointer to name
 * */
char* get_file_name(uint16_t entry) {
	Card* work;

	if (entry_present(entry) != RFS_OK) {
		return NULL;
	}

	work = read_card_entry(entry);

	char* name = malloc((strlen(work->name) + 1) * sizeof(char));
	strcpy(name, work->name);
	free_card(work);

	return name;
}

/**
 * Get entry number of file with given name
 * @param name - File name to look for
 * @return Entry number (-1 if file doesn't exist)
 * */
int get_file_entry(char* name) {
	Card* work;

	for (int i = 0; i < BLOCK_COUNT - 2; i++) {
		if (entry_present(i) == RFS_OK) {
			work = read_card_entry(i);
			if (strcmp(work->name, name) == 0) {
				free_card(work);
				return i;
			}
		}
	}
	free_card(work);
	return -1; //shouldn't get here
}

/**
 * Get entry number of given file name
 * @param name
 * */

/**
 * Free list of file names
 * @param file_names - File names
 * @param size -  Number of file names
 * */
void free_filenames(char** file_names, int size) {
	for (int i = 0; i < size; i++) {
		free(file_names[i]);
	}
	free(file_names);
}

/**
 * Remove card from file system
 * @param entry - Entry to remove
 * */
void remove_card(uint16_t entry) {
	block_erase(entry);
}

/**
 * Remove card by name
 * @param name - Name of card
 * @return RFS_OK if card was successfully removed
 * */
RFS_StatusTypeDef remove_card_byname(char* name) {
	Card* work;

	for (int block = 0; block < BLOCK_COUNT; block++) {
		if (entry_present(block) == RFS_OK) {
			work = read_card_entry(block);
			if (strcmp(name, work->name) == 0) {
				remove_card(block);
				free_card(work);
				return RFS_OK;
			}
		}
		free_card(work);
	}

	return RFS_NO_CARD;
}

/**
 * Calculate the used size of memory in MiB
 * @return size of memory used in MiB
 * */
uint32_t get_used_size(void) {
	int file_count = get_number_files_all();

	return (BLOCK_SIZE * file_count);
}

/**
 * Get size of memory that is free
 * @return size of memory that is free in MiB
 * */
uint32_t get_free_size(void) {
	return MEM_SIZE - get_used_size();
}

/**
 * Get used size as a string (Useful for printing to OLED)
 * @param result - Pointer to string to write to
 * */
void get_used_size_str(char* result) {
	uint32_t used = get_used_size();
	sprintf(result, "%i", used);
}

/**
 * Get free size as a string
 * @param result - Pointer to string to write to
 * */
void get_free_size_str(char* result) {
	uint32_t free = get_free_size();
	sprintf(result, "%i", free);
}

/**
 * Increment the total write count by 1
 * @return RFS_OK if value was successfully incremented
 * */
RFS_StatusTypeDef inc_write_count(void) {
	uint32_t new_total = get_total_writes() + 1;
	uint8_t new_writes[sizeof(uint32_t)] = {new_total, new_total >> 8, new_total >> 16, new_total >> 24};

	block_erase(WRITE_NUM_BLOCK);
	if (MEM_WRITE(BLOCK_PAGECOUNT * WRITE_NUM_BLOCK, 0x0000, new_writes, sizeof(uint32_t)) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}

	return RFS_OK;
}

/**
 * Increment the total read count by 1
 * @return RFS_OK if value was successfully incremented
 * */
RFS_StatusTypeDef inc_read_count(void) {
	uint32_t new_total = get_total_reads() + 1;
	uint8_t new_writes[sizeof(uint32_t)] = {new_total, new_total >> 8, new_total >> 16, new_total >> 24};

	block_erase(READ_NUM_BLOCK);
	if (MEM_WRITE(BLOCK_PAGECOUNT * READ_NUM_BLOCK, 0x0000, new_writes, sizeof(uint32_t)) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}

	return RFS_OK;
}

/**
 * Get total number of writes made to FS (Used in stats task)
 * @return Total number of writes made to mem (Stored at block number WRITE_NUM_BLOCK)
 * */
uint32_t get_total_writes(void) {
	uint8_t writes[sizeof(uint32_t)];

	MEM_READPAGE(BLOCK_PAGECOUNT * WRITE_NUM_BLOCK, 0x0000, writes, sizeof(uint32_t));

	uint32_t writes_u32 = writes[3] << 24 | writes[2] << 16 | writes[1] << 8 | writes[0];
	return writes_u32;
}

/**
 * Get total number of reads made of phyiscaly cards (Used in stats task)
 * @return Total number of reads(Stored at block number READ_NUM_BLOCK)
 * */
uint32_t get_total_reads(void) {
	uint8_t reads[sizeof(uint32_t)];

	MEM_READPAGE(BLOCK_PAGECOUNT * READ_NUM_BLOCK, 0x0000, reads, sizeof(uint32_t));

	uint32_t reads_u32 = reads[3] << 24 | reads[2] << 16 | reads[1] << 8 | reads[0];
	return reads_u32;
}

/**
 * Get total writes as a string
 * @return String of total writes
 * */
char* get_total_writes_str(void) {
	char* result = malloc(sizeof(uint32_t) * sizeof(char));

	sprintf(result, "%i", get_total_writes());
	return(result);
}

/**
 * Get total reads as a string
 * @return String of total reads
 * */
char* get_total_reads_str(void) {
	char* result = malloc(sizeof(uint32_t) * sizeof(char));

	sprintf(result, "%i", get_total_reads());
	return(result);
}
