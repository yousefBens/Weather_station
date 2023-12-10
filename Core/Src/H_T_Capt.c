#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "hts221_reg.h"





/* USER CODE BEGIN PV */
static int16_t data_raw_humidity;
static int16_t data_raw_temperature;
static float humidity_perc;
static float temperature_degC;
static uint8_t tx_buffer[1000];

/* USER CODE END PV */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker-
>Libraries->Small printf
set to 'Yes') calls __io_putchar() */

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len)
{
	/* Write multiple command */
	  reg |= 0x80;
	  HAL_I2C_Mem_Write(handle, HTS221_I2C_ADDRESS, reg,
	                    I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
  return 0;
}

static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
	 /* Read multiple command */
	  reg |= 0x80;
	  HAL_I2C_Mem_Read(handle, HTS221_I2C_ADDRESS, reg,
	                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  return 0;
}


typedef struct {
  float x0;
  float y0;
  float x1;
  float y1;
} lin_t;

float linear_interpolation(lin_t *lin, int16_t x)
{
  return ((lin->y1 - lin->y0) * x + ((lin->x1 * lin->y0) -
                                     (lin->x0 * lin->y1)))
         / (lin->x1 - lin->x0);
}





float Humedite(){
	static uint8_t whoamI;
	  stmdev_ctx_t dev_ctx;
	  dev_ctx.write_reg = platform_write;
	  dev_ctx.read_reg = platform_read;
	  dev_ctx.handle = &hi2c1;
	   whoamI = 0;
	   hts221_device_id_get(&dev_ctx, &whoamI);

	   if ( whoamI != HTS221_ID )
	     while (1); /*manage here device not found */

	   /* Read humidity calibration coefficient */
	   lin_t lin_hum;
	   hts221_hum_adc_point_0_get(&dev_ctx, &lin_hum.x0);
	   hts221_hum_rh_point_0_get(&dev_ctx, &lin_hum.y0);
	   hts221_hum_adc_point_1_get(&dev_ctx, &lin_hum.x1);
	   hts221_hum_rh_point_1_get(&dev_ctx, &lin_hum.y1);
	   /* Read temperature calibration coefficient */
	   lin_t lin_temp;
	   hts221_temp_adc_point_0_get(&dev_ctx, &lin_temp.x0);
	   hts221_temp_deg_point_0_get(&dev_ctx, &lin_temp.y0);
	   hts221_temp_adc_point_1_get(&dev_ctx, &lin_temp.x1);
	   hts221_temp_deg_point_1_get(&dev_ctx, &lin_temp.y1);
	   /* Enable Block Data Update */
	   hts221_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
	   /* Set Output Data Rate */
	   hts221_data_rate_set(&dev_ctx, HTS221_ODR_1Hz);
	   /* Device power on */
	   hts221_power_on_set(&dev_ctx, PROPERTY_ENABLE);
    hts221_reg_t reg;
    hts221_status_get(&dev_ctx, &reg.status_reg);

    if (reg.status_reg.h_da) {
      /* Read humidity data */
      memset(&data_raw_humidity, 0x00, sizeof(int16_t));
      hts221_humidity_raw_get(&dev_ctx, &data_raw_humidity);
      humidity_perc = linear_interpolation(&lin_hum, data_raw_humidity);

      if (humidity_perc < 0) {
        humidity_perc = 0;
      }

      if (humidity_perc > 100) {
        humidity_perc = 100;
      }


    }
    return humidity_perc;
}

void affich_H(){
    sprintf((char *)tx_buffer, "Humidity [%%]:%3.2f\r\n", Humedite()); // @suppress("Float formatting support")
    printf( (char const *)tx_buffer);
}

float Temperature(){
	static uint8_t whoamI;
	  stmdev_ctx_t dev_ctx;
	  dev_ctx.write_reg = platform_write;
	  dev_ctx.read_reg = platform_read;
	  dev_ctx.handle = &hi2c1;
	   whoamI = 0;
	   hts221_device_id_get(&dev_ctx, &whoamI);

	   if ( whoamI != HTS221_ID )
	     while (1); /*manage here device not found */

	   /* Read humidity calibration coefficient */
	   lin_t lin_hum;
	   hts221_hum_adc_point_0_get(&dev_ctx, &lin_hum.x0);
	   hts221_hum_rh_point_0_get(&dev_ctx, &lin_hum.y0);
	   hts221_hum_adc_point_1_get(&dev_ctx, &lin_hum.x1);
	   hts221_hum_rh_point_1_get(&dev_ctx, &lin_hum.y1);
	   /* Read temperature calibration coefficient */
	   lin_t lin_temp;
	   hts221_temp_adc_point_0_get(&dev_ctx, &lin_temp.x0);
	   hts221_temp_deg_point_0_get(&dev_ctx, &lin_temp.y0);
	   hts221_temp_adc_point_1_get(&dev_ctx, &lin_temp.x1);
	   hts221_temp_deg_point_1_get(&dev_ctx, &lin_temp.y1);
	   /* Enable Block Data Update */
	   hts221_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
	   /* Set Output Data Rate */
	   hts221_data_rate_set(&dev_ctx, HTS221_ODR_1Hz);
	   /* Device power on */
	   hts221_power_on_set(&dev_ctx, PROPERTY_ENABLE);
    hts221_reg_t reg;
    hts221_status_get(&dev_ctx, &reg.status_reg);

    if (reg.status_reg.t_da) {
      /* Read temperature data */
      memset(&data_raw_temperature, 0x00, sizeof(int16_t));
      hts221_temperature_raw_get(&dev_ctx, &data_raw_temperature);
      temperature_degC = linear_interpolation(&lin_temp,
                                              data_raw_temperature);

    }

    return temperature_degC;
}

void affiche_T(){
    sprintf((char *)tx_buffer, "Temperature [degC]:%6.2f\r\n", // @suppress("Float formatting support")
            Temperature() );
    printf((char const *)tx_buffer);
}

PUTCHAR_PROTOTYPE
{
/* Place your implementation of fputc here */
/* e.g. write a character to the USART2 and Loop until the end
of transmission */
HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
return ch;
}


