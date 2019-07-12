#include "car.h" 
#include "delay.h"
int carSpeedPeriod = 0;

void car_init(int i)
{
	//��ת������ص����� 
	 GPIO_InitTypeDef GPIO_InitStructure;
 	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
 	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������? 
 	 GPIO_Init(GPIOD, &GPIO_InitStructure);
 	 //��timer��ص�����
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�������� 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	  
	  GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef T;
	
	carSpeedPeriod+= i;
	T.TIM_Period  = carSpeedPeriod;
	T.TIM_Prescaler = 719;
	T.TIM_ClockDivision = 0;
	T.TIM_CounterMode = TIM_CounterMode_Up;
	T.TIM_RepetitionCounter = 0	;
	TIM_TimeBaseInit(TIM3,&T);
	TIM_Cmd(TIM3,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_OCInitTypeDef O;
	O.TIM_OCMode = TIM_OCMode_PWM2;
	O.TIM_Pulse =5;
	O.TIM_OutputState = TIM_OutputState_Enable;
	O.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OC1Init(TIM3,&O);
	TIM_OC2Init(TIM3,&O);
	TIM_OC3Init(TIM3,&O);
	TIM_OC4Init(TIM3,&O);
	  
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	   } 
	   
	   
// ���Ƴ��ӵ���������	   
void gogogo(int direction)
{
	//ǰ�� 
	if(direction == 1)
	{
		carSpeedPeriod = 0;
		car_init(30);
		GPIO_WriteBit(GPIOD, GPIO_Pin_2,Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_4,Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_1,Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_3,Bit_RESET);
	}
	//ͣ�� 
	else if(direction == 0)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE); 
		TIM_Cmd(TIM3,DISABLE);
	}
	//���� 
	else if(direction == -1)
	{
		carSpeedPeriod = 0;
		car_init(30);
		GPIO_WriteBit(GPIOD, GPIO_Pin_2,Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_4,Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_1,Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_3,Bit_SET);
	}
}
	
//ת��,û�п��ǻ��� 
void turnx(int direction)
{
	//��ת 
	if(direction == 1)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_1,Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_3,Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_2,Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_4,Bit_SET);
	}
	//��ת 
	if(direction == 2)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_1,Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_3,Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_2,Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_4,Bit_RESET);
	}
}
void turn(int direction)
{
	//��ת 
	if(direction == 1)
	{
		turnx(1); 
		Delay_ms(1000);
		gogogo(1);//���� 
	}
	//��ת 
	else if(direction == 2)
	{
		turnx(2);
		Delay_ms(1000);
		gogogo(1);
	}	
}

//���³�ʼ������ ����ʱ����
void changeSpeedx(int i)
{
	
	TIM_TimeBaseInitTypeDef T;
	carSpeedPeriod += i;
	T.TIM_Period  = carSpeedPeriod;
	T.TIM_Prescaler = 719;
	T.TIM_ClockDivision = 0;
	T.TIM_CounterMode = TIM_CounterMode_Up;
	T.TIM_RepetitionCounter = 0	;
	TIM_TimeBaseInit(TIM3,&T);
	TIM_Cmd(TIM3,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
}
//�ı��ٶ�,ͨ���ı����� 
void changeSpeed(int i)
{
	if(carSpeedPeriod > 2)//������С�� 
	car_init(i*2);
}

