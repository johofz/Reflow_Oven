/*
 * MAX31855.h
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#ifndef SRC_MAX31855_H_
#define SRC_MAX31855_H_

#include "main.h"
#include <string.h>
#include <math.h>

void Delay_US(uint64_t us);

class MAX31855
{
private:
    SPI_HandleTypeDef *m_spi;    // Handle zu SPI-Treiber
    GPIO_TypeDef *m_port;    // CS-Port
    uint16_t m_pin;    // CS-Pin

    uint32_t m_reading;    // 32bit Reading von IC

    float m_externalTemp;    // Temperatur an Messstelle
    float m_internalTemp;    // Temperatur intern

    uint8_t m_fault;    // Fehler-Flag (allgemein)
    uint8_t m_openCircuit;    // Flag: Offene Messstelle
    uint8_t m_shortToGND;    // Flag: Kurzschluss zu GND
    uint8_t m_shortToVCC;    // Flag: Kurzschluss zu VCC

    void ConvertToExternal();    // Konvertiert m_reading zu m_externalTemp
    void ConvertToInternal();    // Konvertiert m_reading zu m_internalTemp
    void ConvertToFaults();    // Konvertiert m_reading zu Fehler-Flags

public:
    MAX31855(SPI_HandleTypeDef *spi, GPIO_TypeDef *port, uint16_t pin);
    virtual ~MAX31855()
    {
    }

    uint8_t Update();    // Ruft neue Werte von Sensor ab und gibt Fehlerstatus zurück

    float ExternalTemp() const;    // Gibt externe Temp. aus (Messstelle)
    float InternalTemp() const;    // Gibt interne Temp. aus
};

#endif /* SRC_MAX31855_H_ */
