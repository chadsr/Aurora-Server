#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClient.h>

#include "main.h"
#include "definitions.h"
#include "server.h"
#include "eeprom.h"
#include "leds.h"

void setupWireless()
{
  Serial.printf("Connecting to %s\n", WIFI_SSID);

  // Prevent power-saving mode
  WiFi.setSleep(false);
  // WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);

  // Attempt network connection on loop until success
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    i = waitingAnimation(i);
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println("IP address:");
  Serial.println(WiFi.localIP());

  WiFiClient wifiClient;
}

void setup()
{
  Serial.begin(9600);

  setupLEDs();
  setupWireless();
  setupServer();
  toggleOnOff();
}

/*

*/

void apiListenerLoop(void *pvParameters)
{
  while (true)
  { // Infinite loop for the HTTP API listener
    handleAPIClient();
  }
}

void loop()
{
  handleUDPPacket();

  // We are taking advantage of the freeRTOS API function to spin up a separate thread for listening for http API clients
  xTaskCreate(apiListenerLoop, "HTTP API Server Listener", 4096, NULL, 1, NULL);
}
