/*
 * ruthlessfs.c
 *
 *  Created on: 17 Jun. 2024
 *      Author: rhett
 */

#include "stm32f4xx.h"
#include "W25N01GVZEIG.h"
#include "ruthlessfs.h"

RFS_StatusTypeDef enter_card(Card* card) {
	uint16_t free_block = (uint16_t)mem_find_free_block();

	MEM_WRITE(free_block, 0x0000, card->name, strlen(card->name));

}
