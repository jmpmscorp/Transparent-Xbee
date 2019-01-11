#include "ArduinoXbee.h"

Xbee::Xbee(Stream &serial) {
    _serial = &serial;
}

Stream * Xbee::getSerial() {
    return _serial;
}

bool Xbee::enterCommandMode() {
    debugPrintLn(F("+++"));
    _serial->print(F("+++"));

    return waitForOK(2500);
}

bool Xbee::exitCommandMode() {
    sendATCommand(F("CN"));

    return waitForOK();
}

bool Xbee::saveParametersOnNVRAM() {
    sendATCommand(F("WR"));

    return waitForOK(2500);
}

ATResponse Xbee::waitForResponse( char * buffer, size_t size, uint32_t timeout) {
    uint32_t now = millis();

    while(millis() - now < timeout) {
        size_t len = readLine(buffer, size, timeout);

        if(len > 0) {
            if(strncmp_P(buffer, PSTR("OK"), 2) == 0) {
                return ATResponse::OK;
            }
            else if(strncmp_P(buffer, PSTR("ERROR"), 5) == 0) {
                return ATResponse::ERROR;
            }
            else {
                return ATResponse::VALUE;
            }
        }
    }

    return ATResponse::TIMEOUT;
}

bool Xbee::waitForOK(uint32_t timeout) {
    char responseBuffer[6];
    uint32_t now = millis();

    while(millis() - now < timeout) {
        if(waitForResponse(responseBuffer, sizeof(responseBuffer)) == ATResponse::OK) {
            return true;
        }
    }

    return false;

}

void Xbee::setDebugSerial(Stream &serial) {
    _debugSerial = &serial;
}

void Xbee::sendATCommand(const __FlashStringHelper * cmd) {
    sendAT();
    _serial->print(cmd);
    debugPrint(cmd);
    sendCR();
}

void Xbee::sendATCommand(const __FlashStringHelper * cmd, int value) {
    sendAT();
    _serial->print(cmd);
    _serial->print(value, HEX);
    debugPrint(cmd);
    debugPrint(value);
    sendCR();
}

void Xbee::sendATCommand(const __FlashStringHelper * cmd, const char * str) {
    sendAT();
    _serial->print(cmd);
    _serial->print(str);
    debugPrint(cmd);
    debugPrint(str);
    sendCR();
}
void Xbee::sendATCommand(const char * cmd) {
    sendAT();
    _serial->print(cmd);
    debugPrint(cmd);
    sendCR();
}

void Xbee::sendATCommand(const char * cmd, int value) {
    sendAT();
    _serial->print(cmd);
    _serial->print(value, HEX);
    debugPrint(cmd);
    debugPrint(value);
    sendCR();
}

void Xbee::sendATCommand(const char * cmd, const char * str) {
    sendAT();
    _serial->print(cmd);
    _serial->print(str);
    debugPrint(cmd);
    debugPrint(str);
    sendCR();
}

inline void Xbee::sendAT() {
    _serial->print(F("AT"));
    debugPrint(F("AT"));
}

inline void Xbee::sendCR() {
    _serial->print('\r');
    _serial->flush();
    debugPrintLn();
}
// Returns a character from the modem stream if read within _timeout ms or -1 otherwise.
int Xbee::readByte(uint32_t timeout) const
{
    int c;
    uint32_t _startMillis = millis();

    do {
        c = _serial->read();
        if (c >= 0) {
            Serial.print((char)c);
            return c;
        }
    } while (millis() - _startMillis < timeout);

    return -1; // -1 indicates timeout
}

// Fills the given "buffer" with characters read from the modem stream up to "length"
// maximum characters and until the "terminator" character is found or a character read
// times out (whichever happens first).
// The buffer does not contain the "terminator" character or a null terminator explicitly.
// Returns the number of characters written to the buffer, not including null terminator.
size_t Xbee::readBytesUntil(char terminator, char* buffer, size_t length, uint32_t timeout) {
    if (length < 1) {
        return 0;
    }

    size_t index = 0;
    
    while (index < length) {
        int c = readByte(timeout);

        
        if (c < 0 || c == terminator) {
            break;
        }
        
        *buffer++ = static_cast<char>(c);        
        index++;
    }

    if (index < length) {
        *buffer = '\0';
    }

    // TODO distinguise timeout from empty string?
    // TODO return error for overflow?
    return index; // return number of characters, not including null terminator
}

// Fills the given "buffer" with up to "length" characters read from the modem stream.
// It stops when a character read times out or "length" characters have been read.
// Returns the number of characters written to the buffer.
size_t Xbee::readBytes(uint8_t* buffer, size_t length, uint32_t timeout) {
    size_t count = 0;

    while (count < length) {
        int c = readByte(timeout);

        if (c < 0) {
            break;
        }
        
        *buffer++ = static_cast<uint8_t>(c);
        count++;
    }

    // TODO distinguise timeout from empty string?
    // TODO return error for overflow?
    return count;
}

// Reads a line from the modem stream into the "buffer".
// The buffer is terminated with null.
// Returns the number of bytes read, not including the null terminator.
size_t Xbee::readLine(char* buffer, size_t size, uint32_t timeout) {
    // Use size-1 to leave room for a string terminator

    size_t len = readBytesUntil('\r', buffer, size - 1, timeout);
    
    
    // check if the terminator is more than 1 characters, then check if the first character of it exists 
    // in the calculated position and terminate the string there
    if (buffer[len - 1] == '\r') {
        --len;
    }

    // terminate string, there should always be room for it (see size-1 above)
    buffer[len] = '\0';

    return len;
}