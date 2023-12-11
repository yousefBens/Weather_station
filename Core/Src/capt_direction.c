#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include <string.h>



uint16_t GET_Asignal_direction_sensor(){
	uint16_t adc;
	HAL_ADC_Start(&hadc3);
	adc = HAL_ADC_GetValue(&hadc3);
	return adc;
}

void direction_vent(){
	uint16_t signal = GET_Asignal_direction_sensor();
	uint8_t msg[20];
	if((signal < 768)&&(signal > 256)){
		strcpy((char*)msg, "Nord-Est\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;

	}
	if((signal < 1280)&&(signal > 768)){
		strcpy((char*)msg, "Est\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
	if((signal < 1792)&&(signal > 1280)){
		strcpy((char*)msg, "Sud-Est\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
	if((signal < 2304)&&(signal > 1792)){
		strcpy((char*)msg, "Sud\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
	if((signal < 2816)&&(signal > 2304)){
		strcpy((char*)msg, "Sud-Ouest\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
	if((signal < 3328)&&(signal > 2816)){
		strcpy((char*)msg, "Ouest\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
	if((signal < 3840)&&(signal > 3328)){
		strcpy((char*)msg, "Nord\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
	if((signal < 4095)&&(signal > 3840)){
		strcpy((char*)msg, "Nord-Ouest\r\n");
		HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		return;
	}
}

char* Get_direction_vent(){
	uint16_t signal = GET_Asignal_direction_sensor();
	char* msg;
	if((signal <= 768)&&(signal > 256)){
		strcpy((char*)msg, "Nord-Est");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);

	}
	if((signal <= 1280)&&(signal > 768)){
		strcpy((char*)msg, "Est");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	if((signal <= 1792)&&(signal > 1280)){
		strcpy((char*)msg, "Sud-Est");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	if((signal <= 2304)&&(signal > 1792)){
		strcpy((char*)msg, "Sud");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	if((signal <= 2816)&&(signal > 2304)){
		strcpy((char*)msg, "Sud-Ouest");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	if((signal <= 3328)&&(signal > 2816)){
		strcpy((char*)msg, "Ouest");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	if((signal <= 3840)&&(signal > 3328)){
		strcpy((char*)msg, "Nord");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	if((signal <= 4095)&&(signal > 3840)){
		strcpy((char*)msg, "Nord-Ouest");
		//HAL_UART_Transmit(&huart1, msg, strlen((char*)msg), HAL_MAX_DELAY);
		//return;
	}
	return msg;
}

