/*
 * functions.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "main.h"
#include "Defines.h"
#include "Flash_Icons.h"
#include <string.h>
#include <stdio.h>

extern volatile interrupt_data_t interrupt_data;

extern SPI_HandleTypeDef hspi1;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

/* Boot-Funktionen */
void boot_initSystem(context_t *context);

/* Menü-Funktionen */
void menu_init(context_t *context);
void menu_update(context_t *context);

/* Reflow-Funktionen */
void reflow_init(context_t *context);
void reflow_update(context_t *context);

/* Temperierungsfunktionen */
void temperate_init(context_t *context);
void temperate_update(context_t *context);
void temperate_render(context_t *context);

/* Allgemeine Einstellungs-Funktionen */
void options_init(context_t *context);
void options_update(context_t *context);

/* Fehler-Handling */
void error_show(context_t *context);

/* Timing Funktionen */
uint64_t Micros();    // Zeit seit Systemstart in µs
uint64_t Millis();    // Zeit seit Systemstart in ms
uint64_t Seconds();    // Zeit seit Systemstart in s

void Delay_US(uint64_t us);    // Delay in µs
void Delay_MS(uint64_t ms);    // Delay in ms

#endif /* INC_FUNCTIONS_H_ */
