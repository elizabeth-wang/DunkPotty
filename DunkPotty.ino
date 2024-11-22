/*
  Project Name: Dunk Potty
  Description: A button press triggers a Telegram bot, which sends a predefined message to a specified chat.

  Author: Elizabeth Wang
  Date: November 22, 2024
  Version: 2.0
    
  Notes:
    - definitions.h: Define BOT_TOKEN and CHAT_ID
    - APs are accessible if 1) module can't connect to any of the saved connections or 2) the AP config button is pressed
      - Once connected to the AP, use a browser to open the gateway IP 192.168.4.1, configure wifi, save, and it should reboot and connect to the new AP credentials.
*/


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ArduinoJson.h>
#include "definitions.h"

// Initialize WiFi client
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
WiFiManager wifiManager;

// GPIO pin for the msg button
const int msgButtonPin = D1;
const int configButtonPin = D2;
bool lastMsgButtonState = HIGH;
bool lastConfigButtonState = HIGH;
int TIMEOUT = 120; // seconds before config portal times out

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, ESP defaults to STA+AP
  Serial.begin(115200);
  pinMode(msgButtonPin, INPUT_PULLUP); // Configure the button pin as input with a pull-up
  pinMode(configButtonPin, INPUT_PULLUP); // Configure the button pin as input with a pull-up
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  wifiManager.setConfigPortalTimeout(TIMEOUT);

  bool res;
  // Attempts to auto-connect to a saved Access Point, otherwise opens an AP with the below credentials.
  res = wifiManager.autoConnect("DunkPottyAP", "GottaPotty!");
  if(!res) {
        Serial.println("Failed to connect");
        ESP.deepSleep(0);
    } 
    else {
        Serial.println("connected!");
    }
}

void loop() {
  // Message Button
  bool msgButtonState = digitalRead(msgButtonPin);
  if (msgButtonState == LOW && lastMsgButtonState == HIGH) { // Button pressed
    Serial.println("Message button pressed, sending message...");
    bot.sendMessage(CHAT_ID, "Gotta potty!", "");
  }
  lastMsgButtonState = msgButtonState;

  // Config Button
  bool configButtonState = digitalRead(configButtonPin);
  if (configButtonState == LOW && lastConfigButtonState == HIGH) { // Button pressed
    Serial.println("Config button pressed, opening config portal...");
    wifiManager.setConfigPortalTimeout(TIMEOUT);
    wifiManager.startConfigPortal("DunkConfigPortal", "GottaPotty!");
  }
  lastConfigButtonState = configButtonState;
  delay(100); // Short delay to debounce the button
}
