#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_rcc.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


vu8 DATA_SPEED[60]={255,1,0,0,
										255,2,0,0,
										255,3,0,0,
										255,4,0,0,
										255,5,0,0,
										255,6,0,0,
										255,7,0,0,
//										255,8,0,0,
//										255,9,0,0,
//										255,10,0,0
//										255,11,0,0,
//										255,12,0,0,
//										255,13,0,0,
//										255,14,0,0,										
};
#define 	ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)

////////////////////// --KHAI BAO DINH NGHIA PWM-- /////////////////////////////////////
#define	DIA_CHI_START1 												DATA_SPEED[0]=255
#define	ID1 		 															DATA_SPEED[1]=1
#define Mor_FrontRight_thuan 	 					 			DATA_SPEED[2]=0
#define Mor_FrontRight_nghich 								DATA_SPEED[2]=1
#define	Mor_FrontRight			 		 							DATA_SPEED[3]

#define	DIA_CHI_START2 												DATA_SPEED[4]=255
#define	ID2 		 															DATA_SPEED[5]=2
#define Mor_FrontLeft_thuan			 							DATA_SPEED[6]=1
#define Mor_FrontLeft_nghich									DATA_SPEED[6]=0
#define	Mor_FrontLeft 						 						DATA_SPEED[7]

#define	DIA_CHI_START3 												DATA_SPEED[8]=255
#define	ID3 		 															DATA_SPEED[9]=3
#define Mor_RearRight_thuan 									DATA_SPEED[10]=0
#define Mor_RearRight_nghich				 					DATA_SPEED[10]=1
#define	Mor_RearRight				 		 							DATA_SPEED[11]

#define	DIA_CHI_START4 												DATA_SPEED[12]=255
#define	ID4 		 															DATA_SPEED[13]=4
#define Mor_RearLeft_thuan					 					DATA_SPEED[14]=1
#define Mor_RearLeft_nghich										DATA_SPEED[14]=0
#define	Mor_RearLeft 		 											DATA_SPEED[15]


//-----------------------MAM XOAY _ TRUC X Y-----------------//
#define	DIA_CHI_START5												DATA_SPEED[16]=255
#define	ID5	 																	DATA_SPEED[17]=5
#define Mor_Mam_thuan					 								DATA_SPEED[18]=1
#define Mor_Mam_nghich												DATA_SPEED[18]=0
#define	Mor_Mam 		 													DATA_SPEED[19]

#define	DIA_CHI_START6												DATA_SPEED[20]=255
#define	ID6	 																	DATA_SPEED[21]=6
#define Mor_TrucX_thuan					 							DATA_SPEED[22]=1
#define Mor_TrucX_nghich											DATA_SPEED[22]=0
#define	Mor_TrucX 		 												DATA_SPEED[23]

#define	DIA_CHI_START7											  DATA_SPEED[24]=255
#define	ID7																    DATA_SPEED[25]=7
#define Mor_TrucY_thuan					 							DATA_SPEED[26]=1
#define Mor_TrucY_nghich											DATA_SPEED[26]=0
#define	Mor_TrucY 		 												DATA_SPEED[27]



//=============================ADC DMI======================================//

#define cam_bien_laze_phai			        _ADC1_Value[6]
#define cam_bien_laze_trai		        	_ADC1_Value[7]
#define cam_bien_laze_truoc							_ADC1_Value[5]

//=============================BIEN TRO_ER ======================================//

#define bien_tro_xoay_mam										_ADC1_Value[2]			//OK
#define bien_tro_nang_trucY									_ADC1_Value[1]
#define bien_tro_nang_trucX									_ADC1_Value[0]


//==============================CAM BIEN XILANH========================================//
//#define 	CAM_BIEN_XL_NANG_BONG_SILO  			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)
//#define 	CAM_BIEN_XL_THA_BONG_SILO   			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)


//==============================CHUYEN SAN========================================//
#define  DO																GPIO_WriteBit(GPIOC,GPIO_Pin_15,1)
#define  XANH																GPIO_WriteBit(GPIOC,GPIO_Pin_15,0)


//================================INPUT - NUT NHAN ==================================//
#define  NUT_START			GPIO_ReadInputDataBit(GPIOe,GPIO_Pin_1)
#define  NUT_STOP				GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define  NUT_RESET			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)


//================================OUTPUT - XILANH ==================================//
#define  XI_LANH_NANG_BONG_ON								  GPIO_WriteBit(GPIOA,GPIO_Pin_11,1)
#define  XI_LANH_NANG_BONG_OFF								GPIO_WriteBit(GPIOA,GPIO_Pin_11,0)

#define  XI_LANH_THA_BONG_ON							  	GPIO_WriteBit(GPIOA,GPIO_Pin_12,1)
#define  XI_LANH_THA_BONG_OFF									GPIO_WriteBit(GPIOA,GPIO_Pin_12,0)


