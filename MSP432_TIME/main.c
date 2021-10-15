
/* DriverLib Includes */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void time_init(){
    TIMER0->CTL &=~TIMER_CTL_TAEN;//�رն�ʱ��TA���Ա�����������
    TIMER0->CFG |= TIMER_CFG_32_BIT_TIMER;//���Բ�д����Ϊ��λΪ0x0
    //��˳������Ϊ���ڼ�ʱ��������ģʽ����TA��ʱ���ж�
    TIMER0->TAMR |= TIMER_TAMR_TAMR_PERIOD+TIMER_TAMR_TACDIR+TIMER_TAMR_TAMIE;
    TIMER0->IMR |= TIMER_IMR_TAMIM;//�ر�TA��ʱ�����ж��ڸ�
    //���ö�Ӧ�ļ�ʱ�����Ŀǰ����֪������������ʱ����ʱ��Ƶ���Ƕ��٣��ȸ�50000������ʵ��Ч��
    //���Ϊ1s����
    TIMER0->TAILR =50000;
    TIMER0->TAMATCHR=50000;

    TIMER0->CTL |= TIMER_CTL_TAEN;//�򿪶�ʱ��TA
    MAP_IntEnable(INT_TIMER0A);//�򿪶�ʱ��TA�жϣ��ж�ϵͳ���ã��뵼��TI�ҵ�driverlib�⣬����Ҳֻ����һ������
}

void gpio_init(){

    //led_init
    GPION->DIR|=BIT1+BIT0;//D1,D2 light on
    GPION->DEN|=BIT1+BIT0;
}
int timer_A_count;
void TIMER0A_IRQHandler(void)//�̶���ʽ���жϺ��������Խ��жϿ��������жϺ���
{
    if (TIMER0->RIS | TIMER_RIS_TAMRIS){//��TA�������жϣ������Ӧ�жϲ���
        timer_A_count++;
        if (timer_A_count == 500){
            GPION->DATA ^=BIT1;
            timer_A_count=0;
        }
    }
    TIMER0->ICR |= TIMER_ICR_TAMCINT;//����жϱ�־λ�������һ��Ҫ�����
}

int main(void)
{

    //�ʹ�GPIOʱ��һ���������TIMER0ʱ��
    SYSCTL->RCGCTIMER|=SYSCTL_RCGCTIMER_R0;
    while((SYSCTL->RCGCTIMER & SYSCTL_RCGCTIMER_R0) == 0){};

    //��GPIOL����ΪTIMER0CCP0��TIMER0CCP1�ֱ���PL4��PL5����
    //�������ﲻ��PWMʹ�ã����Բ���Ҫ��GPIOL4��5���ж�Ӧ��PWM������Ŷ���
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R8+SYSCTL_RCGCGPIO_R10;               // activate clock for Port L,N,J
    while((SYSCTL->RCGCGPIO & (SYSCTL_PRGPIO_R12+SYSCTL_RCGCGPIO_R8+SYSCTL_RCGCGPIO_R10)) == 0){}; // wait for preparation of Port L,N,J

    time_init();
    gpio_init();


    while(1){
        GPION->DATA |=BIT0;//��һ����֤��ϵͳ��������
    }
}