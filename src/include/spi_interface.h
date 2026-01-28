/**
*   @file spi_interface.h
*   @brief Class interface implementing SPI protocol
*/

#ifndef SPI_INTERFACE_H

    /**	
    *	@def SPI_INTERFACE_H
    *	@brief ISpiBitBang modules macro.
    */
    #define SPI_INTERFACE_H

    #include <cstdint>
    #include <type_traits>

    /**
    *   @typedef
    *   @brief Bit type.
    */
    using bit = bool;

    /**
    *   @typedef
    *   @brief Byte type.
    */
    using byte = uint8_t;

    /**
    *   @typedef
    *   @brief Machine word type.
    */
    using word = uint16_t;

    /**
    *   @typedef
    *   @brief Machine double word type.
    */
    using dword = uint32_t;

    /**
    *   @typedef
    *   @brief Byte array type.
    */
    using byte_array = byte*;

    /**
    *   @typedef
    *   @brief Type of byte array size.
    */
    using array_size = dword;

    /**
    *   @typedef
    *   @brief Template using for efficient arguments transfer into method (function).
    *   @returns 
    *    - <b><TT>const T&</TT></b> if sizeof(void*) < sizeof(T).
    *    - <b><TT>const T</TT></b> else.
    */
    template <typename T>
    using const_type = std::conditional_t<sizeof(void*) < sizeof(T), const T&, const T>;


    /**
    *   @class ISpiBitBang
    *   @brief Interface for SPI protocol base devices.
    */
    class ISpiBitBang {
    public:
        /**
        * @brief Virtual destructor.	
        */	
        virtual ~ISpiBitBang() = default;

        /**
        * @brief Sets @c SS level to @c high.
        */       
        virtual void chipSelect() = 0;
        
	/**
        * @brief Sets @c SS level to @c low.
        */
	virtual void chipDeselect() = 0;

	/**
	* @param data bit value to transfer.
	* @returns bit value. It's meaning can be assigned manually.
        * @brief Transfers input bit value into device.
        */
        virtual byte transferBit(const_type<bit> data) = 0;

	/**
	* @param data byte value to transfer.
	* @returns byte value. It's meaning can be assigned manually.
        * @brief Transfers input byte value into device. 
        */
        virtual byte transferByte(const_type<byte> data) = 0;

	/**
	* @param data byte array value to transfer.
	* @returns byte value. It's meaning can be assigned manually.
        * @brief Transfers input byte array into device. 
        */
        virtual byte_array transferBytes(const byte_array data, const_type<array_size> length) = 0;
    };

#endif
