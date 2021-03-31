/*
 * ControllerPID.cpp
 *
 *  Created on: 16.03.2021
 *      Author: jh
 */

#include "ControllerPID.h"

ControllerPID::ControllerPID()
        : m_kp(PID_KP_DEFAULT), m_ki(PID_KI_DEFAULT), m_kd(PID_KD_DEFAULT),
          m_target(0), m_maxOutput(PID_MAX_OUTPUT_DEFAULT), m_minOutput(PID_MIN_OUTPUT_DEFAULT),
          m_T(PID_DELAY_MS / 1000.0f), m_initialized(0)
{
}

void ControllerPID::Init(float controlInput)
{
    // letzten Fehler initialisieren
    m_lastError = m_target - controlInput;

    // Integral-Anteil initialisieren
    m_integral = 0;

    m_initialized = 1;
}

void ControllerPID::ClampIntegral()
{
    float minIntegral, maxIntegral;

    if (m_maxOutput > m_proportional)
        maxIntegral = m_maxOutput - m_proportional;
    else
        maxIntegral = 0.0f;

    if (m_minOutput < m_proportional)
        minIntegral = m_minOutput - m_proportional;
    else
        minIntegral = 0.0f;

    if (m_integral > maxIntegral)
        m_integral = maxIntegral;
    else if (m_integral < minIntegral)
        m_integral = minIntegral;
}

void ControllerPID::Update(float controlInput)
{
    if (m_initialized == 0)
    {
        Init(controlInput);
    }

    // Regelfehler aktualisieren
    float error = m_target - controlInput;

    // Proportional-Anteil aktualisieren
    m_proportional = m_kp * error;

    // Integral-Anteil aktualisieren
    m_integral +=  m_ki * error;
    ClampIntegral();

    // Differenzial-Anteil aktualisieren
    m_derivative = m_kd * (error - m_lastError);

    // Output aktualisieren
    m_output = m_proportional + m_integral + m_derivative;

    // Output-Clamping
    if (m_output > m_maxOutput)
        m_output = m_maxOutput;
    else if (m_output < m_minOutput)
        m_output = m_minOutput;

    // letzten Fehler aktualisieren
    m_lastError = error;
}

float ControllerPID::GetOutput() const
{
    return m_output;
}

float ControllerPID::GetTarget() const
{
    return m_target;
}

void ControllerPID::SetTarget(float target)
{
    // Soll-Wert überprüfen und setzten
    m_target = target;
    if (m_target > m_maxOutput)
        m_target = m_maxOutput;
    else if (m_target < m_minOutput)
        m_target = m_minOutput;
}

float ControllerPID::GetKp() const
{
    return m_kp;
}

void ControllerPID::SetKp(float kp)
{
    m_kp = kp;
}

float ControllerPID::GetKi() const
{
    return m_ki;
}

void ControllerPID::SetKi(float ki)
{
    m_ki = ki;
}

float ControllerPID::GetKd() const
{
    return m_kd;
}

void ControllerPID::SetKd(float kd)
{
    m_kd = kd;
}

float ControllerPID::GetProportional() const
{
    return m_proportional;
}

float ControllerPID::GetIntegral() const
{
    return m_integral;
}

float ControllerPID::GetDerivative() const
{
    return m_derivative;
}

float ControllerPID::GetMaxOutput() const
{
    return m_maxOutput;
}

void ControllerPID::SetMaxOutput(float maxOutput)
{
    m_maxOutput = maxOutput;
}

void ControllerPID::ResetToDefault()
{
    m_kp = PID_KP_DEFAULT;
    m_ki = PID_KI_DEFAULT;
    m_kd = PID_KD_DEFAULT;
}

ControllerPID::~ControllerPID()
{
}

