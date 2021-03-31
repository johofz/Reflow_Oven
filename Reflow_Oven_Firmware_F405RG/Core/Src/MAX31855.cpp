/*
 * MAX31855.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#include "MAX31855.h"

MAX31855::MAX31855(SPI_HandleTypeDef *spi, GPIO_TypeDef *port, uint16_t pin)
        : m_spi(spi), m_port(port), m_pin(pin)
{
    m_spi = spi;
    m_port = port;
    m_pin = pin;
    Update();
}

void MAX31855::ConvertToExternal()
{
    m_externalTemp = 0;

    m_externalTemp += ((m_reading & (1 << 18)) >> 18) / 4.0f;    // bit 18: 2^-2
    m_externalTemp += ((m_reading & (1 << 19)) >> 19) / 2.0f;    // bit 19: 2^-1
    m_externalTemp += ((m_reading & (1 << 20)) >> 20);    // bit 20: 2^0
    m_externalTemp += ((m_reading & (1 << 21)) >> 21) * 2;    // bit 21: 2^1
    m_externalTemp += ((m_reading & (1 << 22)) >> 22) * 4;    // bit 22: 2^2
    m_externalTemp += ((m_reading & (1 << 23)) >> 23) * 8;    // bit 23: 2^3
    m_externalTemp += ((m_reading & (1 << 24)) >> 24) * 16;    // bit 24: 2^4
    m_externalTemp += ((m_reading & (1 << 25)) >> 25) * 32;    // bit 25: 2^5
    m_externalTemp += ((m_reading & (1 << 26)) >> 26) * 64;    // bit 26: 2^6
    m_externalTemp += ((m_reading & (1 << 27)) >> 27) * 128;    // bit 27: 2^7
    m_externalTemp += ((m_reading & (1 << 28)) >> 28) * 256;    // bit 28: 2^8
    m_externalTemp += ((m_reading & (1 << 29)) >> 29) * 512;    // bit 29: 2^9
    m_externalTemp += ((m_reading & (1 << 30)) >> 30) * 1024;    // bit 30: 2^10

    int sign = ((m_reading & (1 << 31)) >> 31) ? -1 : 1;    // Vorzeichen: 0: pos ; 1: neg
    m_externalTemp *= sign;
}

void MAX31855::ConvertToInternal()
{
    m_internalTemp = 0;

    m_internalTemp += ((m_reading & (1 << 4)) >> 4) / 16.0f;    // bit 4: 2^-4
    m_internalTemp += ((m_reading & (1 << 5)) >> 5) / 8.0f;    // bit 5: 2^-3
    m_internalTemp += ((m_reading & (1 << 6)) >> 6) / 4.0f;    // bit 6: 2^-2
    m_internalTemp += ((m_reading & (1 << 7)) >> 7) / 2.0f;    // bit 7: 2^-1
    m_internalTemp += ((m_reading & (1 << 8)) >> 8);    // bit 8: 2^0
    m_internalTemp += ((m_reading & (1 << 9)) >> 9) * 2;    // bit 9: 2^1
    m_internalTemp += ((m_reading & (1 << 10)) >> 10) * 4;    // bit 10: 2^2
    m_internalTemp += ((m_reading & (1 << 11)) >> 11) * 8;    // bit 11: 2^3
    m_internalTemp += ((m_reading & (1 << 12)) >> 12) * 16;    // bit 12: 2^4
    m_internalTemp += ((m_reading & (1 << 13)) >> 13) * 32;    // bit 13: 2^5
    m_internalTemp += ((m_reading & (1 << 14)) >> 14) * 64;    // bit 14: 2^6

    int sign = ((m_reading & (1 << 15)) >> 15) ? -1 : 1;    // Vorzeichen: 0: pos ; 1: neg
    m_internalTemp *= sign;
}

void MAX31855::ConvertToFaults()
{
    m_fault = (m_reading & (1 << 16)) >> 16;
    m_openCircuit = m_reading & 1;
    m_shortToGND = (m_reading & (1 << 1)) >> 1;
    m_shortToVCC = (m_reading & (1 << 2)) >> 2;
}

uint8_t MAX31855::Update()
{
    m_reading = 0;

    uint8_t tempbuf[4] =
        { 0, 0, 0, 0 };    // Temporärer Buffer für Datenübertragung

    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
    HAL_SPI_Receive(m_spi, &tempbuf[0], 4, 100);    // 4 Bytes über SPI empfangen
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);

    m_reading = (tempbuf[0] << 24) | (tempbuf[1] << 16) | (tempbuf[2] << 8) | (tempbuf[3] << 0);

    ConvertToExternal();
    ConvertToInternal();
    ConvertToFaults();

    return m_fault;
}

float MAX31855::ExternalTemp() const
{
    return m_externalTemp;
}

float MAX31855::InternalTemp() const
{
    return m_internalTemp;
}

