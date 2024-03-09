#include <ArduinoBLE.h>

/*
 *    Rudimentary example of reading BLE Cycling Speed and Cadence Service (CSC) data from a bicycle cadence sensor.
 *    Expects 5 octets (example: 02 2F 0A 44 C1) representing 'crank revolutions' and 'last crank event time'.
 *    Tested with Coospo BK467 sensor on a Nano 33 BLE Sense rev2 [ABX00070].
 *    If you only need the data decoding method then skip to the bottom.
 *    
 *    Known issues which are unlikely to be fixed:
 *    - serial info messages don't always print ('connected', 'subscribed', etc.)
 *    - first couple lines of data are printed weirdly in my terminal even though the correct data is there.
 *    - skips a revolution here and there, but could be an issue with my cheapo sensor.
 */


const char* deviceName = "BK6LC-0050685";  // Replace with the exact name of your device that it broadcasts
const char* characteristicUUID = "2a5b";   // Replace with the characteristic ID. "2a5b" is the default CSC ID.
int crank_revolution;
int last_crank_event_time;

BLEDevice peripheral;
BLECharacteristic cyclingSpeedandCadence;


void setup() {
    Serial.begin(9600);
    while (!Serial);  // Wait for serial monitor to be ready. Remove this if not using serial monitor.
  
    if (!BLE.begin()) {
        Serial.println("Failed to initialize BLE!");
        while (1);
    } else {
        Serial.println("BLE initialized.");
    }
  
    Serial.println("Scanning for device: " + String(deviceName) + " ...");
    BLE.scanForName(deviceName);
}

void loop() {

    BLEDevice foundPeripheral = BLE.available();  // check for discovered peripheral devices

    if (foundPeripheral) {
        if (strcmp(foundPeripheral.localName().c_str(), deviceName) == 0) {
            Serial.println("Device " + String(deviceName) + " found, now will try to connect...");
      
            peripheral = foundPeripheral;
            BLE.stopScan();
      
            if (peripheral.connect()) {
                Serial.println("Connected!");
        
                if (peripheral.discoverAttributes()) {
                    Serial.println("Attributes discovered!");
                    cyclingSpeedandCadence = peripheral.characteristic(characteristicUUID);
        
                    if (!cyclingSpeedandCadence) {
                        Serial.println("Peripheral does not have CSC characteristic!");
                        peripheral.disconnect();
                        return;
                    } else {
                          Serial.println("Subscribing to cadence notifications...");
                          cyclingSpeedandCadence.subscribe();
                    }
                } else {
                      Serial.println("Attribute discovery failed!");
                      peripheral.disconnect();
                      return;
                }
            } else {
                  Serial.println("Failed to connect!");
                  return;
            }
        }
    }

    if (cyclingSpeedandCadence) {
        if (cyclingSpeedandCadence.valueUpdated()) {
            const uint8_t* cscValue = cyclingSpeedandCadence.value();
            size_t len = cyclingSpeedandCadence.valueLength();
    
            String receivedData = "";            
            for (size_t i = 0; i < len; i++) {
                char buf[3];
                sprintf(buf, "%02X", cscValue[i]);  // format the byte value with leading zeros
                receivedData += String(buf);
            }
            
            decode_csc_measurement(receivedData);  // decode the bytes from the device

            Serial.println("Received Data: " + String(receivedData));  // data from the device
            Serial.println("Crank Revolutions: " + String(crank_revolution));  // crank revolution count
            Serial.println("Last Crank Event Time: " + String(last_crank_event_time) + " ms");  // time in milliseconds
            Serial.println();
        }
    }
}


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
