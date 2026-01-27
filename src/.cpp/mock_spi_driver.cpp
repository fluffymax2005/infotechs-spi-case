#include "../include/mock_spi_driver.h"
#include "../include/not_implemented_exception.h"
#include "../include/eeprom_25lc040a.h"
#include <chrono>
#include <stdexcept>
#include <thread>

#include <iostream>

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

    // Stop thread execution for op_delay ms
    std::this_thread::sleep_for(std::chrono::milliseconds(op_delay));

    switch (COMMAND) {
        case EEPROM_25LC040A::CMD_READ:
            return handle_read_command(ADDRESS);
        case EEPROM_25LC040A::CMD_WRITE:
            if (!writeEnabled) {
                writeInitiated = false;
                return nullptr;
            }

            address_data[ADDRESS] = Array_info{data, length};
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

void MockSpi::delay(const_type<dword> ms) {
    op_delay = ms;
}

void MockSpi::setByteArrayResponseByAddress(const_type<pointer_size> address, byte_array data, const_type<array_size> length) {
    if (!data || length < 3)
        return;

    const Array_info info{data, length};
    address_data[address] = info;
}

MockSpi::Array_info MockSpi::getByteArrayByAddress(const_type<pointer_size> address) const {
    const auto it = address_data.find(address);
    if (it == address_data.end())
        throw std::invalid_argument("MockSpi::getByteArrayByAddress: no data found by given address");
    return it->second;
}

byte_array MockSpi::handle_read_command(const_type<pointer_size> address) {
    const auto it = address_data.find(address);
    if (it == address_data.end())
        throw std::runtime_error("MockSpi::transferBytes: CMD_READ command failed. Reason: not data by given address");
    return reinterpret_cast<byte_array>(it->second.ptr);
}
