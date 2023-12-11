#include "main.h"
#include "dma2d.h"
#include "ltdc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"


// Pour circuler dans les pages
enum Page {
  PAGE_1,
  PAGE_2,
  PAGE_3,
  PAGE_4,
  PAGE_5,
  PAGE_6,
  PAGE_7,

};


enum Page currentPage = PAGE_1;






// Fonction pour afficher une case avec titre et variable
void AfficherCase(char *titre, float valeur, uint16_t posX, uint16_t posY) {
	char msg[20];
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	sprintf(msg, titre);
	BSP_LCD_DrawRect(posX, posY, 130, 25);
	BSP_LCD_FillRect(posX, posY, 130, 25);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(posX + 10, posY + 5, (uint8_t *)msg, LEFT_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DrawRect(posX, posY, 130, 90);
	sprintf(msg, "%6.2f", ( float)valeur);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(posX + 10,posY + 45, (uint8_t *)msg, LEFT_MODE);

}
// Afficher variable de type char
void AfficherCaseD(char *titre, char* valeur, uint16_t posX, uint16_t posY) {

	char msg[20];
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	sprintf(msg, titre);
	BSP_LCD_DrawRect(posX, posY, 130, 25);
	BSP_LCD_FillRect(posX, posY, 130, 25);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(posX + 10, posY + 5, (uint8_t *)msg, LEFT_MODE);

	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DrawRect(posX, posY, 130, 90);
	sprintf(msg, "%s", valeur);
	//humidity_measure();
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(posX + 10,posY + 45, (uint8_t *)msg, LEFT_MODE);

}

// Pour affichet le mot next dans la page
void AfficherBouton(char *texte, uint16_t posX, uint16_t posY) {
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  //BSP_LCD_DrawRect(posX, posY, 80, 30);
  BSP_LCD_DisplayStringAt(posX + 10, posY + 10, (uint8_t *)texte, LEFT_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);  // Revenir à la couleur par défaut
}

// La page initial
void init_page(float* T, float* H, float* P, char** D, float* V, float* Pl, int taille) {
	  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, SDRAM_DEVICE_ADDR);
	  BSP_LCD_SetLayerVisible(LTDC_ACTIVE_LAYER, ENABLE);
	  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
	  BSP_LCD_Clear(LCD_COLOR_WHITE);
	  char titre[] = "Station Meteo: Accueil";
	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
		//sprintf(msg, " MENU PRINCIPALE");
		BSP_LCD_FillRect(0, 0,480,30);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);
	  	  AfficherCase("Tempera", T[taille -1], 5, 50);
	  	    AfficherCase("Humidite", H[taille -1], 180, 50);
	  	    AfficherCase("Pression", P[taille -1], 330, 50);
	  	    AfficherCaseD("Direction", D[taille -1], 5, 160);
	  	    AfficherCase("Vitesse", V[taille -1], 180, 160);
	  	    AfficherCase("Pluie", Pl[taille -1] * 100, 330, 160);

	  // Afficher le bouton de changement de page
	      AfficherBouton("Next", BSP_LCD_GetXSize() - 90, BSP_LCD_GetYSize() - 30);

}

// Dessiner l'axe X
void DessinerAxeX(void) {
  // Dessiner l'axe horizontal (X)
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawHLine(10, BSP_LCD_GetYSize() -100, BSP_LCD_GetXSize() - 20);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -110, 10);

  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -120, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -130, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -140, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -150, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -160, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -170, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -180, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -190, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -200, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -210, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -220, 10);
}

// dessiner l'axe X pour le graphe de direction de vent
void DessinerAxeXD(void) {
  // Dessiner l'axe horizontal (X)
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawHLine(10, BSP_LCD_GetYSize() -50, BSP_LCD_GetXSize() - 20);
  //BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -110, 10);

  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -70, 10);
  //BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -130, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -90, 10);
  //BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -150, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -110, 10);
  //BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -170, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -130, 10);
  //BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -190, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -150, 10);
  //BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -210, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -170, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -190, 10);
  BSP_LCD_DrawHLine(95, BSP_LCD_GetYSize() -210, 10);
}

