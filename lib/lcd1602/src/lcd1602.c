#include "lcd1602.h"

extern I2C_HandleTypeDef hi2c1;
#define BME280_I2C &hi2c1

#define PCF8574A_ADDRESS 0x3F
#define PCF8574A_ADDRESS_SHIFT (PCF8574A_ADDRESS << 1)

#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)


// flags = 1 - data | flags = 0 - command

int8_t LCD_Config(void)
{
    LCD_Send(0b00110000, 0);

    LCD_Send(0b00000010, 0);

    LCD_Send(0b00001100, 0);

    LCD_Send(0b00000001, 0);

    return 0;
}

void LCD_SendString(char *str)
{
    while(*str++) { LCD_Send((uint8_t)(*str), 1); }
}

void LCD_Send(uint8_t data, uint8_t flags) 
{
    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t buf[4];
    buf[0] = up | flags | BACKLIGHT | PIN_EN;
    buf[1] = up | flags | BACKLIGHT;
    buf[2] = lo | flags | BACKLIGHT | PIN_EN;
    buf[3] = lo | flags | BACKLIGHT;

    HAL_I2C_Master_Transmit(BME280_I2C, PCF8574A_ADDRESS_SHIFT, buf, sizeof(buf), HAL_MAX_DELAY);
    HAL_Delay(5);
}