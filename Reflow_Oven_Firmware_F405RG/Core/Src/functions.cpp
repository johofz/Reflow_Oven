/*
 * functions.cpp
 *
 *  Created on: Mar 10, 2021
 *      Author: Johan
 */

#include "functions.h"

/*
 * Initialisiert System.
 */
void boot_initSystem(context_t *context)
{
    // Display initialisieren
    context->tft->Reset();
    context->tft->Init();

    context->fan->On();

    // Raumtemperatur bestimmen und PID-Regler auf Raumtemperatur setzten
    context->temp->Update();

    uint8_t roomTemp = (uint8_t) context->temp->ExternalTemp();
    float setTemp = (float) (roomTemp + 1);

    // PID-Regler Initialisieren
    context->pid->SetTarget(setTemp);
    context->pid->Update(context->temp->ExternalTemp());

    // Temperate-Modus inaktiv schalten
    context->temperate_active = 0;

    // Übergabe an Menu-State
    context->currentState = STATE_MENU_INIT;
}

/*
 * Initialisiert Menü.
 */
void menu_init(context_t *context)
{
    context->tft->Fill(COLOR_WHITE);

    uint16_t gap = (HEIGHT - pcbImg.h - thermometerImg.h) / 3;

    pcbImg.x = (WIDTH - pcbImg.w) / 2;
    pcbImg.y = gap;
    context->tft->Image(&pcbImg);

    thermometerImg.x = (WIDTH - thermometerImg.w) / 2;
    thermometerImg.y = pcbImg.y + pcbImg.h + gap;
    context->tft->Image(&thermometerImg);

    context->currentState = STATE_MENU;
}

/*
 * Ruft User-Input über Touch-Screen ab und verarbeitet diesen.
 * Leitet Programfluss entsprechend des User-Inputs weiter.
 */
void menu_update(context_t *context)
{

    if (context->touch->CheckRegion(&pcbImg))
        context->currentState = STATE_REFLOW_INIT;

    if (context->touch->CheckRegion(&thermometerImg))
        context->currentState = STATE_TEMPERATE_INIT;
}

/*
 *
 */
void reflow_init(context_t *context)
{
    context->currentState = STATE_MENU_INIT;    // noch nicht implementiert
}

void reflow_update(context_t *context)
{
}

/*
 * Initialisiert Temperierungs-Modus
 */
void temperate_init(context_t *context)
{
    // Temperaturen aktualisieren
    context->temp->Update();

    context->tft->Fill(COLOR_WHITE);

    uint16_t pad = 10;

    minusImg.x = WIDTH - minusImg.w - pad;
    minusImg.y = pad;
    plusImg.x = minusImg.x - pad - plusImg.w;
    plusImg.y = pad;
    context->tft->Image(&minusImg);
    context->tft->Image(&plusImg);

    uint16_t gap = ((HEIGHT - pad - minusImg.h) - pauseImg.h - lightOnImg.h - returnImg.h) / 4;

    pauseImg.x = WIDTH - pad - pauseImg.w;
    pauseImg.y = minusImg.y + minusImg.h + gap;
    playImg.x = pauseImg.x;
    playImg.y = pauseImg.y;

    if (context->temperate_active)
        context->tft->Image(&pauseImg);
    else
        context->tft->Image(&playImg);

    lightOnImg.x = WIDTH - pad - lightOnImg.w;
    lightOnImg.y = pauseImg.y + pauseImg.h + gap;
    lightOffImg.x = lightOnImg.x;
    lightOffImg.y = lightOnImg.y;

    if (context->light->GetDuty() == 0)
        context->tft->Image(&lightOffImg);
    else
        context->tft->Image(&lightOnImg);

    returnImg.x = WIDTH - pad - returnImg.w;
    returnImg.y = lightOnImg.y + lightOnImg.h + gap;
    context->tft->Image(&returnImg);

    char str[20];

    sprintf(str, "SOLL-TEMP");
    label_t soll_label(str, 1, pad, pad, COLOR_BLACK, COLOR_WHITE);
    context->tft->Print(&soll_label);

    sprintf(str, "IST-TEMP");
    label_t ist_label(str, 1, pad, playImg.y, COLOR_BLACK, COLOR_WHITE);
    context->tft->Print(&ist_label);

    temperate_render(context);

    context->pid_delay_ms = Millis();
    context->temperate_display_delay_ms = Millis();

    context->currentState = STATE_TEMPERATE;
}

