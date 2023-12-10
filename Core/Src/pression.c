#include "pression.h"
#include "main.h"
#include "lps22hh_reg.h"

static float pressure_hPa;
static uint32_t data_raw_pressure;


static uint32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
    HAL_I2C_Mem_Write(&hi2c1, LPS22HH_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t *)bufp, len, 1000);
    return 0;
}

static uint32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    HAL_I2C_Mem_Read(handle, LPS22HH_I2C_ADD_H, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
    return 0;
}

stmdev_ctx_t mesure_pression_dev_ctx;

float MesurePression(void)
{
    lps22hh_reg_t reg;
    mesure_pression_dev_ctx.write_reg = platform_write;
    mesure_pression_dev_ctx.read_reg = platform_read;
    mesure_pression_dev_ctx.handle = &hi2c1;
    lps22hh_reset_set(&mesure_pression_dev_ctx, PROPERTY_ENABLE);
    lps22hh_block_data_update_set(&mesure_pression_dev_ctx, PROPERTY_ENABLE);
    lps22hh_data_rate_set(&mesure_pression_dev_ctx, LPS22HH_10_Hz_LOW_NOISE);
    lps22hh_read_reg(&mesure_pression_dev_ctx, LPS22HH_STATUS, (uint8_t *)&reg, 1);
    memset(&data_raw_pressure, 0x00, sizeof(uint32_t));
    lps22hh_pressure_raw_get(&mesure_pression_dev_ctx, &data_raw_pressure);
    pressure_hPa = lps22hh_from_lsb_to_hpa(data_raw_pressure);
    return pressure_hPa;
}


