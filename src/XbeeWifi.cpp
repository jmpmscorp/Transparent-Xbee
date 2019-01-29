#include "XbeeWifi.h"

XbeeWifi::XbeeWifi(Stream &serial) : Xbee(serial) {
}

XbeeWifi::XbeeWifi(Stream &serial, uint8_t ctsPin, uint8_t sleepRQPin, uint8_t tcpStatusPin) 
    : Xbee(serial, ctsPin, sleepRQPin),
      _tcpStatusPin(tcpStatusPin) 
{
}

void XbeeWifi::init() const
{
    Xbee::init();

    if(_tcpStatusPin > 0)
    {
        pinMode(_tcpStatusPin, INPUT_PULLUP);
    }
}

int8_t XbeeWifi::readTcpStatusPin() const
{
    if(_tcpStatusPin <= 0) {
        return -1;
    }

    return digitalRead(_tcpStatusPin);
}