//=============================ENCODER======================================
#define Encoder_FL	(vs32)((num_over_t1<<16) | TIM_GetCounter(TIM1))
#define Encoder_FR	(vs32)((num_over_t2<<16) | TIM_GetCounter(TIM2))

//#define Encoder_Xoay	(vs32)((num_over_t4<<16) | TIM_GetCounter(TIM4))/100
//#define ENCODER_3		(vs32)((num_over_t3<<16) | TIM_GetCounter(TIM3))/100


///////////////////////KET THUC KHAI BAO  NHUNG CHAN MAC DINH/////////////////////////////////////////

vs32 	sieu_am,num_over_t1=0,num_over_t2=0, num_over_t3=0,num_over_t5=0, num_over_t9=0,CCR2_Val,HMI_LOOP, num_over_t4;;
vs16 	IMU,IMUxoay;
vu8 	data_tx_gyro,en_gyro,dataTxGyro,enGyro, Bien_vong = 0, Bien_nang = 1, Cap_vong = 0, Keo_loxo = 0; 
int 	lazeTruocValue,lazeTraiValue,lazePhaiValue,BienTroTrucXValue,BienTroTrucYValue = 0,BienTroMamXoayValue = 0,i=0;
int 	ctht_Xoay =1, cb_BanT1 =1,cb_BanT2 =1, cb_CON2=1, gocSai=0,cb_CON1=1,la,cb_bt=1, noise, noise1;
vu16 _ADC1_Value[8];

vu8   RX_USART1[15], RX_USART2[15],RX_USART4[15],DATA_COLOR[10];
uint8_t MANG_GAME[10];		

extern unsigned char GP_BTN [15];
extern vu8	Cam;

extern int _robotIMUAngle;
char Vi_tri=0;
int coVong = 0;
int Vitri_tru = 0;
int g0,g1,g2,g3,g4,g5,g6;
int bit_khoa_ham_thay_tuan = 0;
int dang_Ban = 0;

int Max_en_Xoay = 2900;
int Min_en_Xoay = -2950;
int robotIsRun_cc = 0;


//-----------------------khai bao i2c
//long in_min = 500;
//long in_max = 2500;
//long out_min = 0;
//long out_max = 180;
//volatile uint32_t TimingDelay;

//////////////////////////chong nhieu encoder////////////////////
vs32 ENCODER_FL()
{
	vs32 en, enOld = Encoder_FL;
	int i=0;
	while( i < 2)
	{
		en = Encoder_FL;
		if(abs(en - enOld) < 50) i++;
		enOld = en;
	}
	return abs(en);
}

//////////////////////////chong nhieu encoder////////////////////
vs32 ENCODER_FR()
{
	vs32 en, enOld = Encoder_FR;
	int i=0;
	while( i < 2)
	{
		en = Encoder_FR;
		if(abs(en - enOld) < 50) i++;
		enOld = en;
	}
	return abs(en);
}

//vs32 ENCODER_XOAY()
//{
//	//return (vs32)((num_over_t2<<16)|TIM_GetCounter(TIM2))/100;
//	vs32 en, enOld = Encoder_Xoay;
//	int i=0;
//	while( i < 2)
//	{
//		en = Encoder_Xoay;
//		if(abs(en - enOld) < 50) i++;
//		enOld = en;
//	}
//	return en;
//}

vs32 ENCODER_TONG()
{
	return	abs(ENCODER_FL()) + abs(ENCODER_FR());
}

//vs32 ENCODER_BAN()
//{
//	vs32 en, enOld = ENCODER_3;
//	int i=0;
//	while( i < 2)
//	{
//		en = ENCODER_3;
//		if(abs(en - enOld) < 50) i++;
//		enOld = en;
//	}
//	return en;
//}
//-------------delay unit------------
//void Delay(__IO uint32_t nTime)
//{
//   TimingDelay = nTime;
//   while(TimingDelay){
//   }
//}

//------------------------------------ DELAY MICROSECOND--------------------------------------------
void delay_us(vu32 num)
{
	__IO uint32_t index = 0;
	/* default system clock is 180MHz */
	for(index = (18 * num); index != 0; index--);
}
//------------------------------------ DELAY MILISECOND--------------------------------------------
void delay_ms(vu32 num)
{
	__IO uint32_t index = 0;
	/* default system clock is 180MHz */
	for(index = (18000 * num); index != 0; index--);
}

////-----------------config i2c -------------------------------
//void Init_I2C2(){

//   GPIO_InitTypeDef GPIO_InitStructure;
//   I2C_InitTypeDef I2C_InitStruct;

//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
//   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//   GPIO_Init(GPIOB, &GPIO_InitStructure);

//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C2);
//   GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C2);

//   I2C_InitStruct.I2C_ClockSpeed = 100000;
//   I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
//   I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; 
//   I2C_InitStruct.I2C_OwnAddress1 = 0x00;
//   I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
//   I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//   I2C_Init(I2C2, &I2C_InitStruct);

