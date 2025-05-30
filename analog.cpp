#include <Arduino.h>

#include "eeprom.h"
#include "analog.h"
/* https://www.allaboutcircuits.com/tools/voltage-divider-calculator
 * source Voltage = max voltage of the battery
 * R2 = 100k (builtin on wemos D1 mini)
 * R1 = 220k + additional resistance (100k for 4.2v, 1.22m for 16v)
 * target volatge output should be 1v
 * 1.0 * (R1 + R2) / R2
 */

// ADC specs for wemos d1 mini
#define ADC_BITS 10
#define ADC_COUNTS (1 << ADC_BITS)
double offsetI = ADC_COUNTS >> 1;
// #define CT_CALIBRATION 30.0

int samples = 1480;
int SupplyVoltage = 3200;

// float ct_voltage = 0.0;
float ct_current = 0.0;
float ct_power = 0.0;

void analogSetup()
{
}

void analogLoop()
{
  // setup for volt meter reading with 1.2M resistor
  // int raw = analogRead(A0);
  // a_voltage = raw / 1023.0;
  // a_voltage = a_voltage * 15.4;

  // setup for ct sensor reading with 30.0 amps max
  double sumI, sqI, filteredI;
  for (unsigned int n = 0; n < samples; n++)
  {
    int sampleI = analogRead(A0);
    offsetI = (offsetI + (sampleI - offsetI) / 1024);
    filteredI = sampleI - offsetI;
    sqI = filteredI * filteredI;
    sumI += sqI;
  }
  double I_RATIO = ct_calibration * ((SupplyVoltage / 1000.0) / (ADC_COUNTS));

  ct_voltage = ct_voltage;
  ct_current = I_RATIO * sqrt(sumI / samples);

  ct_power = (ct_current * ct_voltage * ct_pf); // watt(s) / power apparent
}
