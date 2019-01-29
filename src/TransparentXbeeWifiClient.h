#pragma once

#ifndef __TRANSPARENT_XBEE_WIFI_CLIENT_H__
#define __TRANSPARENT_XBEE_WIFI_CLIENT_H__

#include "XbeeWifi.h"
#include "Client.h"

class TransparentXbeeWifiClient : public Client {
    public:
        TransparentXbeeWifiClient(XbeeWifi &xbee);

        int connect(IPAddress ip, uint16_t port);
        int connect(const char *host, uint16_t port);
        size_t write(uint8_t c);
        size_t write(const uint8_t *buf, size_t size);
        int available();
        int read();
        int read(uint8_t *buf, size_t size);
        int peek();
        void flush();
        void stop();
        uint8_t connected();
        operator bool() {return connected(); }

    private:
        XbeeWifi * _xbee;
        inline void wakeUpXbee();
};

#endif  // __TRANSPARENT_XBEE_WIFI_CLIENT_H__  