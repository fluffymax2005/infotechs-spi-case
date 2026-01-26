#include "../src/include/mock_spi_driver.h"
#include "test_runner.h"

int main() {
    MockSpi spi;
    TestRunner runner(&spi);
}