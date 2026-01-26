#include "test_runner.h"

void TestRunner::printSummary() const {
    std::cout << std::endl << "=== Tests summary ===" << std::endl;
    std::cout << "Passed: " << testsPassed << std::endl;
    std::cout << "Failed: " << testsFailed << std::endl;
    std::cout << "Total: " << (testsPassed + testsFailed) << std::endl;
}

