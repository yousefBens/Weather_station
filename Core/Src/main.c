/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "i2c.h"
#include "ltdc.h"
#include "rtc.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "capt_direction.h"
#include "H_T_Capt.h"
#include "Ecran.h"
#include "pression.h"
#include "SD_stockage.h"
#include "VitesseVent.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"
#include "controle_led_rgb.h"

/* USER CODE END Includes */


/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

extern UART_HandleTypeDef huart1;
// Variable qui pointe vers la page actuelle
extern enum Page currentPage;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// 	Pour définir les pages qu'on a
enum Page {
  PAGE_1,
  PAGE_2,
  PAGE_3,
  PAGE_4,
  PAGE_5,
  PAGE_6,
  PAGE_7,

};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Les flags du boton et timer 6 et timer 7
uint8_t boton=0;
uint8_t tim6_inter;
uint8_t tim7_inter;

uint16_t x, y;

// Des variables pour calculer le nombre des impulsion des capteurs de pluie et de vitesse
volatile uint32_t impulseCount = 0;
volatile uint8_t pluieDetected = 0;
volatile uint8_t captureFlag = 1;
volatile uint8_t captureDone = 0;
volatile uint16_t captures[2] = {0, 0};
float frequency = 0;
volatile uint16_t diffCapture = 0;
float Vitesse_vent = 0;
char AffichageVitesseVent[40];

// On a définit logarithme pour normaliser les valeurs du capteur de vitesse pour les afficher dans le graphe
// Sinon si nous trouvons des valeurs de vitesse très élevées cela dépassera le nombre de pixels de notre écran
double ln = log( M_E );

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

