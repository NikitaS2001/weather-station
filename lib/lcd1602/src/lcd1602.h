#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <stm32f1xx_hal.h>

int8_t LCD_Config(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendString(char *str);
void LCD_SendData(uint8_t data);

#endif /* __LCD1602_H__ */