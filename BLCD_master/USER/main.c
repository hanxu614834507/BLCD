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
u8 canbuf[8];//can数据缓存区
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
	{		//按键控制板
		key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下,speed+1
		{		speed++;
				canbuf[0]=speed;//填充发送缓冲区 
				res=CAN1_Send_Msg(canbuf,8);//发送8个字节
		}
		if(key==KEY1_PRES)//KEY1按下,刹车
		{
					canbuf[0]=0x00;
				canbuf[1]=0x01;//填充发送缓冲区 
			res=CAN1_Send_Msg(canbuf,8);//发送8个字节 			   
		}
		if(key==KEY2_PRES)//转向
		{
			cnt=-cnt;
		
		if(cnt==1)
		{
			canbuf[2]=0x01;
		}
		else canbuf[2]=0x02;
			res=CAN1_Send_Msg(canbuf,8);//发送8个字节
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
			LED0=!LED0;//提示系统正在运行	
			t=0;
		}	
		delay_ms(10);
	}
}
