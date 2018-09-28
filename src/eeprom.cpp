#include "eeprom.h"

#include <EEPROM.h>

// loadStateToCurrent() - Restores an ledSequence struct from a given EEPROM address index, addr, to CurrentSequence
void loadStateToCurrent(unsigned int addr) {
  EEPROM.get(addr, CurrentSequence);
}

// storeCurrentState() - Stores the CurrentSequence struct at a specified EERPOM address index, addr
void storeCurrentState(unsigned int addr) {
  EEPROM.put(addr, CurrentSequence);
  EEPROM.commit();
}