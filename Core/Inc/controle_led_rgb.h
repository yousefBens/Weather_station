#ifndef __CONTROLE_LED_RGB_H
#define __CONTROLE_LED_RGB_H

#include "main.h"

// Définition des états possibles
typedef enum {
    PriseDeMesure,
    ExtinctionRGB,
    TransfertVersSD,
    Navigation
} EtatLed;

// Prototype de la fonction
void IndicationEtatsParLedRGB(EtatLed etat);

#endif // __LED_CONTROL_H
