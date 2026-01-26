#ifndef EEPROM_25LC040A_H
    #define EEPROM_25LC040A_H

    #define WRITE_BY_BYTE
    #ifndef WRITE_BY_BYTE
        #define WRITE_FULL_BLOCK
    #endif

    #include "spi_interface.h"

    using pointer_size = word;

    class EEPROM_25LC040A {
    public:
        static constexpr pointer_size MAX_ADDRESS = 511;

        enum Command : byte {
            CMD_READ = 0b011,
            CMD_WRITE = 0b010,
            CMD_WREN = 0b110,
            CMD_WRDI = 0b100,
            CMD_RDSR = 0b101,
            CMD_WRSR = 0b001
        };

        using mask_type = word;

        explicit EEPROM_25LC040A(ISpiBitBang* spi) noexcept;

        bit readBit(const_type<pointer_size> address);
        byte readByte(const_type<pointer_size> address);
        byte_array readByteArray(const_type<pointer_size> address, const_type<array_size> length);

        void writeBit(const_type<pointer_size> address, const_type<bit> data);
        void writeByte(const_type<pointer_size> address, const_type<byte> data);
        void writeByteArray(const_type<pointer_size> address, const byte_array data, const_type<array_size> length);

        inline void stop() noexcept;
        inline void resume() noexcept;
    private:
        ISpiBitBang* spi;
        bool isWorking = true;

        inline void validateAddress(const_type<pointer_size> address) const;
        inline void validateState() const;

        inline mask_type createInstruction(const_type<pointer_size> address, const_type<Command> cmd) const noexcept;
    };
#endif
