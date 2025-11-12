/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18F26K40
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set channel_ANA0 aliases
#define channel_ANA0_TRIS                 TRISAbits.TRISA0
#define channel_ANA0_LAT                  LATAbits.LATA0
#define channel_ANA0_PORT                 PORTAbits.RA0
#define channel_ANA0_WPU                  WPUAbits.WPUA0
#define channel_ANA0_OD                   ODCONAbits.ODCA0
#define channel_ANA0_ANS                  ANSELAbits.ANSELA0
#define channel_ANA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define channel_ANA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define channel_ANA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define channel_ANA0_GetValue()           PORTAbits.RA0
#define channel_ANA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define channel_ANA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define channel_ANA0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define channel_ANA0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define channel_ANA0_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define channel_ANA0_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define channel_ANA0_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define channel_ANA0_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set RA5 procedures
#define RA5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define RA5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define RA5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define RA5_GetValue()              PORTAbits.RA5
#define RA5_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define RA5_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define RA5_SetPullup()             do { WPUAbits.WPUA5 = 1; } while(0)
#define RA5_ResetPullup()           do { WPUAbits.WPUA5 = 0; } while(0)
#define RA5_SetAnalogMode()         do { ANSELAbits.ANSELA5 = 1; } while(0)
#define RA5_SetDigitalMode()        do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set STATE_LED aliases
#define STATE_LED_TRIS                 TRISBbits.TRISB0
#define STATE_LED_LAT                  LATBbits.LATB0
#define STATE_LED_PORT                 PORTBbits.RB0
#define STATE_LED_WPU                  WPUBbits.WPUB0
#define STATE_LED_OD                   ODCONBbits.ODCB0
#define STATE_LED_ANS                  ANSELBbits.ANSELB0
#define STATE_LED_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define STATE_LED_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define STATE_LED_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define STATE_LED_GetValue()           PORTBbits.RB0
#define STATE_LED_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define STATE_LED_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define STATE_LED_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define STATE_LED_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define STATE_LED_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define STATE_LED_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define STATE_LED_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define STATE_LED_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set LEAK_OUT aliases
#define LEAK_OUT_TRIS                 TRISCbits.TRISC0
#define LEAK_OUT_LAT                  LATCbits.LATC0
#define LEAK_OUT_PORT                 PORTCbits.RC0
#define LEAK_OUT_WPU                  WPUCbits.WPUC0
#define LEAK_OUT_OD                   ODCONCbits.ODCC0
#define LEAK_OUT_ANS                  ANSELCbits.ANSELC0
#define LEAK_OUT_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define LEAK_OUT_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define LEAK_OUT_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define LEAK_OUT_GetValue()           PORTCbits.RC0
#define LEAK_OUT_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define LEAK_OUT_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define LEAK_OUT_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define LEAK_OUT_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define LEAK_OUT_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define LEAK_OUT_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define LEAK_OUT_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define LEAK_OUT_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set NOR_LED aliases
#define NOR_LED_TRIS                 TRISCbits.TRISC1
#define NOR_LED_LAT                  LATCbits.LATC1
#define NOR_LED_PORT                 PORTCbits.RC1
#define NOR_LED_WPU                  WPUCbits.WPUC1
#define NOR_LED_OD                   ODCONCbits.ODCC1
#define NOR_LED_ANS                  ANSELCbits.ANSELC1
#define NOR_LED_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define NOR_LED_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define NOR_LED_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define NOR_LED_GetValue()           PORTCbits.RC1
#define NOR_LED_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define NOR_LED_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define NOR_LED_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define NOR_LED_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define NOR_LED_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define NOR_LED_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define NOR_LED_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define NOR_LED_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set LEAK_LED aliases
#define LEAK_LED_TRIS                 TRISCbits.TRISC2
#define LEAK_LED_LAT                  LATCbits.LATC2
#define LEAK_LED_PORT                 PORTCbits.RC2
#define LEAK_LED_WPU                  WPUCbits.WPUC2
#define LEAK_LED_OD                   ODCONCbits.ODCC2
#define LEAK_LED_ANS                  ANSELCbits.ANSELC2
#define LEAK_LED_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define LEAK_LED_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define LEAK_LED_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define LEAK_LED_GetValue()           PORTCbits.RC2
#define LEAK_LED_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define LEAK_LED_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define LEAK_LED_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define LEAK_LED_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define LEAK_LED_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define LEAK_LED_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define LEAK_LED_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define LEAK_LED_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSELC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set TX1 aliases
#define TX1_TRIS                 TRISCbits.TRISC6
#define TX1_LAT                  LATCbits.LATC6
#define TX1_PORT                 PORTCbits.RC6
#define TX1_WPU                  WPUCbits.WPUC6
#define TX1_OD                   ODCONCbits.ODCC6
#define TX1_ANS                  ANSELCbits.ANSELC6
#define TX1_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define TX1_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define TX1_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define TX1_GetValue()           PORTCbits.RC6
#define TX1_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define TX1_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define TX1_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define TX1_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define TX1_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define TX1_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define TX1_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define TX1_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/