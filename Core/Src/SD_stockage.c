#include "SD_stockage.h"
#include "controle_led_rgb.h"
#include "stm32746g_discovery_lcd.h"
#include <stdio.h>

#include <string.h>

extern UART_HandleTypeDef huart1;
//static uint8_t tx_buffer[1000];
int v= 0;

/* Function to save weather data to a CSV file */
FRESULT SaveDataToCSV(char (*heures)[6], float *temperatures, float *pressions, float *humidites, char** directions_vent, float *vitesses_vent, float *pluies)
{
	// au debut du stockage on met l'ecran en mode veille
	BSP_LCD_DisplayOff();
	IndicationEtatsParLedRGB(TransfertVersSD);
	uint8_t workBuffer[_MAX_SS];
    FRESULT res; /* FatFs function common result code */
    uint32_t byteswritten; /* File write/read counts */
    uint8_t wtext[100]; /* File write buffer */


    char msg1[] = "Begin\n\r";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg1, strlen(msg1), HAL_MAX_DELAY);

    /* Mount SD Card */
    if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) != FR_OK)
    {
        /* FatFs Initialization Error */
        return FR_INT_ERR;
    }

    /* Create a FAT file system on the logical drive */
    if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
    {
        /* FatFs Format Error */
        return FR_INT_ERR;
    }

    /* Open or create a CSV file with write access */
    if(f_open(&SDFile, "Mesures.CSV", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        /* 'STM32.CSV' file Open for write Error */
        return FR_INT_ERR;
    }

    /* Write header to the CSV file */
    sprintf((char*)wtext, "Heure;Temperature(degC);Pression(hPA);Humidite(%%);Direction_vent;Vitesse_vent(m/s);Pluie(mm)\n");
    res = f_write(&SDFile, wtext, strlen((char*)wtext), (void *)&byteswritten);
    if((byteswritten == 0) || (res != FR_OK))
    {
        /* 'STM32.CSV' file Write or EOF Error */
        return res;
    }

    /* Write data to the CSV file */
    for(int i = 0; i < 24; i++)
    {
        sprintf((char*)wtext, "%s;%f;%f;%f;%s;%f;%f\n", heures[i], temperatures[i], pressions[i],
                humidites[i], directions_vent[i], vitesses_vent[i], pluies[i]);
        res = f_write(&SDFile, wtext, strlen((char*)wtext), (void *)&byteswritten);
        if((byteswritten == 0) || (res != FR_OK))
        {
            /* 'STM32.CSV' file Write or EOF Error */
            return res;
        }
    }
    char msg2[] = "End\n\r";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg2, strlen(msg2), HAL_MAX_DELAY);

    /* Close the open CSV file */
    f_close(&SDFile);

    /* Unmount SD Card */
    f_mount(NULL, (TCHAR const*)SDPath, 0);
    IndicationEtatsParLedRGB(ExtinctionRGB);

    //FATFS_UnLinkDriver(SDPath);


    //HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_SET);

    IndicationEtatsParLedRGB(ExtinctionRGB);
    // fin de mode vielle
    BSP_LCD_DisplayOn();
    return FR_OK; /* Function completed successfully */
}











