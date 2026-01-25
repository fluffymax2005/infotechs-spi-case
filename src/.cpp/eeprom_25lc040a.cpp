#include "../include/eeprom_25lc040a.h"
#include <stdexcept>

EEPROM_25LC040A::EEPROM_25LC040A(ISpiBitBang* spi) noexcept : spi(spi) {}

bit EEPROM_25LC040A::readBit(const_type<pointer_size> address) {
    if (address > MAX_ADDRESS)
        throw std::out_of_range("bit EEPROM_25LC040A::readBit(): given address is bigger than (EEPROM_25LC040A::MAX_ADDRESS + 1);");
    
}