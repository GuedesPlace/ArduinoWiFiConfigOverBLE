#ifndef GPBleWrapper_h
#define GPBleWrapper_h

#include <BLEServer.h>
#include "./GPBleWifiStatus.h"
#include "./GPConfigurationProperties.h"


class GPBleWrapper;

class GPBleUpdateValueCallbacks
{
public:
    virtual ~GPBleUpdateValueCallbacks(){};
    virtual void onWifiCredentialsUpdate(String ssid, String password){};
    virtual void onConfigPropertyUpdate(GPConfigurationProperties *properties){};
    virtual String getSSID() { return ""; };
    virtual String getPassword() { return ""; };
    virtual String getPublicName() { return ""; };
    virtual GPConfigurationProperties* getProperties() {return new GPConfigurationProperties(0);};
};

class GPBleServerCallbacks : public BLEServerCallbacks
{
public:
    GPBleServerCallbacks(GPBleWrapper *wrapper);
    void onConnect(BLEServer *pServer);
    void onDisconnect(BLEServer *pServer);

private:
    GPBleWrapper *m_wrapper;
};

class GPBleWrapper
{
public:
    GPBleWrapper();
    void setup(String deviceName);
    void updateWIFIStatus(GPBLE_WIFISTATUS wifiStatus);
    void registerUpdateValueCallBacks(GPBleUpdateValueCallbacks *callback);
    void execNotifyWifiCredentials(String ssid, String password);
    void execNotifyConfigurationProperties(GPConfigurationProperties *properties);
    String execGetSSID();
    String execGetPassword();
    String getBLEWifiStatusAsString();
    GPConfigurationProperties* getConfigurationProperties();

private:
    GPBLE_WIFISTATUS m_wifiStatus = UNKNOWN;
    BLECharacteristic *m_wifiStatusCharacteristic;
    BLECharacteristic *m_wifiDefinitionCharacteristic;
    BLECharacteristic *m_publicNameCharacteristic;
    BLECharacteristic *m_propertiesCharacteristic;
    GPBleUpdateValueCallbacks *m_updateValueCallbacks = nullptr;
    void createWifiStatusCharacteristic(BLEService *service);
    void createWifiDefinitionCharacteristic(BLEService *service);
    void createPropertiesChararcteristic(BLEService *service);
};
#endif
