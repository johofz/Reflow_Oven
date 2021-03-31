/*
 * Load.cpp
 *
 *  Created on: 11.03.2021
 *      Author: Johan
 */

#include "Load.h"

Load::Load(TIM_HandleTypeDef *timer_duty, TIM_HandleTypeDef *timer_pulse,
        volatile load_state_t *state)
        : m_timer_duty(timer_duty), m_timer_pulse(timer_pulse), m_state(state)
{
    Off();
}

Load::~Load()
{
    Off();
}
void Load::SetDuty(uint8_t duty)
{

    if (duty >= 100)
    {
        m_duty = 100;
        *m_state = LOAD_ON;    // AN
    }
    else if (duty <= 0)
    {
        m_duty = 0;
        *m_state = LOAD_OFF;    // AUS
    }
    else
    {
        m_duty = duty;

        uint16_t timerReload = (100 - duty) * 9000 / 100;
        m_timer_duty->Instance->ARR = timerReload + ON_DELAY;
        m_timer_pulse->Instance->ARR = timerReload + ON_PULSE + ON_DELAY;

        *m_state = LOAD_DUTY;
    }

}

uint8_t Load::GetDuty() const
{
    return m_duty;
}

void Load::On()
{
    SetDuty(100);
}

void Load::Off()
{
    SetDuty(0);
}
