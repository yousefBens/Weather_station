/*
 * cap.h
 *
 *  Created on: Nov 22, 2023
 *      Author: idira
 */

#ifndef INC_CAP_H_
#define INC_CAP_H_

#include "main.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include <stdint.h>

void cap(void);
void cap_start(void);
void cap_stop(void);
uint32_t cap_get_impulse_count(void);
void cap_reset_impulse_count(void);
float cap_calculate_rainfall(void);
void affiche();

#endif /* INC_CAP_H_ */
