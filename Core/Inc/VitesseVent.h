#ifndef VITESSE_VENT
#define INC_VENT_DIR_VIT_H_

#include "stdio.h"
#include "main.h"
#include "math.h"
#include "usart.h"
#include "string.h"
#include "tim.h"

//extern ADC_HandleTypeDef hadc3;




void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
float MesureVitesseVent();


#endif /* INC_VENT_DIR_VIT_H_ */
