#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "can.h"
#include "key.h"

u8 key;
u8 i=0,t=0;
u8 cnt=1;
u8 res;
u8 speed;
u8 canbuf[8];//can���ݻ�����
int main(void)
{
  SystemInit();  
	delay_init(168);
	//GPIO_Config();
	//HALL_GPIO_Init();
	/************************************/ 
	LED_Init();
	KEY_Init();
	CAN_Config();
  uart_init(115200);   
	//KeyControl();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	printf("*** BLDC Control board ***\r\n");
	printf("Date:20180910 V1.0 \r\n");
	while(1)
	{		//�������ư�
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0����,speed+1
		{		speed++;
				canbuf[0]=speed;//��䷢�ͻ����� 
				res=CAN1_Send_Msg(canbuf,8);//����8���ֽ�
		}
		if(key==KEY1_PRES)//KEY1����,ɲ��
		{
					canbuf[0]=0x00;
				canbuf[1]=0x01;//��䷢�ͻ����� 
			res=CAN1_Send_Msg(canbuf,8);//����8���ֽ� 			   
		}
		if(key==KEY2_PRES)//ת��
		{
			cnt=-cnt;
		
		if(cnt==1)
		{
			canbuf[2]=0x01;
		}
		else canbuf[2]=0x02;
			res=CAN1_Send_Msg(canbuf,8);//����8���ֽ�
	}
		if(speed==1)
		{
			LED1=0;
			LED2=1;
		}
			if(speed==2)
		{
			LED2=0;
			LED1=1;
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED0=!LED0;//��ʾϵͳ��������	
			t=0;
		}	
		delay_ms(10);
	}
}
