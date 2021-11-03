/*------------------------------------------------------------------------------
Archivo: Main_Lab9
Microcontrolador: TM4C123
Autor: Andy Bonilla
Compilador: Code Composer Studio V10
Programa: laboratorio 9
Hardware: Launchpad Tiva C

Creado: 2 de noviembre de 2021
Descripcion:
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 ----------------------------L I B R E R I A S---------------------------------
 -----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
/*-----------------------------------------------------------------------------
 -----------------V A R I A B L E S   A   I M P L E M T E N T A R--------------
 -----------------------------------------------------------------------------*/
uint32_t ui32Period;

/*-----------------------------------------------------------------------------
 ------------ P R O T O T I P O S   D E   F U N C I O N E S -------------------
 -----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 --------------------- I N T E R R U P C I O N E S ----------------------------
 -----------------------------------------------------------------------------*/
//**************************************************************************************************************
// Handler de la interrupci�n del TIMER 0 - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************************************************************************************
void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
}

/*-----------------------------------------------------------------------------
 ------------------------------ S E T   U P -----------------------------------
 -----------------------------------------------------------------------------*/
int main(void)
{
    //-------CONFIGURACION DEL RELOJ
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Configuracion del oscilador externo, usando PLL y teniendo una frecuencia de 40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                            // Se habilita el reloj para el puerto F
    //-------CONFIGURACION DE ENTRADAS Y SALIDAS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);           // Se establecen como salidas el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);                                           // Se habilita el reloj para el temporizador
    //-------CONFIGURACION DE TIMER
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);                                        // Configuracion del Timer 0 como temporizador per�odico
    ui32Period = (SysCtlClockGet()) / 2;                                                    // Se calcula el per�odo para el temporizador (1 seg)
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);                                     // Establecer el periodo del temporizador
    //-------CONFIGURACION DE INTERRUPCIONES
    IntEnable(INT_TIMER0A);                                                                 // Se habilita la interrupciOn por el TIMER0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);                                        // Se establece que exista la interrupcion por Timeout
    IntMasterEnable();                                                                      // Se habilitan las interrupciones Globales
    TimerEnable(TIMER0_BASE, TIMER_A);                                                      // Se habilita el Timer
    /*-----------------------------------------------------------------------------
    -------------------------- M A I N   L O O P ---------------------------------
    -----------------------------------------------------------------------------*/
    while (1)
    {
    }
}


/*-----------------------------------------------------------------------------
 ------------------------- F U N C I O N E S ----------------------------------
 -----------------------------------------------------------------------------*/
