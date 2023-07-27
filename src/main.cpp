// Basic MD_AD9833 test file
//
// Initialises the device to default conditions
//
// Connect a pot to A0 to change the frequency by turning the pot
//
#include "Arduino.h"

void setup(void)
{
    Serial.begin(9600);
}

void loop(void)
{
    Serial.println(100);
}
