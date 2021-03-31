/*
 * Load.h
 *
 *  Created on: 11.03.2021
 *      Author: Johan
 */

#ifndef SRC_LOAD_H_
#define SRC_LOAD_H_

#include "main.h"

typedef enum load_state_t
{
    LOAD_OFF, LOAD_ON, LOAD_DUTY
} load_state_t;

#define ON_DELAY 0
#define ON_PULSE 100

class Load
{
private:
    TIM_HandleTypeDef *m_timer_duty;
    TIM_HandleTypeDef *m_timer_pulse;

    volatile load_state_t *m_state;
    uint8_t m_duty;

public:
    Load(TIM_HandleTypeDef *timer_duty, TIM_HandleTypeDef *timer_pulse, volatile load_state_t *state);
    virtual ~Load();

    void SetDuty(uint8_t duty);
    uint8_t GetDuty() const;

    void On();
    void Off();
};

#endif /* SRC_LOAD_H_ */
