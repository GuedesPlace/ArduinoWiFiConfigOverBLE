#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "./gpblewrapper.h"
#include "./callbacks/PropertiesCharacteristicCallBack.h"
#include "./callbacks/WifiStatusCallback.h"

#define SERVICE_UUID "d01c3000-eb86-4576-a46d-3239440100da"
#define WIFI_STATUS_CHARACTERISTIC_UUID "d01c3001-eb86-4576-a46d-3239440100da"
#define WIFI_DEFINITION_CHARACTERISTIC_UUID "d01c3002-eb86-4576-a46d-3239440100da"
//#define PUBLIC_NAME_CHARACTERISTIC_UUID "d01c3003-eb86-4576-a46d-3239440100da"
#define PROPERTY_CHARACTERISTIC_UUID "d01c3004-eb86-4576-a46d-3239440100da"

GPBleServerCallbacks::GPBleServerCallbacks(GPBleWrapper *wrapper)
{
    m_wrapper = wrapper;
}
void GPBleServerCallbacks::onConnect(BLEServer *pServer)
{
    Serial.println("CONNECT");
    BLECharacteristic *wStatus = pServer->getServiceByUUID(SERVICE_UUID)->getCharacteristic(WIFI_STATUS_CHARACTERISTIC_UUID);
}

void GPBleServerCallbacks::onDisconnect(BLEServer *pServer)
{
    Serial.println("NOT CONNECT");
    delay(500);
    pServer->startAdvertising();
}

GPBleWrapper::GPBleWrapper()
{
    m_wifiStatus = UNKNOWN;
    m_wifiStatusCharacteristic = nullptr;
    m_wifiDefinitionCharacteristic = nullptr;
    m_publicNameCharacteristic = nullptr;
    m_updateValueCallbacks = nullptr;
}

void GPBleWrapper::setup(String deviceName)
{
    BLEDevice::init(deviceName.c_str());
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    createWifiDefinitionCharacteristic(pService);
    createWifiStatusCharacteristic(pService);
    createPropertiesChararcteristic(pService);
    pServer->setCallbacks(new GPBleServerCallbacks(this));
    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
    if (this->m_wifiStatusCharacteristic == NULL)
    {
        Serial.println("SETUP:_hmm... null?");
    }
}
void GPBleWrapper::createWifiStatusCharacteristic(BLEService *pService)
{
    Serial.println("handling statusCharacteristic");
    m_wifiStatusCharacteristic = pService->createCharacteristic(
        WIFI_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);
    m_wifiStatusCharacteristic->addDescriptor(new BLE2902());
    m_wifiStatusCharacteristic->setCallbacks(new WifiStatusCharacteristiCallBack(this));
}
void GPBleWrapper::createWifiDefinitionCharacteristic(BLEService *pService)
{
    m_wifiDefinitionCharacteristic = pService->createCharacteristic(
        WIFI_DEFINITION_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE | BLECharacteristic::PROPERTY_WRITE);
    m_wifiDefinitionCharacteristic->addDescriptor(new BLE2902());
    m_wifiDefinitionCharacteristic->setCallbacks(new WifiCharacteristicCallback(this));
}
void GPBleWrapper::createPropertiesChararcteristic(BLEService *pService)
{
    m_propertiesCharacteristic = pService->createCharacteristic(
        PROPERTY_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);
    m_publicNameCharacteristic->addDescriptor(new BLE2902());
    m_publicNameCharacteristic->setCallbacks(new PropertiesCharacteristicCallBack(this));
}

void GPBleWrapper::updateWIFIStatus(GPBLE_WIFISTATUS wifiStatus)
{
    m_wifiStatus = wifiStatus;
    if (m_wifiStatusCharacteristic == NULL)
    {
        Serial.println("hmm... null?");
    }
    String statusAsString = getBLEWifiStatusAsString();
    m_wifiStatusCharacteristic->setValue(statusAsString.c_str());
    m_wifiStatusCharacteristic->notify(true);
}
void GPBleWrapper::registerUpdateValueCallBacks(GPBleUpdateValueCallbacks *callbacks)
{
    m_updateValueCallbacks = callbacks;
}
void GPBleWrapper::execNotifyWifiCredentials(String ssid, String password)
{
    if (m_updateValueCallbacks != nullptr)
    {
        m_updateValueCallbacks->onWifiCredentialsUpdate(ssid, password);
    }
}
void GPBleWrapper::execNotifyConfigurationProperties(GPConfigurationProperties *properties)
{
    if (m_updateValueCallbacks != nullptr)
    {
        m_updateValueCallbacks->onConfigPropertyUpdate(properties);
    }
}
String GPBleWrapper::execGetSSID()
{
    if (m_updateValueCallbacks != nullptr)
    {
        return m_updateValueCallbacks->getSSID();
    }
    return "";
}
String GPBleWrapper::execGetPassword()
{
    if (m_updateValueCallbacks != nullptr)
    {
        return m_updateValueCallbacks->getPassword();
    }
    return "";
}
GPConfigurationProperties *GPBleWrapper::getConfigurationProperties()
{
    if (m_updateValueCallbacks != nullptr)
    {
        return m_updateValueCallbacks->getProperties();
    }
    return nullptr;
}

String GPBleWrapper::getBLEWifiStatusAsString()
{
    switch (m_wifiStatus)
    {
    case UNKNOWN:
        return "unknown";
        break;
    case INITIALIZING:
        return "initalizing";
        break;
    case CONNECTED:
        return "connected";
        break;
    case CONNECTING:
        return "connecting";
        break;
    case DISCONNECTED:
        return "disconnected";
        break;
    case FAILED:
        return "failed";
        break;
    case NOTSUPPORTED:
        return "notsupported";
        break;
    default:
        Serial.println("strange...");
        return "strange";
    }
}
