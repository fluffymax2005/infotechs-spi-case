#ifndef MOCK_SPI_DRIVER
    #define MOCK_SPI_DRIVER

    #include "../src/include/spi_interface.h"
    
    class MockSpi : public ISpiBitBang {
    public:
        MockSpi() = default;  
        ~MockSpi() = default;      

        // ============ OVERRIDES
        
        void chipSelect() override;
        void chipDeselect() override;
        virtual byte transferBit(const_type<bit> data) override;
        byte transferByte(byte data) override;
        byte_array transferBytes(const byte_array data, const_type<array_size> length) override;
        void delay(uint32_t us) override;
        
        /*
        void setNextByteResponse(uint8_t response);
        
        void setByteResponses(const std::vector<uint8_t>& responses);
        
        void setNextBytesResponse(const std::vector<uint8_t>& response);

        void setTransferByteHandler(std::function<uint8_t(uint8_t)> handler);
        
        void setTransferBytesHandler(
            std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> handler);

        CallCounters getCallCounters() const;

        uint8_t getLastTransferredByte() const;
        
        std::vector<uint8_t> getLastTransferredBytes() const;

        void reset();

        void verifyCallCounters(const CallCounters& expected);

        */
    
    private:
       
        /*
        // Очередь предопределённых ответов
        std::queue<uint8_t> byteResponses;
        std::queue<std::vector<uint8_t>> bytesResponses;
        
        // Счётчики вызовов
        struct CallCounters {
            int chipSelect = 0;
            int chipDeselect = 0;
            int transferByte = 0;
            int transferBytes = 0;
            int setSpeed = 0;
            int delayUs = 0;
        } counters;
        
        // Последние переданные данные (для проверки)
        std::vector<uint8_t> lastTransferredBytes;
        uint8_t lastTransferredByte = 0;
        
        // Коллбэки для кастомного поведения
        std::function<uint8_t(uint8_t)> onTransferByte;
        std::function<std::vector<uint8_t>(const std::vector<uint8_t>&)> onTransferBytes;
        */
    
    };

#endif