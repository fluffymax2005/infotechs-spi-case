/**
*   @file not_implemented_exception.h
*   @brief Module for NotImplementedException.
*/

#ifndef NOT_IMPLEMENTED_EXCEPTION_H
    #define NOT_IMPLEMENTED_EXCEPTION_H
    
    #include <exception>

    /**
    *  @class NotImplementedException
    *  @brief Provides interface to handle errors for methods (functions) that are not implemented yet.	
    */
    class NotImplementedException : public std::exception {
    public:

	/**
	* @typedef char_t
	* @brief Type of chars used to generate error message string.
	*/
        using char_t = char;

	/**
	* @typedef const_string
	* @brief String type to generate error message string.
	*/
        using const_string = const char_t*;

	/**
	* @brief Default constructor.
	*/
        NotImplementedException() noexcept;

	/**
	* @param data error message string.
	* @brief Sets error message.
	*/
        NotImplementedException(const_string data) noexcept;

	/**
	* @returns error message string.
	* @brief Get error message string.
	*/
        virtual const_string what() const noexcept override;
    private:
	/**
	* @brief Contains error message
	*/
        const_string string;
    };
#endif