//   I2C_Cmd(I2C2, ENABLE);

//}
////---------------- i2c start and stop  -----------------------------------
//void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction){
//   // wait until I2C1 is not busy anymore
////   while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

//   // Send I2C1 START condition 
//   I2C_GenerateSTART(I2Cx, ENABLE);

//   // wait for I2C1 EV5 --> Slave has acknowledged start condition
////   while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

//   // Send slave Address for write 
//   I2C_Send7bitAddress(I2Cx, address, direction);

//   /* wait for I2C1 EV6, check if 
//    * either Slave has acknowledged Master transmitter or
//    * Master receiver mode, depending on the transmission
//    * direction
//    */ 
////   if(direction == I2C_Direction_Transmitter){
////      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
////   } else if(direction == I2C_Direction_Receiver){
////      while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
////   }
//}


//void I2C_stop(I2C_TypeDef* I2Cx){
//   // Send I2C1 STOP Condition 
//   I2C_GenerateSTOP(I2Cx, ENABLE);
//}




////---------------i2c write-------------------
//void I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
//{
//   I2C_SendData(I2Cx, data);
//   // wait for I2C1 EV8_2 --> byte has been transmitted
////   while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//}

////----------------------utils i2c -------------------------
//uint8_t I2C_read_ack(I2C_TypeDef* I2Cx){
//   uint8_t data;
//   // enable acknowledge of recieved data
//   I2C_AcknowledgeConfig(I2Cx, ENABLE);
//   // wait until one byte has been received
////   while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
//   // read data from I2C data register and return data byte

//   return data;
//}

///* This function reads one byte from the slave device
// * and doesn't acknowledge the recieved data 
// */
//uint8_t I2C_read_nack(I2C_TypeDef* I2Cx){
//   uint8_t data;
//   // disabe acknowledge of received data
//   // nack also generates stop condition after last byte received
//   // see reference manual for more info
//   I2C_AcknowledgeConfig(I2Cx, DISABLE);
//   I2C_GenerateSTOP(I2Cx, ENABLE);
//   // wait until one byte has been received
////   while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
//   // read data from I2C data register and return data byte
//   data = I2C_ReceiveData(I2Cx);
//   return data;
//}

////------------cac ham cho con pca9685 i2c dieu khien servo
//void PCA9685_Write(unsigned char reg,unsigned char data)
//{
//   I2C_start(I2C2,PCA9685_adrr,I2C_Direction_Transmitter);
//   I2C_write(I2C2,reg);
//   I2C_write(I2C2,data);
//   I2C_stop(I2C2);
//}


//u8 PCA9685_Read(unsigned char reg)
//{
//   uint8_t res;
//   I2C_start(I2C2,PCA9685_adrr,I2C_Direction_Transmitter);
//   I2C_write(I2C2,reg);
//   I2C_stop(I2C2);
//   I2C_start(I2C2,PCA9685_adrr|0X01,I2C_Direction_Receiver);
//   res = I2C_read_nack(I2C2);       
//   I2C_stop(I2C2);             
//   return res;  
//}

//void SetPWMFreq(uint8_t freq)
//{
//   uint8_t prescale,oldmode,newmode;
//   double prescaleval;
//   //prescaleval = 25000000.0/(4096.0*freq*0.915);
//   //prescale = (uint8_t)floor(prescaleval+0.5)-1;
//   
//   prescale = 110;	// (25000000.0/(4096.0*60*0.915) ) + 0.5 - 1

//   oldmode = PCA9685_Read(PCA9685_MODE1);
//   newmode = (oldmode&0x7F) | 0x10; // sleep , !PCA9685_MODE1_RESTART = !0x80 = 0x7F, PCA9685_MODE1_SLEEP = 0x10.
//   PCA9685_Write(PCA9685_MODE1, newmode); // go to sleep
//   PCA9685_Write(PCA9685_PRESCALE, prescale); // set the prescaler
//   PCA9685_Write(PCA9685_MODE1, oldmode);
//   PCA9685_Write(PCA9685_MODE1, oldmode | 0xA1); // 0xA1 == PCA9685_MODE1_RESTART = 0x80 & PCA9685_MODE1_AI = 0x20 & PCA9685_MODE1_ALLCALL = 0x01.
//}

//void SetPWM(u8 num, u16 on, u16 off) 
//{
//   PCA9685_Write(LED0_ON_L+4*num,on);
//   PCA9685_Write(LED0_ON_H+4*num,on>>8);
//   PCA9685_Write(LED0_OFF_L+4*num,off);
//   PCA9685_Write(LED0_OFF_H+4*num,off>>8);
//}

//uint16_t Calculate_PWM(u8 angle){
//   //return (uint16_t)(204.8*(0.5+angle*1.0/(out_max / 2.0)));	//50Hz
//   return (uint16_t)(245.85*(0.5+angle*1.0/(out_max / 2.0)));	//60Hz
//}

