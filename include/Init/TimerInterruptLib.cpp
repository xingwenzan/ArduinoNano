//
// Created by 86159 on 2023-05-17.
//
#include "../lib/TimerOne/TimerOne.h"

void callback() {
    Serial.println("Timer1 overflow interrupt triggered.");
}

void setup() {
    Timer1.initialize(1000000);         // initialize timer1,
// set a timer of length 1000000 microseconds (or 1 sec - the default)
    Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
    Serial.begin(9600);
}

void loop() {
    // your program here...
}
