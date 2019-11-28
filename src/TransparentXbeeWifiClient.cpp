#include "TransparentXbeeWifiClient.h"

TransparentXbeeWifiClient::TransparentXbeeWifiClient(XbeeWifi &xbee) {
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
    wakeUpXbee();
    // return _xbee->getSerial()->write(c);
    return write(&c, 1);
}

size_t TransparentXbeeWifiClient::write(const uint8_t *buf, size_t size) {
    wakeUpXbee();

    while(!_xbee->checkCommandTransactionTime(1000)) {
        _xbee->getCustomDelayFn()(100);
    }

    size_t aux = _xbee->getSerial()->write(buf, size);
    _xbee->updateTransparentTransactionTime();
    return aux;
}

int TransparentXbeeWifiClient::available() {
    //wakeUpXbee();
    return _xbee->getSerial()->available();
}

int TransparentXbeeWifiClient::read() {
    //wakeUpXbee();
    return _xbee->getSerial()->read();
}

int TransparentXbeeWifiClient::read(uint8_t *buf, size_t size) {
    //wakeUpXbee();
    return _xbee->getSerial()->readBytes(buf, size);
}

int TransparentXbeeWifiClient::peek() {
    wakeUpXbee();
    return _xbee->getSerial()->peek();
}

void TransparentXbeeWifiClient::flush() {
    wakeUpXbee();
    return _xbee->getSerial()->flush();
}

void TransparentXbeeWifiClient::stop() {
    return;
}

uint8_t TransparentXbeeWifiClient::connected() {
    int8_t state = _xbee->readTcpStatusPin();

    if(state == -1 || state == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

inline void TransparentXbeeWifiClient::wakeUpXbee() {
    if(_xbee->isSleeping()) {
        _xbee->wakeUp();
    }
}