//void Init_PCA9685()
//{
//   PCA9685_Write(PCA9685_MODE1,0x00);

//   SetPWMFreq(60);	//60Hz
//}




//---------------------------- config out mode -----------------------------
void Config_out_mode(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOC, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = 0xffff;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 0xffff;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 0xffff;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 0xffff;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}
//---------------------------- config in mode -----------------------------
void Config_in_mode(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);
	/////////////////////////////////////line
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_2 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

	//============================ config timer6 1ms ===================================
void Config_int_time6(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler=50;//50
	TIM_TimeBaseInitStructure.TIM_Period=999;//999;//1ms
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);

	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM6, 0); 
	TIM_Cmd(TIM6,ENABLE);
}
	//================================= config timer7 1ms ================================
void Config_int_time7(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler=500;//500;
	TIM_TimeBaseInitStructure.TIM_Period=999;//999;//1ms
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);

	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM7, 0); 
	TIM_Cmd(TIM7,ENABLE);
}

//===================================PWM===========================================
void Config_pwm_time_t8(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);// ch1
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);// ch2
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);// ch3
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);// ch4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	//Fpwm=84000000/(TIM_Period*TIM_Prescaler);
	//50=84000000/(40*TIM_Prescaler);
	TIM_TimeBaseInitStructure.TIM_Prescaler=84000000/(2000*255); // fpwm=2khz
	TIM_TimeBaseInitStructure.TIM_Period=3000;
	//  TIM_TimeBaseInitStructure.TIM_Prescaler=186; // fpwm=4khz
	//  TIM_TimeBaseInitStructure.TIM_Period=10000;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV2;//TIM_CKD_DIV2;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStructure); 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_Cmd(TIM8,ENABLE);
	TIM_SetCounter(TIM8, 0);
	//////////////	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);	  
	TIM_CtrlPWMOutputs(TIM8,ENABLE);

}

//===================================PWM TIME 5===========================================
void Config_pwm_time_t5(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);// ch1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);// ch2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);// ch3
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);// ch4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

//	//Fpwm=84000000/(TIM_Period*TIM_Prescaler);
//	//50=84000000/(40*TIM_Prescaler);
	//TIM_TimeBaseInitStructure.TIM_Prescaler=84000000/(1000*255); // fpwm=2khz
	//TIM_TimeBaseInitStructure.TIM_Period=255;
	TIM_TimeBaseInitStructure.TIM_Prescaler=186; 
	TIM_TimeBaseInitStructure.TIM_Period=10000;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure); 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_Cmd(TIM5,ENABLE);
	TIM_SetCounter(TIM5, 0);
	//////////////	
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);	  
	TIM_CtrlPWMOutputs(TIM5,ENABLE);

}



//============================ config pwm timer4 ================================
void Config_pwm_time_t4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // ch1
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);// ch2
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);// ch3
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);  //ch4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*
	TIM_TimeBaseStructure.TIM_Period = 839800000; // 84Mhz/839800000 = 0.1 (Hz)
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // Maximum TIM5 clock: 84.000.000Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	*/
	TIM_TimeBaseInitStructure.TIM_Prescaler=84000000/(2000*255); // fpwm=2khz
	TIM_TimeBaseInitStructure.TIM_Period=3000;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV2;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_Cmd(TIM4,ENABLE);
	TIM_SetCounter(TIM4, 0);

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

}

//===========================PWM RC SECVOR==================================
void Config_pwm_time_t9(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);// ch2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);// ch1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=65535;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1|TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICFilter=200;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	TIM_EncoderInterfaceConfig(TIM9, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
	TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1,ENABLE); 	
	TIM_SetCounter(TIM9, 0); 

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	////
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);// ch2
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM9);// ch1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	//Fpwm=84000000/(TIM_Period*TIM_Prescaler);
//	//50=84000000/(40*TIM_Prescaler);
//	// 	TIM_TimeBaseInitStructure.TIM_Prescaler=84000000/(1000*255); // fpwm=2khz
//	//  TIM_TimeBaseInitStructure.TIM_Period=255;
//	TIM_TimeBaseInitStructure.TIM_Prescaler=186; 
//	TIM_TimeBaseInitStructure.TIM_Period=10000;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
//	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStructure); 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = 0;
//	//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC1Init(TIM9, &TIM_OCInitStructure);
//	TIM_OC2Init(TIM9, &TIM_OCInitStructure);
//	TIM_CtrlPWMOutputs(TIM9,ENABLE);
//	TIM_Cmd(TIM9,ENABLE);
//	TIM_SetCounter(TIM9, 0);
//	//	
//	//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // dir control driver
//	//  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;
//	//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	//  GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
//	//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	//  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//============================ config encoder timer2 && timer3 ===========================
void Config_encoder_timer2_timer3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2); // tim2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3); // tim3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//  GPIO_InitStructure.GPIO_Pin = ;
	//  GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=65535;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1|TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICFilter=20;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM2,ENABLE); 
	TIM_Cmd(TIM3,ENABLE);

	TIM_SetCounter(TIM2, 0); 
	TIM_SetCounter(TIM3, 0);

	////////////
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//========================== config encoder timer2 && timer3 ================================
void Config_encoder_timer1(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); // tim5
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	
	
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=65535;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1|TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICFilter=10;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);	
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM1,ENABLE); 	
	TIM_SetCounter(TIM1, 0); 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//---------------------------- KHAI BAO ENCODER TIMER 1-----------------------------
