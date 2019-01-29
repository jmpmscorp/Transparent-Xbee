#pragma once

#ifndef __ARDUINO_XBEE_H__
#define __ARDUINO_XBEE_H__

#include <Arduino.h>
#include "Stream.h"

#define debugPrint(...) { if (_debugSerial) _debugSerial->print(__VA_ARGS__); }
#define debugPrintLn(...) { if (_debugSerial) _debugSerial->println(__VA_ARGS__); }

enum class ATResponse : int8_t {
    TIMEOUT = -2,
    ERROR = -1,
    OK = 0,
    VALUE = 1
};

typedef void (*WakeUpCallback) ();

class Xbee {
    public:
        Xbee(Stream &serial);
        Xbee(Stream &serial, uint8_t ctsPin, uint8_t sleepRQPin);
        
        void init();
        void sleep();
        bool isSleeping();
        void wakeUp();
        void onWakeUp(void ( *callback) ());

        bool enterCommandMode();
        bool exitCommandMode();
        bool saveParametersOnNVRAM();

        bool waitForOK(uint32_t timeout = 500);
        ATResponse Xbee::waitForResponse( char * buffer, size_t size, uint32_t timeout = 500);

        Stream * getSerial();
        void setDebugSerial(Stream &serial);

        void sendATCommand(const __FlashStringHelper * cmd);
        void sendATCommand(const __FlashStringHelper * cmd, int value);
        void sendATCommand(const __FlashStringHelper * cmd, const char * str);
        void sendATCommand(const char * cmd);
        void sendATCommand(const char * cmd, int value);
        void sendATCommand(const char * cmd, const char * str);

        inline void sendAT();
        inline void sendCR();

        size_t readLine(char * buffer, size_t length, uint32_t timeout = 500);
    
    protected:
        int readByte(uint32_t timeout = 500) const;
        size_t readBytes(uint8_t * buffer, size_t length, uint32_t timeout = 500);
        size_t readBytesUntil(char terminator, char * buffer, size_t length, uint32_t timeout = 500);

    private:
        Stream * _serial;
        Stream * _debugSerial;

        uint8_t _ctsPin = 0;
        uint8_t _sleepRQPin = 0;

        WakeUpCallback _wakeUpCallback; 
};


#endif // __ARDUINO_XBEE_H__