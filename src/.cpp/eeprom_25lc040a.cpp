#include "../include/eeprom_25lc040a.h"
#include <stdexcept>

#include <iostream>

EEPROM_25LC040A::EEPROM_25LC040A(ISpiBitBang* spi) noexcept : spi(spi) {}

const bit EEPROM_25LC040A::readBit(const_type<pointer_size> address) const {
    if (!spi)
        throw std::runtime_error("EEPROM_25LC040A::readBit(): \"spi\" is nullptr");
    validateAddress(address);
    validateState();

    const word instruction = createInstruction(address, CMD_READ);
    byte arr[4];
    arr[0] = instruction & 0x00FF;
    arr[1] = (instruction & 0xFF00) >> 8;

    word* length_ptr = reinterpret_cast<word*>(arr + 2);
    length_ptr[0] = 1;

    spi->chipDeselect();
    const auto response = spi->transferBytes(arr, 4);
    spi->chipSelect();

    const bit bit = response[0] >> 7;
    delete[] response;

    return bit;
}

const byte EEPROM_25LC040A::readByte(const_type<pointer_size> address) const {
    if (!spi)
        throw std::runtime_error("EEPROM_25LC040A::readByte(): \"spi\" is nullptr");
    validateAddress(address);
    validateState();

    // Instruction format: 0000<9_bit_address>011
    word instruction = createInstruction(address, CMD_READ);
    byte arr[4];
    arr[0] = instruction & 0x00FF;
    arr[1] = (instruction & 0xFF00) >> 8;

    word* length_ptr = reinterpret_cast<word*>(arr + 2);
    length_ptr[0] = 1;

    spi->chipDeselect();
    auto response = spi->transferBytes(arr, 4);
    spi->chipSelect();

    const byte byte = response[0];
    delete[] response;

    return byte;
}

const byte_array EEPROM_25LC040A::readByteArray(const_type<pointer_size> address, const_type<array_size> length) const {
    if (!spi)
        throw std::runtime_error("EEPROM_25LC040A::readByteArray(): \"spi\" is nullptr");
    if (!length)
        throw std::invalid_argument("EEPROM_25LC040A::readByteArray(): \"length\" is null");
    validateAddress(address);
    validateState();

    // Instruction format: 0000<9_bit_address>011
    word instruction = createInstruction(address, CMD_READ);
    byte arr[4];
    arr[0] = instruction & 0x00FF;
    arr[1] = (instruction & 0xFF00) >> 8;

    word* length_ptr = reinterpret_cast<word*>(arr + 2);
    length_ptr[0] = length;

    spi->chipDeselect();
    const auto result = spi->transferBytes(arr, length);
    spi->chipSelect();

    return result;
}

void EEPROM_25LC040A::writeBit(const_type<pointer_size> address, const_type<bit> data) const {
    validateAddress(address);
    validateState();

    // Write order:
    // 1. Set CS low.
    // 2. Push CMD_WREN instruction.
    // 3. Set CS high.
    // 4. All data is written? Yes - go 5, else go 8.
    // 5. Write 1 byte.
    // 6. Set CS high (to complete ensurance that data is written).
    // 7. Go 4.
    // 8. Set CS low.
    // 9. Push CMD_WRDI instruction.
    // 10. Set CS high.
    
    // Note: BIT must be written then other 7 bites of bytes cannot be changed. Furthermore, firstly 1 bytes must be read and saved.
    // After this happens bit must be put and written into memory. For example:
    // 1000_0000 - byte in memory. We need to put '0' then
    // 0000_0000 - will be after record.

    // 1. Save current byte
    word instruction = createInstruction(address, CMD_READ);
    byte arr[5];
    arr[0] = instruction & 0x00FF;
    arr[1] = (instruction & 0xFF00) >> 8;

    word* length_ptr = reinterpret_cast<word*>(arr + 2);
    length_ptr[0] = 1;

    spi->chipDeselect();
    const auto save = spi->transferBytes(arr, 4);
    spi->chipSelect();

    // 2. Enable writing
    instruction = createInstruction(address, CMD_WREN);

    spi->chipDeselect();
    spi->transferBytes(reinterpret_cast<byte_array>(&instruction), sizeof(instruction));
    spi->chipSelect();

    // 3. Write byte array
    instruction = createInstruction(address, CMD_WRITE);
    arr[0] = instruction & 0x00FF; // 1st lowest byte
    arr[1] = (instruction & 0xFF00) >> 8; // 2nd lowest byte

    length_ptr[0] = 1; // arr[2] and arr[3] are length to write
    arr[4] = data << 7 | *save & 0x7F; // 3rd lowest byte, 1st new bit, other are saved

    spi->chipDeselect();
    spi->transferBytes(arr, sizeof(arr));
    spi->chipSelect();

    // 4. Disable writing
    instruction = createInstruction(address, CMD_WRDI);

    spi->chipDeselect();
    spi->transferBytes(reinterpret_cast<byte_array>(&instruction), sizeof(instruction));
    spi->chipSelect();
}

