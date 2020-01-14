#ifndef _CONFIG_H_	
#define _CONFIG_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif

#include <math.h>
#include <string.h>			//字符串比较
#include <ctype.h>			//大写转换
#include <stdlib.h>
#include <stdio.h>

#include "stm32f4xx.h"		//STM32固件库
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
	
//#include "bsp.h"			//板级初始化
//#include "static_init.h"	//printf
#include "GPIO_Init.h"
//#include "BKP_Init.h"
//#include "ADC.h"   
//#include "Time2.h"

#include "BLDCTIM_Init.h" 


//#include "usb_lib.h"
//#include "hw_config.h"
//#include "usb_pwr.h"

/* 私有类型定义 --------------------------------------------------------------*/
typedef enum
{
  CW=0,  // 顺时针方向(对着电机轴看)，一般定义为反转
  CCW=1  // 逆时针方向(对着电机轴看)，一般定义为正转
}MOTOR_DIR;

typedef enum 
{
  STOP=0,  // 停机
  RUN=1    // 运行
}MOTOR_STATE;

typedef struct
{
  __IO int          motor_speed;        // 电机转速(RPM):0..3500
  __IO MOTOR_STATE  motor_state;        // 电机旋转方向
  __IO MOTOR_DIR    motor_direction;    // 电机转动方向
  __IO uint32_t     step_counter;       // 霍尔传感器步数，用于测量电机转速
  __IO uint16_t     stalling_count;     // 停止标志，如该值超过2000，认为电机停止旋转
}MOTOR_DEVICE;
extern MOTOR_DEVICE bldc_dev;
//定义PID结构体
typedef struct 
{
   __IO int      SetPoint;      //设定目标 Desired Value
   __IO double   Proportion;    //比例常数 Proportional Const
   __IO double   Integral;      //积分常数 Integral Const
   __IO double   Derivative;    //微分常数 Derivative Const
   __IO int      LastError;     //Error[-1]
   __IO int      PrevError;     //Error[-2]
}PID;

/* 私有宏定义 ----------------------------------------------------------------*/
/*************************************/
//定义PID相关宏
//这三个参数设定对电机运行影响非常大
/*************************************/
#define  P_DATA                   0.5               //P参数
#define  I_DATA                   0.06              //I参数
#define  D_DATA                   0                 //D参数

//#define  MOTOR_RATED_SPEED        2500              //确定转速
#define  MOTOR_MAX_SPEED          10000              //空载转速
#define  MOTOR_MIN_SPEED          100               
#define  MOTOR_POLE_PAIR_NUM      4                 //极对数(8 磁极数)



/************************************/
#define LED_RED_OFF()   	GPIO_SetBits(GPIOB, GPIO_Pin_9 );  	   //LED1 
#define LED_RED_ON()  		GPIO_ResetBits(GPIOB, GPIO_Pin_9 ); 	  

#define LED_GREEN_OFF()   	GPIO_SetBits(GPIOB, GPIO_Pin_8 );  	   //LED2 
#define LED_GREEN_ON()  	GPIO_ResetBits(GPIOB, GPIO_Pin_8 ); 	  

#define Ctrl_Brake_OFF()   	GPIO_SetBits(GPIOB, GPIO_Pin_12 );  	  
#define Ctrl_Brake_ON()  	GPIO_ResetBits(GPIOB, GPIO_Pin_12 ); 	  


extern const unsigned char Ascill_16[];

#define Enable_PrintfDebug

void DebugPrintf(const char *fmt,...);
void TIM1_PWM_Init(void);


#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}

#endif

#endif
