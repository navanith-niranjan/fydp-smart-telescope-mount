#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <WiFi.h>
#include <WebSocketsClient.h>
#include "config.h"

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