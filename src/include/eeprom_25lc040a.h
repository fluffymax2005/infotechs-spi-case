#ifndef EEPROM_25LC040A_H
    #define EEPROM_25LC040A_H

    #include "spi_interface.h"

    using dword = uint16_t;
    using pointer_size = dword;

    class EEPROM_25LC040A {
    public:
        static constexpr dword MAX_ADDRESS = 511;

        explicit EEPROM_25LC040A(ISpiBitBang* spi) noexcept;

        bit readBit(const_type<pointer_size> address);
        byte readByte(const_type<pointer_size> address);
        byte_array readByteArray(const_type<pointer_size> address, const_type<array_size> length);

        void writeBit(const_type<pointer_size> address, const_type<bit> data);
        void writeByte(const_type<pointer_size> address, const_type<byte> data);
        void writeByteArray(const_type<pointer_size> address, const byte_array data, const_type<array_size> length);

        bool isReady() const noexcept;
    private:
        ISpiBitBang* spi;

        static constexpr byte COMMAND_BIT_SIZE = 3;
        enum Command : byte {
            CMD_READ = 0b011,
            CMD_WRITE = 0b010,
            CMD_WREN = 0b110,
            CMD_WRDI = 0b100,
            CMD_RDSR = 0b101,
            CMD_WRSR = 0b001
        };

        void enableWrite();
        void waitForWriteComplete();
    };
#endif