void Config_encoder_timer4(void)
{
 	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;	
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
  TIM_TimeBaseInitStructure.TIM_Period=65535;
  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
  
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // tim5
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	
  TIM_ICInitStructure.TIM_Channel=TIM_Channel_1|TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICFilter=200;
  TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);	
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4,ENABLE); 	
  TIM_SetCounter(TIM4, 10000); 
   ///////////////////////////
		TIM4->EGR = 0;           // Generate an update event
    TIM4->CR1 = 1;           // Enable the counter
   /////////////////
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//========================== config encoder timer2 && timer3 ================================
//---------------------- config encoder timer2 && timer3 ---------------------
//---------------------------- KHAI BAO ENCODER TIMER 1-----------------------------
void Config_encoder_timer5(void)
{
 	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
  TIM_TimeBaseInitStructure.TIM_Period=65535;
  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); // tim5
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

		
  TIM_ICInitStructure.TIM_Channel=TIM_Channel_1|TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICFilter=200;
  TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);	
  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5,ENABLE); 	
  TIM_SetCounter(TIM5, 10000); 
   ///////////////////////////
		TIM5->EGR = 0;           // Generate an update event
    TIM5->CR1 = 1;           // Enable the counter
   /////////////////
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
//------------------------------------ config ADC DMA ----------------------------- 
void Config_ADC1_DMA(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&_ADC1_Value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	//	config_ngat_dma();// cau hinh ngat dma phai dat sau lenh enable kenh dma.

	/* Configure ADC1 Channel7 pin as analog input ******************************/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//ADC_DMAAccessMode_1;//
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 8;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC3 regular channel7 configuration *************************************/
	#define sample_time_adc1 ADC_SampleTime_84Cycles
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, sample_time_adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, sample_time_adc1);//OK
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, sample_time_adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, sample_time_adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, sample_time_adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, sample_time_adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, sample_time_adc1);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, sample_time_adc1);
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, sample_time_adc1);
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, sample_time_adc1); // cac so rank la so thu tu chuyen doi adc 

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	//ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
	}
	//----------------------------------- config int uart1 ------------------------------
	void Config_int_uart1_rx(void)
	{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;// 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // muc do uu tien ngat thu 15
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// cho phep su dung ngat
	NVIC_Init(&NVIC_InitStructure);  	// cau hinh cho bang vector ngat

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void UART1_DMA_RX(u32 baudrate)
{		
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);

	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the UART4 RX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA2_Stream2);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RX_USART1;
	DMA_InitStructure.DMA_BufferSize = 10;//(uint16_t)sizeof(Buffer);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	DMA_Init(DMA2_Stream2, &DMA_InitStructure); 
	/* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); // Enable USART Rx DMA Request
	DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);  
	/* Enable the DMA RX Stream */
	DMA_Cmd(DMA2_Stream2, ENABLE);


}

//---------------------------- KHAI BAO UART 2-----------------------------
void UART2_DMA_TX(u32 baudrate)
{		
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
   /*-------------------------- GPIO Configuration ----------------------------*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

   /* Connect USART pins to AF */
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	

   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
   USART_Init(USART2, &USART_InitStructure); 
   USART_Cmd(USART2, ENABLE);


/////////////////////////////////
 /* Configure the Priority Group to 2 bits */
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   /* Enable the UART4 RX DMA Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
	 DMA_DeInit(DMA1_Stream6);
   DMA_InitStructure.DMA_Channel = DMA_Channel_4;
   DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Receive
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DATA_SPEED;
   DMA_InitStructure.DMA_BufferSize =(uint16_t)sizeof(DATA_SPEED);//(uint16_t)sizeof(Buffer_SPEED)-1 ;
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
   DMA_Init(DMA1_Stream6, &DMA_InitStructure); 
   /* Enable the USART Rx DMA request */
   USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); 
   /* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
   USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); // Enable USART Rx DMA Request
	 DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);  
   /* Enable the DMA RX Stream */
   DMA_Cmd(DMA1_Stream6, ENABLE);	

}

