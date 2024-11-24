#ifndef WifiWrapper_h
#define WifiWrapper_h
#include <WiFi.h>
#include "./GPBleWifiStatus.h"

class GPWifiStatusCallbacks {
    public:
        virtual ~GPWifiStatusCallbacks(){};
        virtual void onWifiStatusUpdate(GPBLE_WIFISTATUS wifiStatus);
};

class GPWifiWrapper {
public:
    GPWifiWrapper();
    void setup(String deviceName);
    void updateWifiCredentials(String ssid, String pw);
    GPBLE_WIFISTATUS getStatus();
    void registerWifiStatusCallbacks(GPWifiStatusCallbacks* callbacks);
    void checkStatus();
    bool isConnected();

private:
    void updateStatusByWifiStatus(wl_status_t newStatus);
    GPBLE_WIFISTATUS m_status;
    GPWifiStatusCallbacks* m_callbacks;
    wl_status_t m_lastStatus;
};

#endif