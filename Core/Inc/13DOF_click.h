/*
 * 13DOF_click.h
 *
 *  Created on: Sep 15, 2023
 *      Author: Uros Cvjetinovic
 */

#ifndef INC_13DOF_CLICK_H_
#define INC_13DOF_CLICK_H_

#include "bmi08_defs.h"
#include "bmm150_defs.h"
#include "bme680_defs.h"

/**\name    Acc unique chip identifier, missing from driver */
#define BMI08_ACC_CHIP_ID UINT8_C(0x1E)
/**\name    Threshold to detect change with gyroscope */
#define BMI088_DEGREEPERSECOND_THRESHOLD ((uint16_t) 70u)

typedef struct SensorConnectionStruct {
  uint8_t BMM150;
  uint8_t BMI088_acc;
  uint8_t BMI088_gyro;
  uint8_t BME680;
} SensorConnectionStruct;

void InitBMI088(void);
void ReadConnectionStatus(SensorConnectionStruct *conectionStatus);
void StartBMI088Sensor(void);
void StopBMI088Sensor(void);
int16_t ReadBMI088Temperature(void);
void ReadBMI088Acceleration(struct bmi08_sensor_data * acceleration);
void ReadBMI088Orientation(struct bmi08_sensor_data * orientation);


#endif /* INC_13DOF_CLICK_H_ */
