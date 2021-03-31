/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Defines.h"
#include "ILI9341.h"
#include "MAX31855.h"
#include "Touch.h"
#include "ControllerPID.h"
#include "Load.h"
#include "functions.h"
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

/* USER CODE BEGIN PV */
volatile interrupt_data_t interrupt_data =
    { 0 };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_SPI1_Init();
    MX_TIM6_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM7_Init();
    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_TIM9_Init();
    MX_TIM10_Init();
    /* USER CODE BEGIN 2 */

    SPI_Repair();    // Rekonfiguration des SPI-Peripherals für MAX31855

    HAL_TIM_Base_Start_IT(&htim2);    // Globalen Timer starten.

    ILI9341 tft(TFT_ROATION_PORTRAIT);    // TFT-Display mit Rotation 4 (Ladnscape)
    MAX31855 temp(&hspi1, TEMP_CS_GPIO_Port, TEMP_CS_Pin);
    Touch touch(&hadc2, &hadc1, &interrupt_data.touchReadyFlag, &htim10);
    ControllerPID pid;    // PID-Controller

    Load heater(&htim3, &htim4, &interrupt_data.heater_state);
    Load fan(&htim5, &htim6, &interrupt_data.fan_state);
    Load light(&htim7, &htim9, &interrupt_data.light_state);

    context_t context(&tft, &temp, &touch, &pid, &heater, &fan, &light);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        //////////////////////////////////
        //   ---- STATE-MACHINE ----    //
        //////////////////////////////////

        switch (context.currentState) {

            case STATE_BOOT:
                boot_initSystem(&context);
                break;

            case STATE_MENU_INIT:
                menu_init(&context);
                break;

            case STATE_MENU:
                menu_update(&context);
                break;

            case STATE_REFLOW_INIT:
                reflow_init(&context);
                break;

            case STATE_REFLOW:
                reflow_update(&context);
                break;

            case STATE_TEMPERATE_INIT:
                temperate_init(&context);
                break;

            case STATE_TEMPERATE:
                temperate_update(&context);
                break;

            case STATE_OPTIONS_INIT:
                options_init(&context);
                break;

            case STATE_OPTIONS:
                options_update(&context);
                break;

            case STATE_ERROR:
                error_show(&context);
                while (1);
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
    RCC_OscInitTypeDef RCC_OscInitStruct =
        { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct =
        { 0 };

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
            | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
        case ZCD_Pin:
            // HEATER
            if (interrupt_data.heater_state == LOAD_DUTY)
            {
                HAL_TIM_Base_Start_IT(&htim3);
                HAL_TIM_Base_Start_IT(&htim4);
            }
            else if (interrupt_data.heater_state == LOAD_ON)
                HEATER_GPIO_Port->BSRR = HEATER_Pin;

            else if (interrupt_data.heater_state == LOAD_OFF)
                HEATER_GPIO_Port->BSRR = (uint32_t) HEATER_Pin << 16U;

            // FAN
            if (interrupt_data.fan_state == LOAD_DUTY)
            {
                HAL_TIM_Base_Start_IT(&htim5);
                HAL_TIM_Base_Start_IT(&htim6);
            }
            else if (interrupt_data.fan_state == LOAD_ON)
                FAN_GPIO_Port->BSRR = FAN_Pin;

            else if (interrupt_data.fan_state == LOAD_OFF)
                FAN_GPIO_Port->BSRR = (uint32_t) FAN_Pin << 16U;

            // LIGHT
            if (interrupt_data.light_state == LOAD_DUTY)
            {
                HAL_TIM_Base_Start_IT(&htim7);
                HAL_TIM_Base_Start_IT(&htim9);
            }
            else if (interrupt_data.light_state == LOAD_ON)
                LIGHT_GPIO_Port->BSRR = LIGHT_Pin;

            else if (interrupt_data.light_state == LOAD_OFF)
                LIGHT_GPIO_Port->BSRR = (uint32_t) LIGHT_Pin << 16U;

            break;

        default:
            break;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2)
    {
        interrupt_data.timerOverflow++;
    }
    else if (htim == &htim3)    // Heizelement AN - Impuls START
    {
        HEATER_GPIO_Port->BSRR = HEATER_Pin;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;

    }
    else if (htim == &htim4)    // Heizelement AN - Impuls STOP
    {
        HEATER_GPIO_Port->BSRR = (uint32_t) HEATER_Pin << 16U;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;
    }
    else if (htim == &htim5)    // Lüfter AN - Impuls START
    {
        FAN_GPIO_Port->BSRR = FAN_Pin;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;
    }
    else if (htim == &htim6)    // Lüfter AN - Impuls STOP
    {
        FAN_GPIO_Port->BSRR = (uint32_t) FAN_Pin << 16U;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;
    }
    else if (htim == &htim7)    // Licht AN - Impuls START
    {
        LIGHT_GPIO_Port->BSRR = LIGHT_Pin;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;
    }
    else if (htim == &htim9)    // Licht AN - Impuls STOP
    {
        LIGHT_GPIO_Port->BSRR = (uint32_t) LIGHT_Pin << 16U;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;
    }
    else if (htim == &htim10)    // Touch-Delay
    {
        interrupt_data.touchReadyFlag = 1;
        HAL_TIM_Base_Stop_IT(htim);
        htim->Instance->CNT = 0;
    }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
