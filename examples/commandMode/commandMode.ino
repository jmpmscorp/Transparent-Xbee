#include "ArduinoXbee.h"

Xbee xbeeWifi(Serial1);

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);

    xbeeWifi.setDebugSerial(Serial);

    if(xbeeWifi.enterCommandMode()) {
        xbeeWifi.exitCommandMode();
    }
}

void loop() {
    while(Serial1.available()) {
        Serial.print((char)Serial1.read());
    }
}