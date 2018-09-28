#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "leds.h" // For NUM_LEDS

struct __attribute__ ((packed)) colour {
  unsigned int red;
  unsigned int green;
  unsigned int blue;
};

// Request a section to be the same colour and brightness
struct __attribute__ ((packed)) requestSingleColour {
  struct colour colour;
  unsigned int startPosition;
  unsigned int endPosition;
};

// Request a full unique sequence, where each LED has its own RGB colour value
struct __attribute__ ((packed)) ledSequence {
  struct colour leds[NUM_LEDS];
};

#endif
