#ifndef PRESSION_H
#define PRESSION_H

#include "main.h"
#include "main.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
//#include "graph_display.h"
#include "lps22hh_reg.h"
#include "string.h"



float MesurePression(void);
//static float pressure_hPa;
//static uint32_t data_raw_pressure;

//static uint32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
         //                     uint16_t len);

//static uint32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
           //                  uint16_t len);
#endif // PRESSION_H
