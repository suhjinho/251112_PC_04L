# 1 "snr_tcn75a.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/suhji/.mchp_packs/Microchip/PIC18F-K_DFP/1.4.87/xc8\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "snr_tcn75a.c" 2
# 22 "snr_tcn75a.c"
# 1 "./snr_tcn75a.h" 1
# 26 "./snr_tcn75a.h"
# 1 "./i2c.h" 1
# 22 "./i2c.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned __int24 uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef __int24 intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 23 "./i2c.h" 2
# 60 "./i2c.h"
    void I2C_Initialize (void);


    void I2C_WriteR8D8 (uint8_t slaveAddr, uint8_t regAddr, uint8_t data);


    uint8_t I2C_ReadR8D8 (uint8_t slaveAddr, uint8_t regAddr);


    void I2C_WriteR8D16 (uint8_t slaveAddr, uint8_t regAddr, uint16_t value);


    uint16_t I2C_ReadR8D16 (uint8_t slaveAddr, uint8_t regAddr);
# 27 "./snr_tcn75a.h" 2
# 76 "./snr_tcn75a.h"
uint8_t TEMP_Initialize (void);
uint16_t TEMP_Read (void);
uint8_t TEMP_Calucation (int16_t rTemp);
# 23 "snr_tcn75a.c" 2
# 32 "snr_tcn75a.c"
uint8_t TEMP_Initialize (void)
{
    uint8_t configData = 0, returnData = 0;


    I2C_Initialize();



    configData = (0b01100000) | (0b00011000) ;


    I2C_WriteR8D8 ((0x48), (0x01), configData);
    returnData = I2C_ReadR8D8 ((0x48), (0x01));

    return returnData;
}




uint16_t TEMP_Read (void)
{
    uint16_t tmpData = 0;

    tmpData = I2C_ReadR8D16 ((0x48), (0x00)) >> 4;

    return tmpData;
}




uint8_t TEMP_Calucation (int16_t rTemp)
{
    const uint8_t realScale[16] = { 0,
                                    1,
                                    1,
                                    2,
                                    2,
                                    3,
                                    4,
                                    4,
                                    5,
                                    6,
                                    6,
                                    7,
                                    8,
                                    8,
                                    9,
                                    9
                                };

    int8_t tempInt = 0;
    uint8_t tempReal = 0, index = 0;
    int16_t tempData = 0;


    if(rTemp > 2047)
            tempData = (rTemp - 4096);
    else tempData = (int16_t)rTemp;


    tempInt = (int8_t)(tempData >> 4);
    tempReal = realScale[(uint8_t)(tempData & 0x000F)];
    return tempInt;
}
