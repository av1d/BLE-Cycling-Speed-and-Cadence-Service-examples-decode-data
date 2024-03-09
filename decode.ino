#include <ArduinoBLE.h>

int crank_revolution;
int last_crank_event_time;

void setup() {
  // your setup here
}

voil loop() {
  // your main loop here
}

/*
 * call this function to decode
 */
void decode_csc_measurement(String hex_data) {  // decode the bytes received from the sensor
    int dataLength = hex_data.length();
    const int maxDataSize = 64;  // maximum expected data size
    byte bytes[maxDataSize];

    for (int i = 0; i < dataLength; i += 2) {
        byte b = 0;
        b = (hexDigitToDec(hex_data[i]) << 4) + hexDigitToDec(hex_data[i + 1]);
        bytes[i / 2] = b;
    }

    byte flags = bytes[0];
    bool crank_revolution_present = flags & 0b10;
    bool last_crank_event_time_present = flags & 0b10;

    crank_revolution = -1;
    last_crank_event_time = -1;

    if (crank_revolution_present && dataLength >= 3) {
        crank_revolution = bytes[1] + (bytes[2] << 8);
    }

    if (last_crank_event_time_present && dataLength >= 5) {
        last_crank_event_time = bytes[3] + (bytes[4] << 8);
    }
    
    // put your returns or whatever here
}

int hexDigitToDec(char digit) {
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    } else if (digit >= 'A' && digit <= 'F') {
        return digit - 'A' + 10;
    } else if (digit >= 'a' && digit <= 'f') {
        return digit - 'a' + 10;
    } else {
        return 0;  // or handle error
    }
}
