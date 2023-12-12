/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : FMC.h
  * Description        : This file provides code for the configuration
  *                      of the FMC peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ECRAN_H
#define __ECRAN_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

//extern SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

 void init_page(float* T, float* H, float* P, char** D, float* V, float* Pl, int taille);
 void AfficherCase(char *titre, uint32_t valeur, uint16_t posX, uint16_t posY);
 void AfficherCaseD(char *titre, char valeur, uint16_t posX, uint16_t posY);
 void AfficherBouton(char *texte, uint16_t posX, uint16_t posY);
 void DessinerAxeX(void);
 void DessinerAxeXD(void);
 void DessinerAxeY(void);
 void DessinerGraphe(float* y);
 void DessinerGrapheP(float* y);
 void DessinerGrapheD(char** y);
 void DessinerGrapheH(float* y);
 void ChangerDePage(float* T, float* H, float* P, char** D, float* V, float* Pl, int taille);


/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ECRAN_H */

/**
  * @}
  */

/**
  * @}
  */
