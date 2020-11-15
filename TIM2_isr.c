#include "stm32f10x.h"
#include "Init.h"
extern TypeDefADSR ADSR;



void TIM2_IRQHandler(void)
{
  static int8_t state = 0;
  static int16_t cnt = -1;
  static uint16_t *p;
  
  GPIO_SetBits(GPIOA, GPIO_Pin_9);
  
  if (cnt < 0)
  {
    switch(state)
    {
    case ATTACK: 
      TIM_Cmd(TIM2, DISABLE);
      TIM_SetAutoreload(TIM2, ADSR.attack_time);
      TIM2->CNT = 0;
      cnt = ADSR.attack_data_size-1;
      p = ADSR.attack_data;
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      TIM_Cmd(TIM2, ENABLE);
      break;
    case DECAY: 
      TIM_Cmd(TIM2, DISABLE);
      TIM_SetAutoreload(TIM2, ADSR.decay_time);
      TIM2->CNT = 0;
      cnt = ADSR.decay_data_size-1;
      p = ADSR.decay_data;
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      TIM_Cmd(TIM2, ENABLE);
      break;
    case SUSTAIN: 
      TIM_Cmd(TIM2, DISABLE);
      TIM_SetAutoreload(TIM2, ADSR.sustain_time);
      TIM2->CNT = 0;
      cnt = ADSR.sustain_data_size-1;
      p = ADSR.sustain_data;
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      TIM_Cmd(TIM2, ENABLE);
      break;  
    case RELEASE: 
      TIM_Cmd(TIM2, DISABLE);
      TIM_SetAutoreload(TIM2, ADSR.release_time);
      TIM2->CNT = 0;
      cnt = ADSR.release_data_size-1;
      p = ADSR.release_data;
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      TIM_Cmd(TIM2, ENABLE);
      break;
    default:
      break;
    }
  }

  TIM1->CCR1 = *p;
  p++;
  cnt--;

  if (cnt < 0)
  {
    if (state == RELEASE)
      state = ATTACK;
    else
      state++;
  }

 GPIO_ResetBits(GPIOA, GPIO_Pin_9);
 TIM_ClearFlag(TIM2, TIM_FLAG_Update);

}
