/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
uint32_t systemClock;
void PWM_init(){
    MAP_GPIOPinConfigure(GPIO_PF1_M0PWM1);
    MAP_GPIOPinTypePWM(GPIO_PORTF_BASE,  GPIO_PIN_1);
    //启用分频并且二分频，对我来说不分频更好算
//    PWM0->CC|=PWM_CC_USEPWM+PWM_CC_PWMDIV_2;
    //关闭PWM Block以便正常配置相关参数
    PWM0->_0_CTL&=~PWM_0_CTL_ENABLE;
    //同样在CTL中进行模式配置和同步配置，但是用寄存器定义出来的PWM波却不是
    //占空比50％，而是25、75。用函数配置则是正常的占空比。时间关系先不做深入
//    PWM0->_0_CTL|=PWM_0_CTL_MODE;
    MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);
    //增计数计数器和比较器B匹配时PWMB拉低，计数器匹配加载值时PWMB拉高
    PWM0->_0_GENB|=PWM_0_GENB_ACTCMPBU_ZERO+PWM_0_GENB_ACTLOAD_ONE;
    //这里系统时钟120MHZ，默认PWM分2频，实际为60Mhz，要得到1Khz信号则加载60000（为了数据更精确需要减一实际上为59999）,占空50则赋30000
    PWM0->_0_LOAD=0xEA5F;
    PWM0->_0_CMPB=0x7530;//最后实际图形为1.00003Khz的方波，占空50％
    PWM0->ENABLE|=PWM_ENABLE_PWM1EN;
    PWM0->_0_CTL|=PWM_0_CTL_ENABLE;
}

void gpio_init(){

    //led_init
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}

int main(void)
{
    //系统时钟设定为120Mhz
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);
    //和打开GPIO时钟一样，这里打开PWM时钟
    SYSCTL->RCGCPWM|=SYSCTL_RCGCPWM_R0;
    while((SYSCTL->RCGCPWM & SYSCTL_RCGCPWM_R0) == 0){};
    //打开GPIO时钟
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R5;               // activate clock for Port M,N
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R5)) == 0){}; // wait for preparation of Port M,N

    PWM_init();
    gpio_init();


    while(1){
        GPION->DATA |=BIT0;//亮一个灯证明系统正常运行
    }
}
