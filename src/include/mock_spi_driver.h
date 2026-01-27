#ifndef MOCK_SPI_DRIVER
    #define MOCK_SPI_DRIVER

    #include "eeprom_25lc040a.h"
    #include "spi_interface.h"

    #include <unordered_map>

    class MockSpi : public ISpiBitBang {
    public:
        struct Array_info {
            void* ptr;
            array_size length;
        };

        MockSpi() = default;
        ~MockSpi() = default;

        void chipSelect() override;
        void chipDeselect() override;
        virtual byte transferBit(const_type<bit> data) override;
        virtual byte transferByte(const_type<byte> data) override;
        virtual byte_array transferBytes(const byte_array data, const_type<array_size> length) override;
        virtual void delay(const_type<dword> ms) override;

        void setByteArrayResponseByAddress(const_type<pointer_size> address, byte_array data, const_type<array_size> length);
        Array_info getByteArrayByAddress(const_type<pointer_size> address) const;

    private:
        bit SS;
        bit writeInitiated, writeEnabled;
        dword op_delay = 1;

        // Hash table. Contains <void*> array which has got size of <array_size>. Array can be got by <pointer_size> fictious address
        std::unordered_map<pointer_size, Array_info> address_data;

        enum PinState : byte {
            LOW = 0,
            HIGH = 1
        };

        byte_array handle_read_command(const_type<pointer_size> address);
    };

#endif