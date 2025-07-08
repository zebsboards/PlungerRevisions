#include "Arduino.h"

#ifndef at24c256_h
#define at24c256_h
//---------------------------------------------------------------------------------------------------------
class at24c256 {
  public:
    at24c256(uint8_t);                            // EEPROM function
    void init();                                  // start EEPROM library
    void close();                                 // disconnect - set clock to 1700000
    void write(uint16_t m_addr, uint8_t m_data);  // store new data (location, value)
    void update(uint16_t m_addr, uint8_t m_data); // update existing data
    uint8_t read(uint16_t m_addr);                // read stored data (location)
  private:
    uint8_t m_chip_addr;                          // set i2c address of EEPROM
};
//---------------------------------------------------------------------------------------------------------
#endif
