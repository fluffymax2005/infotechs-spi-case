#include "../include/mock_spi_driver.h"
#include "../include/not_implemented_exception.h"
#include "../include/eeprom_25lc040a.h"
#include <stdexcept>

void MockSpi::chipSelect() {
    SS = HIGH;
    if (writeInitiated)
        writeEnabled = true;
}

void MockSpi::chipDeselect() {
    SS = LOW;
}

byte MockSpi::transferBit(const_type<bit> data) {
    throw NotImplementedException("MockSpi::transferBit: implementation is not provided");
}

byte MockSpi::transferByte(const_type<byte> data) {
    throw NotImplementedException("MockSpi::transferByte: implementation is not provided");
}

byte_array MockSpi::transferBytes(const byte_array data, const_type<array_size> length) {
    if (length < 2)
        throw std::invalid_argument("MockSpi::transferBytes: length must be greater than 1");
    if (!data)
        throw std::invalid_argument("MockSpi::transferBytes: data is nullptr");
    if (SS == HIGH)
        throw std::runtime_error("MockSpi::transferBytes: SS latch state is HIGH");

    const word instruction = data[0] | data[1] << 8;
    const byte COMMAND = instruction & 0x0007;
    const dword ADDRESS = (instruction & 0x0FF8) >> 3;

    switch (COMMAND) {
        case EEPROM_25LC040A::CMD_READ:
            if (length < 3)
                throw std::invalid_argument("MockSpi::transferBytes: bytes count to read is not provided");
            return handle_read_command(ADDRESS, *reinterpret_cast<pointer_size*>(data + 2));
        case EEPROM_25LC040A::CMD_WRITE:
            if (!writeEnabled) {
                writeInitiated = false;
                return nullptr;
            }

            handle_write_command(ADDRESS, data + 4, *reinterpret_cast<pointer_size*>(data + 2));
            return nullptr;
        case EEPROM_25LC040A::CMD_WREN:
            writeInitiated = true;
            return nullptr;
        case EEPROM_25LC040A::CMD_WRDI:
            writeEnabled = writeInitiated = false;
            return nullptr;
        default:
            throw std::runtime_error("MockSpi::transferBytes: invalid instruction is provided");
    }
}

void MockSpi::setByteArrayByAddress(const_type<pointer_size> address, byte_array data, const_type<array_size> length) {
    if (!data || length < 1)
        return;
    for (array_size i = 0; i < length; ++i)
        memory[(address + i) % (EEPROM_25LC040A::MAX_ADDRESS + 1)] = data[i];
}

const byte_array MockSpi::getByteArrayByAddress(const_type<pointer_size> address) const {
    if (address > EEPROM_25LC040A::MAX_ADDRESS)
        return nullptr;

    return (byte_array)(memory + address);
}

byte_array MockSpi::handle_read_command(const_type<pointer_size> address, pointer_size length) const {
    if (length > EEPROM_25LC040A::MAX_ADDRESS - length)
        length = EEPROM_25LC040A::MAX_ADDRESS - length;
    if (!length)
        return nullptr;

    byte_array buf = new (std::nothrow) byte[length];
    if (!buf)
        throw std::runtime_error("MockSpi::transferBytes: failed to create byte array buffer");
    for (pointer_size i = 0; i < length; ++i)
        buf[i] = memory[(address + i) % (EEPROM_25LC040A::MAX_ADDRESS + 1)];
    return buf;
}

void MockSpi::handle_write_command(const_type<pointer_size> address, const byte_array data, array_size length) {
    if (address > EEPROM_25LC040A::MAX_ADDRESS)
        throw std::invalid_argument("MockSpi::transferBytes: given address is too big");
    if (!length)
        return;
    length = length > EEPROM_25LC040A::MAX_ADDRESS ? EEPROM_25LC040A::MAX_ADDRESS : length;

    for (byte i = 0; i < length; ++i)
        memory[(address + i) % (EEPROM_25LC040A::MAX_ADDRESS + 1)] = data[i];
}
