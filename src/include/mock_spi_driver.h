/**
* @file mock_driver.h
* @brief SPI driver mock implementation.
*/

#ifndef MOCK_SPI_DRIVER

    /**
    * @def
    * @brief Include module macros.
    */
    #define MOCK_SPI_DRIVER

    #include "eeprom_25lc040a.h"
    #include "spi_interface.h"

    #include <unordered_map>

    /**
    * @class MockSpi
    * @brief SPI driver mock implementation. Emulates EEPROM memory type based 25LC040A microchip.
    */
    class MockSpi : public ISpiBitBang {
    public:
	/**
	* @brief Default constructor.
	*/
        MockSpi() = default;

	/**
	* @brief Virtual destructor.
	*/
        ~MockSpi() = default;

	/**
	* @brief Sets SS level to high.
	*/
        void chipSelect() override;

	/**
	* @brief Sets SS level to low.
	*/
        void chipDeselect() override;

	/**
	* @throw NotImplementedException Method is not implemented.
	* @brief Not implemented.
	*/
        virtual bit transferBit(const_type<bit> data) override;

	/**
	* @throw NotImplementedException Method is not implemented.
	* @brief Not implemented.
	*/
        virtual byte transferByte(const_type<byte> data) override;

	/**
	* @param data byte array to transfer. If pointer to not allocated memory is provided then undefined behaviour might be caused.
	* @param length byte array length.
	* @throw std::invalid_argument @c length is less than 2.
	* @throw std::invalid_argument @c data is nullptr.
	* @throw std::runtime_error <TT>SS</TT>'s state is high.
	* @throw std::invalid_argument @c length is less than 4. Causes when <TT>read/write</TT> operation is in progress.
	* @throw std::runtime_error Not enough free memory space to create byte array. Device has not got enough space to allocate for byte array while trying reading.
	* @throw std::invalid_argument Given address is to read/write is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* @throw std::runtime_error Invalid instruction is provided. See @ref mock_spi_notes "valid commands".
	* @returns
	* - @c nullptr if @ref EEPROM_25LC040A::Command::CMD_WRITE is provided and writing is successful.
	* - pointer to emulated @c memory if @ref EEPROM_25LC040A::Command::CMD_READ is provided and reading is successful. <b>NOTE</b>: byte_array must be released manually using free() or delete[].
	* - @c nullptr if @ref EEPROM_25LC040A::Command::CMD_WREN or @ref EEPROM_25LC040A::Command::CMD_WRDI is provided.
	*/
        virtual byte_array transferBytes(const byte_array data, const_type<array_size> length) override;

	/**
	* @brief Debugging method to set accurate byte array data conviniently.
	* @param address virtual @c memory address to write @c data at.
	* @param data byte array to write.
	* @param length count of bytes to write.
	*/
        void setByteArrayByAddress(const_type<pointer_size> address, byte_array data, const_type<array_size> length);

	/**
	* @brief Debugging method to get byte array by given virtual @c memory address.
	* @param address virtual memory @c address to get pointer array from.
	* @returns
	* - @c nullptr if @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - @c pointer to requested segment.
	*/
        const byte_array getByteArrayByAddress(const_type<pointer_size> address) const;

    private:
	/**
	* @brief SS state.
	*/
        bit SS{LOW};

	/**
	* @brief Whether correct EEPROM_25LC040A::Command::CMD_WREN instruction is given.
	*/
        bit writeInitiated{false};

	/**
	* @brief Whether writing is allowed
	*/
	bit writeEnabled{false};

	/**
	* @brief Emulated memory storage for microchip.
	*/
        byte memory[EEPROM_25LC040A::MAX_ADDRESS + 1]{};

	/**
	* @brief Possible states of SS.
	*/
        enum PinState : byte {
            LOW = 0, ///< Low level signal
            HIGH = 1 ///< High level signal
        };

	/**
	* @param address address of @c memory to read from.
	* @param length byte count to read.
	* @throw std::invalid_argument Given address is to read/write is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* @returns
	* - @c nullptr if length == 0.
	* - pointer to emulated @c memory.
	* @warning if <TT>address + length > EEPROM_25LC040A::MAX_ADDRESS</TT>. When internal "pointer" of reading data from @c memory will reach <TT>EEPROM_25LC040A::MAX_ADDRESS + 1</TT> "pointer" will be assigned @c NULL and will continue reading unless all requested data is read.	
	* @brief Auxiliary method to handle read command
	*/
        byte_array handle_read_command(const_type<pointer_size> address, pointer_size length) const;

	/**
	* @param address address of @c memory to write.
	* @param length byte count to write.
	* @throw std::invalid_argument Given address is to read/write is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* @returns
	* - @c nullptr if length == 0.
	* - pointer to emulated @c memory.
	* @warning if <TT>address + length > EEPROM_25LC040A::MAX_ADDRESS<TT>. When internal "pointer" of write data from @c memory will reach <TT>EEPROM_25LC040A::MAX_ADDRESS + 1</TT> "pointer" will be assigned @c NULL and will continue writing unless all given data is written.
	* @brief Auxiliary method to handle read command
	*/
        void handle_write_command(const_type<pointer_size> address, const byte_array data, array_size length);
    };

#endif

/**
* @page Mock_Spi_page MockSpi interaction manual
*
* @section mock_spi_intro Introduction
* This page contains information about data format of interacting with MockSpi. The only way to interact is using MockSpi::transferBytes. Other methods are @b not implemented and will raised NotImplementedException.
*
* @section mock_spi_data_structures Data structures
Request to EEPROM based 25LC040A microchip to <TT>read/write</TT> must contain information about address and command code (See @ref EEPROM_25LC040A::Command). Address and command code combination is instruction. Instruction consists of two bytes with following mask: <TT><b>0000xc</b></TT>. "x" is 9 bits address because 512 addresses are able to be accessed. "c" is command code that takes 3 bits.
The request is an array of bytes where:
1. 1st and 2nd bytes are instruction.
2. 3rd and 4th bytes are bytes count to read/write. If EEPROM_25LC040A::Command::CMD_WREN is provided these bytes and following ones are ignored.
3. 5th - nth bytes are bytes to write by given address. If EEPROM_25LC040A::Command::CMD_READ is provided these bytes and following ones are ignored.

To <TT>enable/disable</TT> writing the following instruction mask must be supplied: <TT><b>0000xc</b></TT>. "x" is any combination of 9 bits. "c" is command code that takes 3 bits.

@section mock_spi_notes Notes
The @b only command codes that can be provided are EEPROM_25LC040A::Command::CMD_READ, EEPROM_25LC040A::Command::CMD_WRITE, EEPROM_25LC040A::Command::CMD_WREN and EEPROM_25LC040A::Command::CMD_WRDI.
*/
