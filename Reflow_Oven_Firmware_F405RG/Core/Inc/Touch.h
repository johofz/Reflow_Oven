/*
 * Touch.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#ifndef SRC_TOUCH_H_
#define SRC_TOUCH_H_

#include "main.h"
#include "Flash_Icons.h"

#define YP_PIN      GPIO_PIN_5
#define YP_PORT     GPIOC
#define YM_PIN      GPIO_PIN_11
#define YM_PORT     GPIOA
#define XP_PIN      GPIO_PIN_10
#define XP_PORT     GPIOA
#define XM_PIN      GPIO_PIN_4
#define XM_PORT     GPIOC

#define OFFSET_X    400
#define OFFSET_Y    300

#define RAW_MAX     3200

#define MAP_X       240
#define MAP_Y       320

#define DELAY_TOUCH_MS 300


typedef struct point_t
{
    uint16_t x;
    uint16_t y;
} point_t;

class Touch
{
private:
    ADC_HandleTypeDef *m_adcX;
    ADC_HandleTypeDef *m_adcY;

    point_t m_point;

    volatile uint8_t *m_touchReady;

    TIM_HandleTypeDef *m_timer;

    void Update();

    void ReadX();
    void ReadY();

    uint16_t MapX(uint16_t rawX);
    uint16_t MapY(uint16_t rawY);

    void StartTouchDelay();

public:
    Touch(ADC_HandleTypeDef *adcX, ADC_HandleTypeDef *adcY, volatile uint8_t *touchReadyFlag,
            TIM_HandleTypeDef *timer);
    virtual ~Touch()
    {
    }

    uint8_t CheckRegion(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h);
    uint8_t CheckRegion(img_data_t *img);

    point_t& getPoint();
};

#endif /* SRC_TOUCH_H_ */
