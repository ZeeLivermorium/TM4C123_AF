/*
 * M0PWM1.c
 * Use M0PWM1/PB7 to generate pulse-width modulated outputs.
 * ----------
 * Inspired by the PWM example in ValvanoWare by Dr. Jonathan Valvano
 * as well as his book Embedded Systems: Real-Time Interfacing to Arm Cortex-M Microcontrollers
 * You can find VavanoWare at http://users.ece.utexas.edu/~valvano/arm/downloadmsp432.html
 * You can find his book at https://www.amazon.com/gp/product/1463590156/ref=oh_aui_detailpage_o05_s00?ie=UTF8&psc=1
 * You can find more of his work at http://users.ece.utexas.edu/~valvano/
 * ----------
 * Zee Livermorium
 * Dec 20, 2017
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"   // put tm4c123gh6pm.h in your project folder or change this line

/*
 * Function: void M0PWM1_Init(uint16_t period, uint16_t duty)
 * Parameters:
 *   - period: period of the PWM signal
 *   - duty: duty cycle of the PWM signal
 * Discription: Initialize M0PWM1/PB7.
 */
void M0PWM1_Init(uint16_t period, uint16_t duty) {
    /* PWM Module and Port Set Up */
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;                 // activate PWM Module 0
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;               // activate Port B
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) == 0){};    // allow time to finish activating
    
    /* Port B Set Up */
    GPIO_PORTB_AFSEL_R |= 0x80;                            // enable alt funct on PB7
    GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB7_M;                 // clear bit fields for PB7
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB7_M0PWM1;             // configure PB7 as M0PWM1
    GPIO_PORTB_AMSEL_R &= ~0x80;                           // disable analog functionality on PB7
    GPIO_PORTB_DEN_R |= 0x80;                              // enable digital I/O on PB7
    
    /* System Control Run-Mode Clock Configuration (RCC) Set Up */
    SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;                  // use PWM divider
    SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;                  // clear PWM divider field
    SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;                   // configure for /2 divider
    
    /* M0PWM1 Set Up */
    PWM0_0_CTL_R &= ~PWM_0_CTL_MODE;                       // re-loading down-counting mode
    PWM0_0_GENB_R |= PWM_0_GENB_ACTLOAD_ZERO;              // PB7 goes low on LOAD
    PWM0_0_GENB_R |= PWM_0_GENB_ACTCMPBD_ONE;              // PB7 goes high on CMPB down
    PWM0_0_LOAD_R = period - 1;                            // cycles needed to count down to 0
    PWM0_0_CMPB_R = duty - 1;                              // count value when output rises
    PWM0_0_CTL_R |= PWM_0_CTL_ENABLE;                      // enable Generator 0 for PWM Module 0
    PWM0_ENABLE_R |= PWM_ENABLE_PWM1EN;                    // enable M0PWM1 output
}

/*
 * Function: void M0PWM1_Set_Duty(uint16_t duty)
 * Parameters:
 *   - duty: new duty cycle to be set.
 * Discription: Set duty cycle of the PWM signal to a new value.
 */
void M0PWM1_Set_Duty(uint16_t duty){
    PWM0_0_CMPB_R = duty - 1;                              // count value when output rises
}
