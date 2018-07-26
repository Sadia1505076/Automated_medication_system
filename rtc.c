// ---------------------------------------------------------------------------
// DS1307 RTC ROUTINES
#define DS1307 0xD0 // I2C bus address of DS1307 RTC
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define CONTROL_REGISTER 0x07
#define RAM_BEGIN 0x08
#define RAM_END 0x3F

#include "i2c.c"
typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;

void DS1307_GetTime(byte *hours, byte *minutes, byte *seconds)
// returns hours, minutes, and seconds in BCD format
{
*hours = I2C_ReadRegister(DS1307,HOURS_REGISTER);
*minutes = I2C_ReadRegister(DS1307,MINUTES_REGISTER);
*seconds = I2C_ReadRegister(DS1307,SECONDS_REGISTER);
if (*hours & 0x40) // 12hr mode:
*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
else *hours &= 0x3F; // 24hr mode: use bottom 6 bits
}



void DS1307_GetDate(byte *months, byte *days, byte *years)
// returns months, days, and years in BCD format
{
*months = I2C_ReadRegister(DS1307,MONTHS_REGISTER);
*days = I2C_ReadRegister(DS1307,DAYS_REGISTER);
*years = I2C_ReadRegister(DS1307,YEARS_REGISTER);
}



void SetTimeDate()
// simple, hard-coded way to set the date.
{
I2C_WriteRegister(DS1307,MONTHS_REGISTER, 0x06);
I2C_WriteRegister(DS1307,DAYS_REGISTER, 0x03);
I2C_WriteRegister(DS1307,YEARS_REGISTER, 0x18);
I2C_WriteRegister(DS1307,HOURS_REGISTER, 0x15); // add 0x40 for PM
I2C_WriteRegister(DS1307,MINUTES_REGISTER, 0x48);
I2C_WriteRegister(DS1307,SECONDS_REGISTER, 0x00);
}
