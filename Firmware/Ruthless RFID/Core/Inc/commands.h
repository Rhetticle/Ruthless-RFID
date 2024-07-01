/*
 * commands.h
 *
 *  Created on: 30 Jun. 2024
 *      Author: rhett
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_


typedef enum {
	CMD_OK = 0x00,
	CMD_LS_ERROR = 0x01
}CMD_StatusTypeDef;

CMD_StatusTypeDef cmd_ls(void);
CMD_StatusTypeDef cmd_parse(char* cmd);
void cmd_build(char** current, char input);

#endif /* INC_COMMANDS_H_ */
