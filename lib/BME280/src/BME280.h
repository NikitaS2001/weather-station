#ifndef BME280_H
#define BME280_H

#include <stm32f1xx.h>

#define NORMAL_MODE	0x03

class BME280
{
private:
    
    int _i2c_address = 0x76;
    uint8_t _operating_mode = NORMAL_MODE;

public:
    BME280(void);
    ~BME280();
};



#endif /* BME280_H */