#include "stm32f10x.h"
#include "Init.h"
#include "math.h"
TypeDefADSR ADSR;
uint16_t attack_data[256];
uint16_t decay_data[256];
uint16_t sustain_data[256];
uint16_t release_data[256];
uint16_t *p;
uint32_t test;

void main()
{

Init();

for (int i=0;i<ARR;i++)
  attack_data[i] = i*i/144;

p = attack_data;
test = (uint32_t)p;

for (int i=0;i<ARR/2;i++)
  decay_data[i] = ARR-i;

for (int i=0;i<ARR;i++)
  sustain_data[i] = ARR/2 + (int)(sinf((float)(i)/5)*10);

for (int i=0;i<ARR/2;i++)
  release_data[i] = ARR/2 - i*i/72;

ADSR.attack_data = attack_data;
ADSR.decay_data = decay_data;
ADSR.sustain_data = sustain_data;
ADSR.release_data = release_data;

ADSR.attack_data_size = ARR;
ADSR.decay_data_size = ARR/2;
ADSR.sustain_data_size = ARR;
ADSR.release_data_size = ARR/2;

ADSR.attack_time = 750*2;
ADSR.decay_time = 188*2;
ADSR.sustain_time = 1250*2;
ADSR.release_time = 1500*2;

DMA_Cmd(DMA1_Channel2, DISABLE);

TIM_Cmd(TIM2, DISABLE);
TIM_SetAutoreload(TIM2,1);
TIM2->CNT = 0;
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
TIM_Cmd(TIM2, ENABLE);
TIM_Cmd(TIM1, ENABLE);
while(1)
{
//  if ((DMA1_Channel2->CCR & DMA_CCR2_EN) == DMA_CCR2_EN)
//      while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
//  DMA_Cmd(DMA1_Channel2, DISABLE);
//  TIM_Cmd(TIM1, DISABLE);
//  DMA_ClearFlag(DMA1_FLAG_TC2);
//  DMA_SetCurrDataCounter(DMA1_Channel2, ADSR.attack_data_size); 
//  DMA1_Channel2->CMAR = (uint32_t)ADSR.attack_data;
//  DMA_Cmd(DMA1_Channel2, ENABLE);
//  TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
//  TIM_Cmd(TIM1, ENABLE);
  
  
}
  
}