float MesureVitesseVent(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


stmdev_ctx_t dev_ctx;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	// La taille des tableaux initialiser par 0 si on stocker une valeur la taille va augmenter par un
	int taille = 0;

	// Les tableaux de taille 24 pour stocker les mesures
	  float* T;
	  float* H;
	  char** D;
	  float* P;
	  float* V;
	  float* Pl;
	  char heures[24][6] = {"00:00", "01:00", "02:00", "03:00", "04:00", "05:00", "06:00", "07:00", "08:00", "09:00", "10:00", "11:00",
	                        "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00", "19:00", "20:00", "21:00", "22:00", "23:00"};
	  char temp[20] = "No where";
	  T = (float*)malloc(24 * sizeof(float));
	  H = (float*)malloc(24 * sizeof(float));
	  D = (char**)malloc(24 * sizeof(char*));
	  P = (float*)malloc(24 * sizeof(float));
	  V = (float*)malloc(24 * sizeof(float));
	  Pl = (float*)malloc(24 * sizeof(float));

	  // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
          T[i] = 0.0;
      }
      // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
          H[i] = 30.0;
      }
      // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
    	  //char temp[20] = "No where";
          D[i] = (char*)malloc(50 * sizeof(char));
          strcpy(D[i], temp);
      }

      // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
           P[i] = 0.0;
      }
      // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
          V[i] = 0.0;
      }
      float* Va = (float*)malloc(24 * sizeof(float));

      // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
          Va[i] = 0.0;
      }
      // Remplir le tableau avec 24 valeurs initial
      for (int i = 0; i < 24; ++i) {
          Pl[i] = 0.0;
      }

      // Variables où nous récupérerons les mesures à chaque fois pour les stocker dans des tableaux
	  float v = 0.0;
	  float va = 0.0;
	  float Te = 5;
	  float Hu;
	  char* De;
	  float Pr;
	  float pluie;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC3_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_LTDC_Init();
  MX_SDMMC1_SD_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  MX_DMA_Init();
  //MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  BSP_LCD_Init();
  MX_TIM7_Init();

  // start timer 7
  HAL_TIM_Base_Start_IT(&htim7);
  // start timer 6
  HAL_TIM_Base_Start_IT(&htim6);
  // start timer 2
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  // start timer 1
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);


  // initialiser l'ecran
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
  BSP_LCD_SetLayerVisible(LTDC_ACTIVE_LAYER, ENABLE);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
  BSP_LCD_Clear(LCD_COLOR_WHITE);



  /* USER CODE END 2 */



  /* Infinite loop */

  // La taille est égale à 1 car on va afficher les valeurs initiales
  taille = 1;
  // Afficher la page d'accueil
  init_page(T, H, P, D, V, Pl, taille);
   // taille à 0 pour faire 24 mesures et les afficher dans l'accueil et dans les graphes
  taille = 0;
  // Counter de timer 7 revient à 0
  __HAL_TIM_SET_COUNTER(&htim7, 0);



  /* USER CODE BEGIN WHILE */
  while (1)
  {
	    /* USER CODE END WHILE */

	  // récupérer la mesure du vitesse
	  // On a utilisé deux variables : "va" pour la page d'accueil c'est la valeur réelle
	  // et "v" pour le graphique c'est la valeur normalisée(adapté pour l'affichage en courbe).
	  va = MesureVitesseVent();
	  v = MesureVitesseVent();

	  // Si v est compris entre 0 et 1 alors il restera le même
	  // Sinon, il return log(v)
	  // On a fait ca car log n'est pas définie sur 0 et ilest negatif entre 0 et 1
	  if(v == 0){
		  v = 0;
	  }else{
		  if((v<=1) && (v>0)){
			  v = v;
		  }else{
			  v = log(v);
		  }

	  }

	  //sprintf(AffichageVitesseVent, "Vitesse du vent: %.2f km/h\n\r", v);
	  //HAL_Delay(300);
	  //HAL_UART_Transmit(&huart1, (uint8_t*) AffichageVitesseVent, strlen(AffichageVitesseVent), HAL_MAX_DELAY);

	  // Pour detecter le nombres des impulsion envoyer par le capteur de pluie
	  if (pluieDetected == 1) {


		  IndicationEtatsParLedRGB(PriseDeMesure);

		    impulseCount++;

		    //printf("pluie detected  %d \r \n ", impulseCount  );

		    pluieDetected = 0;
			  IndicationEtatsParLedRGB(ExtinctionRGB);

	  }

	  // Flag de timer 6 pour récupérer et afficher les mesures

		  if(tim6_inter == 1){
			  sprintf(AffichageVitesseVent, "Tim6\n\r");

			  HAL_UART_Transmit(&huart1, (uint8_t*) AffichageVitesseVent, strlen(AffichageVitesseVent), HAL_MAX_DELAY);
			  // Si les tableux sont plien alors on va les stocker dans la carte SD
			  // et en vba initialiser les tableaux autre fois pour met des nouvaux mesures
			  if(taille == 24){
				  // Stockages des données
				  if(SaveDataToCSV(heures, T, P, H, D, V, Pl) !=  FR_OK ) Error_Handler();
				  // taille revient à 0
				  taille = 0;

				  // initialiser les tableaux
			      for (int i = 0; i < 24; ++i) {
			          T[i] = 0.0;
			      }
			      for (int i = 0; i < 24; ++i) {
			          H[i] = 30.0;
			      }
			      for (int i = 0; i < 24; ++i) {
			    	  //char tempp[20] = "No where";

			          strcpy(D[i], temp);
			      }
			      for (int i = 0; i < 24; ++i) {
			          P[i] = 930.0;
			      }
			      for (int i = 0; i < 24; ++i) {
			          Pl[i] = 0.0;
			      }
			      for (int i = 0; i < 24; ++i) {
			          V[i] = 0.0;
			      }
			      for (int i = 0; i < 24; ++i) {
			          Va[i] = 0.0;
			      }

			  }
			  // alumer RGB
			  IndicationEtatsParLedRGB(PriseDeMesure);
			  // prend les mesures
			  Te = Temperature();
			  Hu = Humedite();
			  Pr = MesurePression();
			  De = Get_direction_vent();
			  IndicationEtatsParLedRGB(ExtinctionRGB);
			  pluie = impulseCount * 0.279;

			  // le nombre des impulsion revient à 0 pour une nouvel prochaine mesure
			  impulseCount = 0;
			  // Stocker les valeurs dans Les tableaux
			  Pl[taille] = pluie;
			  T[taille] = Te;
			  H[taille] = Hu;
			  V[taille] = v;
			  Va[taille] = va;

			  strcpy(D[taille], De);

			  P[taille] = Pr;
			  // la taille augmenter par un
			  taille ++;
			  // Si on est dans la pages d'accueil faire update a la page
			  if(currentPage == PAGE_1){
				  init_page(T, H, P, D, Va, Pl, taille);
			  }



			  // Flag timer 6 revient à 0
			  tim6_inter = 0;
		  }


		  // Flag boton pour circuler dans les pages
		    if (boton == 1) {
		    	// Si on appuyer sur le boton LCD va Display On et le timer 7 va se met a 0
		    	// Timer 7 qui gére le mode veille de l'ecran
		    	BSP_LCD_DisplayOn();
		    	__HAL_TIM_SET_COUNTER(&htim7, 0);
		      // Changer de page en réponse au clic sur le bouton
		      ChangerDePage(T, H, P, D, V, Pl, taille);

		      // Flag du boton revient à 0
		      boton = 0;

		    }

		    // Flag Timer 7 qui gére le mode veille de l'ecran
		    if(tim7_inter == 1){
		    	// Display Off L'ecran et enrer dans le mode Sleep
				  sprintf(AffichageVitesseVent, "Tim7\n\r");

				  HAL_UART_Transmit(&huart1, (uint8_t*) AffichageVitesseVent, strlen(AffichageVitesseVent), HAL_MAX_DELAY);
		    	BSP_LCD_DisplayOff();
		    	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

		    	// Flag timer 7 revient à 0
		    	tim7_inter = 0;
		    }
		    if((tim7_inter == 0) && (boton == 0) && (tim6_inter == 0)){
				   //Mise en someil sinon

				  //HAL_SuspendTick();
				  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		    }


	    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 210;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// Calculer le fréquence du timer de vitesse

float MesureVitesseVent(void)
{
	Vitesse_vent = 0;

    if (captureDone)
    {
          if(captures[1]>=captures[0])

          {
              diffCapture = (captures[1] - captures[0]);
          }
          else
          {
              diffCapture = (TIM1->ARR + captures[0]) - captures[1];
          }

          frequency =  (float)((2*HAL_RCC_GetPCLK1Freq()) / ((TIM1->PSC + 1) * diffCapture));
          Vitesse_vent=frequency*2.4;
          captureFlag=1;
          captureDone=0;
          //sprintf(AffichageVitesseVent, "Vitesse du vent: %.2f km/h\n\r", Vitesse_vent);
          //HAL_UART_Transmit(&huart1, (uint8_t*) AffichageVitesseVent, strlen(AffichageVitesseVent), HAL_MAX_DELAY);

    }
    return Vitesse_vent;
}

// Gérer Les flages des Timer 1 et Timer 2

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {


	if(htim->Instance == TIM2) {

		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
			pluieDetected=1;
		}
		}

    if (captureFlag)
    {
        captures[1]=HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
        captureFlag = 0;
        captureDone = 1;
    }
    else
    {
        captures[1]=HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
        captureFlag = 1;
    }

}



/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  /*while (1)
  {
  }*/
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
