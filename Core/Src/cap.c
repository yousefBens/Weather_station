/*
 * cap.c
 *
 *  Created on: Nov 22, 2023
 *      Author: idira
 */
#include "cap.h"
#include "main.h"
#include "stdio.h"
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

volatile uint8_t cinq_impulsions=0;
static volatile uint32_t impulseCount = 0;
static volatile uint8_t pluieDetected = 0;

void cap(void) {
  // Ajoutez ici l'initialisation nécessaire, si nécessaire.
}

void cap_start(void) {
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
}

void cap_stop(void) {
  HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
}

uint32_t cap_get_impulse_count(void) {
  return impulseCount;
}

void cap_reset_impulse_count(void) {
  impulseCount = 0;
}

float cap_calculate_rainfall(void) {
  return impulseCount * 0.279;
}


float Get_impulse_pluie()
 {
		//int impulseCount = 0;
	  if (pluieDetected == 1) {

		  	//cinq_impulsions++;
		    impulseCount++;

		    //printf("pluie detected  %d \r \n ", impulseCount  );

		    pluieDetected = 0;
	  }



	  /*if(cinq_impulsions==5){

		  float quantPluie = impulseCount * 0.279;
		  return quantPluie;

		  impulseCount = 0;
		  cinq_impulsions=0;

	  }*/
	  return impulseCount;
 }


PUTCHAR_PROTOTYPE {
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
  return ch;
}


