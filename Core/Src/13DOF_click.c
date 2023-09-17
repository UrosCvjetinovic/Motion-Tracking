/* USER CODE BEGIN Header */
/**
  *  Created on: Sep 15, 2023
  *      Author: Uros Cvjetinovic
  ******************************************************************************
  * @file           : 13DOF_click.c
  * @brief          : Driver for 13DOF click
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "i2c.h"
#include "13DOF_click.h"
#include <limits.h>

/* USER CODE END Includes */

struct bme680_dev gasSensor;

int16_t getSignedValue(uint16_t u)
{
  int16_t i;

  if (u <= (uint16_t) 1u << 15u - 1u)
    i = (int16_t)u;
  else
    i = -(int16_t)~u - 1;
  return i;
}
void user_delay_ms(uint32_t period)
{
  HAL_Delay(period);
}

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    HAL_I2C_Mem_Read(&hi2c2, dev_id << 1u, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, HAL_MAX_DELAY);

    return rslt;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    HAL_I2C_Mem_Write(&hi2c2, dev_id << 1u, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, HAL_MAX_DELAY);

    return rslt;
}

/**
  * @brief Checks if 13DOF click is connected
  * @retval None
  */
void ReadConnectionStatus(SensorConnectionStruct *conectionStatus)
{
  uint8_t id_BME680 = 0u; // Expected :  0xD0
  uint8_t id_BMI088_acc = 0u; // Expected : 0x1E
  uint8_t id_BMI088_gyro = 0u; // Expected : 0x0F
  uint8_t id_BMM150 = 0u; // Expected : 0x32

  HAL_I2C_Mem_Read(&hi2c2, BMM150_DEFAULT_I2C_ADDRESS << 1u, BMM150_REG_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &id_BMM150, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &id_BMI088_acc, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &id_BMI088_gyro, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BME680_I2C_ADDR_PRIMARY << 1u, BME680_CHIP_ID_ADDR, I2C_MEMADD_SIZE_8BIT, &id_BME680, 1u, HAL_MAX_DELAY);

  conectionStatus->BMM150 = id_BMM150 == BMM150_CHIP_ID;
  conectionStatus->BMI088_acc = id_BMI088_acc == BMI08_ACC_CHIP_ID;
  conectionStatus->BMI088_gyro = id_BMI088_gyro == BMI08_GYRO_CHIP_ID;
  conectionStatus->BME680 = id_BME680 == BME680_CHIP_ID;
}

/**
  * @brief Initialize BMI088
  * @retval None
  */
void InitBMI088(void)
{
  // Initialize accelerometer
  uint8_t dataToWrite = BMI08_ACCEL_PM_ACTIVE;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_PWR_CONF, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);

  dataToWrite = BMI08_ACCEL_BW_NORMAL << BMI08_ACCEL_BW_POS | BMI08_ACCEL_ODR_200_HZ;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_CONF, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);

  dataToWrite = 0u;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_RANGE, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);

  dataToWrite = BMI08_ACCEL_POWER_ENABLE;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_PWR_CTRL, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);

  // Initialize gyroscope
  dataToWrite = BMI08_GYRO_RANGE_125_DPS;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_RANGE, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);

  dataToWrite = BMI08_GYRO_BW_32_ODR_100_HZ;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_BANDWIDTH, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);
}

/**
  * @brief Initialize BME680
  * @retval None
  */
void InitBME680(void)
{
  int8_t rslt = BME680_OK;
  uint8_t setRequiredSettings;

  gasSensor.dev_id = BME680_I2C_ADDR_PRIMARY;
  gasSensor.intf = BME680_I2C_INTF;
  gasSensor.read = user_i2c_read;
  gasSensor.write = user_i2c_write;
  gasSensor.delay_ms = user_delay_ms;
  /* amb_temp can be set to 25 prior to configuring the gas sensor
   * or by performing a few temperature readings without operating the gas sensor.
   */
  gasSensor.amb_temp = board.temperature;

  rslt = bme680_init(&gas_sensor);

  if (rslt == BME680_OK)
  {
    /* Set the temperature, pressure and humidity settings */
    gasSensor.tph_sett.os_hum = BME680_OS_2X;
    gasSensor.tph_sett.os_pres = BME680_OS_4X;
    gasSensor.tph_sett.os_temp = BME680_OS_8X;
    gasSensor.tph_sett.filter = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    gasSensor.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    gasSensor.gas_sett.heatr_temp = 320; /* degree Celsius */
    gasSensor.gas_sett.heatr_dur = 150; /* milliseconds */

    /* Set the required sensor settings needed */
    setRequiredSettings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL
        | BME680_GAS_SENSOR_SEL;

    /* Set the desired sensor configuration */
    rslt = bme680_set_sensor_settings(setRequiredSettings, &gasSensor);
  }
}


