#include "stm32f10x.h"
#include "Init.h"
extern TypeDefADSR ADSR;

void EXTI15_10_IRQHandler(void)
{
//rising
  if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == Bit_SET)
    {
      ADSR.state = ATTACK;
      TIM_SetAutoreload(TIM2, 2);
      TIM2->CNT = 0;
      TIM_Cmd(TIM2, ENABLE);
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
      ADSR.flag = 0;
    }
//falling
  else
    {
      ADSR.state = RELEASE;
      
    }
    


EXTI_ClearFlag(EXTI_Line12);
}