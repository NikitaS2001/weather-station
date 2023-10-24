#ifndef BME280_H_
#define BME280_H_

#include "stm32f1xx_hal.h"

typedef struct 
{
    uint8_t Oversampling_t;
    uint8_t Oversampling_p;
    uint8_t Oversampling_h;
    uint8_t Mode;
    uint8_t Standby_Time;
    uint8_t Filter_Coef;
} BME280_HandleTypeDef;

// Address sensor
#define BME280_ADDRESS 0x76
#define BME280_ADDRESS_SHIFT 0xEC // 0x76 << 1

// Oversampling definitions
#define OSRS_OFF       0x00
#define OSRS_1         0x01
#define OSRS_2         0x02
#define OSRS_4         0x03
#define OSRS_8         0x04
#define OSRS_16        0x05

// MODE Definitions
#define MODE_SLEEP     0x00
#define MODE_FORCED    0x01
#define MODE_NORMAL    0x03

// Standby Time
#define T_SB_0p5       0x00
#define T_SB_62p5      0x01
#define T_SB_125       0x02
#define T_SB_250       0x03
#define T_SB_500       0x04
#define T_SB_1000      0x05
#define T_SB_10        0x06
#define T_SB_20        0x07

// IIR Filter Coefficients
#define IIR_OFF        0x00
#define IIR_2          0x01
#define IIR_4          0x02
#define IIR_8          0x03
#define IIR_16         0x04


// REGISTERS DEFINITIONS
#define ID_REG         0xD0
#define RESET_REG  	   0xE0
#define CTRL_HUM_REG   0xF2
#define STATUS_REG     0xF3
#define CTRL_MEAS_REG  0xF4
#define CONFIG_REG     0xF5
#define PRESS_MSB_REG  0xF7

typedef struct
{
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
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
    int32_t t_fine;
} Calibration_Data;

int8_t BME280_Config(BME280_HandleTypeDef *TypeDef);
void BME280_Read_All(float *t, float *p, float *h);

#endif /* BME280_H_ */