/**
  * @brief Starts BMI088 sensor measurements
  * @retval None
  */
void StartBMI088Sensor(void)
{
  StartBMI088Accelerometer();
  StartBMI088Gyroscope();
}

/**
  * @brief Starts BMI088 sensor measurement of acceleration
  * @retval None
  */
void StartBMI088Accelerometer(void)
{
  uint8_t dataToWrite = BMI08_ACCEL_PM_ACTIVE;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_PWR_CONF, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);
  HAL_Delay(50);
}

/**
  * @brief Starts BMI088 sensor measurement of angular velocity
  * @retval None
  */
void StartBMI088Gyroscope(void)
{
  uint8_t write = BMI08_GYRO_PM_NORMAL;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_LPM1, I2C_MEMADD_SIZE_8BIT, &write, 1u, HAL_MAX_DELAY);
  HAL_Delay(50);
}

/**
  * @brief Starts BME680 sensor measurements
  * @retval None
  */
void StartBME680Sensor(void)
{
  /* Select the power mode */
  /* Must be set before writing the sensor configuration */
  gas_sensor.power_mode = BME680_FORCED_MODE;

  /* Set the power mode */
  bme680_set_sensor_mode(&gas_sensor);
}


/**
  * @brief Stop BMI088 sensor measurements
  * @retval None
  */
void StopBMI088Sensor(void)
{
  StopBMI088Accelerometer();
  StopBMI088Gyroscope();
}

/**
  * @brief Stops BMI088 sensor measurement of acceleration
  * @retval None
  */
void StopBMI088Accelerometer(void)
{
  uint8_t dataToWrite = BMI08_ACCEL_PM_SUSPEND;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_PWR_CONF, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);
}

/**
  * @brief Stops BMI088 sensor measurement of angular velocity
  * @retval None
  */
void StopBMI088Gyroscope(void)
{
  uint8_t dataToWrite = BMI08_GYRO_PM_SUSPEND;
  HAL_I2C_Mem_Write(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_LPM1, I2C_MEMADD_SIZE_8BIT, &dataToWrite, 1u, HAL_MAX_DELAY);
}

/**
  * @brief Sto[s BME680 sensor measurements
  * @retval None
  */
void StopBME680Sensor(void)
{
  /* Select the power mode */
  /* Must be set before writing the sensor configuration */
  gasSensor.power_mode = BME680_SLEEP_MODE;

  /* Set the power mode */
  bme680_set_sensor_mode(&gas_sensor);
}

/**
  * @brief Read temperature from BMI088
  * @retval None
  */
void ReadBMI088Temperature(int16_t *temperature)
{
  uint8_t temperatureMSB;
  uint8_t temperatureLSB;

  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_TEMP_MSB, I2C_MEMADD_SIZE_8BIT, &temperatureMSB, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_TEMP_LSB, I2C_MEMADD_SIZE_8BIT, &temperatureLSB, 1u, HAL_MAX_DELAY);

  //Recalculate temperature
  int16_t temp = getSignedValue((uint16_t)(temperatureMSB << 3) + (uint16_t)(temperatureLSB >> 5));
  if (temp > 1023)
  {
    temp = temp - 2048;
  }
  *temperature = temp * 0.125 + 23;
}

/**
  * @brief Read acceleration of BMI088 gyroscope
  * @retval None
  */
