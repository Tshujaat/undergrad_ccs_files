/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

// Global variable used to hold the current state
char currentState;

// Global variable used to hold the number of pushes
long int pushes;


/*
 * Initialization function runs once before the state machine while(1) loop.
 */
void init(void)
{
    MAP_WDT_A_holdTimer();                // Stop watchdog timer
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);    // Set P1.0 to output direction
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2)); // Set P2, bits 2, 1, 0 to output
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, (GPIO_PIN1 + GPIO_PIN4));

    currentState = 0;                       // Global variable init
    pushes = 0;                             // Global variable init

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2));

}

// Check S1 Button (counter reverse order)
char readS1Input(void)
{
    char local = 0;

    // The P1IN register holds the logical values of the pins that are
    // configured as inputs

    if (MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0) {           // Check for S1 input
        pushes = pushes + 1;       // Code that executes when S1 button is pushed
        local = 1;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0)   {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
        }
        __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
    }
    return local;
}


// Check S2 Button (counter forward order)
char readS2Input(void)
{
    char local = 0;

    // The P1IN registers hold the logical values of the pins that are
    // configured as inputs

    if (MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0) {           // Check for S2 input
        pushes = pushes + 1;       // Code that executes when S2 button is pushed
        local = 2;
        __delay_cycles(10000);          // Wait for debounce to end (bad code)
        while(MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0)   {
            // Wait for Up button to return to "normal"
            // Must re-read the input port
            // Not good code because loop could run forever
        }
        __delay_cycles(10000);      // Again wait for debounce to end (again bad code)
    }
    return local;
}

//keep in mind that P2.0 pertains to the red LED, P2.1 to green, and P2.2 to blue
void countUp(void)
{
    switch (currentState){
    case 0 :
        currentState = 1;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); //red

        break;
    case 1 :
        currentState = 2;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1); //green

        break;
    case 2 :
        currentState = 3;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1)); //yellow

        break;
    case 3 :
        currentState = 4;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2); //blue

        break;
    case 4 :
        currentState = 5;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2)); //pink/violet
        break;
    case 5:
        currentState = 6;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2)); //Skyblue
        break;
    case 6:
        currentState = 7;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2)); //White
        break;
    case 7 :
        currentState = 0;
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2)); //nothing
        break;
    }
}

//basically the opposite of count up
void countDown(void)
{
    switch (currentState){

    case 0 :
        currentState = 7;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2)); //White

        break;
    case 1 :
        currentState = 0;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2)); //Nothing
        break;
    case 2 :
        currentState = 1;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        break;
    case 3 :
        currentState = 2;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1); //green
        break;
    case 4 :
        currentState = 3;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1)); //yellow
        break;
    case 5 :
        currentState = 4;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN1));
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2); //blue
        break;
    case 6:
        currentState = 5;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN0 + GPIO_PIN2)); //pink/violet
        break;
    case 7:
        currentState = 6;
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, (GPIO_PIN1 + GPIO_PIN2)); //Skyblue
        break;

    }
}



/**
 * main.c
 */
void main(void)
{
    //WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;       // stop watchdog timer
    char enable = 0;            // Holds status of the button
    init();                     // One time initialization function

    /*
     * The main while loop of the state machine will constantly scan the inputs
     * and update the system based on those inputs
     */
    while(1) {
        enable = readS1Input();   // Check the S1 button
        if(!enable)                   // If S1 button is not pressed then check S2
            enable = readS2Input();   // Check the S2 button

        switch(enable) {
        case 0 :
            // Do nothing if neither button is pressed
            break;
        case 1 :
            // If S1 button is pressed then allow the counter to increment once
            countDown();
            break;
        case 2 :
            // If S2 button is pressed then allow the counter to increment once
            countUp();
            break;
        }

    }
}