// Afficher l'axe Y
void DessinerAxeY(void) {
  // Dessiner l'axe vertical (Y)
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawVLine(100, 30, BSP_LCD_GetYSize() - 20);

}

// Dessiner le graphe de pluis

void DessinerGraphePl(float* y) {
  // Exemple de coordonnées X et Y
  int x[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  //int y[] = {100, 50, 130, 80, 160, 120};
  uint16_t pointSize = 4;

  // Trouver le milieu de l'écran
  int midX = 100;
  int midY = BSP_LCD_GetYSize() -100;

  // Dessiner le graphe centré
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 24; ++i) {
    BSP_LCD_FillCircle(midX + x[i], midY - (y[i]*100), pointSize);
  }

  // Dessiner la ligne connectant les points
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 23; ++i) {
    BSP_LCD_DrawLine(midX + x[i], midY - (y[i]*100), midX + x[i + 1], midY - (y[i + 1]*100));
  }

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

// dessiner les graphe de temperature

void DessinerGraphe(float* y) {
  // Exemple de coordonnées X et Y
  int x[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  //int y[] = {100, 50, 130, 80, 160, 120};
  uint16_t pointSize = 4;

  // Trouver le milieu de l'écran
  int midX = 100;
  int midY = BSP_LCD_GetYSize() -100;

  // Dessiner le graphe centré
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 24; ++i) {
    BSP_LCD_FillCircle(midX + x[i], midY - (y[i]*2), pointSize);
  }

  // Dessiner la ligne connectant les points
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 23; ++i) {
    BSP_LCD_DrawLine(midX + x[i], midY - (y[i]*2), midX + x[i + 1], midY - (y[i + 1]*2));
  }

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

// dessiner le graphe d'humidité
void DessinerGrapheH(float* y) {
  // Exemple de coordonnées X et Y
  int x[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  //int y[] = {100, 50, 130, 80, 160, 120};
  uint16_t pointSize = 4;

  // Trouver le milieu de l'écran
  int midX = 100;
  int midY = BSP_LCD_GetYSize() -100;

  // Dessiner le graphe centré
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 24; ++i) {
    BSP_LCD_FillCircle(midX + x[i], midY - (y[i]-30), pointSize);
  }

  // Dessiner la ligne connectant les points
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 23; ++i) {
    BSP_LCD_DrawLine(midX + x[i], midY - (y[i]-30), midX + x[i + 1], midY - (y[i + 1]-30));
  }

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

// Dessiner le graphe de direction du vent
void DessinerGrapheD(char** y) {
  // Exemple de coordonnées X et Y
  int x[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};

  uint16_t pointSize = 4;

  // Trouver le milieu de l'écran
  int midX = 100;
  int midY = BSP_LCD_GetYSize() -50;

  // Dessiner le graphe centré
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);


  for (int i = 0; i < 24; ++i) {
      if (strcmp(y[i], "Nord") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 20, pointSize);
      } else if (strcmp(y[i], "Sud") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 40, pointSize);
      } else if (strcmp(y[i], "Est") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 60, pointSize);
      } else if (strcmp(y[i], "Ouest") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 80, pointSize);
      } else if (strcmp(y[i], "Nord-Est") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 100, pointSize);
      } else if (strcmp(y[i], "Sud-Est") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 120, pointSize);
      } else if (strcmp(y[i], "Sud-Ouest") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 140, pointSize);
      } else if (strcmp(y[i], "Nord-Ouest") == 0) {
          BSP_LCD_FillCircle(midX + x[i], midY - 160, pointSize);
      } else {
          BSP_LCD_FillCircle(midX + x[i], midY, pointSize);
      }
  }


  AfficherBouton("Next", BSP_LCD_GetXSize() - 90, BSP_LCD_GetYSize() - 40);

}

