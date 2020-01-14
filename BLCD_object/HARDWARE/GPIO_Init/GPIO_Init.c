#include "GPIO_Init.h"

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

     RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA|
                            RCC_AHB1Periph_GPIOE, ENABLE);	//???

    //usb ctrl	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6 ;									
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//?????50M
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//???????????	
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
//    GPIO_Init(GPIOA, &GPIO_InitStructure);	
//		GPIO_ResetBits(GPIOA, GPIO_Pin_6 );
//		GPIO_ResetBits(GPIOA, GPIO_Pin_3 );
//		GPIO_ResetBits(GPIOA, GPIO_Pin_4 );
		
    //LED														 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;									
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//?????50M
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//???????????
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????	
//    GPIO_Init(GPIOE, &GPIO_InitStructure);	
//		GPIO_SetBits(GPIOE, GPIO_Pin_10 );
//		GPIO_SetBits(GPIOE, GPIO_Pin_11 );
	
	//ctrl brake													 
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;									
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//?????50M
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//???????????	
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
//    GPIO_Init(GPIOB, &GPIO_InitStructure);	
//		GPIO_SetBits(GPIOB, GPIO_Pin_12 );

	
	 //??????
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  

    GPIO_Init(GPIOE, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   				//PWM IN
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void HALL_GPIO_Init(void)
{
   /* ??IO?????????? */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ??????????????? */
//  EXTI_InitTypeDef EXTI_InitStructure;
//  /* ??????????????????? */
//  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ???IO???????????????? */
  RCC_AHB1PeriphClockCmd(HALL_RCC_CLOCKGPIO,ENABLE);
  
  /* ????IO?? */
  GPIO_InitStructure.GPIO_Pin = HALL_U_GPIO_PIN;  
  /* ????IO????? */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  
  /* ???KEY2????IO */
  GPIO_Init(HALL_GPIO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = HALL_V_GPIO_PIN; 
  GPIO_Init(HALL_GPIO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = HALL_W_GPIO_PIN; 
  GPIO_Init(HALL_GPIO, &GPIO_InitStructure);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//??SYSCFG??
  /* ????????? */
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,HALL_U_GPIO_PINSOURCE);
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,HALL_V_GPIO_PINSOURCE);
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,HALL_W_GPIO_PINSOURCE);
//  
  /* ?????? */
//  EXTI_InitStructure.EXTI_Line=HALL_U_EXITLINE|HALL_V_EXITLINE|HALL_W_EXITLINE;
//  /* ?????? */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  /* ??????? */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  /* ???? */
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  /* ??EXTI_InitStruct???????????EXTI??? */
//  EXTI_Init(&EXTI_InitStructure);
//  
//  /* ???????? */
//  NVIC_InitStructure.NVIC_IRQChannel = HALL_UEXTI_IRQn;
//  /* ?????????1 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
//  /* ???????1 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//  /* ???????? */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
//  /* ?????????????? */
//  NVIC_Init(&NVIC_InitStructure); 
//    /* ???????? */
//  NVIC_InitStructure.NVIC_IRQChannel = HALL_VEXTI_IRQn;
//  /* ?????????1 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
//  /* ???????1 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//  /* ???????? */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
//  /* ?????????????? */
//  NVIC_Init(&NVIC_InitStructure); 
//    /* ???????? */
//  NVIC_InitStructure.NVIC_IRQChannel = HALL_WEXTI_IRQn;
//  /* ?????????1 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
//  /* ???????1 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//  /* ???????? */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
//  /* ?????????????? */
//  NVIC_Init(&NVIC_InitStructure); 
  
}







