/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "MFRC.h"
#include "OLED.h"
#include "queue.h"
#include "W25N01GVZEIG.h"
#include "ruthlessfs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* Definitions for PERIPHINIT */
osThreadId_t PERIPHINITHandle;
const osThreadAttr_t PERIPHINIT_attributes = {
  .name = "PERIPHINIT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ReadCard */
osThreadId_t ReadCardHandle;
const osThreadAttr_t ReadCard_attributes = {
  .name = "ReadCard",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for WriteCard */
osThreadId_t WriteCardHandle;
const osThreadAttr_t WriteCard_attributes = {
  .name = "WriteCard",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Home */
osThreadId_t HomeHandle;
const osThreadAttr_t Home_attributes = {
  .name = "Home",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CardFound */
osThreadId_t CardFoundHandle;
const osThreadAttr_t CardFound_attributes = {
  .name = "CardFound",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ShowFiles */
osThreadId_t ShowFilesHandle;
const osThreadAttr_t ShowFiles_attributes = {
  .name = "ShowFiles",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UidtoFound */
osMessageQueueId_t UidtoFoundHandle;
const osMessageQueueAttr_t UidtoFound_attributes = {
  .name = "UidtoFound"
};
/* USER CODE BEGIN PV */
char TC[]="HVE strongly condemns malicious use of it's products.The Ruthless RFID is sold as an educational device. HVE is not liable for damages caused by misuse.";

typedef struct {
	int id;
	char name[100];
}message;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
void Start_Init(void *argument);
void StartReadCard(void *argument);
void StartWriteCard(void *argument);
void StartHome(void *argument);
void CardFoundStart(void *argument);
void StartShowFiles(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void FIFO_Check(void){
	uint8_t data[3]={0x00,0x00,0x00};
	uint8_t rec[3];
	uint8_t lev;
	MFRC_REGW(FIFO_LEV,0x80);
	MFRC_FIFOW(data, 3);
	MFRC_REGR(FIFO_LEV,&lev);
	HAL_Delay(1);
	MFRC_FIFOR(rec, 3);
	CDC_Transmit_FS(rec, 3);
	HAL_Delay(1);
	CDC_Transmit_FS(&lev, 1);
}

void BUZZ(void){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_Delay(1000);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3);
}

int choose (const Screen* screen,int* flag, uint32_t* count, int max, int restopt) {
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==0){
	 	    __HAL_TIM_SET_COUNTER(&htim3,0);
	 	    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)==0){
	 	    	HAL_TIM_Base_Start(&htim3);
	 	    	if(__HAL_TIM_GET_COUNTER(&htim3)==999){
	 	    		HAL_TIM_Base_Stop(&htim3);
	 	    		*flag = 1;
	 	    		return(*count);
	 	    		}
	 	    	}
	 	    (*count)++;
	 	    if (*count == max) {
	 	    	*count = 0;
	 	    }
	 	    OLED_SELECT(screen, *count, restopt);
	 	    HAL_TIM_Base_Stop(&htim3);
	 	}
}

HAL_StatusTypeDef writecard (uint8_t* carddump) {
	uint32_t* address[2];
	findfreeaddr(address);

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of UidtoFound */
  UidtoFoundHandle = osMessageQueueNew (1, sizeof(Card*), &UidtoFound_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of PERIPHINIT */
  PERIPHINITHandle = osThreadNew(Start_Init, NULL, &PERIPHINIT_attributes);

  /* creation of ReadCard */
  ReadCardHandle = osThreadNew(StartReadCard, NULL, &ReadCard_attributes);

  /* creation of WriteCard */
  WriteCardHandle = osThreadNew(StartWriteCard, NULL, &WriteCard_attributes);

  /* creation of Home */
  HomeHandle = osThreadNew(StartHome, NULL, &Home_attributes);

  /* creation of CardFound */
  CardFoundHandle = osThreadNew(CardFoundStart, NULL, &CardFound_attributes);

  /* creation of ShowFiles */
  ShowFilesHandle = osThreadNew(StartShowFiles, NULL, &ShowFiles_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 420-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 25-1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 42000-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OLED_RST_Pin|CS_OLED_Pin|CS_MEM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|MFRC_NRST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_RST_Pin CS_OLED_Pin CS_MEM_Pin */
  GPIO_InitStruct.Pin = OLED_RST_Pin|CS_OLED_Pin|CS_MEM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 MFRC_NRST_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|MFRC_NRST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_Start_Init */
/**
  * @brief  Function implementing the PERIPHINIT thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_Init */
void Start_Init(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	vTaskSuspend(ReadCardHandle);
    vTaskSuspend(WriteCardHandle);
    vTaskSuspend(HomeHandle);
    vTaskSuspend(CardFoundHandle);
    vTaskSuspend(ShowFilesHandle);

    MFRC_INIT();
    MFRC_ANTOFF();
    OLED_INIT();
    OLED_Print(TC);
    MEM_INIT();
    block_erase(0x0000);

    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) != 0);
    vTaskResume(HomeHandle);
    osDelay(10);
    vTaskSuspend(NULL);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartReadCard */
/**
* @brief Function implementing the ReadCard thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartReadCard */
void StartReadCard(void *argument)
{
  /* USER CODE BEGIN StartReadCard */
	Card* read_card = malloc(sizeof(Card)); //Store our read card here
	int ranonce = 0;
  /* Infinite loop */
  for(;;)
  {
	int suspend = 0;
	MFRC_ANTON();
	if (ranonce == 0){
		OLED_SCREEN(&SCRN_ReadCard, NORMAL);
		ranonce++;
	}
	if(UL_readcard(read_card) == PCD_OK){
			BUZZ();
			MFRC_ANTOFF();
			xQueueSend(UidtoFoundHandle,&read_card,0); //Send a pointer to our string to the Card Found task to use
			suspend = 1;
		}
	if (suspend == 1) {
		vTaskResume(CardFoundHandle);
		ranonce = 0;
		vTaskSuspend(NULL);
		}

	}
  /* USER CODE END StartReadCard */
}

/* USER CODE BEGIN Header_StartWriteCard */
/**
* @brief Function implementing the WriteCard thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWriteCard */
void StartWriteCard(void *argument)
{
  /* USER CODE BEGIN StartWriteCard */
  int ranonce = 0;
  /* Infinite loop */
  for(;;)
  {
	  if (ranonce == 0){
	  	OLED_SCREEN(&SCRN_WriteCard, NORMAL);
	  	ranonce++;
	  }
	  osDelay(1);
  }
  /* USER CODE END StartWriteCard */
}

/* USER CODE BEGIN Header_StartHome */
/**
* @brief Function implementing the Home thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHome */
void StartHome(void *argument)
{
  /* USER CODE BEGIN StartHome */
	uint32_t count = 0;
	int ranonce = 0;
  /* Infinite loop */
  for(;;)
  {
	  int suspend = 0;
	  if (ranonce == 0) {
		  OLED_SCREEN(&SCRN_Home, NORMAL);
		  OLED_SELECT(&SCRN_Home, count, OLED_RESTORE);
		  ranonce++;
	  }
	  choose(&SCRN_Home,&suspend,&count,6,OLED_RESTORE);
	  if (suspend == 1) {
		switch(count) {
			case 0:
				vTaskResume(ReadCardHandle);
				break;
			case 1:
				vTaskResume(WriteCardHandle);
				break;
			case 2:
				vTaskResume(ShowFilesHandle);
				break;
		}
		ranonce = 0;
		vTaskSuspend(NULL);
	  }


  }
  /* USER CODE END StartHome */
}

/* USER CODE BEGIN Header_CardFoundStart */
/**
* @brief Function implementing the CardFound thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CardFoundStart */
void CardFoundStart(void *argument)
{
  /* USER CODE BEGIN CardFoundStart */
	 int count = 0;
	 int ranonce = 0;
	 Card* read_card;
  /* Infinite loop */
  for(;;)
  {
	int suspend = 0;
	if (ranonce == 0) {
		while(xQueueReceive(UidtoFoundHandle, &read_card, 0) != pdTRUE);
		char* uid_str = uid_tostring(read_card->uid, read_card->uidsize);
		OLED_SCREEN(&SCRN_CardFound, NORMAL);
		OLED_SCRNREF(&SCRN_CardFound, UID_LOC, uid_str);
		OLED_SCRNREF(&SCRN_CardFound, CARDTYPE_LOC, read_card->type);
		OLED_SELECT(&SCRN_CardFound, count, OLED_NORESTORE);
		ranonce++;
		free(uid_str);
	}

	choose(&SCRN_CardFound, &suspend, &count, 2, OLED_NORESTORE);
 	if (suspend == 1) {
 		vTaskResume(HomeHandle);
 		ranonce = 0;
 		count = 0;
 		if (count == 0) {
 			enter_card(read_card, mem_find_free_block());
 		}
 		vTaskSuspend(NULL);
 	}

  }
  /* USER CODE END CardFoundStart */
}

/* USER CODE BEGIN Header_StartShowFiles */
/**
* @brief Function implementing the ShowFiles thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartShowFiles */
void StartShowFiles(void *argument)
{
  /* USER CODE BEGIN StartShowFiles */
  int count = 0;
  int ranonce = 0;
	/* Infinite loop */
  for(;;)
  {
	  if (ranonce == 0) {
		  OLED_SCREEN(&SCRN_ShowFiles, NORMAL);
		  OLED_SELECT(&SCRN_ShowFiles, 0, OLED_RESTORE);
		  OLED_display_files(&SCRN_ShowFiles, 0);
		  ranonce++;
	  }
  }
  /* USER CODE END StartShowFiles */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
