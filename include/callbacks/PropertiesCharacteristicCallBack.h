#ifndef GPBPropertiesCharacteristicCallBack_h
#define GPBPropertiesCharacteristicCallBack_h

#include <BLECharacteristic.h>
#include "../GPBleWrapper.h"
#include <ArduinoJson.h>

class PropertiesCharacteristicCallBack : public BLECharacteristicCallbacks
{
public:
    PropertiesCharacteristicCallBack(GPBleWrapper *wrapper)
    {
        m_wrapper = wrapper;
    }
    void onRead(BLECharacteristic *pCharacteristic)
    {
        GPConfigurationProperties *properties = m_wrapper->getConfigurationProperties();
        JsonDocument doc;
        JsonArray docAsArrray = doc.to<JsonArray>();
        if (properties->getCapacity() > 0)
        {
            for (int x=0; x < properties->getCapacity(); x++)
            {
                JsonObject jsonProperty = docAsArrray.add<JsonObject>();
                gpConfigProperty property = properties->getProperty(x);
                jsonProperty["key"] = property.key;
                jsonProperty["label"] = property.label;
                jsonProperty["value"] = property.value;
            }
        }
        String message;
        serializeJson(doc, message);
        pCharacteristic->setValue(message.c_str());
    }
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String value = pCharacteristic->getValue().c_str();
        JsonDocument doc;
        deserializeJson(doc, value);
        if (!doc.isNull())
        {
            JsonArray docAsArrray = doc.as<JsonArray>();
            GPConfigurationProperties* properties = new GPConfigurationProperties(docAsArrray.size());
            int counter = 0;
            for (JsonVariant content : docAsArrray)
            {
                gpConfigProperty property = {content["key"],content["label"],content["value"]};
                properties->setProperty(property, counter);
                counter++;
            }
            m_wrapper->execNotifyConfigurationProperties(properties);
        }
    }

private:
    GPBleWrapper *m_wrapper;
};

#endif