void temperate_update(context_t *context)
{
    uint8_t updateDisplay = 0;

    if (context->touch->CheckRegion(&plusImg))
    {
        uint16_t target = context->pid->GetTarget();
        target++;
        context->pid->SetTarget(target);
        updateDisplay = 1;
    }

    if (context->touch->CheckRegion(&minusImg))
    {
        uint16_t target = context->pid->GetTarget();
        if (target != 0)
            target--;
        context->pid->SetTarget(target);
        updateDisplay = 1;
    }

    if (context->temperate_active == 0)
    {
        if (context->touch->CheckRegion(&playImg))
        {
            context->tft->Image(&pauseImg);
            context->temperate_active = 1;
        }
    }
    else
    {
        if (context->touch->CheckRegion(&pauseImg))
        {
            context->tft->Image(&playImg);
            context->temperate_active = 0;
        }
    }

    if (context->touch->CheckRegion(&lightOnImg))
    {
        if (context->light->GetDuty() == 0)
        {
            context->light->On();
            context->tft->Image(&lightOnImg);
        }
        else
        {
            context->light->Off();
            context->tft->Image(&lightOffImg);
        }
    }

    if (context->touch->CheckRegion(&returnImg))
    {
        context->currentState = STATE_MENU_INIT;
    }

    // PID-Controller aktualisieren und Heater ansteuern, wenn aktiv.
    if (Millis() > context->pid_delay_ms)
    {
        if (context->temperate_active)
        {
            context->temp->Update();
            context->pid->Update(context->temp->ExternalTemp());
            uint8_t duty = (uint8_t) context->pid->GetOutput();
            context->heater->SetDuty(duty);
        }
        else
        {
            context->heater->Off();
        }

        context->pid_delay_ms = Millis() + PID_DELAY_MS;
    }

    // Display aktualisieren, wenn Input erfolgt, oder Delay abgelaufen ist.
    if (updateDisplay || Millis() > context->temperate_display_delay_ms)
    {
        context->temp->Update();
        temperate_render(context);

        context->temperate_display_delay_ms = Millis() + DELAY_DISPLAY_UPDATE_MS;
    }
}

/*
 * Hilfsfunktion, die die Benutzeroberfläche im Temperier-Modus aktualisiert
 */
void temperate_render(context_t *context)
{
    uint16_t pad = 10;
    static label_t soll_value(3, pad, CHAR_HEIGHT + 2 * pad, COLOR_BLUE, COLOR_WHITE);
    static label_t ist_value(3, pad, playImg.y + CHAR_HEIGHT + pad, COLOR_BLUE, COLOR_WHITE);
    static label_t duty_label(1, pad, lightOnImg.y, COLOR_BLACK, COLOR_WHITE);
    static label_t p_label(2, pad, lightOnImg.y + lightOnImg.h + pad, COLOR_BLACK, COLOR_WHITE);
    static label_t i_label(2, pad, p_label.y + p_label.size * CHAR_HEIGHT + pad, COLOR_BLACK, COLOR_WHITE);
    static label_t d_label(2, pad, i_label.y + i_label.size * CHAR_HEIGHT + pad, COLOR_BLACK, COLOR_WHITE);

    sprintf(soll_value.str, "%.2f", context->pid->GetTarget());
    sprintf(ist_value.str, "%.2f", context->temp->ExternalTemp());
    sprintf(duty_label.str, "Duty: %3i", context->heater->GetDuty());
    sprintf(p_label.str, "P %.2f", context->pid->GetProportional());
    sprintf(i_label.str, "I %.2f", context->pid->GetIntegral());
    sprintf(d_label.str, "D %.2f", context->pid->GetDerivative());

    context->tft->Print(&soll_value);
    context->tft->Print(&ist_value);
    context->tft->Print(&duty_label);
    context->tft->Print(&p_label);
    context->tft->Print(&i_label);
    context->tft->Print(&d_label);

    uint16_t x1 = pad + (context->heater->GetDuty() * (lightOnImg.x - 2 * pad) / 100);

    context->tft->FillRect(pad, duty_label.y + duty_label.size * CHAR_HEIGHT + pad,
            lightOnImg.x - pad, lightOnImg.y + lightOnImg.h, COLOR_WHITE);
    context->tft->DrawRect(pad, duty_label.y + duty_label.size * CHAR_HEIGHT + pad,
            lightOnImg.x - pad, lightOnImg.y + lightOnImg.h, COLOR_BLACK);
    context->tft->FillRect(pad, duty_label.y + duty_label.size * CHAR_HEIGHT + pad,
            x1, lightOnImg.y + lightOnImg.h, COLOR_RED);
}

/*
 * Initialisiert Optionen.
 */
void options_init(context_t *context)
{
    context->currentState = STATE_OPTIONS;
}

/*
 * Initialisiert System.
 */
void options_update(context_t *context)
{
    context->currentState = STATE_MENU_INIT;
}

/*
 * Gibt Fehlermeldung auf Display aus.
 */
void error_show(context_t *context)
{
}

/*
 * Zeit seit Systemstart in µs.
 * Basis für alle Zeiterfassung ist TIM2 (32-bit timer).
 * Zusammen mit interrupt_data.timer_overflow kann eine Gesamtzeit von ca. 3257 Tagen erreicht werden
 */
uint64_t Micros()
{
    return TIM2->CNT + (interrupt_data.timerOverflow - 1) * 0xffffffff;
}

/*
 * Zeit seit Systemstart in ms.
 */
uint64_t Millis()
{
    return Micros() / 1000;
}

/*
 * Zeit seit Systemstart in s.
 */
uint64_t Seconds()
{
    return Micros() / 1000000;
}

/*
 * Blocking Delay in µs.
 * Maximal 65535µs.
 */
void Delay_US(uint64_t us)
{
    uint64_t delay = Micros() + us;
    while (Micros() < delay);
}

/*
 * Blocking Delay in ms.
 * Maximal 65535ms.
 */
void Delay_MS(uint64_t ms)
{
    uint64_t delay = Millis() + ms;
    while (Millis() < delay);
}

