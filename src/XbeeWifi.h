#pragma once

#ifndef __ARDUINO_XBEE_WIFI_H__
#define __ARDUINO_XBEE_WIFI_H__

#include "ArduinoXbee.h"

class XbeeWifi : public Xbee 
{
    public:
        XbeeWifi(Stream &serial);
        XbeeWifi(Stream &serial, uint8_t ctsPin, uint8_t sleepRQPin, uint8_t tcpStatusPin);
        void init() const;
        int8_t readTcpStatusPin() const;

    private:
        uint8_t _tcpStatusPin = 0;
}; 

#endif