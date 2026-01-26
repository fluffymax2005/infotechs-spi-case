#include "../src/include/mock_spi_driver.h"
#include "test_runner.h"
#include <cassert>

MockSpi spi;

void testReadByte();

int main() {
    TestRunner runner;
    runner.runTest("ReadByte", testReadByte);
}

void testReadByte() {
    constexpr pointer_size ADDRESS = 0xA7;
    constexpr byte VALUE = 0xAB;

    // Response to read is [0x00, 0x00, 0xAB]
    byte response[] = {0x00, 0x00, VALUE};
    spi.setByteArrayResponseByAddress(ADDRESS, response, 3);

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Read" byte
    auto result = eeprom.readByte(ADDRESS);

    // Assert result
    assert(result == VALUE);
}