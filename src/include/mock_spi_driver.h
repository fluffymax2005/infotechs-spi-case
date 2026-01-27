#ifndef MOCK_SPI_DRIVER
    #define MOCK_SPI_DRIVER

    #include "eeprom_25lc040a.h"
    #include "spi_interface.h"

    #include <unordered_map>

    class MockSpi : public ISpiBitBang {
    public:
        MockSpi() = default;
        ~MockSpi() = default;

        void chipSelect() override;
        void chipDeselect() override;
        virtual byte transferBit(const_type<bit> data) override;
        virtual byte transferByte(const_type<byte> data) override;
        virtual byte_array transferBytes(const byte_array data, const_type<array_size> length) override;

        void setByteArrayByAddress(const_type<pointer_size> address, byte_array data, const_type<array_size> length);
        const byte_array getByteArrayByAddress(const_type<pointer_size> address) const;

    private:
        bit SS{LOW};
        bit writeInitiated{false}, writeEnabled{false};
        byte memory[EEPROM_25LC040A::MAX_ADDRESS + 1]{};

        enum PinState : byte {
            LOW = 0,
            HIGH = 1
        };

        byte_array handle_read_command(const_type<pointer_size> address, pointer_size length) const;
        void handle_write_command(const_type<pointer_size> address, const byte_array data, array_size length);
    };

#endif