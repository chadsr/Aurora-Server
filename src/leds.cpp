#include "leds.h"
#include "definitions.h"
#include "eeprom.h"

#include <FastLED.h>

/*
LED Controlling
*/

CRGB Leds[NUM_LEDS];
ledSequence CurrentSequence = {0};
bool StripOn = false;

void setupLEDs()
{
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOUR_ORDER>(Leds, NUM_LEDS);
}

int waitingAnimation(int location)
{
  if (location >= NUM_LEDS)
  {
    location = 0;
    Leds[NUM_LEDS - 1] = CRGB(0, 0, 0);
  }
  else if (location > 0)
  {
    Leds[location - 1] = CRGB(0, 0, 0);
  }

  Leds[location] = CRGB(255, 255, 255);

  FastLED.show();

  return location + 1;
}

void resetLeds()
{
  struct requestSingleColour requestBlack;
  requestBlack.colour.red = 0;
  requestBlack.colour.green = 0;
  requestBlack.colour.blue = 0;

  requestBlack.startPosition = 0;
  requestBlack.endPosition = NUM_LEDS;

  loadSingleColour(requestBlack, true);
}

void toggleOnOff()
{
  if (StripOn)
  {
    storeCurrentState(START_ADDRESS);
    resetLeds();
    StripOn = false;
  }
  else
  {
    resetLeds();
    loadStateToCurrent(START_ADDRESS);
    loadCurrentSequence();
    StripOn = true;
  }
}

void loadSingleColour(struct requestSingleColour request, bool clear = true)
{
  if (clear == true)
  {
    FastLED.clear();
    FastLED.clearData();
  }

  CRGB crgb = CRGB(request.colour.red, request.colour.green, request.colour.blue);
  for (unsigned int i = request.startPosition - 1; i < request.endPosition; i++)
  {
    Leds[i] = crgb;
    CurrentSequence.leds[i] = request.colour;
  }

  FastLED.show();
  storeCurrentState(START_ADDRESS);
}

void loadCurrentSequence()
{
  FastLED.clear();
  FastLED.clearData();

  for (int i = 0; i < NUM_LEDS; i++)
  {
    Leds[i] = CRGB(CurrentSequence.leds[i].red, CurrentSequence.leds[i].green, CurrentSequence.leds[i].blue);
  }

  FastLED.show();
  storeCurrentState(START_ADDRESS);
}