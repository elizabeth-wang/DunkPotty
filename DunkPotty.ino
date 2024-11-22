/*
  Project Name: Dunk Potty
  Description: A button press triggers a Telegram bot, which sends a predefined message to a specified chat.

  Author: Elizabeth Wang
  Date: November 21, 2024
  Version: 1.0
    
  Notes:
    - Fill out definitions.h with your WIFI_SSID, WIFI_PASSWORD, BOT_TOKEN, and CHAT_ID
*/


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "definitions.h"

// Initialize WiFi client
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// GPIO pin for the button
const int buttonPin = D4;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT); // Configure the button pin as input with a pull-up

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
}

void loop() {
  // Check button state
  bool buttonState = digitalRead(buttonPin);
  Serial.println("buttonState");
  Serial.println(buttonState);
  Serial.println("lastButtonState");
  Serial.println(lastButtonState);
  if (buttonState == LOW && lastButtonState == HIGH) { // Button pressed
    Serial.println("Button pressed, sending message...");
    bot.sendMessage(CHAT_ID, "Gotta potty!", "");
  }
  lastButtonState = buttonState;

  delay(100); // Short delay to debounce the button
}
