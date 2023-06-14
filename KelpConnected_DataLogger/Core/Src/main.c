/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author			: Miloh Padgett
  * @date			: 06-09-2023
  ******************************************************************************
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include <string.h>
#define MAX_BUFFER 16384
#define CONNECT 2
#define CONFIGURE 3
#define DOWNLOAD 4
#define FIN 1
void startup_delay();
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t buffer_index = 0;
uint8_t *write_buff;
uint8_t err[] = "error";
uint8_t cmd[4];
uint8_t buff[MAX_BUFFER];
uint8_t byte;
int pos = 0;
int ready = 0;
int auton = 0;
int sleep = 0;
int download = 0;
long elapsedTime = 0;


struct Config{
	uint32_t t_between;
	uint32_t smpl_duration;
	uint32_t start_delay;
}config;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART3)
  {
	  /*communication with the PC*/

	  uint8_t ack = '8';
	  uint8_t err = '9';
	  int type = (int)cmd[0];
	  int DB1 = (int)cmd[1];
	  int DB2 = (int)cmd[2];
	  int DB3 = (int)cmd[3];

	  switch(type){
	  	  case CONNECT:
	  		  /*transmit an ack and wait for command*/
	  		  HAL_UART_Transmit(&huart3, &ack, 1, 100);
	  		  HAL_UART_Receive_IT(&huart3, cmd, 4);
	  		  break;
	  	  case CONFIGURE:
	  		/*transmit an ack and parse configuration settings*/
	  		  HAL_UART_Transmit(&huart3, &ack, 1, 100);
	  		  config.t_between = 60*DB1;
	  		  config.smpl_duration = DB2;
	  		  config.start_delay = 3600*DB3;
	  		  HAL_UART_Receive_IT(&huart3, cmd, 4);
	  		  break;
	  	  case FIN:
	  		  /*transmit a deployment ack*/
	  		  HAL_UART_Transmit(&huart3, &ack, 1, 100);
	  		  ready = 1;
	  		  break;
	  	  default:
	  		  HAL_UART_Transmit(&huart3, &err, 1, 100);
	  		  break;
	  }



  }

  if (huart->Instance == USART1){
	  /*communication with the sensor*/

	  if ((elapsedTime <= config.smpl_duration))
	  {
		  /*if sample time has not elapsed, store byte from sensor*/
		  buff[pos] = byte;
	  	  pos++;
	  	  HAL_UART_Receive_IT(&huart1, &byte, 1);
	  }
	  else{

		  /*if sample time has elapsed, turn off sensor and stop timer*/
		  GPIOB->BRR = (UP1_Pin | UP2_Pin | SENSOR_POW_Pin);
		  HAL_TIM_Base_Stop_IT(&htim2);
		  elapsedTime = 0;
		  sleep = 1;
	  }

  }

}

void format_SD(){
	/*format the SD card on startup*/
	FATFS fs;
	FRESULT res; /* FatFs function common result code */
	uint8_t rtext[_MAX_SS];/* File read buffer */
	GPIOB->BSRR = (SDP1_Pin | SDP2_Pin);
	HAL_Delay(5);

	res = f_mount(&SDFatFS, SDPath, 1);
	if(res != FR_OK) {
		Error_Handler();
	}
	else
	{
		if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, rtext, sizeof(rtext)) != FR_OK)
		{
			Error_Handler();
		}
		else
		{
			//Open file for writing (Create)
			if(f_open(&SDFile, "LOG.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
			{
				Error_Handler();
			}
			f_close(&SDFile);
		}
	}
	/*unmount and turn off sd card*/
	f_mount(&SDFatFS, (TCHAR const*)NULL, 0);
	FATFS_UnLinkDriver(SDPath);
	HAL_Delay(5);
	GPIOB->BRR = (SDP1_Pin | SDP2_Pin);

}

