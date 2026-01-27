#include "../src/include/mock_spi_driver.h"
#include "test_runner.h"
#include <cassert>

//#define INVALID_TEST_RUN

void testReadBadAddress();
void testReadBit();
void testReadByte();
void testReadByteArray();

void testWriteBadAddress();
void testWriteBit();
void testWriteByte();
void testWriteByteArray();

int main() {
    TestRunner runner;

    // === READ tests
    #ifdef INVALID_TEST_RUN
        runner.runTest(("ReadInvalidAddress"), testReadBadAddress);
    #endif
    runner.runTest("ReadBit", testReadBit);
    runner.runTest("ReadByte", testReadByte);
    runner.runTest("ReadByteArray", testReadByteArray);

    #ifdef INVALID_TEST_RUN
        runner.runTest("WriteInvalidAddress", testWriteBadAddress);
    #endif
    runner.runTest("WriteBit", testWriteBit);
    runner.runTest("WriteByte", testWriteByte);
    //runner.runTest("WriteByteArray", testWriteByteArray);
}

void testReadBadAddress() {
    MockSpi spi;
    const pointer_size ADDRESS = EEPROM_25LC040A::MAX_ADDRESS + 1; // invalid address

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Read" byte
    auto result = eeprom.readByte(ADDRESS);
}

void testReadBit() {
    MockSpi spi;
    const pointer_size ADDRESS = std::rand() % 512; // random address [0; 511]
    const byte VALUE = (std::rand() % 2) << 7 | std::rand() % 256 & 0x7F; // random byte value to set

    spi.setByteArrayByAddress(ADDRESS, const_cast<byte_array>(&VALUE), 1);

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Read" bit
    auto result = eeprom.readBit(ADDRESS);

    // Assert result
    assert(result == VALUE >> 7);
}

void testReadByte() {
    MockSpi spi;
    const pointer_size ADDRESS = std::rand() % 512; // random address [0; 511]
    const byte VALUE = std::rand() % 256; // random byte value

    spi.setByteArrayByAddress(ADDRESS, const_cast<byte_array>(&VALUE), 1);

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Read" byte
    auto result = eeprom.readByte(ADDRESS);

    // Assert result
    assert(result == VALUE);
}

void testReadByteArray() {
    MockSpi spi;
    const pointer_size ADDRESS = std::rand() % 512; // random address [0; 511]
    const array_size length = std::rand() % 256; // random response array size

    byte* response = new (std::nothrow) byte[length] {0x00, 0x00};
    if (!response)
        throw std::runtime_error("testReadByteArray: no memory for response array");
    for (array_size i = 0; i < length; ++i)
        response[i] = std::rand() % 256; // random byte value

    spi.setByteArrayByAddress(ADDRESS, response, length);

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Read" byte array. Length = length - 2 because trash data is not read
    auto result = eeprom.readByteArray(ADDRESS, length);

    // Assert result
    for (array_size i = 0; i < length - 2; ++i)
        assert(result[i] == response[i]);
}

void testWriteBadAddress() {
    MockSpi spi;
    const pointer_size ADDRESS = EEPROM_25LC040A::MAX_ADDRESS + 1; // invalid address
    const byte VALUE = std::rand() % 256; // random byte value

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Write" byte
    eeprom.writeByte(ADDRESS, VALUE);
}

void testWriteBit() {
    MockSpi spi;
    const pointer_size ADDRESS = std::rand() % 512; // random address [0; 511]
    const byte VALUE = std::rand() % 255; // random byte value

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Write" bit
    eeprom.writeBit(ADDRESS, VALUE);

    // Assert record result
    const auto result = spi.getByteArrayByAddress(ADDRESS);
    const auto v = *result >> 7;
    assert(v == VALUE);
}


void testWriteByte() {
    MockSpi spi;
    const pointer_size ADDRESS = std::rand() % 512; // random address [0; 511]
    const byte VALUE = std::rand() % 256; // random byte value

    // Make EEPROM
    EEPROM_25LC040A eeprom(&spi);

    // "Write" byte
    eeprom.writeByte(ADDRESS, VALUE);

    // Assert record result
    const auto result = spi.getByteArrayByAddress(ADDRESS);
    const auto v = *result;
    assert(v == VALUE);
}



