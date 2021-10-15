/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void key_init(){
    GPIOJ->DIR&=~(BIT0+BIT1); //set as input
    GPIOJ->PUR|=BIT0+BIT1;  //Input set to pull-up resistor
    GPIOJ->DEN|=BIT0+BIT1;  //Digital enable
    GPIOJ->DATA|=BIT0+BIT1;//Output set as high
}
void key_interrupt_init(){

    GPIOJ->IS &=~BIT1;
    GPIOJ->IBE &=~BIT1;
    GPIOJ->IEV &=~BIT1;//==  MAP_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_1, GPIO_FALLING_EDGE);
    GPIOJ->IM |=BIT1;//  ==    MAP_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_1);
    MAP_IntEnable(INT_GPIOJ);//打开GPIOJ中断，关于总中断的寄存器写法我没有找到，直接用driverlib库里的即可，也挺便捷的。
}
void led_init(){
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}

void GPIOJ_IRQHandler(void)
{
    if (GPIOJ->RIS & BIT1){
        GPION->DATA ^=BIT1;
    }
    GPION->DATA ^=BIT0;
    GPIOJ->ICR |=BIT1;
}

int main(void)
{

    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R8;               // activate clock for Port N,J
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R8)) == 0){}; // wait for preparation of Port N,J

    key_init();
    key_interrupt_init();
    led_init();


    while(1){// if switch is pressed, led light on
        //switch1 -> PJ0 -> control D2->PN0
        if ((GPIOJ->DATA) & BIT0)
            GPION->DATA&=~BIT0;
        else
            GPION->DATA|=BIT0;
    }
}
