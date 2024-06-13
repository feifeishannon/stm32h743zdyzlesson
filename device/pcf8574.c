#include "pcf8574.h"

#define CLR_NACK NACK = 0

extern I2C_HandleTypeDef hi2c2;
#define pcf8574_I2C &hi2c2
unsigned char NACK;

// ****************************************************************************
//! @fn          void pcf8574InitDefault(pcf8574Regs* Regs)
//! @brief
//!				 
//!
//! ****************************************************************************
void pcf8574InitDefault(pcf8574Regs* Regs)
{
	Regs->Output.all = 0xFF;
}


// ****************************************************************************
//! @fn          void pcf8574InitI2CReg(pcf8574Regs* Regs)
//! @brief
//! @return		 Returns I2C success or failure
//!
// ****************************************************************************
unsigned char pcf8574InitI2CReg(pcf8574Regs* Regs)
{
	unsigned char return_Value = I2C_OPERATION_SUCCESSFUL;
	CLR_NACK;
	
	HAL_I2C_Mem_Write(  pcf8574_I2C, 
                        pcf8574_W_ADDRESS, 
                        pcf8574_OUTPUT_REG, 
                        I2C_MEMADD_SIZE_8BIT, 
                        (unsigned char*)&Regs->Output.all, 
                        1, 
                        1000);
						
	
	if(NACK)
	{
		NACK = 0;

		return_Value = I2C_OPERATION_FAIL;
	}
	return return_Value;
}

// ****************************************************************************
//! @fn          void pcf8574ReadInputReg(pcf8574Regs* Regs)
//! @brief		 Stores input register values to MSP430 Memory map
//! @return		 Returns I2C success or failure
//!
// ****************************************************************************
unsigned char pcf8574ReadInputReg(pcf8574Regs* Regs)
{
	unsigned char return_Value = I2C_OPERATION_SUCCESSFUL;
	CLR_NACK;
	HAL_I2C_Mem_Read(  	pcf8574_I2C, 
                        pcf8574_R_ADDRESS, 
                        pcf8574_INPUT_REG, 
                        I2C_MEMADD_SIZE_8BIT, 
                        (unsigned char*)&Regs->Input, 
                        1, 
                        1000);
	if(NACK)
	{
		NACK = 0;
		return_Value = I2C_OPERATION_FAIL;
	}
	return return_Value;
}


// ****************************************************************************
//! @fn          void pcf8574WriteOutput(pcf8574Regs* Regs)
//! @brief		 Writes output value from MCU memory map to pcf8574
//! @return		 Returns I2C success or failure
//!
// ****************************************************************************
unsigned char pcf8574WriteOutput(pcf8574Regs * Regs)
{
	unsigned char return_Value = I2C_OPERATION_SUCCESSFUL;
	CLR_NACK;
	HAL_I2C_Mem_Write(  pcf8574_I2C, 
                        pcf8574_W_ADDRESS, 
                        pcf8574_OUTPUT_REG, 
                        I2C_MEMADD_SIZE_8BIT, 
                        (unsigned char*)&Regs->Output, 
                        1, 
                        1000);
	if(NACK)
	{
		NACK = 0;
		return_Value = I2C_OPERATION_FAIL;
	}
	return return_Value;
}



