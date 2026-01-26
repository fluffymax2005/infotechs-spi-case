#ifndef TEST_RUNNER_H
    #define TEST_RUNNER_H

    #include "../src/include/eeprom_25lc040a.h"

    #include <exception>
    #include <ostream>
    #include <string>
    #include <iostream>

    class TestRunner {
    public:
        TestRunner() = default;

        template <typename Func>
        void runTest(const std::string& name, Func func) {
            std::cout << std::endl << "=== RUNNING test: " << name << std::endl;

            try {
                func();
                ++testsPassed;
                std::cout << "[PASS] " << name << std::endl;
            } catch (const std::exception& e) {
                ++testsFailed;
                std::cout << "[FAIL] " << name << ": " << e.what() << std::endl;
            }
        }

        void printSummary() const;

    private:
        int testsPassed{0};
        int testsFailed{0};
    };

#endif