#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <stm32f1xx_hal.h>

int8_t LCD_Config(void);
void LCD_Send(uint8_t cmd, uint8_t flags);
void LCD_SendString(char *str);

#endif /* __LCD1602_H__ */