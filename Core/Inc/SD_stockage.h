#ifndef SD_STOCKAGE_H
#define SD_STOCKAGE_H

#include "fatfs.h"
#include "dma.h"
#include "sdmmc.h"
#include "main.h"

FRESULT SaveDataToCSV(char (*heures)[6], float *temperatures, float *pressions, float *humidites, char** directions_vent, float *vitesses_vent, float *pluies);

#endif /* SD_STOCKAGE_H */
