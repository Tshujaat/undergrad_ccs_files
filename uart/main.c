/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 Develop an MSP432 application with a serial port (UART) user interface (UI). The UI should provide a
menu that provides the following choices for the user.
• Toggle RGB Red LED2 (P2.0)
• Toggle RGB Green LED2 (P2.1)
• Toggle RGB Blue LED2 (P2.2)
In addition to control from the UART, the program should also allow the user to toggle LED1 by pressing
the S1 switch.
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"

/*
 *  ======== mainThread ========
 */
void *uart(void *arg0)
{
    char        input;
    const char  startPrompt[] = "- Type 'b' or 'B' to toggle blue LED \n- Type 'g' or 'G' to toggle green LED\n- Type 'r' or 'R' to toggle red LED\n";

    UART_Handle uart;
    UART_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    /* Configures the RGB LED pins, not P1.0 */
    // GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_LED3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);



    /* Turn on user LED */
   // GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
    while (1);

    }

    //UART_write(uart, startPrompt, sizeof(startPrompt));

    /*  */
    while (1) {
        UART_write(uart, startPrompt, sizeof(startPrompt));
        UART_read(uart, &input, 1);
        UART_write(uart, &input, 1);

        if (input == 'B')
        {
        UART_write(uart, "Blue\n", 21);
        GPIO_toggle(Board_GPIO_LED3);
        }

        if (input == 'b')
        {
        UART_write(uart, "Blue\n", 21);
        GPIO_toggle(Board_GPIO_LED3);
        }

        if (input == 'G')
        {
        UART_write(uart, "Green\n", 22);
        GPIO_toggle(Board_GPIO_LED2);
        }

        if (input == 'g')
        {
        UART_write(uart, "Green\n", 21);
        GPIO_toggle(Board_GPIO_LED3);
        }


        if (input == 'R')
        {
        UART_write(uart, "Red\n", 20);
        GPIO_toggle(Board_GPIO_LED1);
        }

        if (input == 'r')
        {
        UART_write(uart, "Red\n", 21);
        GPIO_toggle(Board_GPIO_LED3);
        }

        else
        {
        UART_write(uart, "No command given/readable", 26);
        }


    }
}
