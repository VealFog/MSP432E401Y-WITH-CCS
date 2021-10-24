/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
uint32_t systemClock;
void PWM_init(){
    MAP_GPIOPinConfigure(GPIO_PF1_M0PWM1);
    MAP_GPIOPinTypePWM(GPIO_PORTF_BASE,  GPIO_PIN_1);
    //���÷�Ƶ���Ҷ���Ƶ��������˵����Ƶ������
//    PWM0->CC|=PWM_CC_USEPWM+PWM_CC_PWMDIV_2;
    //�ر�PWM Block�Ա�����������ز���
    PWM0->_0_CTL&=~PWM_0_CTL_ENABLE;
    //ͬ����CTL�н���ģʽ���ú�ͬ�����ã������üĴ������������PWM��ȴ����
    //ռ�ձ�50��������25��75���ú�����������������ռ�ձȡ�ʱ���ϵ�Ȳ�������
//    PWM0->_0_CTL|=PWM_0_CTL_MODE;
    MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);
    //�������������ͱȽ���Bƥ��ʱPWMB���ͣ�������ƥ�����ֵʱPWMB����
    PWM0->_0_GENB|=PWM_0_GENB_ACTCMPBU_ZERO+PWM_0_GENB_ACTLOAD_ONE;
    //����ϵͳʱ��120MHZ��Ĭ��PWM��2Ƶ��ʵ��Ϊ60Mhz��Ҫ�õ�1Khz�ź������60000��Ϊ�����ݸ���ȷ��Ҫ��һʵ����Ϊ59999��,ռ��50��30000
    PWM0->_0_LOAD=0xEA5F;
    PWM0->_0_CMPB=0x7530;//���ʵ��ͼ��Ϊ1.00003Khz�ķ�����ռ��50��
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
    //ϵͳʱ���趨Ϊ120Mhz
    systemClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                          SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                                          120000000);
    //�ʹ�GPIOʱ��һ���������PWMʱ��
    SYSCTL->RCGCPWM|=SYSCTL_RCGCPWM_R0;
    while((SYSCTL->RCGCPWM & SYSCTL_RCGCPWM_R0) == 0){};
    //��GPIOʱ��
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R5;               // activate clock for Port M,N
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R5)) == 0){}; // wait for preparation of Port M,N

    PWM_init();
    gpio_init();


    while(1){
        GPION->DATA |=BIT0;//��һ����֤��ϵͳ��������
    }
}