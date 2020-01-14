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
extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值 
u8 key=0;
u8 i=0,t=0;
u8 cnt=0;
u8 res;
u8 canbuf[8];//can数据缓存区
u8 canres[8];//can返回值
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
	 	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1Mhz的频率计数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	printf("*** BLDC Control board ***\r\n");
	printf("Date:20180910 V1.0 \r\n");
	while(1)
	{		//pwm输出板can
		
		key=CAN1_Receive_Msg(canbuf);
		if(key)//接收到有数据
		{			
			canres[0]=0x00;
			res=CAN1_Send_Msg(canres,8);//发送8个字节 res=1发送失败，0发送成功 
		}
		 if(canbuf[0]==0x00)
		{
			speed_duty=300;
			TIM_SetCompare1(TIM1,speed_duty);
			delay_ms(10);
			TIM_SetCompare1(TIM1,TIM_GetCapture1(TIM1)+1); 
			if(TIM_GetCapture1(TIM1)==300)TIM_SetCompare1(TIM1,0);			 
			if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;		   //得到总的高电平时间
			printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;			     //开启下一次捕获
		}
		}
		if(canbuf[0]==0x01)
		{
			speed_duty=200;
			TIM_SetCompare1(TIM1,speed_duty);
			delay_ms(10);
			TIM_SetCompare1(TIM1,TIM_GetCapture1(TIM1)+1); 
			if(TIM_GetCapture1(TIM1)==300)TIM_SetCompare1(TIM1,0);			 
			if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL;		   //得到总的高电平时间
			printf("HIGH:%lld us\r\n",temp); //打印总的高点平时间
			TIM5CH1_CAPTURE_STA=0;			     //开启下一次捕获
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
			GPIO_SetBits(GPIOD, GPIO_Pin_15);//刹车
		}
		if(canbuf[2]==0x01)
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_14 );//反转
		}
		else if(canbuf[2]==0x02)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_14 );//正转
		}
		//KeyControl();
		//U1ProtocolAnalysis();//串口解析
}
}
/*void KeyControl(void)
{
	
	if(KEY_RUNBRAKE == ON)  //run  //PA3输检测低电平，开启
	{
	  if(bldc_dev.motor_state==STOP) // 电机启动
	  {
		bldc_dev.motor_state=RUN;
		bldc_dev.step_counter=0;//霍尔传感器步数，电机转速0
		bldc_dev.stalling_count=0;//停止标志
		//IncPIDInit();
		speed_duty=200; //%20 start  速度占空比
		NVIC_EnableIRQ(HALL_UEXTI_IRQn);//外部中断3
		NVIC_EnableIRQ(HALL_VEXTI_IRQn);//外部中断4
		NVIC_EnableIRQ(HALL_WEXTI_IRQn);		//外部中断9_5
		HALL_EXTI_Callback();
	  } 
	
	}else // stop
	{
		BLDC_PHASE_STOP();
		while(KEY_RUNBRAKE);
	}
	
	if(KEY_SPEED_HIGHT == ON)   //速度挡位
	{
		
	}else
	{
		
	}
	
	if(KEY_DIR == ON)		//方向
	{
		bldc_dev.motor_direction=CCW;
	}
	else
	{
		bldc_dev.motor_direction=CW;
	}
	
	//CH3 总电机电流检测
//	if(Current_filter(ADCPrimevalValue[1]) > 3000)//电机过电流
//	{
//		Ctrl_Brake_ON();//PB12电平低
//		BLDC_PHASE_STOP();//相位电源关闭
//		LED_RED_ON();//GPIO9电平低。亮
//		while(1);
//	}
//	
	//CH2 外部滑动变阻器
//			if((bldc_dev.motor_speed*10/25)>70)
//		  speed_duty=bldc_dev.motor_speed*10/25;// *10/25为转速和占空比一个转换，转速（0~2500），占空比（0~1000）
//		else
//		  speed_duty=70;
//		
//	bldc_dev.motor_speed = SpeedVolt_filter(ADCPrimevalValue[0])*2;//  4096/1000 = 占空比
//	
//	if(bldc_dev.motor_speed<MOTOR_MIN_SPEED)
//		bldc_dev.motor_speed=MOTOR_MIN_SPEED;

//	if(bldc_dev.motor_speed>MOTOR_MAX_SPEED)
//		bldc_dev.motor_speed=MOTOR_MAX_SPEED;
	
	//bldc_pid.SetPoint=bldc_dev.motor_speed;
	
	
}*/
/**
  * 函数功能: 外部中断服务函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void HALL_EXTI_Callback(void)
{
  __IO uint8_t uwStep = 0;
  static __IO int8_t uwStep0=0;
  
  uint16_t hall_read=(HALL_GPIO->IDR)&0x0038; // 获取霍尔传感器状态 pin3 4 5
  
  if(bldc_dev.motor_state==STOP)return;
  
  uwStep=hall_read>>3; //此处需要根据实际更改顺序
  if(bldc_dev.motor_direction==CW) // 方向判断  
	uwStep=7-uwStep;
  
  BLDC_PHASE_CHANGE(uwStep);   // 驱动换相
  if(uwStep0!=uwStep) // 确保电机已经换相
  {    
    bldc_dev.step_counter++;
    bldc_dev.stalling_count=0;
  }
  uwStep0=uwStep;
}


void EXTI3_IRQHandler(void)		//霍尔中断
{
	/* 确认是否产生了EXTI LINE中断*/
	if(EXTI_GetITStatus(HALL_U_EXITLINE) != RESET)
	{
	 HALL_EXTI_Callback();
	}
	/* 清除中断标志位	*/
	EXTI_ClearITPendingBit(HALL_U_EXITLINE); 
}
void EXTI4_IRQHandler(void)		//霍尔中断
{
	/* 确认是否产生了EXTI LINE中断*/
	if(EXTI_GetITStatus(HALL_V_EXITLINE) != RESET)
	{
	HALL_EXTI_Callback();
	}
	/* 清除中断标志位	*/
	EXTI_ClearITPendingBit(HALL_V_EXITLINE|HALL_W_EXITLINE); 
}
void EXTI9_5_IRQHandler(void)		//霍尔中断
{
	/* 确认是否产生了EXTI LINE中断*/
	if(EXTI_GetITStatus(HALL_W_EXITLINE) != RESET)
	{
	HALL_EXTI_Callback();
	}
	/* 清除中断标志位	*/
	EXTI_ClearITPendingBit(HALL_V_EXITLINE|HALL_W_EXITLINE); 
}

