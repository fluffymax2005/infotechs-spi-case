/** 
* @file
* @brief Provides driver for EEPROM_24LC040A.
*/

#ifndef EEPROM_25LC040A_H
	
    /**
    * @def EEPROM_25LC040A_H
    * @brief Include module macro.
    */    
    #define EEPROM_25LC040A_H

    #include "spi_interface.h"

    /**
    * @typedef pointer_size
    * @brief Size of pointer for EEPROM_25LC040A.
    */
    using pointer_size = word;

    /**
    * @class EEPROM_25LC040A
    * @brief Driver class for EEPROM_25LC040A. Provides high level interface to interact to EEPROM_25LC040A.
    */
    class EEPROM_25LC040A {
    public:
	/**
	* @brief Maximum address value for device.
	*/
        static constexpr pointer_size MAX_ADDRESS = 511;

	/**
	* @enum Command
	* @brief Set of possible commands for EEPROM_25LC040A.
	*/
        enum Command : byte {
            CMD_READ = 0b011, ///< Read.
            CMD_WRITE = 0b010, ///< Write.
            CMD_WREN = 0b110, ///< Enable writing.
            CMD_WRDI = 0b100, ///< Disable writing.
            CMD_RDSR = 0b101, ///< Read STATUS register. 
            CMD_WRSR = 0b001 ///< Write STATUS register.
        };

	/**
	* @typedef mask_type.
	* @brief Instruction mask type.
	*/
        using mask_type = word;

	/**
	* @param spi SPI protocol compatible driver for device.
	* @brief Constructs 25LC040A driver with SPI compatible driver.
	*/
       explicit EEPROM_25LC040A(ISpiBitBang* spi) noexcept;

	/**
	* @param address address to read bit from.
	* @throw 
	* - std::runtime_error if spi == nullptr.
	* - std::out_of_range @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - std::runtime_error device is stopped. Should use EEPROM_25LC040A::resume.
	* - std::exception See MockSpi::transferbytes for information.
	* @return read bit value.
	* @brief Read bit value by address.
	*/
        const bit readBit(const_type<pointer_size> address) const;

	/**
	* @brief Read byte value by address.
        * @param address address to read byte from.
        * @throw 
	* - std::runtime_error if spi == nullptr.
	* - std::out_of_range @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - std::runtime_error device is stopped. Should use EEPROM_25LC040A::resume.
	* - std::exception See MockSpi::transferbytes for information.
        * @return read byte value.
        */	
        const byte readByte(const_type<pointer_size> address) const;

	/**
	* @brief Read byte array by address.
        * @param address address to read byte array from.
	* @param length bytes count to read.
        * @throw 
	* - std::runtime_error if spi == nullptr.
	* - std::invalid_argument if length == 0.
	* - std::out_of_range @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - std::runtime_error device is stopped. Should use EEPROM_25LC040A::resume.
	* - std::exception See MockSpi::transferbytes for information.
        * @return pointer to requested segment.
	* @note received byte array must be released <TT><b>manually</b></TT>.
	* @warning if <TT>address + length > EEPROM_25LC040A::MAX_ADDRESS</TT>. When internal "pointer" of reading data from @c memory will reach <TT>EEPROM_25LC040A::MAX_ADDRESS + 1</TT> "pointer" will be assigned @c NULL and will continue reading unless all requested data is read.
        */
        const byte_array readByteArray(const_type<pointer_size> address, const_type<array_size> length) const;

	/**
	* @param address address to write bit to.
	* @param data bit value to write.
	* @throw std::exception See MockSpi::transferbytes for information.
	* - std::out_of_range @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - std::runtime_error device is stopped. Should use EEPROM_25LC040A::resume.
	* @note Rewrite first bit by given address. Preserves other 7 bits in current byte.
	* @brief Write bit value by address.
	*/
        void writeBit(const_type<pointer_size> address, const_type<bit> data) const;

	/**
	* @param address address to write byte to.
	* @param data byte value to write.
	* @throw std::exception See MockSpi::transferbytes for information.
	* - std::out_of_range @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - std::runtime_error device is stopped. Should use EEPROM_25LC040A::resume.
	* @brief Write byte value by address.
	*/ 
        void writeByte(const_type<pointer_size> address, const_type<byte> data) const;

	/**
	* @param address address to write byte array to.
	* @param data pointer to byte array to write.
	* @param length count of bytes to write.
	* @throw std::exception See MockSpi::transferbytes for information.
	* - std::out_of_range @c address is greater than EEPROM_25LC040A::MAX_ADDRESS.
	* - std::runtime_error device is stopped. Should use EEPROM_25LC040A::resume.
	* @warning if <TT>address + length > EEPROM_25LC040A::MAX_ADDRESS</TT>. When internal "pointer" of writing data from @c memory will reach <TT>EEPROM_25LC040A::MAX_ADDRESS + 1</TT> "pointer" will be assigned @c NULL and will continue writing unless all requested data is written.
	* @brief Write byte array by address.
	*/
        void writeByteArray(const_type<pointer_size> address, const byte_array data, const_type<array_size> length) const;

	/**
	* @brief Stop device.
	*/
        inline void stop() noexcept;

	/**
	* @brief Resume device.
	*/
        inline void resume() noexcept;
    private:
	/**
	* @brief SPI protocol compatible driver.
	*/
        ISpiBitBang* spi;

	/**
	* @brief Device is working now.
	*/
        bool isWorking = true;

	/**
	* @param address address to validate.
	* @throw std::out_of_range address is greater than  EEPROM_25LC040A::MAX_ADDRESS.
	* @brief Validate address.
	*/ 
        static inline void validateAddress(const_type<pointer_size> address);

	/**
	* @throw std::runtime_error device is not working (EEPROM_25LC040A::isWorking == false).
	* @brief Validate whether device is working.
	*/
        inline void validateState() const;

	/**
	* @param address address to execute command to.
	* @param cmd command to execute. See EEPROM_25LC040A::Command.
	* @return instruction.
	* @brief Create instruction to execute.
	*/
        static inline mask_type createInstruction(const_type<pointer_size> address, const_type<Command> cmd) noexcept;
    };
#endif
