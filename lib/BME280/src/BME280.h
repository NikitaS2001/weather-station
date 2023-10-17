#ifndef BME280_H
#define BME280_H

#include "i2c.h"
#include "delay.h"

#define NORMAL_MODE	0x03
#define FORCED_MODE	0x02

#define STANDBY_500US  0x00
#define STANDBY_10MS   0x06
#define STANDBY_20MS   0x07
#define STANDBY_6250US 0x01
#define STANDBY_125MS  0x02
#define STANDBY_250MS  0x03
#define STANDBY_500MS  0x04
#define STANDBY_1000MS 0x05

#define MODULE_DISABLE		  0x00
#define OVERSAMPLING_1        0x01
#define OVERSAMPLING_2        0x02
#define OVERSAMPLING_4        0x03
#define OVERSAMPLING_8        0x04
#define OVERSAMPLING_16       0x05

#define FILTER_DISABLE  0x00
#define FILTER_COEF_2   0x01
#define FILTER_COEF_4   0x02
#define FILTER_COEF_8   0x03
#define FILTER_COEF_16  0x04


// ================================= CLASS ===================================

class bme280
{
public:
	bme280(uint8_t address = 0x76);
	~bme280();

private:
	//============================== DEFAULT SETTINGS ========================================|
    int _i2c_address = 0x76;					// BME280 address on I2C bus                  |
    uint8_t _operating_mode = NORMAL_MODE;		// Sensor operation mode                      |
    uint8_t _standby_time = STANDBY_250MS;		// Time between measurements in NORMAL_MODE	  |
    uint8_t _filter_coef = FILTER_COEF_16; 		// Filter ratio IIR                           |
    uint8_t _temp_oversampl = OVERSAMPLING_4;	// Temperature module oversampling parameter  |
    uint8_t _hum_oversampl = OVERSAMPLING_1;	// Humidity module oversampling parameter     |
    uint8_t _press_oversampl = OVERSAMPLING_2;	// Pressure module oversampling parameter     |
    //========================================================================================|
    
    bool reset(void);                                   	// BME280 software reset 
    int32_t readTempInt();                              	// Temperature reading in integers for the function of reading
    void readCalibrationData(void);                     	// Read all cells containing calibration values
    uint8_t readRegister(uint8_t address);   // Read one 8-bit BME280 register
    uint32_t readRegister24(uint8_t address);        		// Read and combine three BME280 registers
    void readRegisters(uint8_t address, uint8_t *data, uint8_t size);
	bool writeRegister(uint8_t address , uint8_t data);		// Write one 8-bit BME280 register

    struct {												// Structure to store all calibration values
        uint16_t _T1;
        int16_t _T2;
        int16_t _T3;
        uint16_t _P1;
        int16_t _P2;
        int16_t _P3;
        int16_t _P4;
        int16_t _P5;
        int16_t _P6;
        int16_t _P7;
        int16_t _P8;
        int16_t _P9;
        uint8_t _H1;
        int16_t _H2;
        uint8_t _H3;
        int16_t _H4;
        int16_t _H5;
        int8_t _H6;
    } CalibrationData;
};

float pressureToMmHg(float pressure);		// Convert [Pa] to [mm Hg]  
float pressureToAltitude(float pressure);	// Convert pressure to altitude
#endif /* BME280_H */