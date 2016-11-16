/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    reflectance.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "reflectance.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

REFLECTANCE_DATA reflectanceData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void REFLECTANCE_Initialize ( void )

  Remarks:
    See prototype in reflectance.h.
 */

void REFLECTANCE_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    reflectanceData.state = REFLECTANCE_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    data_ready = false;
    
    timer_LED_ON = xTimerCreate("timer_LED_ON", pdMS_TO_TICKS( 100 ), pdTRUE, ( void * ) 0, callback_LED_ON);
    xTimerStart(timer_LED_ON, 0);

    timer_LED_OFF = xTimerCreate("timer_LED_OFF", pdMS_TO_TICKS( 2 ), pdTRUE, ( void * ) 0, callback_LED_OFF);

    timer_LED_INPUT = xTimerCreate("timer_LED_INPUT", pdMS_TO_TICKS( 1 ), pdTRUE, ( void * ) 0, callback_LED_INPUT);
    
    /* init stuff, configure devices*/
}


/******************************************************************************
  Function:
    void REFLECTANCE_Tasks ( void )

  Remarks:
    See prototype in reflectance.h.
 */

void REFLECTANCE_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( reflectanceData.state )
    {
        /* Application's initial state. */
        case REFLECTANCE_STATE_INIT:
        {
            bool appInitialized = true;
            
            start_LED_ON = 0;
            start_LED_OFF = 0;
            start_LED_INPUT = 0;
        
            if (appInitialized)
            {
                reflectanceData.state = REFLECTANCE_STATE_OFF;
            }
            break;
        }

        case REFLECTANCE_STATE_OFF:
        {   
            if (start) {
                reflectanceData.state = REFLECTANCE_STATE_ON;
            }
            break;
        }
        
        case REFLECTANCE_STATE_ON:
        {
            if (data_ready) {
                //dbgOutputVal(reflectance_output);
                
                reflectanceData.tx_data[1] = reflectance_output;
                reflectanceData.tx_data[0] = (INT_MSG | 0x01 << 4 | REFLECTANCE_THREAD_ID << 2 | CONTROL_THREAD_ID );
                dbgOutputVal(reflectanceData.tx_data[1]);
                xQueueSend( MessageQueueControl, reflectanceData.tx_data, pdFAIL );
                data_ready = false;
                
            }
            

            //reflectanceData.state = REFLECTANCE_STATE_OFF;
            
            break;
        }
        

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void startReflectance() {
    start = true;
}

void stopReflectance() {
    start = false;
}


/*******************************************************************************
 End of File
 */
