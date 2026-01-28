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
        static constexpr byte MAX_BYTES_TO_WRITE_AT_ONCE = 16;

        enum Command : byte {
            CMD_READ = 0b011, 
            CMD_WRITE = 0b010, ///< Test
            CMD_WREN = 0b110,
            CMD_WRDI = 0b100,
            CMD_RDSR = 0b101,
            CMD_WRSR = 0b001
        };

        using mask_type = word;

        explicit EEPROM_25LC040A(ISpiBitBang* spi) noexcept;

        const bit readBit(const_type<pointer_size> address) const;
        const byte readByte(const_type<pointer_size> address) const;
        const byte_array readByteArray(const_type<pointer_size> address, const_type<array_size> length) const;

        void writeBit(const_type<pointer_size> address, const_type<bit> data) const;
        void writeByte(const_type<pointer_size> address, const_type<byte> data) const;
        void writeByteArray(const_type<pointer_size> address, const byte_array data, const_type<array_size> length) const;

        inline void stop() noexcept;
        inline void resume() noexcept;
    private:
        ISpiBitBang* spi;
        bool isWorking = true;

        static inline void validateAddress(const_type<pointer_size> address);
        inline void validateState() const;

        static inline mask_type createInstruction(const_type<pointer_size> address, const_type<Command> cmd) noexcept;
    };
#endif
