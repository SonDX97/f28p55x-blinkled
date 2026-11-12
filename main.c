//#############################################################################
//
// FILE:   main.c
//
// TITLE:  Blink LED on LAUNCHXL-F28P55X
//
// Board  : LAUNCHXL-F28P55X (TMS320F28P550SJ9)
// LEDs   : LED4 -> GPIO20 (DEVICE_GPIO_PIN_LED1)
//          LED5 -> GPIO21 (DEVICE_GPIO_PIN_LED2)
//          Both LEDs are active-low (write 0 = LED ON).
//
// Behaviour:
//   - LED4 and LED5 blink alternately every 500 ms.
//
// Build configurations (see CCS/blinkled.projectspec or Makefile):
//   - CPU1_LAUNCHXL_FLASH : program runs from Flash (survives power cycle)
//   - CPU1_LAUNCHXL_RAM   : program runs from RAM (debug only)
//
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"

//
// Defines
//
#define BLINK_DELAY_US      500000U     // 500 ms

#define LED_ON              0U          // LEDs on the LaunchPad are active-low
#define LED_OFF             1U

//
// Function Prototypes
//
static void initLED(uint32_t pin, uint32_t pinConfig);

//
// Main
//
void main(void)
{
    //
    // Initialize device clock (150 MHz SYSCLK), watchdog and peripherals.
    // In _FLASH builds this also copies time-critical code to RAM and
    // configures flash wait-states.
    //
    Device_init();

    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell ISRs.
    //
    Interrupt_initVectorTable();

    //
    // Configure GPIO20 (LED4) and GPIO21 (LED5) as push-pull outputs.
    //
    initLED(DEVICE_GPIO_PIN_LED1, DEVICE_GPIO_CFG_LED1);
    initLED(DEVICE_GPIO_PIN_LED2, DEVICE_GPIO_CFG_LED2);

    //
    // Enable Global Interrupt (INTM) and real-time interrupt (DBGM).
    //
    EINT;
    ERTM;

    //
    // Loop forever: blink LED4 and LED5 alternately.
    //
    for(;;)
    {
        GPIO_writePin(DEVICE_GPIO_PIN_LED1, LED_ON);
        GPIO_writePin(DEVICE_GPIO_PIN_LED2, LED_OFF);
        DEVICE_DELAY_US(BLINK_DELAY_US);

        GPIO_writePin(DEVICE_GPIO_PIN_LED1, LED_OFF);
        GPIO_writePin(DEVICE_GPIO_PIN_LED2, LED_ON);
        DEVICE_DELAY_US(BLINK_DELAY_US);
    }
}

//
// initLED - Configure one GPIO pin as a digital push-pull output,
//           controlled by CPU1, initial state = LED off.
//
static void initLED(uint32_t pin, uint32_t pinConfig)
{
    //
    // GPIO20/GPIO21 are shared with analog functions on F28P55x,
    // make sure the digital function is selected.
    //
    GPIO_setAnalogMode(pin, GPIO_ANALOG_DISABLED);
    GPIO_setPinConfig(pinConfig);
    GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(pin, GPIO_DIR_MODE_OUT);
    GPIO_setControllerCore(pin, GPIO_CORE_CPU1);
    GPIO_writePin(pin, LED_OFF);
}

//
// End of File
//
