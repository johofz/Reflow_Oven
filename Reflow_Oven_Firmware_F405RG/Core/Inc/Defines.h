/*
 * Defines.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#ifndef INC_DEFINES_H_
#define INC_DEFINES_H_

#include "ILI9341.h"
#include "MAX31855.h"
#include "Touch.h"
#include "Load.h"
#include "ControllerPID.h"

#define TFT_ROATION_PORTRAIT 0
#define TFT_ROATION_LANDSCAPE 1

#define MAX_TEMP_INTERNAL 50.0f     // Höchste interne Temperatur (Chip)
#define MAX_TEMP_TEMPERATE 100.0f   // Höchste Temperatur in Temperiermodus (Messstelle)

#define DELAY_DISPLAY_UPDATE_MS 1000     // entspricht 1 Hz Update-Rate

#define WIDTH       (context->tft->Width())
#define HEIGHT      (context->tft->Height())

typedef enum state_t
{
    STATE_BOOT,

    STATE_MENU_INIT, STATE_MENU,

    STATE_REFLOW_INIT, STATE_REFLOW,

    STATE_TEMPERATE_INIT, STATE_TEMPERATE,

    STATE_OPTIONS_INIT, STATE_OPTIONS,

    STATE_ERROR
} state_t;

typedef enum error_t
{
    ERROR_NO_ERROR, ERROR_UNKNOWN
} error_t;

typedef struct interrupt_data_t
{
    uint64_t timerOverflow;

    uint8_t touchReadyFlag;

    load_state_t heater_state;
    load_state_t fan_state;
    load_state_t light_state;

} interrupt_data_t;

typedef struct context_t
{
public:
    ILI9341 *tft;
    MAX31855 *temp;
    Touch *touch;
    ControllerPID *pid;

    Load *heater;
    Load *fan;
    Load *light;

    state_t currentState;
    error_t error;

    uint8_t temperate_active;

    uint64_t pid_delay_ms;
    uint64_t temperate_display_delay_ms;

    context_t(ILI9341 *_tft, MAX31855 *_temp, Touch *_touch, ControllerPID *_pid, Load *_heater,
            Load *_fan, Load *_light)
            : tft(_tft), temp(_temp), touch(_touch), pid(_pid), heater(_heater), fan(_fan), light(
                    _light), currentState(STATE_BOOT), error(ERROR_NO_ERROR)
    {
    }
} context_t;

#endif /* INC_DEFINES_H_ */
