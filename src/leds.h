#ifndef LEDS_H
#define LEDS_H

#define LED_PIN 0 // GPIO0 AKA D3 on D1 Mini
#define NUM_LEDS 720
#define LED_TYPE WS2812
#define COLOUR_ORDER GRB

void setupLEDs();
void resetLeds();
void toggleOnOff();
int waitingAnimation(int location);

void loadSingleColour(struct requestSingleColour request, bool clear);
void loadCurrentSequence();

#endif
