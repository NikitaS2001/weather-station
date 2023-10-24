#include "lcd1602.h"

extern I2C_HandleTypeDef hi2c1;
#define BME280_I2C &hi2c1

#define PCF8574A_ADDRESS 0x3F
#define PCF8574A_ADDRESS_SHIFT (PCF8574A_ADDRESS << 1)

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)


int8_t LCD_Config(void)
{
    LCD_SendCommand(0b00110000);

    LCD_SendCommand(0b00000010);

    LCD_SendCommand(0b00001100);

    LCD_SendCommand(0b00000001);

    return 0;
}

void LCD_SendString(char *str)
{
    while(*str++) { LCD_SendData((uint8_t)(*str)); }
}

void LCD_SendHalfByte(uint8_t data, uint8_t flags) 
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

void LCD_SendCommand(uint8_t cmd)
{
    LCD_SendHalfByte(cmd, 0);
}

void LCD_SendData(uint8_t data)
{
    LCD_SendHalfByte(data, PIN_RS);
}