#ifndef EEPROM_H
#define EEPROM_H

#include "definitions.h"

const unsigned int START_ADDRESS = 1; // Address location to start storing on EEPROM

extern ledSequence CurrentSequence;

void loadStateToCurrent(unsigned int addr);
void storeCurrentState(unsigned int addr);

#endif
