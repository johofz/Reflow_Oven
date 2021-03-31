/*
 * ControllerPID.h
 *
 *  Created on: 16.03.2021
 *      Author: jh
 */

#ifndef SRC_CONTROLLERPID_H_
#define SRC_CONTROLLERPID_H_

#include "main.h"

#define MAX_I_OUTPUT 30

#define PID_KP_DEFAULT 5.0f                    // Propotrionalanteil Temperaturregler
#define PID_KI_DEFAULT 0.01f                    // Integralanteil Temperaturregler
#define PID_KD_DEFAULT 0.0f                    // Differentialanteil Temperaturregler


#define PID_MAX_OUTPUT_DEFAULT 100.0f
#define PID_MIN_OUTPUT_DEFAULT 0.0f
#define PID_DELAY_MS 1000

class ControllerPID
{
private:
    // PID Faktoren
    float m_kp;
    float m_ki;
    float m_kd;

    // PID-Sollwert
    float m_target;

    // Output-Grenzwerte
    float m_maxOutput;
    float m_minOutput;

    // Regelfehler
    float m_lastError;

    // Zeitkonstanten
    float m_T;    // Peridodendauer der Regelung

    float m_proportional;
    float m_integral;
    float m_derivative;

    // PID-Output
    float m_output;

    uint8_t m_initialized;

    void Init(float controlInput);
    void ClampIntegral();

public:
    ControllerPID();
    virtual ~ControllerPID();

    void Update(float controlInput);

    // Getter & Setter
    float GetTarget() const;
    void SetTarget(float target);

    float GetMaxOutput() const;
    void SetMaxOutput(float maxOutput);

    void ResetToDefault();

    float GetOutput() const;

    float GetKp() const;
    void SetKp(float kp);

    float GetKi() const;
    void SetKi(float ki);

    float GetKd() const;
    void SetKd(float kd);

    float GetProportional() const;
    float GetIntegral() const;
    float GetDerivative() const;

};

#endif /* SRC_CONTROLLERPID_H_ */
