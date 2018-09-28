#include "main.h"
#include "definitions.h"
#include "server.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

WebServer Server(API_PORT);

// Our UDP instance for sending and receiving packets
WiFiUDP Udp;

/*
Helper Functions
*/

bool isValidRGB(unsigned int red, unsigned int green, unsigned int blue) {
  if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255) {
    return true;
  } else {
    return false;
  }
}

bool isValidPosition(unsigned int startPos, unsigned int endPos) {
  if ((startPos > 0 && startPos <= NUM_LEDS) && (endPos > 0 && endPos <= NUM_LEDS) && startPos <= endPos) {
    return true;
  } else {
    return false;
  }
}

void getCurrentSequenceJSON(char* dest, size_t size) {
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;

  doc["count"].set(NUM_LEDS);
  serializeJson(doc, dest, size);
}

/*
API STUFF
*/

void handleAPIClient() {
  Server.handleClient();
}

void sendSuccessReply() {
  Server.send(200, "text/json", "{\"success\": true}");
}

void sendFailReply(const char* msg) {
  StaticJsonDocument<JSON_ERROR_SIZE> doc;
  char stringBuffer[JSON_ERROR_SIZE];

  doc["success"] = false;
  doc["message"] = msg;

  serializeJson(doc, stringBuffer, JSON_ERROR_SIZE);
  doc.clear();

  Server.send(400, "text/json", String(stringBuffer));
}

void handleNotFound() {
    Server.send(404, "text/plain", "Page Not Found");
}

void handleGet() {
  char stringBuffer[BUFF_SIZE];
  getCurrentSequenceJSON(stringBuffer, BUFF_SIZE);
  Server.send(200, "text/json", String(stringBuffer));
}

int loadJsonSections(JsonArray sections, bool clearPrevious) {
  if (!sections.isNull()) {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int startPos;
    unsigned int endPos;

    if (clearPrevious) {
      CurrentSequence = {0};
    }

    // Iterate over the section objects
    for (JsonVariant section : sections) {
      red = section.getMember("r").as<unsigned int>();
      green = section.getMember("g").as<unsigned int>();
      blue = section.getMember("b").as<unsigned int>();

      startPos = section.getMember("startPosition").as<unsigned int>();
      endPos = section.getMember("endPosition").as<unsigned int>();

      // Check if the values received are valid colours and within the required range
      if (isValidRGB(red, green, blue) && (isValidPosition(startPos, endPos))) {
        for (unsigned int i = startPos - 1; i < endPos; i++) {
          CurrentSequence.leds[i].red = red;
          CurrentSequence.leds[i].green = green;
          CurrentSequence.leds[i].blue = blue;
        }
      } else {
        sendFailReply("invalid colour/position values received");
        return 1;
      }
    }
  } else {
    sendFailReply("sections object missing");
    return 1;
  }
  return 0;
}

void handleToggle() {
  toggleOnOff();
  sendSuccessReply();
}

void handleSet() {
  Serial.println("Received sequence request via JSON API");

  DynamicJsonDocument doc(JSON_BUFF_SIZE);
  DeserializationError err = deserializeJson(doc, Server.arg("plain"));
  if(err) {
    sendFailReply("Failed parsing JSON request");
    Serial.println(err.c_str());
  }

  JsonArray sections = doc.getMember("sections").as<JsonArray>();
  bool clearPrevious = doc.getMember("clearPrevious").as<bool>();

  doc.clear();

  int res = loadJsonSections(sections, clearPrevious);
  if (res == EXIT_SUCCESS) {
    loadCurrentSequence();
    sendSuccessReply();
  }
}

/*
UDP STUFF (WIP)
*/

void sendCount(IPAddress remoteIp) {
    Udp.beginPacket(remoteIp, UDP_PORT);
    Udp.printf("%i", NUM_LEDS);
    Udp.endPacket();
}

void handleUDPPacket() {
  uint16_t bytes = Udp.parsePacket();
  IPAddress remoteIp = Udp.remoteIP();

  switch (bytes) {
    case 0: // Send the current led count back to client
    sendCount(remoteIp);
    break;

    case 1: // Toggle lights pn/off
    toggleOnOff();
    break;

    case sizeof(struct requestSingleColour): // Request for a section of LEDs the same colour (6 bytes)
    struct requestSingleColour request;
    Udp.read((char*) &request, sizeof(struct requestSingleColour));
    loadSingleColour(request, true);
    break;

    case sizeof(struct ledSequence): // Request for a full custom sequence (4 bytes * NUM_LEDS)
    Udp.read((char*) &CurrentSequence, sizeof(struct ledSequence));
    loadCurrentSequence();
    break;

    default:
    break;
  }

  Udp.flush();
}

// Setup
void setupServer() {
  Udp.begin(UDP_PORT);
  Serial.printf("Listening on UDP port: %u\n", UDP_PORT);

  // API Server event hooks
  Server.onNotFound(handleNotFound);
  Server.on("/leds", HTTP_GET, handleGet);
  Server.on("/leds", HTTP_POST, handleSet);
  Server.on("/leds/toggle", HTTP_POST, handleToggle);

  Server.begin();
}
