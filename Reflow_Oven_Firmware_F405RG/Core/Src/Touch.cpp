/*
 * Touch.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#include "Touch.h"

Touch::Touch(ADC_HandleTypeDef *adcX, ADC_HandleTypeDef *adcY, volatile uint8_t *touchReadyFlag,
        TIM_HandleTypeDef *timer)
        : m_adcX(adcX), m_adcY(adcY), m_touchReady(touchReadyFlag), m_timer(timer)
{
    uint16_t timer_fequency_hz = 84000000 / (m_timer->Instance->PSC + 1);
    uint16_t timer_reload_counter = timer_fequency_hz / 1000 * DELAY_TOUCH_MS;

    if (timer_reload_counter > 0xffff)
        timer_reload_counter = 0xffff;

    m_timer->Instance->ARR = timer_reload_counter - 1;
    HAL_TIM_Base_Start_IT(m_timer);
}

void Touch::Update()
{
    ReadX();
    ReadY();
}

point_t& Touch::getPoint()
{
    Update();
    return m_point;
}

uint8_t Touch::CheckRegion(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h)
{
    Update();

    if (*m_touchReady)
    {
        uint16_t x1 = x0 + w, y1 = y0 + h;

        if (m_point.x > x0 && m_point.x < x1 && m_point.y > y0 && m_point.y < y1)
        {
            StartTouchDelay();
            return 1;
        }
    }

    return 0;
}

uint8_t Touch::CheckRegion(img_data_t *img)
{
    Update();

    if (*m_touchReady)
    {
        uint16_t x0 = img->x, x1 = x0 + img->w, y0 = img->y, y1 = y0 + img->h;

        if (m_point.x > x0 && m_point.x < x1 && m_point.y > y0 && m_point.y < y1)
        {
            StartTouchDelay();
            return 1;
        }
    }

    return 0;
}

void Touch::StartTouchDelay()
{
    *m_touchReady = 0;
    HAL_TIM_Base_Start_IT(m_timer);
}

void Touch::ReadX()
{
    uint16_t raw_x;

    uint32_t tempC = GPIOC->MODER;
    uint32_t tempA = GPIOA->MODER;

    GPIOC->MODER |= GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER11;
    XP_PORT->BSRR = XP_PIN;
    XM_PORT->BSRR = (uint32_t) XM_PIN << 16U;

    HAL_ADC_Start(m_adcX);
    HAL_ADC_PollForConversion(m_adcX, 100);
    raw_x = HAL_ADC_GetValue(m_adcX);

    GPIOC->MODER = tempC;
    GPIOA->MODER = tempA;

    if (raw_x > 4095)
        raw_x = 0;

    raw_x -= OFFSET_X;
    m_point.x = MapX(raw_x);
}

void Touch::ReadY()
{
    uint16_t raw_y;

    uint32_t tempC = GPIOC->MODER;
    uint32_t tempA = GPIOA->MODER;

    GPIOC->MODER |= GPIO_MODER_MODER4;    // TFT_D0 als analoger Input
    GPIOA->MODER |= GPIO_MODER_MODER10;    // TFT_RS als analoger Input
    YP_PORT->BSRR = YP_PIN;
    YM_PORT->BSRR = (uint32_t) YM_PIN << 16U;

    HAL_ADC_Start(m_adcY);
    HAL_ADC_PollForConversion(m_adcY, 100);
    raw_y = HAL_ADC_GetValue(m_adcY);

    GPIOC->MODER = tempC;
    GPIOA->MODER = tempA;

    if (raw_y > 4095)
        raw_y = 0;

    raw_y -= OFFSET_Y;
    m_point.y = MapY(raw_y);
}

uint16_t Touch::MapX(uint16_t rawX)
{
    return rawX * MAP_X / RAW_MAX;    // (RAW_MAX - rawX) * MAP_X / RAW_MAX;
}

uint16_t Touch::MapY(uint16_t rawY)
{
    return rawY * MAP_Y / RAW_MAX;
}