void EEPROM_25LC040A::writeByte(const_type<pointer_size> address, const_type<byte> data) const {
    validateAddress(address);
    validateState();

    // 1. Enable writing
    word instruction = createInstruction(address, CMD_WREN);

    spi->chipDeselect();
    spi->transferBytes(reinterpret_cast<byte_array>(&instruction), sizeof(instruction));
    spi->chipSelect();

    // 2. Create byte array which consists of instruction (2 bytes) and saved bites with one bit changed (1 bytes)

    instruction = createInstruction(address, CMD_WRITE);
    byte arr[5];
    arr[0] = instruction & 0x00FF; // 1st lowest byte
    arr[1] = (instruction & 0xFF00) >> 8; // 2nd lowest byte

    word* length_ptr = reinterpret_cast<word*>(arr + 2);
    length_ptr[0] = 1; // arr[2] and arr[3] are length to write
    arr[4] = data; // 3rd lowest byte, 1st new bit, other are saved

    spi->chipDeselect();
    spi->transferBytes(arr, sizeof(arr));
    spi->chipSelect();

    // 3. Disable writing
    instruction = createInstruction(address, CMD_WRDI);

    spi->chipDeselect();
    spi->transferBytes(reinterpret_cast<byte_array>(&instruction), sizeof(instruction));
    spi->chipSelect();
}

void EEPROM_25LC040A::writeByteArray(const_type<pointer_size> address, const byte_array data, const_type<array_size> length) const {
    if (!length)
        throw std::invalid_argument("EEPROM_25LC040A::writeByteArray(): \"length\" is null");
    if (!data)
        throw std::invalid_argument("EEPROM_25LC040A::writeByteArray(): \"data\" is nullptr");
    validateAddress(address);
    validateState();

    // 1. Enable writing
    word instruction = createInstruction(address, CMD_WREN);

    spi->chipDeselect();
    spi->transferBytes(reinterpret_cast<byte_array>(&instruction), sizeof(instruction));
    spi->chipSelect();

    // 2. Write
    instruction = createInstruction(address, CMD_WRITE);
    byte arr[4 + length];
    arr[0] = instruction & 0x00FF; // 1st lowest byte
    arr[1] = (instruction & 0xFF00) >> 8; // 2nd lowest byte

    word* length_ptr = reinterpret_cast<word*>(arr + 2);
    length_ptr[0] = length; // arr[2] and arr[3] are length to write

    for (pointer_size i = 0; i < length; ++i)
        arr[4 + i] = data[i];

    spi->chipDeselect();
    spi->transferBytes(arr, sizeof(arr));
    spi->chipSelect();

    // 3. Disable data writing
    instruction = createInstruction(address, CMD_WRDI);

    spi->chipDeselect();
    spi->transferBytes(reinterpret_cast<byte_array>(&instruction), sizeof(instruction));
    spi->chipSelect();
}

inline void EEPROM_25LC040A::stop() noexcept {
    isWorking = false;
}
inline void EEPROM_25LC040A::resume() noexcept {
    isWorking = true;
}

inline void EEPROM_25LC040A::validateAddress(const_type<pointer_size> address) {
    if (address > MAX_ADDRESS)
        throw std::out_of_range("EEPROM_25LC040A::validateAddress(): given \"address\" is bigger than EEPROM_25LC040A::MAX_ADDRESS");
}
inline void EEPROM_25LC040A::validateState() const {
    if (!isWorking)
        throw std::runtime_error("EEPROM_25LC040A::validateState(): device isn't working");
}

inline EEPROM_25LC040A::mask_type EEPROM_25LC040A::createInstruction(const_type<pointer_size> address, const_type<Command> cmd) noexcept {
    mask_type instruction = cmd;
    return instruction | address << 3;
}
