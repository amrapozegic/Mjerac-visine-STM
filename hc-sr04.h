#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "stm32f4xx.h"

void initGPIO_sensor1();
void TrigHIGH_sensor1();

void initGPIO_sensor2();
void TrigHIGH_sensor2();

double Distance_sensor1();
double Distance_sensor2();

#endif
