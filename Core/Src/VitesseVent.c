#include "main.h"
#include <string.h>
#include <stdio.h>
#include "VitesseVent.h"

float frequency = 0;
volatile uint16_t diffCapture = 0;
float Vitesse_vent = 0;
char AffichageVitesseVent[40];

extern uint8_t captureFlag = 1;
extern uint8_t captureDone = 0;
extern uint16_t captures[2] = {0, 0};








/*void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)

{
      if (captureFlag)
      {
          captures[1]=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
          captureFlag = 0;
          captureDone = 1;
      }
      else
      {
          captures[1]=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
          captureFlag = 1;
      }
}*/
