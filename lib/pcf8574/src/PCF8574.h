#ifndef PCF8574_H_
#define PCF8574_H_


#include "i2c.h"
#include "delay.h"

// #define SLAVE_ADDRESS_LCD 0x4E
#define SLAVE_ADDRESS_LCD 0x70 << 1

void LCD_Init(void);
void LCD_SendCmd(char cmd);
void LCD_SendData(char data);
void LCD_SendString(char* str);
void LCD_Clear(void);

#endif /* PCF8574_H_ */