#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <WiFi.h>
#include <WebSocketsClient.h>

#define WIFI_SSID "yourSSID"          // Replace with your WIFI SSID
#define WIFI_PASSWORD "yourPASSWORD"  // Replace with your WIFI PASSWORD
#define SERVER_ADDRESS "192.168.x.x"  // Replace with your server's IP
#define SERVER_PORT 8000              // Replace with your server's port
#define WEBSOCKET_PATH "/ws/esp32"    // WebSocket endpoint path

class NetworkManager {
    public:
        NetworkManager();
        void loop();

    private:
        WebSocketsClient webSocket;

        void connectToWiFi();
        void setupWebSocket();
        void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
};

static NetworkManager networkManager;

#define NETWORK_LOOP() networkManager.loop()

#endif