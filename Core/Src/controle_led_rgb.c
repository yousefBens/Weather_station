#include "controle_led_rgb.h"


// Fonction pour gérer l'état des LEDs
void IndicationEtatsParLedRGB(EtatLed etat) {
    switch (etat) {
        case PriseDeMesure:
            // Allumer le blanc (Rouge + Vert, Bleu non contrôlable)
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET); // Vert ON
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET); // Rouge ON
            break;

        case TransfertVersSD:
            // Allumer le Magenta (Rouge, Bleu)
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET); // Vert OFF
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET); // Rouge ON
            break;

        case ExtinctionRGB:
            // Éteindre toutes les couleurs (sauf Bleu)
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET); // Vert OFF
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET); // Rouge OFF
            break;

        case Navigation:
            // Allumer le Cyan (Vert, Bleu)
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET); // Vert ON
            HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET); // Rouge OFF
            break;

        default:
            // Gestion d'état non défini
            break;
    }
}
