#ifndef NOT_IMPLEMENTED_EXCEPTION_H
    #define NOT_IMPLEMENTED_EXCEPTION_H
    
    #include <exception>

    class NotImplementedException : public std::exception {
    public:
        using char_t = char;
        using const_string = const char_t*;

        NotImplementedException() noexcept;
        NotImplementedException(const_string data) noexcept;

        virtual const_string what() const noexcept override;
    private:
        const_string string;
    };
#endif