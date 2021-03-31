/*
 * ILI9341.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#ifndef SRC_ILI9341_H_
#define SRC_ILI9341_H_

#include "main.h"
#include "ILI9341_Instructions.h"
#include "Flash_Icons.h"

#define RST_PIN     GPIO_PIN_12
#define RST_PORT    GPIOA
#define CS_PIN      GPIO_PIN_11
#define CS_PORT     GPIOA
#define RS_PIN      GPIO_PIN_10
#define RS_PORT     GPIOA
#define WR_PIN      GPIO_PIN_9
#define WR_PORT     GPIOA
#define RD_PIN      GPIO_PIN_8
#define RD_PORT     GPIOA

#define BUS_PORT    GPIOC
#define BUS_OFFSET  4

#define SELECT()    CS_PORT->BSRR = (uint32_t) CS_PIN << 16U
#define DESELECT()  CS_PORT->BSRR = CS_PIN
#define COMMAND()   RS_PORT->BSRR = (uint32_t) RS_PIN << 16U
#define DATA()      RS_PORT->BSRR = RS_PIN
#define WRITE()     WR_PORT->BSRR = (uint32_t) WR_PIN << 16U;\
                    WR_PORT->BSRR = WR_PIN

typedef enum tft_location_t
{
    LOCATION_TOP, LOCATION_MID, LOCATION_BOTTOM
} tft_location_t;


class ILI9341
{
private:
    uint8_t m_rotation;
    uint16_t m_width;
    uint16_t m_height;
    uint32_t m_pixelCount;

    void SendCommand(uint8_t com);
    void SendData(uint8_t data);
    void SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

    void DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta,
            uint16_t color);

public:
    ILI9341(uint8_t rotation);
    virtual ~ILI9341()
    {
    }

    void Reset();
    void Init();

    void SetRotation(uint8_t rotation);
    uint8_t GetRotation() const;

    uint16_t Width() const;
    uint16_t Height() const;

    void DrawPixel(uint16_t x, uint16_t y, uint16_t color);

    void Fill(uint16_t color);
    void FillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    void DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

    void DrawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    void DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
            uint16_t color);
    void FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2,
            uint16_t color);

    void DrawChar(int16_t x, int16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
    void Print(char text[], int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size);
    void Print(label_t *label);

    void Image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data, uint32_t size);
    void Image(img_data_t *img);
};

#endif /* SRC_ILI9341_H_ */
