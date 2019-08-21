/**
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
//#include "inc/tm4c123gh6pm.h"
#include "Tiva_spi.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"

/***for UART***/
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"


void
InitConsole(void)
{
    //
    // Enable GPIO port E which is used for UART7 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ///
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART7 functions on port E0 and E1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART7 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
    //UARTStdioConfig(0, 115200, 16000000);
}


///over

int main(void)
{
    int32_t temp;
    uint32_t buffer;
    buffer = 0;
    ///UART init_
    InitConsole();

        UARTprintf("ADXL Getting to work...");

    // Board Support Package


    /********************************************************************************************/

    // Set the clocking to run directly from the external crystal/oscillator.
    // Use the PLL (400 MHz to generate 400 MHz clock
    // Divide 400 MHz / 2 = 200 MHz
    // Divide 200 MHz / SYSCTL_SYSDIV_4 to generate a 20MHz Clock
    SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_10);

     temp = SysCtlClockGet() / 1000;

    initSPI3();


    while (1)
    {
        while (SSIBusy(SSI3_BASE)) ;
        SSIDataPut(SSI3_BASE, 0x80); //Puts a data element into the SSI transmit FIFO
        while (SSIBusy(SSI3_BASE));
        SSIDataGet(SSI3_BASE, &buffer);
        UARTprintf("\n\t%d", buffer);
     }

    //return 0;
}
