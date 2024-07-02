/*
 * commands.h
 *
 *  Created on: 30 Jun. 2024
 *      Author: rhett
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

#include <stdint.h>

typedef enum {
	CMD_OK = 0x00,
	CMD_LS_ERROR = 0x01,
	CMD_RM_ERROR = 0x02
}CMD_StatusTypeDef;

CMD_StatusTypeDef cmd_ls(void);
CMD_StatusTypeDef cmd_parse(char* cmd);
void cmd_build(char** current, char input);
char** cmd_split(char* cmd, char split);
uint32_t get_token_count(char* cmd);
char* cmd_strip(char* cmd);
void free_tokens(char** tokens, uint32_t size);

#endif /* INC_COMMANDS_H_ */
