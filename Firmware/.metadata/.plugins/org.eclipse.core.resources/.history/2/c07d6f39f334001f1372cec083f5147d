/*
 * button.c
 *
 *  Created on: Jun 24, 2024
 *      Author: rhett
 */

#include "button.h"
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os2.h"

extern TIM_HandleTypeDef htim3;
extern osMessageQueueId_t UserInputHandle;

/****************************************Button Interrupt Handler**************************************
 * Interrupt is triggered on falling (button pressed) and rising (button released) edges. On release the
 * value of timer 3 is checked to see if the time elapsed makes the press a short or long press
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	uint16_t tim_val = __HAL_TIM_GET_COUNTER(&htim3);
	uint8_t button_state = NO_PRESS;

	if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 1) && (tim_val > 0)) {
		HAL_TIM_Base_Stop(&htim3);
		__HAL_TIM_SET_COUNTER(&htim3, 0);

		if (tim_val > LONG_PRESS_THRESH) {
			button_state = LONG_PRESS;
		} else {
			button_state = SHORT_PRESS;
		}
		xQueueSendFromISR(UserInputHandle, &button_state, 0);
	} else {
		HAL_TIM_Base_Start(&htim3);
	}
}
