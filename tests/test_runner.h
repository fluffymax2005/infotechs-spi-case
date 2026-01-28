/** 
* @file test_runner.h
* @brief Provides basic test runner for project.
*/

#ifndef TEST_RUNNER_H

    /**
    * @def TEST_RUNNER_H
    * @brief Include module macro.
    */
    #define TEST_RUNNER_H

    #include "../src/include/eeprom_25lc040a.h"

    #include <exception>
    #include <ostream>
    #include <string>
    #include <iostream>

    /**
    * @class TestRunner
    * @brief Test runner class for project.
    */
    class TestRunner {
    public:
	/**
	* @brief Default constructor.
	*/
        TestRunner() = default;

	/**
	* @param name name of the test.
	* @param func function to test.
	* @brief Execute test.
	*/
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

	/**
	* @brief Print tests summary.
	*/
        void printSummary() const;

    private:
	/**
	* @brief count of passed tests.
	*/
        int testsPassed{0};

	/**
	* @brief count of failed test.
	*/
        int testsFailed{0};
    };

#endif
