#include "bme280.h"
#include <math.h>

/* ============ Utilities ============ */

float pressureToAltitude(float pressure) {
    if (!pressure) return 0;									// If the pressure module has been disabled return '0'
    pressure /= 100.0F;											// Convert [Pa] to [hPa]
    return 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));	// Сalculate altitude
}

float pressureToMmHg(float pressure) {							
    return (float)(pressure * 0.00750061683f);					// Convert [Pa] to [mm Hg]
}


bme280::bme280(uint8_t address = 0x76)
{
    _i2c_address = address;
    /* === Start I2C bus & check BME280 === */
    

}

bme280::~bme280(){ }

/* ============ Private ============ */

bool bme280::writeRegister(uint8_t address , uint8_t data)
{
    I2C1_Start();
    I2C1_SendAddress(_i2c_address, I2C_TRANSMITTER);
    I2C1_SendData(address);

    I2C1_SendData(data);

    I2C1_Stop();
}

void bme280::readRegisters(uint8_t address, uint8_t *data, uint8_t size)
{
    uint8_t count = 0;

    // I2C Send
	I2C1_Start();
	I2C1_SendAddress(_i2c_address, I2C_TRANSMITTER);
	I2C1_SendData(address);

	// I2C Receive
	I2C1_Start();
	I2C1_SendAddress(_i2c_address, I2C_RECEIVER);

    for (count = 0; count < size - 1; ++count ) {
		data[count] = I2C1_ReceiveData(I2C_ACK);
	}

	I2C1_Stop();
	data[count] = I2C1_ReceiveData(I2C_NACK);
}

uint8_t bme280::readRegister(uint8_t address)
{
    uint8_t data[1] = {0};

    bme280::readRegisters(address, data, 1);

    return data[0];
}

uint8_t bme280::readRegister24(uint8_t address)
{
    uint8_t data[3] = {0, 0, 0};

    bme280::readRegisters(address, data, 3);
}