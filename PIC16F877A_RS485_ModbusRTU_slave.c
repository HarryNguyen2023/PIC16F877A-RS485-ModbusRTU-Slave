#include "config.h"
#include "RS485_ModbusRTU_slave.h"
#include "LCD16x2.h"

// Define the external crystal frequency
#define _XTAL_FREQ 16000000
#define BUFFER_LEN 25

// Function for ModbusRTU message handling
void modbusMesHandling(void);

// Global variables
uint8_t rcv_buffer[BUFFER_LEN];
uint8_t tx_buffer[BUFFER_LEN];
uint8_t coil_val[] = {0xCD, 0x21};
uint16_t register_val[] = {115, 104};

// Error message
char error_msg[] = "Transmission error";

// Function to handle interrupt service
void __interrupt() ISR(void)
{
    if(RCIF)
    {
        if(modbusRcvRequest(rcv_buffer))
        {
            LCD_Clear();
            LCD_Set_Cursor(1, 1);
            modbusMesHandling();
        }
    }
}

void main(void) {
    // Initiate UART module
    modbusRTUInit();
    
    // Initiate the LCD 
    LCD_Init();
    LCD_Clear();
    
    // Main duty
    while(1)
    {
        
    }
    return;
}

// Function for ModbusRTU message handling
void modbusMesHandling()
{
    uint8_t function = rcv_buffer[1];
    // Function handling
    if(function == 1 || function == 2)
    {
        if(! modbusResponseCoilsRead(tx_buffer, BUFFER_LEN, rcv_buffer, coil_val))
        {
            LCD_Write_String(error_msg);
        }
    }   
    else if(function == 3 || function == 4)
    {
        if(! modbusResponseRegistersRead(tx_buffer, BUFFER_LEN, rcv_buffer, register_val))
        {
            LCD_Write_String(error_msg);
        }
    }
    else if(function == 5 || function == 6)
    {
        if(! modbusResponseSingleWrite(tx_buffer, BUFFER_LEN, rcv_buffer))
        {
            LCD_Write_String(error_msg);
        }
    }
    else if(function == 15 || function == 16)
    {
        if(! modbusResponseMultipleWrite(tx_buffer, BUFFER_LEN, rcv_buffer))
        {
            LCD_Write_String(error_msg);
        }
    }
    return;
}