#include "stm32f10x.h"
#include "Init.h"



void Init()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//72 MHz clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//36 MHz clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  
/********************************Настройка GPIO *********************/  
  GPIO_DeInit(GPIOA);
  GPIO_InitTypeDef GPIO_InitStruct;
  
  //для TIMER1
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//ШИМ
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  //тестовый
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//тестовый
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  //для запуска огибающей
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
/********************************Настройка Timer1 для ШИМ**********************/
  TIM_DeInit(TIM1);//clock 72MHz
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = ARR;//500 kHz
  TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
  
  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  
  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct.TIM_Pulse = 0;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
  TIM_OC1Init(TIM1, &TIM_OCInitStruct);
  //TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_OC1PreloadConfig(TIM1,   TIM_OCPreload_Disable);

  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  //TIM_DMAConfig(TIM1, TIM_DMABase_CCR1, TIM_DMABurstLength_1Transfer);
  TIM_DMACmd(TIM1, TIM_DMA_CC1, DISABLE);
  TIM_Cmd(TIM1, DISABLE);
  
  /********************************Настройка Timer2***************************/
  TIM_DeInit(TIM2);
  //36 MHz clock
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 0xffff;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
  
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  TIM_Cmd(TIM2, ENABLE);
  
  /********************************Настройка DMA******************************/
  DMA_DeInit(DMA1_Channel2);
  DMA_InitTypeDef DMA_InitStruct;

  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&TIM1->CCR1;
  DMA_InitStruct.DMA_MemoryBaseAddr = 0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_BufferSize = 0;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;  
  
  DMA_Init(DMA1_Channel2, &DMA_InitStruct);

  /********************************Настройка NVIC******************************/
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
}