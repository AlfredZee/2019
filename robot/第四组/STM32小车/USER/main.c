#include "stm32f10x.h"
#include "car.h"
#include "delay.h"
#include "bluetooth.h"
#include "echo.h"
int go_flag=0;
int echo_flag=0;
int main(void)
{
	

	bluetooth_init(); 
    SysTick_Init();//��ʱ��ʼ��
	echo_init(); 

	
	
	while(1)
	{
		
			Delay_ms(500); 
			/*if(get_length()<0.005)
			{
				if(go_flag ==1)//ignore back
				{
						gogogo(0);
				}
				
			}
			else if(go_flag == 1)
			{
				gogogo(1);
			}*/
	}
	
}

//ÿ��timer֮�� 
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
//ÿ��TIM5����,��ʼһ�γ�������� 
void TIM5_IRQHandler(void)
{
	/*if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		if(go_flag&&echo_flag)	//��������˻������� 
			if(get_length()<0.005)
			{
			turnx(1);	
			Delay_ms(1500);
			turnx(2);
			}
		
	}*/
}
void USART1_IRQHandler(void)
{
	int order =0;	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		order = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		USART_ClearFlag(USART1,USART_IT_RXNE);
			switch(order)
			{
				case 1:case'1'://ǰ�� 
					go_flag=1;
					gogogo(1);
					break;
				case 2:case '2'://���� 
					go_flag=2; 
					gogogo(-1);
					break;
				case 3:case'3'://���� 
					changeSpeed(-1);
					break;
				case 4:case '4'://���� 
					changeSpeed(1);
					break;
				case 5:case '5'://��ת 
					//go_flag=1;
					turnx(1);
					break;
				case 6:case'6'://��ת 
					//go_flag=1;
					turnx(2);
					break;
				case 7:case'7'://ͣ�� 
					gogogo(0);
					go_flag=0;
					break;
				case 8:case'8'://����������
					echo_flag=1;
					break;
				case 9:case '9'://�رճ��� 
					echo_flag=0;
					break; 
			}
		
	}

}
	
