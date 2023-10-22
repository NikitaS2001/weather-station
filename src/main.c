#include "main.h"
#include "delay.h"

#define BME280_ADDRESS 0x76

I2C_HandleTypeDef hi2c1;
Delay delay;

uint16_t dig_T1;
int16_t dig_T2;
int16_t dig_T3;
uint16_t dig_P1;
int16_t dig_P2;
int16_t dig_P3;
int16_t dig_P4;
int16_t dig_P5;
int16_t dig_P6;
int16_t dig_P7;
int16_t dig_P8;
int16_t dig_P9;
uint8_t  dig_H1;
int16_t dig_H2;
uint8_t  dig_H3;
int16_t dig_H4;
int16_t dig_H5;
int8_t  dig_H6;
int32_t t_fine;

float t, p, h;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
int8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t size_data);
int8_t BME280_init(void);
void BME280_Read_All(float *t, float *p, float *h);
float BME280_Compensate_Temperature(int32_t *adc_T);
float BME280_Compensate_Pressure(int32_t *adc_P);
float BME280_Compensate_Humidity(int32_t *adc_H);


int main(void)
{

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    BME280_init();



    while (1)
    {
        if (Delay_nb_timeout(&delay))
        {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            BME280_Read_All(&t, &p, &h);
            Delay_nb_ms(&delay, 1000);
        }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

int8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t size_data)
{
    HAL_I2C_Master_Transmit(&hi2c1, (addr << 1), &reg, 1, 10);
    HAL_I2C_Master_Receive(&hi2c1, (addr << 1), data, size_data, 10);
    return 0;
}

int8_t BME280_init(void)
{
    uint8_t buf[25] = {0};
    int16_t dig_H4_msb, dig_H4_lsb, dig_H5_msb, dig_H5_lsb;

    i2c_read(BME280_ADDRESS, 0x88, buf, 25);
    dig_T1 = (buf[0] | (buf[1] << 8));
    dig_T2 = (buf[2] | (buf[3] << 8));
    dig_T3 = (buf[4] | (buf[5] << 8));
    dig_P1 = (buf[6] | (buf[7] << 8));
    dig_P2 = (buf[8] | (buf[9] << 8));
    dig_P3 = (buf[10] | (buf[11] << 8));
    dig_P4 = (buf[12] | (buf[13] << 8));
    dig_P5 = (buf[14] | (buf[15] << 8));
    dig_P6 = (buf[16] | (buf[17] << 8));
    dig_P7 = (buf[18] | (buf[19] << 8));
    dig_P8 = (buf[20] | (buf[21] << 8));
    dig_P9 = (buf[22] | (buf[23] << 8));
    dig_H1 = buf[24];

    i2c_read(BME280_ADDRESS, 0xE1, buf, 8);
    dig_H2 = (buf[0] | (buf[1] << 8));
    dig_H3 = buf[2];

    dig_H4_msb = (int16_t)(int8_t)buf[3] * 16;
    dig_H4_lsb = (int16_t)(buf[4] & 0x0F);
    dig_H4 = dig_H4_msb | dig_H4_lsb;

    dig_H5_msb = (int16_t)(int8_t)buf[5] * 16;
	dig_H5_lsb = (int16_t)(buf[4] >> 4);
    dig_H5 = dig_H5_msb | dig_H5_lsb;
    dig_H6 = (int8_t)buf[6];

    return 0;
}

void BME280_Read_All(float *t, float *p, float *h)
{


    int32_t adc_T = 0;
    int32_t adc_P = 0;
    int32_t adc_H = 0;
	uint8_t buf[8];

    i2c_read(BME280_ADDRESS, 0xF7, buf, 8);

    adc_P = (int32_t)((buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4));
    adc_T = (int32_t)((buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4));
    adc_H = (int32_t)(buf[6] << 8 | buf[7]);

    *t = BME280_Compensate_Temperature(&adc_T);
    *p = BME280_Compensate_Pressure(&adc_P);
    *h = BME280_Compensate_Humidity(&adc_H);
}

float BME280_Compensate_Temperature(int32_t *adc_T)
{
    int32_t var1, var2, t;

	var1 = ((((*adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
	var2 = (((((*adc_T >> 4) - (int32_t)dig_T1) * ((*adc_T >> 4) - (int32_t)dig_T1)) >> 12) *
			(int32_t)dig_T3) >> 14;
	t_fine = var1 + var2;
	t = (t_fine * 5 + 128) >> 8;

    return (float)t / 100.0;
}

float BME280_Compensate_Pressure(int32_t *adc_P)
{
    int64_t var1, var2, p;

	var1 = (int64_t)t_fine - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
	var2 = var2 + ((int64_t)dig_P4 << 35);
	var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
	var1 = ((((int64_t)1 << 47) + var1)) * (int64_t)dig_P1 >> 33;

	if (var1 == 0) { return 0; }

	p = 1048576 - *adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = ((int64_t)dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t)dig_P8 * p) >> 19;
	p = ((p + var1 + var2) >> 8) + ((int64_t)dig_P7 << 4);

	return (float)p / 256.0;
}

float BME280_Compensate_Humidity(int32_t *adc_H)
{
    int32_t value = (t_fine - ((int32_t)76800));
    value = (((((*adc_H << 14) - (((int32_t)dig_H4) << 20) -
    (((int32_t)dig_H5) * value)) +((int32_t)16384)) >> 15) * 
    (((((((value * ((int32_t)dig_H6)) >> 10) *(((value * 
    ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
    value = (value - (((((value >> 15) * (value >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    value = (value < 0) ? 0 : value;
    value = (value > 419430400) ? 419430400 : value;
    float h = (value >> 12);

    return h / 1024.0;
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
  while (1) {}
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
