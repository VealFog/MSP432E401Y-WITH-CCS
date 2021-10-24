
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
uint32_t systemClock;
void PWM_init(){
    TIMER2->CTL &=~TIMER_CTL_TAEN;//�رն�ʱ��TA���Ա�����������

    TIMER2->CFG |= TIMER_CFG_16_BIT ;//д��0x4��ѡ��16λ��ʱ��
    TIMER2->TAMR &=~TIMER_TAMR_TACMR;//���CMR
    //��˳������Ϊ����PWMģʽ������ģʽ
    TIMER2->TAMR |= TIMER_TAMR_TAAMS+TIMER_TAMR_TAMR_PERIOD;
    TIMER2->CTL &=~ TIMER_CTL_TAPWML;//Ĭ�����״̬����1Ϊ�������
    //���Ϊ2KHZ������ռ�ձ�Ϊ66������Ϊϵͳʱ��Ϊ120��HZ������Ƶ�ʿ��Բο������д���޸ġ�
    TIMER2->TAILR =systemClock/2000;
    TIMER2->TAMATCHR=systemClock/6000;

    TIMER2->CTL |= TIMER_CTL_TAEN;//�򿪶�ʱ��TA
    //Ŀǰ�����������������壬ֱ�����ùٷ��⺯��������ǵײ㶨������û��PCTL��PCMn����ض���
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
    //�޸�ϵͳʱ��Ϊ120��HZ
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);
    //�ʹ�GPIOʱ��һ���������TIMER2ʱ��
    SYSCTL->RCGCTIMER|=SYSCTL_RCGCTIMER_R2;
    while((SYSCTL->RCGCTIMER & SYSCTL_RCGCTIMER_R2) == 0){};

    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R11;               // activate clock for Port M,N
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R11)) == 0){}; // wait for preparation of Port M,N

    PWM_init();
    gpio_init();


    while(1){
        GPION->DATA |=BIT0;//��һ����֤��ϵͳ��������
    }
}