void startup_delay(){
	/*format SD card and then delay for specified time*/
	format_SD();
	if(config.start_delay>0){
		HAL_SuspendTick();
		HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, config.start_delay, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

		/* Enter STOP 2 mode */
		HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
		HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
		SystemClock_Config();
		HAL_ResumeTick();
	}
	ready = 0;
	auton = 1;

}

void collect_loop(){
	 /*start listening to Sensor, and turn on timer*/

	 //turn on sensor
	 GPIOB->BSRR = (UP1_Pin | UP2_Pin | SENSOR_POW_Pin);
	 HAL_Delay(5);

	 /*timer*/
	 HAL_TIM_Base_Start_IT(&htim2);
	 HAL_UART_Receive_IT(&huart1, &byte, 1);
	 /*dont enter this loop right now*/
	 auton = 0;

}

int cbuff(uint8_t* new_buff){
	/*ignore incomplete lines of data*/
	int begin = 0;
	int end = MAX_BUFFER - 1;
	int len = 0;
	while((buff[begin] != '\n') && (begin < pos)){
		begin ++;
	}
	while((buff[end] != '\n') && (end > 0)){
		end --;
	}
	end++;
	len = end - begin;
	memcpy(new_buff, &buff[begin], len);
	return len;

}

void write_SD(uint8_t *new_buff, int len){
	/*write data to the SD card*/
	FRESULT res;
	uint32_t byteswritten;

	GPIOB->BSRR = (SDP1_Pin | SDP2_Pin);
	HAL_Delay(100);
	MX_FATFS_Init();
	res = f_mount(&SDFatFS, SDPath, 1);
	if(res != FR_OK) {
		Error_Handler();
	}
	else{
		//Open file for writing (Append)
		if(f_open(&SDFile, "LOG.TXT", FA_OPEN_APPEND | FA_WRITE) != FR_OK)
		{
			Error_Handler();
		}
		else
		{
			//Write to the text file
			res = f_write(&SDFile, new_buff, len, (void *)&byteswritten);
			if((byteswritten == 0) || (res != FR_OK))
			{
				Error_Handler();
			}
			else{
				f_close(&SDFile);
			}
		}
	}
	/*unmount and turn off SD card*/
	f_mount(&SDFatFS, (TCHAR const*)NULL, 0);
	FATFS_UnLinkDriver(SDPath);
	HAL_Delay(5);
	GPIOB->BRR = (SDP1_Pin | SDP2_Pin);


}

void sleep_between(){
	  /*write data to sd card and then sleep for interval*/
	  uint8_t new_buff[MAX_BUFFER];
	  int len;
	  len = cbuff(new_buff);
	  pos = 0;

	  /*write to SD card*/
	  write_SD(new_buff, len);
	  for(int i= 0; i< MAX_BUFFER; i++){
		  buff[i] = 0;
	  }

	  HAL_SuspendTick();
	  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, config.t_between, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

	  /* Enter STOP 2 mode */
	  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
	  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	  SystemClock_Config();
	  HAL_ResumeTick();

	  auton = 1;
	  sleep = 0;

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
		elapsedTime++;
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
  MX_USART1_UART_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /*begin reveiving commands*/
  HAL_UART_Receive_IT(&huart3, cmd, 4);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*state machine*/
	  if(ready == 1){
		  startup_delay();
	  }
	  if(auton == 1){
		  collect_loop();
	  }
	  if(sleep == 1){
		  sleep_between();
	  }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 19200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, UP1_Pin|UP2_Pin|SENSOR_POW_Pin|SDP1_Pin
                          |SDP2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA12 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : UP1_Pin UP2_Pin SENSOR_POW_Pin SDP1_Pin
                           SDP2_Pin */
  GPIO_InitStruct.Pin = UP1_Pin|UP2_Pin|SENSOR_POW_Pin|SDP1_Pin
                          |SDP2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	//HAL_UART_Transmit(&huart2, err, 5, 100);
  //__disable_irq();
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
