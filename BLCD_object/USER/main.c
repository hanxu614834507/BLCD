#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "config.h"
#include "can.h"
#include "key.h"
#include "timer.h"
MOTOR_DEVICE bldc_dev  ={1000,STOP,CCW,0,0};
static PID bldc_pid;
static unsigned char PID_Status = 0;

const unsigned char  Ascill_16[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

unsigned int Program_Cycle_Nums = 0;
void PrintCommandsList(void);
unsigned int U1ProtocolAnalysis(void);
void KeyControl(void);
void HALL_EXTI_Callback(void);
void HAL_SYSTICK_Callback(void);
void IncPIDInit(void);
int IncPIDCalc(int NextPoint);
unsigned int Current_filter(unsigned int advalue);
unsigned int SpeedVolt_filter(unsigned int advalue);
extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ 
u8 key=0;
u8 i=0,t=0;
u8 cnt=0;
u8 res;
u8 canbuf[8];//can���ݻ�����
u8 canres[8];//can����ֵ
long long temp=0;  
int main(void)
{
  SystemInit();  
	delay_init(168);
	//GPIO_Config();
	//HALL_GPIO_Init();
	/************************************/ 
	CAN_Config();
	BLDC_TIMx_PWM_Init();
	LED_Init();
  uart_init(115200);   
	//KeyControl();
	 	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1Mhz��Ƶ�ʼ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	printf("*** BLDC Control board ***\r\n");
	printf("Date:20180910 V1.0 \r\n");
	while(1)
	{		//pwm�����can
		
		key=CAN1_Receive_Msg(canbuf);
		if(key)//���յ�������
		{			
			canres[0]=0x00;
			res=CAN1_Send_Msg(canres,8);//����8���ֽ� res=1����ʧ�ܣ�0���ͳɹ� 
		}
		 if(canbuf[0]==0x00)
		{
			speed_duty=300;
			TIM_SetCompare1(TIM1,speed_duty);
			delay_ms(10);
			TIM_SetCompare1(TIM1,TIM_GetCapture1(TIM1)+1); 
			if(TIM_GetCapture1(TIM1)==300)TIM_SetCompare1(TIM1,0);			 
			if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;			     //������һ�β���
		}
		}
		if(canbuf[0]==0x01)
		{
			speed_duty=200;
			TIM_SetCompare1(TIM1,speed_duty);
			delay_ms(10);
			TIM_SetCompare1(TIM1,TIM_GetCapture1(TIM1)+1); 
			if(TIM_GetCapture1(TIM1)==300)TIM_SetCompare1(TIM1,0);			 
			if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp); //��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;			     //������һ�β���
		}
		}
		 if(canbuf[0]==0x02)
		 {
			 speed_duty=10;
				TIM_SetCompare1(TIM1,speed_duty);
		 }
		 
		if(canbuf[1]==0x01)
		{		
			GPIO_ResetBits(GPIOD, GPIO_Pin_15);
			delay_ms(500);
			GPIO_SetBits(GPIOD, GPIO_Pin_15);//ɲ��
		}
		if(canbuf[2]==0x01)
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_14 );//��ת
		}
		else if(canbuf[2]==0x02)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_14 );//��ת
		}
		//KeyControl();
		//U1ProtocolAnalysis();//���ڽ���
}
}
/*void KeyControl(void)
{
	
	if(KEY_RUNBRAKE == ON)  //run  //PA3����͵�ƽ������
	{
	  if(bldc_dev.motor_state==STOP) // �������
	  {
		bldc_dev.motor_state=RUN;
		bldc_dev.step_counter=0;//�������������������ת��0
		bldc_dev.stalling_count=0;//ֹͣ��־
		//IncPIDInit();
		speed_duty=200; //%20 start  �ٶ�ռ�ձ�
		NVIC_EnableIRQ(HALL_UEXTI_IRQn);//�ⲿ�ж�3
		NVIC_EnableIRQ(HALL_VEXTI_IRQn);//�ⲿ�ж�4
		NVIC_EnableIRQ(HALL_WEXTI_IRQn);		//�ⲿ�ж�9_5
		HALL_EXTI_Callback();
	  } 
	
	}else // stop
	{
		BLDC_PHASE_STOP();
		while(KEY_RUNBRAKE);
	}
	
	if(KEY_SPEED_HIGHT == ON)   //�ٶȵ�λ
	{
		
	}else
	{
		
	}
	
	if(KEY_DIR == ON)		//����
	{
		bldc_dev.motor_direction=CCW;
	}
	else
	{
		bldc_dev.motor_direction=CW;
	}
	
	//CH3 �ܵ���������
//	if(Current_filter(ADCPrimevalValue[1]) > 3000)//���������
//	{
//		Ctrl_Brake_ON();//PB12��ƽ��
//		BLDC_PHASE_STOP();//��λ��Դ�ر�
//		LED_RED_ON();//GPIO9��ƽ�͡���
//		while(1);
//	}
//	
	//CH2 �ⲿ����������
//			if((bldc_dev.motor_speed*10/25)>70)
//		  speed_duty=bldc_dev.motor_speed*10/25;// *10/25Ϊת�ٺ�ռ�ձ�һ��ת����ת�٣�0~2500����ռ�ձȣ�0~1000��
//		else
//		  speed_duty=70;
//		
//	bldc_dev.motor_speed = SpeedVolt_filter(ADCPrimevalValue[0])*2;//  4096/1000 = ռ�ձ�
//	
//	if(bldc_dev.motor_speed<MOTOR_MIN_SPEED)
//		bldc_dev.motor_speed=MOTOR_MIN_SPEED;

//	if(bldc_dev.motor_speed>MOTOR_MAX_SPEED)
//		bldc_dev.motor_speed=MOTOR_MAX_SPEED;
	
	//bldc_pid.SetPoint=bldc_dev.motor_speed;
	
	
}*/
/**
  * ��������: �ⲿ�жϷ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HALL_EXTI_Callback(void)
{
  __IO uint8_t uwStep = 0;
  static __IO int8_t uwStep0=0;
  
  uint16_t hall_read=(HALL_GPIO->IDR)&0x0038; // ��ȡ����������״̬ pin3 4 5
  
  if(bldc_dev.motor_state==STOP)return;
  
  uwStep=hall_read>>3; //�˴���Ҫ����ʵ�ʸ���˳��
  if(bldc_dev.motor_direction==CW) // �����ж�  
	uwStep=7-uwStep;
  
  BLDC_PHASE_CHANGE(uwStep);   // ��������
  if(uwStep0!=uwStep) // ȷ������Ѿ�����
  {    
    bldc_dev.step_counter++;
    bldc_dev.stalling_count=0;
  }
  uwStep0=uwStep;
}


void EXTI3_IRQHandler(void)		//�����ж�
{
	/* ȷ���Ƿ������EXTI LINE�ж�*/
	if(EXTI_GetITStatus(HALL_U_EXITLINE) != RESET)
	{
	 HALL_EXTI_Callback();
	}
	/* ����жϱ�־λ	*/
	EXTI_ClearITPendingBit(HALL_U_EXITLINE); 
}
void EXTI4_IRQHandler(void)		//�����ж�
{
	/* ȷ���Ƿ������EXTI LINE�ж�*/
	if(EXTI_GetITStatus(HALL_V_EXITLINE) != RESET)
	{
	HALL_EXTI_Callback();
	}
	/* ����жϱ�־λ	*/
	EXTI_ClearITPendingBit(HALL_V_EXITLINE|HALL_W_EXITLINE); 
}
void EXTI9_5_IRQHandler(void)		//�����ж�
{
	/* ȷ���Ƿ������EXTI LINE�ж�*/
	if(EXTI_GetITStatus(HALL_W_EXITLINE) != RESET)
	{
	HALL_EXTI_Callback();
	}
	/* ����жϱ�־λ	*/
	EXTI_ClearITPendingBit(HALL_V_EXITLINE|HALL_W_EXITLINE); 
}

