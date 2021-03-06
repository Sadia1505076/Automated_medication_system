// ---------------------------------------------------------------------------
// I2C (TWI) ROUTINES
//
// On the AVRmega series, PA4 is the data line (SDA) and PA5 is the clock (SCL
// The standard clock rate is 100 KHz, and set by I2C_Init. It depends on the AVR osc. freq.
#define F_SCL 100000L // I2C clock speed 100 KHz
#define READ 1
#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK 0xC4 // return ACK to slave
#define TW_NACK 0x84 // don't return ACK to slave
#define TW_SEND 0x84 // send data (TWINT,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register
#define I2C_Stop() TWCR = TW_STOP // inline macro for stop condition

typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;

void I2C_Init()
// at 16 MHz, the SCL frequency will be 16/(16+2(TWBR)), assuming prescalar of 0.
// so for 100KHz SCL, TWBR = ((F_CPU/F_SCL)-16)/2 = ((16/0.1)-16)/2 = 144/2 = 72.
{
TWSR = 0; // set prescalar to zero
TWBR = ((F_CPU/F_SCL)-16)/2; // set SCL frequency in TWI bit register
}



byte I2C_Detect(byte addr)
// look for device at specified address; return 1=found, 0=not found
{
TWCR = TW_START; // send start condition
while (!TW_READY); // wait
TWDR = addr; // load device's bus address
TWCR = TW_SEND; // and send it
while (!TW_READY); // wait
return (TW_STATUS==0x18); // return 1 if found; 0 otherwise
}



byte I2C_FindDevice(byte start)
// returns with address of first device found; 0=not found
{byte addr;
for ( addr=start;addr<0xFF;addr++) // search all 256 addresses
{
if (I2C_Detect(addr)) // I2C detected?
return addr; // leave as soon as one is found
}
return 0; // none detected, so return 0.
}



void I2C_Start (byte slaveAddr)
{
I2C_Detect(slaveAddr);
}



byte I2C_Write (byte data) // sends a data byte to slave
{
TWDR = data; // load data to be sent
TWCR = TW_SEND; // and send it
while (!TW_READY); // wait
return (TW_STATUS!=0x28);
}



byte I2C_ReadACK () // reads a data byte from slave
{
TWCR = TW_ACK; // ack = will read more data
while (!TW_READY); // wait
return TWDR;
//return (TW_STATUS!=0x28);
}



byte I2C_ReadNACK () // reads a data byte from slave
{
TWCR = TW_NACK; // nack = not reading more data
while (!TW_READY); // wait
return TWDR;
//return (TW_STATUS!=0x28);
}



void I2C_WriteByte(byte busAddr, byte data)
{
I2C_Start(busAddr); // send bus address
I2C_Write(data); // then send the data byte
I2C_Stop();
}




void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data)
{
I2C_Start(busAddr); // send bus address
I2C_Write(deviceRegister); // first byte = device register address
I2C_Write(data); // second byte = data for device register
I2C_Stop();
}



byte I2C_ReadRegister(byte busAddr, byte deviceRegister)
{
byte data = 0;
I2C_Start(busAddr); // send device address
I2C_Write(deviceRegister); // set register pointer
I2C_Start(busAddr+READ); // restart as a read operation
data = I2C_ReadNACK(); // read the register data
I2C_Stop(); // stop
return data;
}
