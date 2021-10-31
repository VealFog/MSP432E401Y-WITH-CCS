/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Include for serial console */
#include "uartstdio.h"
uint32_t systemClock;

//void UART0_IRQHandler(void){
//    if ((UART0->RIS & UART_RIS_RXRIS) == UART_RIS_RXRIS){
//        if (GPION->DATA & BIT1)
//            GPION->DATA &=~BIT1;
//        else
//            GPION->DATA |=BIT1;
//        UART0->DR |= UART_DR_DATA_M;
//        UART0->ICR |= UART_ICR_RXIC;
//    }
//}

//void UART0_init(){
//    GPIOA->AFSEL |=BIT1+BIT0;
//    GPIOA->DEN |=BIT1+BIT0;
//    GPIOA->PUR |=BIT1+BIT0;
//    GPIOA->PCTL |=0x000000FF;

//    UART0->IBRD = 65;
//    UART0->FBRD = 7;
//    UART0->LCRH |= UART_LCRH_WLEN_8;
//    MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_ALTCLK);
//    UART0->CC |= UART_CC_CS_M;
//    UART0->IM |=UART_IM_RXIM;
//    UART0->CTL |= UART_CTL_UARTEN;
//    MAP_UARTEnable(UART0_BASE);
//    MAP_IntEnable(INT_UART0);
//}

void UART0_init(){
    /* Configure the GPIO Port A for UART 0 */
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configure the UART for 115200 bps 8-N-1 format with internal 120 MHz
     * oscillator as the UART clock source */
    MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_ALTCLK);
    UARTStdioConfig(0, 115200, 16000000);
}

void gpio_init(){
    //led_init
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}

int main(void)
{
    //120Mhz���ڽ��ջ�������룬����16Mhz���ᡣ����ȡ16Mhz
    //ϵͳʱ���趨Ϊ16Mhz
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          16000000);
    //�ʹ�GPIOʱ��һ���������UARTʱ��
    SYSCTL->RCGCUART|=SYSCTL_RCGCUART_R0;
    while((SYSCTL->RCGCUART & SYSCTL_RCGCUART_R0) == 0){};
    //��GPIOʱ��,UART0��Ӧ��������PA0��PA1
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R0;               // activate clock for Port M,N
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R0)) == 0){}; // wait for preparation of Port M,N

    UART0_init();
    gpio_init();


    while(1){
        UARTprintf("UART-123456789123456789 ");
        MAP_SysCtlDelay(systemClock / 3);
        GPION->DATA |=BIT0;//��һ����֤��ϵͳ��������
    }
}