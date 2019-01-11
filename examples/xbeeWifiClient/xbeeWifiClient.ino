#include "TransparentXbeeWifiClient.h"

Xbee xbee(Serial1);
TransparentXbeeWifiClient netClient(xbee);

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);

    xbee.setDebugSerial(Serial);

    netClient.connect("C0A80122", 185);
}

void loop() {
    static unsigned long milliseconds = millis();
    while(netClient.available()) {
        Serial.print(netClient.read());
    }

    if(millis() - milliseconds > 5000) {
        milliseconds = millis();

        char buf[] = "Soy Arduino";
        netClient.write(buf, sizeof(buf));
    }
}