void ReadBMI088Acceleration(struct bmi08_sensor_data * acceleration)
{
  uint8_t acc_x_lsb;
  uint8_t acc_x_msb;
  uint8_t acc_y_lsb;
  uint8_t acc_y_msb;
  uint8_t acc_z_lsb;
  uint8_t acc_z_msb;

  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_X_LSB, I2C_MEMADD_SIZE_8BIT, &acc_x_lsb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_X_MSB, I2C_MEMADD_SIZE_8BIT, &acc_x_msb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_Y_LSB, I2C_MEMADD_SIZE_8BIT, &acc_y_lsb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_Y_MSB, I2C_MEMADD_SIZE_8BIT, &acc_y_msb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_Z_LSB, I2C_MEMADD_SIZE_8BIT, &acc_z_lsb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_ACCEL_I2C_ADDR_PRIMARY << 1u, BMI08_REG_ACCEL_Z_MSB, I2C_MEMADD_SIZE_8BIT, &acc_z_msb, 1u, HAL_MAX_DELAY);

  acceleration->x = getSignedValue((uint16_t)acc_x_msb << 8u | acc_x_lsb);
  acceleration->y = getSignedValue((uint16_t)acc_y_msb << 8u | acc_y_lsb);
  acceleration->z = getSignedValue((uint16_t)acc_z_msb << 8u | acc_z_lsb);

  //Recalculate acceleration
  acceleration->x = ((acceleration->x / 32768) * 2000);
  acceleration->y = ((acceleration->y / 32768) * 2000);
  acceleration->z = ((acceleration->z / 32768) * 2000);
}

/**
  * @brief Read angular velocity of BMI088 gyroscope
  * @retval None
  */
void ReadBMI088Orientation(struct bmi08_sensor_data * orientation)
{
  uint8_t ori_x_lsb;
  uint8_t ori_x_msb;
  uint8_t ori_y_lsb;
  uint8_t ori_y_msb;
  uint8_t ori_z_lsb;
  uint8_t ori_z_msb;

  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_X_LSB, I2C_MEMADD_SIZE_8BIT, &ori_x_lsb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_X_MSB, I2C_MEMADD_SIZE_8BIT, &ori_x_msb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_Y_LSB, I2C_MEMADD_SIZE_8BIT, &ori_y_lsb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_Y_MSB, I2C_MEMADD_SIZE_8BIT, &ori_y_msb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_Z_LSB, I2C_MEMADD_SIZE_8BIT, &ori_z_lsb, 1u, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c2, BMI08_GYRO_I2C_ADDR_PRIMARY << 1u, BMI08_REG_GYRO_Z_MSB, I2C_MEMADD_SIZE_8BIT, &ori_z_msb, 1u, HAL_MAX_DELAY);

  orientation->x = getSignedValue((uint16_t)ori_x_msb << 8u | ori_x_lsb);
  orientation->y = getSignedValue((uint16_t)ori_y_msb << 8u | ori_y_lsb);
  orientation->z = getSignedValue((uint16_t)ori_z_msb << 8u | ori_z_lsb);

  //Recalculate orientation
  orientation->x = ((orientation->x * 125) / 32768);
  orientation->y = ((orientation->y * 125) / 32768);
  orientation->z = ((orientation->z * 125) / 32768);
}

/**
  * @brief Read temperature from BMI088
  * @retval Returns temperature in C
  */
void ReadBME680(int16_t *temperature, uint32_t *pressure, uint32_t *humidity, uint32_t *gasResistance)
{
  struct bme680_field_data data;
  int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
  /* Trigger the next measurement if you would like to read data out continuously */
  if (gasSensor.power_mode == BME680_FORCED_MODE) {
      rslt = bme680_set_sensor_mode(&gasSensor);
  }

  /* Get the total measurement duration so as to sleep or wait till the
   * measurement is complete */
  uint16_t meas_period;
  bme680_get_profile_dur(&meas_period, &gasSensor);

  user_delay_ms(meas_period); /* Delay till the measurement is ready */

  rslt = bme680_get_sensor_data(&data, &gasSensor);

  if (rslt == BME680_OK)
  {
    *temperature = data.temperature / 100u;
    *pressure = data.pressure / 1000;
    *humidity = data.humidity / 1000;
    *gasResistance = data.gas_resistance / 1000;
  }

}

