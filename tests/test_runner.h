#ifndef TEST_RUNNER_H
    #define TEST_RUNNER_H

    #include "../src/include/eeprom_25lc040a.h"

    #include <exception>
    #include <ostream>
    #include <string>
    #include <iostream>

    class TestRunner {
    public:
        TestRunner(ISpiBitBang* spi);

        template <typename Func>
        void runTest(const std::string& name, Func func);

        void printSummary() const;

    private:
        int testsPassed{0};
        int testsFailed{0};

        EEPROM_25LC040A eeprom;
    };

#endif