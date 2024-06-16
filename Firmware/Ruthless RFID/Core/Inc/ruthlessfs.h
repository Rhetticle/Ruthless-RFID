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
	RFS_CARD_OVERSIZE = 0x02
} RFS_StatusTypeDef;

typedef struct {
	uint8_t* uid;
	char* name;
	uint8_t* contents;
}Card;

#endif /* INC_RUTHLESSFS_H_ */
