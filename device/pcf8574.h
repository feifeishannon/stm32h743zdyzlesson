#include "main.h"

/************************** I2C Address ***************************************/
#define pcf8574_R_ADDRESS		0x41 		// I2C Address 0100 00 + ADDR + R
#define pcf8574_W_ADDRESS		0x40 		// I2C Address 0100 00 + ADDR + W
											// ADDR tied to Gnd (permanent)
/************************** I2C Registers *************************************/
#define pcf8574_INPUT_REG 		0x00		// Input status register
#define pcf8574_OUTPUT_REG		0x01		// Output register to change state of output BIT set to 1, output set HIGH
#define pcf8574_POLARITY_REG 	0x02		// Polarity inversion register. BIT '1' inverts input polarity of register 0x00
#define pcf8574_CONFIG_REG		0x03		// Configuration register. BIT = '1' sets port to input BIT = '0' sets port to output

//PCF8574各个IO的功能
// #define BEEP_IO         0		//蜂鸣器控制引脚  	P0
// #define AP_INT_IO       1   	//AP3216C中断引脚	P1
// #define DCMI_PWDN_IO    2    	//DCMI的电源控制引脚	P2
// #define USB_PWR_IO      3    	//USB电源控制引脚	P3
// #define EX_IO      		4    	//扩展IO,自定义使用 	P4
// #define MPU_INT_IO      5   	//MPU9250中断引脚	P5
// #define RS485_RE_IO     6    	//RS485_RE引脚		P6
// #define ETH_RESET_IO    7    	//以太网复位引脚		P7

/****************************************************************************************
 * 	The following structs and unions are used to configure a memory map of the pcf8574 *
 * 	on the MSP430																		*
 * 																						*
 * 	They allow the user to change the entire byte or alter the registers bit by bit		*
 ***************************************************************************************/

/************************** pcf8574 Bit struct *********************************/
// struct pcf8574_sbit{
// 	unsigned char P0:1;
// 	unsigned char P1:1;
// 	unsigned char P2:1;
// 	unsigned char P3:1;
// 	unsigned char P4:1;
// 	unsigned char P5:1;
// 	unsigned char P6:1;
// 	unsigned char P7:1;
// };

struct pcf8574_sbit{
	unsigned char BEEP_IO		:1;
	unsigned char AP_INT_IO		:1;
	unsigned char DCMI_PWDN_IO	:1;
	unsigned char USB_PWR_IO	:1;
	unsigned char EX_IO			:1;
	unsigned char MPU_INT_IO	:1;
	unsigned char RS485_RE_IO	:1;
	unsigned char ETH_RESET_IO	:1;
};

enum I2C_State{
	I2C_OPERATION_SUCCESSFUL =0,
	I2C_OPERATION_FAIL = 1

};

typedef enum
{
	COOLINGSTATEPORT 	= 3,
	LIQUIDLEVELPORT 	= 4,
	LIQUIDFLOWPORT 		= 5,
	THERMALENDPORT 		= 6,
	FANSTATEPORT 		= 7,
} PSWPortTpyeDef;

union pcf8574_uInput{
	unsigned char 				all;
	struct pcf8574_sbit	 	bit;
};

union pcf8574_uOutput{
	unsigned char 				all;
	struct pcf8574_sbit		bit;
};

union pcf8574_uPolarityInversion{
	unsigned char				all;
	struct pcf8574_sbit 		bit;
};

union pcf8574_uConfig{
	unsigned char						all;
	struct pcf8574_sbit				bit;
};


typedef struct {
	union pcf8574_uInput 				Input;
	union pcf8574_uOutput				Output;
} pcf8574Regs;

extern unsigned char NACK;

unsigned char pcf8574WriteOutput(pcf8574Regs * Regs);

void pcf8574InitDefault(pcf8574Regs* Regs);
unsigned char pcf8574InitI2CReg(pcf8574Regs* Regs);
unsigned char pcf8574ReadInputReg(pcf8574Regs* Regs);
