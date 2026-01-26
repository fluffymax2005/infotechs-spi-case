#include "test_runner.h"

TestRunner::TestRunner(ISpiBitBang* spi) : eeprom(spi) {}

template <typename Func>
void TestRunner::runTest(const std::string& name, Func func) {
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

void TestRunner::printSummary() const {
    std::cout << std::endl << "=== Tests summary ===" << std::endl;
    std::cout << "Passed: " << testsPassed << std::endl;
    std::cout << "Failed: " << testsFailed << std::endl;
    std::cout << "Total: " << (testsPassed + testsFailed) << std::endl;
}

