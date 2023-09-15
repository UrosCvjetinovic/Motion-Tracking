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

unsigned int u;
int i;

int16_t getSignedValue(uint16_t u)
{
  int16_t i;
  const uint16_t maxSignedValue = (uint16_t) 1u << 15u - 1u;

  if (u <= maxSignedValue)
    i = (int16_t)u;
  else
    i = -(int16_t)~u - 1;
  return i;
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
  * @brief Read temperature from BMI088
  * @retval Returns temperature in C
  */
int16_t ReadBMI088Temperature(void)
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
  return temp * 0.125 + 23;
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