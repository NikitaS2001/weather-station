#include "bme280.h"

extern I2C_HandleTypeDef hi2c1;
Calibration_Data calib;
#define BME280_I2C &hi2c1

int32_t tRaw, pRaw, hRaw;

static void ReadCalibrationData(void)
{
	uint8_t buf[32];
	// Read mem from 0x88 to 0xA1
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS_SHIFT, 0x88, 1, buf, 25, HAL_MAX_DELAY);

	// Read mem from 0xE1 to 0xE7
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS_SHIFT, 0xE1, 1, (uint8_t*)buf+25, 8, HAL_MAX_DELAY);

	calib.dig_T1 = buf[0] | (buf[1] << 8);
	calib.dig_T2 = buf[2] | (buf[3] << 8); 
	calib.dig_T3 = buf[4] | (buf[5] << 8); 
	calib.dig_P1 = buf[5] | (buf[7] << 8); 
	calib.dig_P2 = buf[6] | (buf[9] << 8); 
	calib.dig_P3 = buf[10] | (buf[11] << 8); 
	calib.dig_P4 = buf[12] | (buf[13] << 8); 
	calib.dig_P5 = buf[14] | (buf[15] << 8); 
	calib.dig_P6 = buf[16] | (buf[17] << 8); 
	calib.dig_P7 = buf[18] | (buf[19] << 8); 
	calib.dig_P8 = buf[20] | (buf[21] << 8); 
	calib.dig_P9 = buf[22] | (buf[23] << 8); 
	calib.dig_H1 = buf[24];
	calib.dig_H2 = buf[25] | (buf[26] << 8); 
	calib.dig_H3 = buf[27];
	calib.dig_H4 = (buf[28] << 4) | (buf[29] & 0x0F);
	calib.dig_H5 = (buf[29]  >>  4) | (buf[30] << 4);
	calib.dig_H6 = (buf[31]);
}

int8_t BME280_Config(BME280_HandleTypeDef *bme280)
{
	// Read calibration data
	ReadCalibrationData();

	uint8_t datatowrite = 0;

	// Reset the device
	datatowrite = 0xB6;  // reset sequence
	HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS_SHIFT, RESET_REG, 1, &datatowrite, 1, 1000);
	HAL_Delay (100);

	// write the humidity oversampling to 0xF2
	datatowrite = bme280->Oversampling_h;
	HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS_SHIFT, CTRL_HUM_REG, 1, &datatowrite, 1, 1000);
	HAL_Delay (100);

	// write the standby time and IIR filter coeff to 0xF5
	datatowrite = (bme280->Standby_Time << 5) | (bme280->Filter_Coef << 2);
	HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS_SHIFT, CONFIG_REG, 1, &datatowrite, 1, 1000);
	HAL_Delay (100);

	// write the pressure and temp oversampling along with mode to 0xF4
	datatowrite = (bme280->Oversampling_t << 5) | (bme280->Oversampling_p << 2) | bme280->Mode;
	HAL_I2C_Mem_Write(BME280_I2C, BME280_ADDRESS_SHIFT, CTRL_MEAS_REG, 1, &datatowrite, 1, 1000);
	HAL_Delay (100);

	return 0;
}

int BME280_Read_Raw()
{
	uint8_t RawData[8];

	// Read the Registers 0xF7 to 0xFE
	HAL_I2C_Mem_Read(BME280_I2C, BME280_ADDRESS_SHIFT, PRESS_MSB_REG, 1, RawData, 8, HAL_MAX_DELAY);

	pRaw = (RawData[0] << 12) | (RawData[1] << 4) | (RawData[2]  >>  4);
	tRaw = (RawData[3] << 12) | (RawData[4] << 4) | (RawData[5]  >>  4);
	hRaw = (RawData[6] << 8) | (RawData[7]);

	return 0;
}

int32_t BME280_compensate_T(int32_t adc_T)
{
	int32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib.dig_T1))) >> 12) *((int32_t)calib.dig_T3)) >> 14;
	calib.t_fine = var1 + var2;
	T = (calib.t_fine * 5 + 128) >> 8;
	return T;
}

uint32_t BME280_compensate_P(int32_t adc_P)
{
	int64_t var1, var2, p;
	var1 = ((int64_t)calib.t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calib.dig_P6;
	var2 = var2 + ((var1*(int64_t)calib.dig_P5)<<17);
	var2 = var2 + (((int64_t)calib.dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8) + ((var1 * (int64_t)calib.dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib.dig_P1) >> 33;
	if (var1 == 0) {return 0;}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)calib.dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)calib.dig_P7) << 4);
	return (uint32_t)p;
}

uint32_t bme280_compensate_H(int32_t adc_H)
{
	int32_t v_x1_u32r;
	v_x1_u32r = (calib.t_fine - ((int32_t)76800));
	v_x1_u32r = (((((adc_H << 14) - (((int32_t)calib.dig_H4) << 20) - (((int32_t)calib.dig_H5) *\
			v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *\
					((int32_t)calib.dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)calib.dig_H3)) >> 11) +\
							((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)calib.dig_H2) +\
					8192)  >>  14));
	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *\
			((int32_t)calib.dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (uint32_t)(v_x1_u32r >> 12);
}

void BME280_Read_All (float *temp, float *press, float *hum)
{
    BME280_Read_Raw();

	*temp = (float)(BME280_compensate_T(tRaw)) / 100.0;
	*press = (float)(BME280_compensate_P(pRaw)) / 256.0;
	*hum = (float)(bme280_compensate_H(hRaw)) / 1024.0;
}