//============================================
void UART3_DMA_RX(u32 baudrate)
{		
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
   /*-------------------------- GPIO Configuration ----------------------------*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIO_InitStructure);
   /* Connect USART pins to AF */
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
   USART_Init(USART3, &USART_InitStructure); 
   USART_Cmd(USART3, ENABLE);

/////////////////////////////////////////////	   
   
   /* Configure the Priority Group to 2 bits */
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   /* Enable the UART4 RX DMA Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
	 DMA_DeInit(DMA1_Stream1);
   DMA_InitStructure.DMA_Channel = DMA_Channel_4;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)MANG_GAME;
   DMA_InitStructure.DMA_BufferSize = 8;//(uint16_t)sizeof(Buffer);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
   DMA_Init(DMA1_Stream1, &DMA_InitStructure); 
   /* Enable the USART Rx DMA request */
   USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); 
   /* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
   USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); // Enable USART Rx DMA Request
	 DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);  
   /* Enable the DMA RX Stream */
   DMA_Cmd(DMA1_Stream1, ENABLE);	
}

//============================UART4=======================================
void UART4_DMA_RX(u32 baudrate)
{		
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(UART4, &USART_InitStructure); 
	USART_Cmd(UART4, ENABLE);

	/////////////////////////////////////////////	   

	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the UART4 RX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Stream2);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RX_USART4;
	DMA_InitStructure.DMA_BufferSize = 1;//(uint16_t)sizeof(Buffer);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	DMA_Init(DMA1_Stream2, &DMA_InitStructure); 
	/* Enable the USART Rx DMA request */
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); 
	/* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); // Enable USART Rx DMA Request
	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);  
	/* Enable the DMA RX Stream */
	DMA_Cmd(DMA1_Stream2, ENABLE);	
}
//////////////////////////////
void UART6_DMA_RX(u32 baudrate)
{		
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
   /*-------------------------- GPIO Configuration ----------------------------*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   /* Connect USART pins to AF */
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

   USART_InitStructure.USART_BaudRate = baudrate;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
   USART_Init(USART6, &USART_InitStructure); 
   USART_Cmd(USART6, ENABLE);

   /* Configure the Priority Group to 2 bits */
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   /* Enable the UART4 RX DMA Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
	
	 DMA_DeInit(DMA2_Stream1);
   DMA_InitStructure.DMA_Channel = DMA_Channel_5;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
   DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DATA_COLOR;
   DMA_InitStructure.DMA_BufferSize =1;//(uint16_t)sizeof(DATA_COLOR);
   DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
   DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
   DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
   DMA_Init(DMA2_Stream1, &DMA_InitStructure); 
   /* Enable DMA Stream Half Transfer and Transfer Complete interrupt */
   USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE); // Enable USART Rx DMA Request
//	 DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);  
   /* Enable the DMA RX Stream */
   DMA_Cmd(DMA2_Stream1, ENABLE);
	 

}
//========================================================================
void UART5_DMA_TX(u32 baudrate)
{		
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC, ENABLE);
	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect USART pins to AF */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the UART8 TX DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the UART8 RX DMA Interrupt */
	//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//  NVIC_Init(&NVIC_InitStructure);
	///////////////////////////

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
	USART_Init(UART5, &USART_InitStructure); 
	USART_Cmd(UART5, ENABLE);

	/////////////////////////////////////////////	   



	// UART8_TX DMA Channel 5, DMA1, Stream0

	DMA_DeInit(DMA1_Stream7);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Buffer_HMI;
	DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(Buffer_HMI)-1 ;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream7, &DMA_InitStructure);

	/* Enable the USART Tx DMA request */
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);

	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE);

	/* Enable the DMA TX Stream */

	DMA_Cmd(DMA1_Stream7, ENABLE);

	////////////////////////////////////////////////////////////////////////////////
	// UART8_RX DMA Channel 5, DMA1, Stream6

	//  DMA_DeInit(DMA1_Stream0);

	//  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
	//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Buffer_HMI;
	//  DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(Buffer_HMI) - 1;
	//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART5->DR;
	//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	//  DMA_Init(DMA1_Stream0, &DMA_InitStructure);

	//  /* Enable the USART Rx DMA request */
	//  USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);

	//  /* Enable DMA Stream Transfer Complete interrupt */
	//  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);

	//  /* Enable the DMA RX Stream */
	//  DMA_Cmd(DMA1_Stream0, ENABLE);

}

