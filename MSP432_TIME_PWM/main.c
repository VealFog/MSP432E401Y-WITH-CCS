
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
uint32_t systemClock;
void PWM_init(){
    TIMER2->CTL &=~TIMER_CTL_TAEN;//关闭定时器TA，以便进行相关配置

    TIMER2->CFG |= TIMER_CFG_16_BIT ;//写入0x4，选择16位定时器
    TIMER2->TAMR &=~TIMER_TAMR_TACMR;//清除CMR
    //按顺序配置为启用PWM模式，周期模式
    TIMER2->TAMR |= TIMER_TAMR_TAAMS+TIMER_TAMR_TAMR_PERIOD;
    TIMER2->CTL &=~ TIMER_CTL_TAPWML;//默认输出状态，置1为反向输出
    //输出为2KHZ方波，占空比为66％，因为系统时钟为120兆HZ。具体频率可以参考下面的写法修改。
    TIMER2->TAILR =systemClock/2000;
    TIMER2->TAMATCHR=systemClock/6000;

    TIMER2->CTL |= TIMER_CTL_TAEN;//打开定时器TA
    //目前还不会下面三个定义，直接套用官方库函数，其次是底层定义里面没有PCTL中PCMn的相关定义
    MAP_GPIOPinConfigure(GPIO_PM0_T2CCP0);
    MAP_GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_0 );
    MAP_TimerEnable(TIMER2_BASE, TIMER_A);

}

void gpio_init(){

    //led_init
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}

int main(void)
{
    //修改系统时钟为120兆HZ
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);
    //和打开GPIO时钟一样，这里打开TIMER2时钟
    SYSCTL->RCGCTIMER|=SYSCTL_RCGCTIMER_R2;
    while((SYSCTL->RCGCTIMER & SYSCTL_RCGCTIMER_R2) == 0){};

    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R11;               // activate clock for Port M,N
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R11)) == 0){}; // wait for preparation of Port M,N

    PWM_init();
    gpio_init();


    while(1){
        GPION->DATA |=BIT0;//亮一个灯证明系统正常运行
    }
}