// dessiner le graphes de pression
void DessinerGrapheP(float* y) {
  // Exemple de coordonnées X et Y
  int x[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  //int y[] = {100, 50, 130, 80, 160, 120};
  uint16_t pointSize = 4;

  // Trouver le milieu de l'écran
  int midX = 100;
  int midY = BSP_LCD_GetYSize() -100;

  // Dessiner le graphe centré
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  for (int i = 0; i < 24; ++i) {
    BSP_LCD_FillCircle(midX + x[i], midY - (y[i]-930), pointSize);
  }

  // Dessiner la ligne connectant les points
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);


  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}


// La fonction pour changer les pages
void ChangerDePage(float* T, float* H, float* P, char** D, float* V, float* Pl, int taille) {
  // Mettre à jour la page actuelle
  if (currentPage == PAGE_1) {
    currentPage = PAGE_2;
  }else{
	  if(currentPage == PAGE_2){
		  	currentPage = PAGE_3;
	  }else {
		  if(currentPage == PAGE_3){
			  	currentPage = PAGE_4;
		  }else{
			  if(currentPage == PAGE_4){
				  	currentPage = PAGE_5;
			  }else{
				  if(currentPage == PAGE_5){
					  	currentPage = PAGE_6;
				  }else{
					  if(currentPage == PAGE_6){
						  	currentPage = PAGE_7;
					  }else{
						   if(currentPage == PAGE_7){
							  	currentPage = PAGE_1;
						  }
					  }
				  }
			  }
		  }
	  }
  }









  // Effacer l'écran
  BSP_LCD_Clear(LCD_COLOR_WHITE);


  if (currentPage == PAGE_1) {
	  // Afficher le contenu de la page 1 qui est la page d'accueil
	  init_page(T, H, Pl, D, V, Pl, taille);
  } else {if (currentPage == PAGE_2){
	  // Afficher le contenu de la page 2
	  char titre[] = "Graphe de Temperature";
	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

		BSP_LCD_FillRect(0, 0,480,30);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);


	  //********************************

	  DessinerAxeX();
	  DessinerAxeY();

	  DessinerGraphe(T);

  }else {if (currentPage == PAGE_3){
	    // Afficher le contenu de la page 3
	  char titre[] = "Graphe d'Humidite";
	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

		BSP_LCD_FillRect(0, 0,480,30);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);
		  //*********************************
		  DessinerAxeX();
		  DessinerAxeY();

		  DessinerGrapheH(H);
	  }else {if (currentPage == PAGE_4){
		    // Afficher le contenu de la page 4
		  char titre[] = "Graphe de Pression";
		  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
			BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

			BSP_LCD_FillRect(0, 0,480,30);
			BSP_LCD_SetFont(&Font16);
			BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);
			  //*********************************
			  DessinerAxeX();
			  DessinerAxeY();

			  DessinerGrapheP(P);
		  }else {if (currentPage == PAGE_5){
			    // Afficher le contenu de la page 5
			  char titre[] = "Graphe de Direction de vent";
			  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
				BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

				BSP_LCD_FillRect(0, 0,480,30);
				BSP_LCD_SetFont(&Font16);
				BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);
				  //*****************************************
				  DessinerAxeXD();
				  DessinerAxeY();

				  DessinerGrapheD(D);
			  }else {if (currentPage == PAGE_6){
				    // Afficher le contenu de la page 6
				  char titre[] = "Graphe de Vitesse de vent";
				  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
					BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

					BSP_LCD_FillRect(0, 0,480,30);
					BSP_LCD_SetFont(&Font16);
					BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);

					  DessinerAxeX();
					  DessinerAxeY();

					  DessinerGraphe(V);

				  }else {if (currentPage == PAGE_7){
					    // Afficher le contenu de la page 7
					  char titre[] = "Graphe de Pluie";
					  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
						BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

						BSP_LCD_FillRect(0, 0,480,30);
						BSP_LCD_SetFont(&Font16);
						BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
						BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
						BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)titre, CENTER_MODE);
						  //***********************************
						  DessinerAxeX();
						  DessinerAxeY();

						  DessinerGraphePl(Pl);
					  }
					  }
				  }
			  }
		  }
	  }
  }

  // Afficher le bouton de changement de page
  AfficherBouton("Next", BSP_LCD_GetXSize() - 90, BSP_LCD_GetYSize() - 40);
}
