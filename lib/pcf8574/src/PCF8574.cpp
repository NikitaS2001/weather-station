#include "PCF8574.h"

void LCD_Init(void)
{
    LCD_SendCmd(0x30);
    DWT_delay_ms(5);
    LCD_SendCmd(0x30);
    DWT_delay_ms(50);
    LCD_SendCmd(0x30);
    DWT_delay_ms(10);
    LCD_SendCmd(0x20); // 4bit mode
    DWT_delay_ms(10);

    LCD_SendCmd(0x28);
    DWT_delay_ms(1);
    LCD_SendCmd(0x08);
    DWT_delay_ms(1);
    LCD_SendCmd(0x01);
    DWT_delay_ms(1);
    DWT_delay_ms(1);
    LCD_SendCmd(0x06);
    DWT_delay_ms(1);
    LCD_SendCmd(0x0C);
}

void LCD_SendCmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = cmd & 0xf0;
    data_l = (cmd << 4) & 0xf0;
    data_t[0] = data_u | 0x0C;
    data_t[1] = data_u | 0x08;
    data_t[2] = data_l | 0x0C;
    data_t[3] = data_l | 0x08;

    // I2C Send
	I2C1_Start();
	I2C1_SendAddress(SLAVE_ADDRESS_LCD, I2C_TRANSMITTER);

	for (int8_t i = 0; i < 4; ++i) {
		I2C1_SendData(data_t[i]);
	}

	I2C1_Stop();
    
    // DWT_delay_ms(100);

}

void LCD_SendData(char data)
{
    char data_u, data_l;
	uint8_t data_t[4];
	data_u = data & 0xf0;
	data_l = (data << 4) & 0xf0;
	data_t[0] = data_u | 0x0D;
	data_t[1] = data_u | 0x09;
	data_t[2] = data_l | 0x0D;
	data_t[3] = data_l | 0x09;

    // I2C Send
	I2C1_Start();
	I2C1_SendAddress(SLAVE_ADDRESS_LCD, I2C_TRANSMITTER);

	for (int8_t i = 0; i < 4; ++i)
    {
		I2C1_SendData(data_t[i]);
	}

	I2C1_Stop();
    
    DWT_delay_ms(100);
}
void LCD_SendString(char* str)
{
    while (*str) { LCD_SendData(*str++); }
}

void LCD_Clear(void)
{
    LCD_SendCmd(0x80);
    for (uint8_t i = 0; i < 70; i++)
    {
        LCD_SendData(' ');
    }
}