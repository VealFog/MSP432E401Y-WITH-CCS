
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void time_init(){
    TIMER0->CTL &=~TIMER_CTL_TAEN;//关闭定时器TA，以便进行相关配置
    TIMER0->CFG |= TIMER_CFG_32_BIT_TIMER;//可以不写，因为该位为0x0
    //按顺序配置为周期计时，增计数模式，打开TA计时器中断
    TIMER0->TAMR |= TIMER_TAMR_TAMR_PERIOD+TIMER_TAMR_TACDIR+TIMER_TAMR_TAMIE;
    TIMER0->IMR |= TIMER_IMR_TAMIM;//关闭TA定时器的中断掩盖
    //设置对应的计时间隔，目前还不知道分配给这个定时器的时钟频率是多少，先给50000，根据实际效果
    //大概为1s左右
    TIMER0->TAILR =50000;
    TIMER0->TAMATCHR=50000;

    TIMER0->CTL |= TIMER_CTL_TAEN;//打开定时器TA
    MAP_IntEnable(INT_TIMER0A);//打开定时器TA中断，中断系统配置，请导入TI家的driverlib库，不过也只有这一句用了
}

void gpio_init(){

    //led_init
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}
int timer_A_count;
void TIMER0A_IRQHandler(void)//固定形式，中断函数，可以进中断看看其他中断函数
{
    if (TIMER0->RIS | TIMER_RIS_TAMRIS){//当TA触发了中断，进入对应中断操作
        timer_A_count++;
        if (timer_A_count == 500){
            GPION->DATA ^=BIT1;
            timer_A_count=0;
        }
    }
    TIMER0->ICR |= TIMER_ICR_TAMCINT;//清除中断标志位，这个是一定要清除的
}

int main(void)
{

    //和打开GPIO时钟一样，这里打开TIMER0时钟
    SYSCTL->RCGCTIMER|=SYSCTL_RCGCTIMER_R0;
    while((SYSCTL->RCGCTIMER & SYSCTL_RCGCTIMER_R0) == 0){};

    //打开GPIOL，因为TIMER0CCP0和TIMER0CCP1分别在PL4和PL5上面
    //但是这里不做PWM使用，所以不需要对GPIOL4和5进行对应的PWM输出引脚定义
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R8+SYSCTL_RCGCGPIO_R10;               // activate clock for Port L,N,J
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R8+SYSCTL_RCGCGPIO_R10)) == 0){}; // wait for preparation of Port L,N,J

    time_init();
    gpio_init();


    while(1){
        GPION->DATA |=BIT0;//亮一个灯证明系统正常运行
    }
}
