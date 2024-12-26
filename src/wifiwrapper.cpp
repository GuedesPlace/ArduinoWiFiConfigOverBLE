#include <Arduino.h>
#include <WiFi.h>
#include "./GPWiFiWrapper.h"
#include "./GPBleWifiStatus.h"

GPWifiWrapper::GPWifiWrapper()
{
    m_status = UNKNOWN;
    m_lastStatus = WL_DISCONNECTED;
    m_callbacks = nullptr;
}
void GPWifiWrapper::setup(String deviceName)
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(deviceName.c_str());
}
void GPWifiWrapper::updateWifiCredentials(String ssid, String pw)
{
    if (m_status == CONNECTED) {
        WiFi.disconnect();
    }
    WiFi.begin(ssid.c_str(), pw.c_str());
    Serial.print("Start connecting to: ");
    Serial.println(ssid.c_str());
    if (m_status == UNKNOWN) {
        m_status = INITIALIZING;
    }
}
GPBLE_WIFISTATUS GPWifiWrapper::getStatus()
{
    return m_status;
}
void GPWifiWrapper::registerWifiStatusCallbacks(GPWifiStatusCallbacks *callbacks)
{
    m_callbacks = callbacks;
}
void GPWifiWrapper::checkStatus()
{
    if (m_status == UNKNOWN)
    {
        return;
    }
    wl_status_t currentStatus = WiFi.status();
    if (currentStatus == m_lastStatus)
    {
        return;
    }
    updateStatusByWifiStatus(currentStatus);
}
void GPWifiWrapper::updateStatusByWifiStatus(wl_status_t newStatus)
{
    switch (newStatus)
    {
    // WL_CONNECTED: assigned when connected to a WiFi network;
    case WL_CONNECTED:
        m_status = CONNECTED;
        break;
    // WL_NO_SHIELD: assigned when no WiFi shield is present;
    case WL_NO_SHIELD:
        m_status = NOTSUPPORTED;
        break;
    // WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
    case WL_IDLE_STATUS:
        m_status = CONNECTING;
        break;
    // WL_NO_SSID_AVAIL: assigned when no SSID are available;
    // WL_SCAN_COMPLETED: assigned when the scan networks is completed;
    case WL_NO_SSID_AVAIL:
        break;
    case WL_SCAN_COMPLETED:
        break;
    // WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
    case WL_CONNECT_FAILED:
        m_status = FAILED;
        break;
    // WL_CONNECTION_LOST: assigned when the connection is lost;
    case WL_CONNECTION_LOST:
        m_status = DISCONNECTED;
        break;
    // WL_DISCONNECTED
    case WL_DISCONNECTED:
        m_status = DISCONNECTED;
        break;
    }
    m_lastStatus = newStatus;
    if (m_callbacks != nullptr)
    {
        m_callbacks->onWifiStatusUpdate(m_status);
    }
}
bool GPWifiWrapper::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
