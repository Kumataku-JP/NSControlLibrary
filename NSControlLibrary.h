#pragma once

#include "./SwitchControlLibrary/src/SwitchControlLibrary.h"

void pushButton(uint16_t button, unsigned long input_time = 40, unsigned long delay_time = 40, unsigned int loop = 1);
void holdButton(uint16_t button, unsigned long delay_time = 0);
void releaseButton(uint16_t button, unsigned long delay_time = 0);

void pushHat(uint8_t hat, unsigned long input_time = 40, unsigned long delay_time = 40, unsigned int loop = 1);
void holdHat(uint8_t hat, unsigned long delay_time = 0);
void releaseHat(unsigned long delay_time = 0);

void leftStickTilt(int16_t angle_deg, uint8_t strength, unsigned long delay_time = 0);
void rightStickTilt(int16_t angle_deg, uint8_t strength, unsigned long delay_time = 0);
void leftStickNeutral(unsigned long delay_time = 0);
void rightStickNeutral(unsigned long delay_time = 0);
