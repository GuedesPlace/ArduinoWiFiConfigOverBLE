#ifndef GPWifiStatusCharacteristiCallBack_h
#define GPWifiStatusCharacteristiCallBack_h

#include <BLECharacteristic.h>
#include <ArduinoJson.h>
#include "../GPBleWrapper.h"

class WifiStatusCharacteristiCallBack : public BLECharacteristicCallbacks
{
public:
    WifiStatusCharacteristiCallBack(GPBleWrapper *wrapper)
    {
        m_wrapper = wrapper;
    }
    void onRead(BLECharacteristic *pCharacteristic)
    {
        String message = m_wrapper->getBLEWifiStatusAsString();
        pCharacteristic->setValue(message.c_str());
    }

private:
    GPBleWrapper *m_wrapper;
};

class WifiCharacteristicCallback : public BLECharacteristicCallbacks
{
public:
    WifiCharacteristicCallback(GPBleWrapper *wrapper)
    {
        m_wrapper = wrapper;
    }
    void onRead(BLECharacteristic *pCharacteristic)
    {
        Serial.println("Read WIFIDATA");
        JsonDocument doc;
        doc["ssid"] = m_wrapper->execGetSSID();
        doc["pw"] = m_wrapper->execGetPassword();
        String message;
        serializeJson(doc, message);
        Serial.print("Payload: ");
        Serial.println(message);
        pCharacteristic->setValue(message.c_str());
    }
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String value = pCharacteristic->getValue().c_str();
        JsonDocument doc;
        deserializeJson(doc, value);
        m_wrapper->execNotifyWifiCredentials(doc["ssid"], doc["pw"]);
    }

private:
    GPBleWrapper *m_wrapper;
};
#endif