//================================INT=========================================
void ngat_ngoai(void)   
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PB5 PB6 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Connect EXTI Line5, Line6 to PB5,PB6 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);
	//   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);

	/* Configure EXTI Line5, Line6 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line5, Line6 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn  ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//------------------------------ send uart1 ------------------------

void SEND_UART(u8 _UART,u8 _GIA_TRI)
{
	if(_UART==1) 	  {  USART_SendData(USART1, (uint8_t) _GIA_TRI); 	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){} }
	else if(_UART==3) {  USART_SendData(USART3, (uint8_t) _GIA_TRI); 	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){} }
	else if(_UART==4) {  USART_SendData(UART4, (uint8_t) _GIA_TRI); 	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET){} }
	else if(_UART==5) {  USART_SendData(UART5, (uint8_t) _GIA_TRI); 	while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET){} }

}
//--------------------------- reset encoder doc -------------------------




void RESET_ENCODER_CHAY(void)
{
	TIM1->CNT = 0;num_over_t1 = 0;
	TIM4->CNT = 0;num_over_t4 = 0;
}
void RESET_ENCODER(void)
{
	TIM1->CNT = 0;num_over_t1 = 0;
	TIM2->CNT = 0;num_over_t2 = 0;
	TIM3->CNT = 0;num_over_t3 = 0;
	TIM4->CNT = 0;num_over_t4 = 0;
}
void RESET_ENCODER_XOAY(void)
{
	TIM2->CNT = 0;num_over_t2 = 0;
}

void RESET_ENCODER_BAN(void)
{
	TIM3->CNT = 0;num_over_t3 = 0;
}

//-------------------------------- reset uart-----------------------------------------
void run_read_gyro_uart1(void)
{ 
	u32 i;
	en_gyro=0;
	for(i=0;i<2;i++)
	{
		SEND_UART(1,'a');
		delay_ms(50);
	}
	en_gyro=1;
	data_tx_gyro='z';
}

void run_read_gyro_uart3(void)
{ 
	u32 i;
	enGyro=0;
	for(i=0;i<2;i++)
	{
		SEND_UART(3,'a');
		delay_ms(50);
	}
	enGyro=1;
	dataTxGyro='z';
}
///////======================================================================================

//void HMI_TRAN(vs32 _so_dong)
//{
//	vs32 _i,_chua_cac_bit=0;
//	char _ghep_bit[40];
//	char _chu_cac_bit[40];
//	_ghep_bit[0]=0;
//	_chu_cac_bit[0]=0;
//	
//switch (_so_dong) {
//		case 0:
//			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){
//				GPIO_WriteBit(GPIOC,GPIO_Pin_15,0);
//				HMI_DMI("ROBOT DANG CHAY SAN XANH:   ",	GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_15),0);
//			}
//			else{
//				GPIO_WriteBit(GPIOC,GPIO_Pin_15,1);
//				HMI_DMI("ROBOT DANG CHAY SAN DO:   ",	GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_15),0);
//			}		
//			break;
//		case 1:
//			HMI_DMI("IMU:..............", _robotIMUAngle,1);
//			break;
//		case 2:
//			HMI_DMI("EN_TRAI:..........",ENCODER_T(),2);
//			break;
//		case 3:
//			HMI_DMI("EN_PHAI:..........",ENCODER(),3);
//			break;
//		case 4:
//			HMI_DMI("EN_TONG:..........",ENCODER_TONG(),4);
//			break;
//		case 5:
//			HMI_DMI("GAME_PAD:.........", MANG_GAME[0],5);	
//			break;
//		case 6:
//			HMI_DMI("LAZER_PHAI:.........", lazePhaiValue,6);
//			break;
//		case 7:
//			HMI_DMI("LAZER_TRAI:.......",lazeTraiValue,7);
//			break;
//		case 8:
//			HMI_DMI("LAZER_SAU:........", lazeSauValue,8); //ADC 1;
//			break;
//		case 9:
//			HMI_DMI("VI TRI:...........", Vi_tri,9);  
//			break;
//		case 10:
//			HMI_DMI("VTRETRY...",Vi_tri,10);
//			break;
//		case 11:
//		//	strcat(_chu_cac_bit,"1");
//			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15));		
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2));	
//			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0));		
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1));	
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2));		
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14));	
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15));		
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10));	
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11));		
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12));	
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13));		
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14));	
////			strcat(_chu_cac_bit,_ghep_bit);
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15));		
////			strcat(_chu_cac_bit,_ghep_bit);		
////			sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10));	
////			strcat(_chu_cac_bit,_ghep_bit);
//			HMI_PUTS("IN: ",_chu_cac_bit,11);
//			break;
//		case 12:
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_10));		
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_3));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_4));		
//			strcat(_chu_cac_bit,_ghep_bit);		
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_5));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_6));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_7));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_0));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_1));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_2));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_4));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_5));	
//			strcat(_chu_cac_bit,_ghep_bit);
//			HMI_PUTS("OUT:",_chu_cac_bit,12);
//			break;	
////		case 13:
////			sprintf(_ghep_bit,"%d",LINE_0);	
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_1);		
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_2);		
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_3);		
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_4);		
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_5);		
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_6);		
////			strcat(_chu_cac_bit,_ghep_bit);
////			strcat(_chu_cac_bit," ");
////			sprintf(_ghep_bit,"%d",LINE_7);		
////			strcat(_chu_cac_bit,_ghep_bit);			
////			HMI_PUTS("LINE:",_chu_cac_bit,13);
////			break;		
//		case 14:
//			HMI_DMI("value:..............",bien_hmi,13);
//			break;	
//		case 15:
//			HMI_PUT_j(_ADC1_Value[4]/41);
//			break;			
//		//default:
//		//	phai xoa chuoi
//	}
//}
///
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//---------------------------- TRUYEN DU LIEU VAO MANG-----------------------------
void HMI_TRAN(vs32 _so_dong)
{
		char*camera="11111111111111111111110111111111";
									vs32 _i,_chua_cac_bit=0;
									char _ghep_bit[40];
									char _chu_cac_bit[40];
									_ghep_bit[0]=0;
									_chu_cac_bit[0]=0;
							switch (_so_dong) {
										case 0:
										if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)){
											GPIO_WriteBit(GPIOC,GPIO_Pin_15,0);
											HMI_DMI("ROBOT 2 DANG CHAY SAN XANH:   ",	GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_15),0);
										}
										else{
											GPIO_WriteBit(GPIOC,GPIO_Pin_15,1);
											HMI_DMI("ROBOT 2 DANG CHAY SAN DO:   ",	GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_15),0);
										}		
										
										break;
									case 1:
										HMI_DMI("IMU:", _robotIMUAngle,1);
										break;
									case 2:
										HMI_DMI("Camera:",Cam,2);
										break;
									case 3:
										HMI_DMI("LAZER_TRAI:",lazeTraiValue,3);
										break;
									case 4:
										HMI_DMI("LAZER_PHAI:",lazePhaiValue,4);   
										break;
									case 5:
										HMI_DMI("BIEN TRO MAM:",bien_tro_xoay_mam,5);	
										break;
									case 6:
										HMI_DMI("BIEN TRO Y:",bien_tro_nang_trucY	,6);
										break;
									case 7:
										HMI_DMI(" DATA_COLOR[0]:", DATA_COLOR[0],7);
										break;
									case 8:
										HMI_DMI("EN_FR:", ENCODER_FR(),10); //ADC 1;
										break;
									case 9:
										HMI_DMI("EN_FL:",ENCODER_FL(),9);
										break;
//										if(NUT_CHUYEN_SAN == 1){
//											HMI_DMI("NUT CHUYEN SAN: DO",NUT_CHUYEN_SAN,9);
//										}
//										else{
//											HMI_DMI("NUT CHUYEN SAN: XANH",NUT_CHUYEN_SAN,9);
//										}
										break;
									case 10:
										HMI_DMI("GAME_PAD:",GP_BTN[0],8);
										break;
									case 11:
										//HMI_DMI("TEST1 :",0,11);  					
										break;
									case 12:
										//HMI_DMI("TEST2:",0,12);  						
										break;
									case 13:
										//HMI_DMI("CTHT_BONG_LEN :",CTHT_BONG_LEN,13);   
										break;
									case 14:
										//HMI_DMI("CB_TAY_BAN:",CAM_BIEN_TU_TAY_BAN,14);  
										break;		
									case 15:
										//HMI_DMI("TESSSSt:",servo_Sau_Trai,15); 
										break;	
									case 16:
									//	hien thi phan ngo vao
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0));	// CTHT TAY HA TAY BAN
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1));	// NUT NHAN TRAI
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2));		// NUT NHAN TRAI
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3));	// CTHT XILANH CO
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5));	//cam bien quang nhan vong
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6));	//cam bien tu nong ban	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)); /// CAM BIEN TU	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8));	// CTHT XILANH DUOI	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10));	  
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12));		
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14));		
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));		
										strcat(_chu_cac_bit,_ghep_bit);		
										sprintf(_ghep_bit,"%d",GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14));	
    								strcat(_chu_cac_bit,_ghep_bit);
//										sprintf(_ghep_bit,"%d",cb_dung_chan_da);	
//										strcat(_chu_cac_bit,_ghep_bit);

										HMI_PUTS("I:",_chu_cac_bit,16);
										break;
									case 17:
										// hien thi phan ngo ra
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7));		
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_8));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_9));		
										strcat(_chu_cac_bit,_ghep_bit);		
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_10));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_11));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_14));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_15));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_10));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_14));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_11));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12));	
										strcat(_chu_cac_bit,_ghep_bit);
										sprintf(_ghep_bit,"%d",GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6));	
										strcat(_chu_cac_bit,_ghep_bit);
										HMI_PUTS("OUT:",_chu_cac_bit,17);
										break;	
								
						}
}

//////////////////////////
void DATA_SPEED_RUN_LOOP()///TOC DO <254
	{int _k,_i;

	DMA_Cmd(DMA1_Stream6,ENABLE);
						
		}
	////////////////////////////////

void HMI_RUN_LOOP(vs32 _cause)
{
	if(HMI_LOOP>_cause)HMI_LOOP=0;
	else HMI_LOOP++;
	HMI_TRAN(HMI_LOOP);
}

