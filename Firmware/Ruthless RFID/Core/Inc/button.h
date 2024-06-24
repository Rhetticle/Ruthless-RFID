/*
 * button.h
 *
 *  Created on: Jun 24, 2024
 *      Author: rhett
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "stdint.h"

typedef uint8_t Button_StateTypeDef;

#define NO_PRESS 0
#define LONG_PRESS 1
#define SHORT_PRESS 2
#define LONG_PRESS_THRESH 1000 //Time in ms for a press to be considered a LONG_PRESS

#endif /* INC_BUTTON_H_ */
