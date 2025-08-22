/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "gpio.h"

#include "ssd1306.h"

#include <stdio.h>

void SystemClock_Config(void);


extern I2C_HandleTypeDef hi2c3;

uint8_t data[32];

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  // MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();

  SSD1306_Init();
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  sprintf ((char*)data, "ssd1315");
  SSD1306_GotoXY(0, 0);
  SSD1306_Puts((char*)data, &Font_11x18, SSD1306_COLOR_WHITE);
  sprintf ((char*)data, "test");
  SSD1306_GotoXY(0, 20);
  SSD1306_Puts((char*)data, &Font_11x18, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  
  uint8_t i2c_data[] = {1,2,3,4};
  int32_t pressure_raw;
  float pressure = 0.0f;
  float pressure_mid = 0.0f;
  /* Infinite loop */
  while (1)
  {
    for(uint8_t i = 0; i < 20; i++)
    {
      i2c_data[0] = 0x0A;
      HAL_I2C_Mem_Write(&hi2c3, 0x6D << 1, 0x30, 1, i2c_data, 1, 100);
      // HAL_Delay(1);
      HAL_I2C_Mem_Read(&hi2c3, 0x6D << 1, 0x06, 1, i2c_data, 3, 100);
      pressure_raw = i2c_data[0] << 24;
      pressure_raw += i2c_data[1] << 16;
      pressure_raw += i2c_data[2] << 8;
      pressure_raw = pressure_raw >> 8;
      pressure = (float)pressure_raw / 4096.0f;
      pressure_mid = (pressure_mid + 0.05f * pressure) / 1.05f;
    }
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    sprintf ((char*)data, "%+.1f", pressure_mid);
    SSD1306_GotoXY(0, 0);
    SSD1306_Puts((char*)data, &Font_11x18, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    // HAL_Delay(500);
  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
