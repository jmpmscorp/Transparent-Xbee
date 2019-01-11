#include "TransparentXbeeWifiClient.h"

TransparentXbeeWifiClient::TransparentXbeeWifiClient(Xbee &xbee) {
    _xbee = &xbee;
}

int TransparentXbeeWifiClient::connect(IPAddress ip, uint16_t port) {
    return -1;
}

int TransparentXbeeWifiClient::connect(const char *host, uint16_t port) {
    if(_xbee->enterCommandMode()) {
        _xbee->sendATCommand(F("DL"), host);

        if(!_xbee->waitForOK()) {
            return -1;
        }

        _xbee->sendATCommand(F("C0"), port);

        if(!_xbee->waitForOK()) {
            return -1;
        }

        _xbee->sendATCommand(F("DE"), port);

        if(!_xbee->waitForOK()) {
            return -1;
        }

        _xbee->saveParametersOnNVRAM();
        _xbee->exitCommandMode();      
    }
}

size_t TransparentXbeeWifiClient::write(uint8_t c) {
    return _xbee->getSerial()->write(c);
}

size_t TransparentXbeeWifiClient::write(const uint8_t *buf, size_t size) {
    return _xbee->getSerial()->write(buf, size);
}

int TransparentXbeeWifiClient::available() {
    return _xbee->getSerial()->available();
}

int TransparentXbeeWifiClient::read() {
    return _xbee->getSerial()->read();
}

int TransparentXbeeWifiClient::read(uint8_t *buf, size_t size) {
    return _xbee->getSerial()->readBytes(buf, size);
}

int TransparentXbeeWifiClient::peek() {
    return _xbee->getSerial()->peek();
}

void TransparentXbeeWifiClient::flush() {
    return _xbee->getSerial()->flush();
}

void TransparentXbeeWifiClient::stop() {
    return;
}

uint8_t TransparentXbeeWifiClient::connected() {
    return 1;
}