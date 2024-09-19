#include "NetworkManager.h"

// Constructor
NetworkManager::NetworkManager() {
    Serial.begin(115200);
    delay(1000);

    connectToWiFi();
    setupWebSocket();
}

void NetworkManager::connectToWiFi() {
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void NetworkManager::setupWebSocket() {
    webSocket.begin(SERVER_ADDRESS, SERVER_PORT, WEBSOCKET_PATH);
    webSocket.onEvent(std::bind(&NetworkManager::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    Serial.println("WebSocket setup complete");
}

void NetworkManager::loop() {
    webSocket.loop();
}

void NetworkManager::webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.println("Disconnected from WebSocket server");
            break;
        case WStype_CONNECTED:
            Serial.println("Connected to WebSocket server");
            break;
        case WStype_BIN:
            processCommand(payload, length);
            break;
        case WStype_ERROR:
            Serial.println("WebSocket error");
            break;
        case WStype_PING:
            break;
        case WStype_PONG:
            break;
        default:
            break;
    }
}

void processCommand(uint8_t* data, size_t length) {
    if (length == 1) {
        uint8_t commandCode = data[0];
        switch (commandCode) {
            case 1:
                moveUp();
                break;
            case 2:
                moveDown();
                break;
            case 3:
                turnLeft();
                break;
            case 4:
                turnRight();
                break;
            default:
                Serial.println("Unknown command");
                break;
        }
    } else {
        Serial.println("Invalid command length");
    }
}