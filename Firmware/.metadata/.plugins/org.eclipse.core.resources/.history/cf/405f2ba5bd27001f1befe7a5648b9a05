/*
 * screen.c
 *
 *  Created on: Jun 11, 2024
 *      Author: rhett
 */
#include "screen.h"
#include "stdint.h"

static const char* HOME_SCREEN[7]={"Ruthless RFID","1.READ CARD","2.WRITE CARD","3.SHOW FILES","4.DISP","5.STATS","6.EXIT"};
static const uint8_t HOME_DATLOC[7][2]={{0,0},{2,START_COL},{4,START_COL},{6,START_COL},{2,MID_COL+15},{4,MID_COL+15},{6,MID_COL+15}};
static const uint8_t HOME_SEL[6][2]={{2,START_COL},{4,START_COL},{6,START_COL},{2,MID_COL+15},{4,MID_COL+15},{6,MID_COL+15}}; //Select arrow info for HOME screen format {Page,column,replacesize}}

static const char* READ_SCREEN[3]={"READ CARD","STANDARD: ISO 14443","STATUS: NO CARD"};
static const uint8_t READ_DATLOC[3][2]={{0,0},{2,START_COL},{4,START_COL}};
static const uint8_t READ_SEL[1][2]={{0,0}};

static const char* CARD_FOUNDSCREEN[5]={"CARD FOUND","UID: ","IC: ","SAVE","EXIT"};
static const uint8_t CARD_FOUNDATLOC[5][2]={{0,0},{2,START_COL},{4,START_COL},{7,START_COL+12},{7,END_COL-24}};
static const uint8_t CARD_FOUNDSEL[2][2]={{7,START_COL},{7,END_COL-36}};

static const char* WRITE_SCREEN[4]={"WRITE CARD","STANDARD: ISO 14443","SRC FILE: NONE","STATUS: NO CARD"};
static const uint8_t WRITE_DATLOC[4][2]={{0,0},{2,START_COL},{4,START_COL},{6,START_COL}};
static const uint8_t WRITE_SEL[1][2] = {{0,0}};

/**
 * Screens
 *
 * */

const Screen SCRN_Home = {
		.datsize = sizeof(HOME_SCREEN)/sizeof(HOME_SCREEN[0]),
		.selsize = sizeof(HOME_SEL)/sizeof(HOME_SEL[0]),
		.data = HOME_SCREEN,
		.dataloc = HOME_DATLOC,
		.seldata = HOME_SEL,

};

const Screen SCRN_ReadCard = {
		.datsize = sizeof(READ_SCREEN)/sizeof(READ_SCREEN[0]),
		.selsize = sizeof(READ_SEL)/sizeof(READ_SEL[0]),
		.data = READ_SCREEN,
		.dataloc = READ_DATLOC,
		.seldata = READ_SEL,

};

const Screen SCRN_CardFound = {
		.datsize = sizeof(CARD_FOUNDSCREEN)/sizeof(CARD_FOUNDSCREEN[0]),
		.selsize = sizeof(CARD_FOUNDSEL)/sizeof(CARD_FOUNDSEL[0]),
		.data = CARD_FOUNDSCREEN,
		.dataloc = CARD_FOUNDATLOC,
		.seldata = CARD_FOUNDSEL,

};

const Screen SCRN_WriteCard = {
		.datsize = sizeof(WRITE_SCREEN)/sizeof(WRITE_SCREEN[0]),
		.selsize = sizeof(WRITE_SEL)/sizeof(WRITE_SEL[0]),
		.data = WRITE_SCREEN,
		.dataloc = WRITE_DATLOC,
		.seldata = WRITE_SEL,

};
