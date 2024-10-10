// STM32L432KC_USART.c
// Source code for USART functions

#include "STM32L432KC.h"
#include "STM32L432KC_USART.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"

USART_TypeDef * id2Port(int USART_ID) {
    USART_TypeDef * USART;
    switch(USART_ID){
        case(USART1_ID) :
            USART = USART1;
            break;
        case(USART2_ID) :
            USART = USART2;
            break;
        default :
            USART = 0;
    }
    return USART;
}

USART_TypeDef * initUSART(int USART_ID, int baud_rate) {
    gpioEnable(GPIO_PORT_A);  // Enable clock for GPIOA
    RCC->CR |= RCC_CR_HSION;  // Turn on HSI 16 MHz clock

    USART_TypeDef * USART = id2Port(USART_ID); // Get pointer to USART

    switch(USART_ID){
        case USART1_ID :
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Set USART1EN
            RCC->CCIPR |= (0b10 << RCC_CCIPR_USART1SEL_Pos); // Set HSI16 (16 MHz) as USART clock source

            GPIOA->AFR[1] |= (0b111 << GPIO_AFRH_AFSEL9_Pos) | (0b111 << GPIO_AFRH_AFSEL10_Pos);

            // Configure pin modes as ALT function
            pinMode(PA9, GPIO_ALT); // TX
            pinMode(PA10, GPIO_ALT); // RX

            break;
        case USART2_ID :
            RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; // Set USART2EN
            RCC->CCIPR |= (0b10 << RCC_CCIPR_USART2SEL_Pos); // Set HSI16 (16 MHz) as USART clock source

            // Configure pin modes as ALT function
            pinMode(PA2, GPIO_ALT); // TX
            pinMode(PA15, GPIO_ALT); // RX

            // Configure correct alternate functions
            GPIOA->AFR[0] |= (0b111 << GPIO_AFRL_AFSEL2_Pos);   //AF7
            GPIOA->AFR[1] |= (0b011 << GPIO_AFRH_AFSEL15_Pos);  //AF3
            break;
    }

    // Set M = 00
    // TODO: Configure options
    USART->CR1 &=     // M=00 corresponds to 1 start bit, 8 data bits, n stop bits
    USART->CR1 &=  // Set to 16 times sampling freq
    USART->CR2 &=   // 0b00 corresponds to 1 stop bit

    // Tx/Rx baud = f_CK/USARTDIV (since oversampling by 16)
    // f_CK = 16 MHz (HSI)

    USART->BRR = 

    USART->CR1 |= ;     // Enable USART
    USART->CR1 |= ; // Enable transmission and reception

    return USART;
}

void sendChar(USART_TypeDef * USART, char data){
  // TODO: Implement code to send character

}

void sendString(USART_TypeDef * USART, char * charArray){

    uint32_t i = 0;
    do{
        sendChar(USART, charArray[i]);
        i++;
    }
    while(charArray[i] != 0);
}

char readChar(USART_TypeDef * USART) {
        char data = USART->RDR;
        return data;
}

void readString(USART_TypeDef * USART, char* charArray){
    int i = 0;
    do{
        charArray[i] = readChar(USART);
        i++;
    }
    while(USART->ISR & USART_ISR_RXNE);
}