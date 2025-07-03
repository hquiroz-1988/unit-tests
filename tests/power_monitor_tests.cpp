
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

// #include "power_monitor.hpp"

extern "C"
{
    
}

TEST_GROUP(PowerMonitor_tests)
{
    void setup()
    {
    }

    void teardown()
    {
        // mock("BusVoltage").checkExpectations();
        // mock().clear();
    }
};

TEST(PowerMonitor_tests, taskRun)
{
    // mock("BusVoltage")
    //     .expectOneCall("getVoltage")
    //     .andReturnValue(230.0f);

    // taskRun();
}
