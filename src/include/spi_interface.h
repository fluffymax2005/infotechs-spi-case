#ifndef SPI_INTERFACE_H
    #define SPI_INTERFACE_H

    #include <cstdint>
    #include <type_traits>
    #include <gmock/gmock.h>

    using bit = bool;
    using byte = uint8_t;
    using word = uint16_t;
    using dword = uint32_t;
    using byte_array = byte*;
    using array_size = dword;

    template <typename T>
    using const_type = std::conditional_t<sizeof(void*) < sizeof(T), const T&, const T>;

    class ISpiBitBang {
    public:
        virtual ~ISpiBitBang() = default;

        virtual void chipSelect() = 0; // set SS
        virtual void chipDeselect() = 0; // unset SS
        virtual byte transferBit(const_type<bit> data) = 0; // transfer 1 bit
        virtual byte transferByte(const_type<byte> data) = 0; // transfer 1 byte
        virtual byte_array transferBytes(const byte_array data, const_type<array_size> length) = 0; // transfer byte[]
    };

#endif