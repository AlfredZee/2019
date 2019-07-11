#include "echo.h"
#include "delay.h"
int toofar=0;
int psc = 71;	//��Ƶϵ��  72 000 000 /72 =1us , *1000 0=10ms(1.5��)��ζ�㹻Զ,���迼�� 

void echo_clock_close(void)//��ʱ���� 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);

}
void echo_clock_init(void)//ÿ����һ��ʱ��,���Զ���ʼ��������� 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseInitStructer.TIM_Period=19999;// ��ʱ����Ϊ 7200 0000 *2 2s
	TIM_TimeBaseInitStructer.TIM_Prescaler=7100; 
	TIM_TimeBaseInitStructer.TIM_ClockDivision=0; 
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0	;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	NVIC_InitTypeDef NVIC_InitStructer;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructer.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	TIM_Cmd(TIM5,ENABLE);
}	

/* ��ʼ��ģ��� GPIO �Լ���ʼ����ʱ�� TIM2*/
void echo_init(void)
{
	echo_clock_init();
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitStructer.TIM_Period=9999;// ��ʱ����Ϊ 10000
	TIM_TimeBaseInitStructer.TIM_Prescaler=psc; 
	TIM_TimeBaseInitStructer.TIM_ClockDivision=0; 
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructer.TIM_RepetitionCounter = 0	;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	NVIC_InitTypeDef NVIC_InitStructer;
	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructer.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructer);
	TIM_Cmd(TIM2,DISABLE);// �رն�ʱ��ʹ��




	GPIO_InitTypeDef GPIO_InitStructer;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*TRIG �ź� */
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructer);
	/*ECOH �ź� */
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOB, & GPIO_InitStructer);
}
 	
 	
double get_length(void)
{
	double length=0;
	long int t;
	double time; 
		GPIO_SetBits(GPIOB,GPIO_Pin_8); 
		Delay_us(20); 
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);  
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==RESET);
		TIM_Cmd(TIM2,ENABLE);// �����źŵ�����������ʱ������
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==SET);
		TIM_Cmd(TIM2,DISABLE);// �رն�ʱ��
		if(!toofar)	//û�����һ�μ�������,�����벻�Ǻ�Զ 
		{
			t=TIM_GetCounter(TIM2);// ��ȡ TIM2 �Ĵ����еļ���ֵ
			time = t/1000000; //����ʱ��,��λs 
			length =  340*time/2; //���̳���,��λm 
			TIM2->CNT=0; // �� TIM2 �����Ĵ����ļ���ֵ����
		}
		else
		{
			toofar = 0; 
			length =99;	//�㹻Զ,���ÿ��� 
			TIM2->CNT=0; // �� TIM2 �����Ĵ����ļ���ֵ����
		}
	return length;
}
void TIM2_IRQHandler(void) // �жϣ��������㹻Զʱ,����toofar = 1,�����ټ��� 
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	  {
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);// ����жϱ�־
		toofar = 1; 
	  }
}
