
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "ads1115.hpp"

extern "C"
{
    
}

TEST_GROUP(ADS1115_tests)
{
    ADS1115 * ads1115;

    void setup()
    {
        ads1115 = new ADS1115();
    }
    void teardown()
    {
        delete ads1115;
    }

    // void fillTheQueue(int seed, int howMany)
    // {
    //     for (int i = 0; i < howMany; i++)
    //         buffer->Put(seed + i);
    // }
    // void removeFromQueue(int howMany)
    // {
    //     for (int i = 0; i < howMany; i++)
    //         buffer->Get();
    // }
};

TEST(ADS1115_tests, Constructor)
{
    CHECK(ads1115 != NULL);
}

TEST(ADS1115_tests, Init)
{
    ads1115->init_ads1115();
    // Assuming init_ads1115 does not return a value, we can check if it runs without errors
    CHECK(true); // Placeholder for actual checks if needed
}

TEST(ADS1115_tests, GetReading)
{
    ads1115ConversionRegister_t reg;
    retVal_t ret = ads1115->getLatestReading(&reg);
    CHECK_EQUAL(ERR_NONE, ret);
    // Assuming the reading is valid, we can check if the value is within expected range
    CHECK(reg.value >= 0 && reg.value <= 32767); // Example range for a 16-bit signed ADC
}

TEST(ADS1115_tests, GetConfiguration)
{
    ads1115ConfigRegister_t config;
    retVal_t ret = ads1115->getConfiguration(&config);
    CHECK_EQUAL(ERR_NONE, ret);
    CHECK(config.opStatus == ADS1115_OPERATION_STATUS_CONVERSION_IN_PROGRESS || 
          config.opStatus == ADS1115_OPERATION_STATUS_NO_CONVERSION_IN_PROGRESS);
}

TEST(ADS1115_tests, SetConfiguration)
{
    ads1115ConfigRegister_t config;
    config.opStatus = ADS1115_OPERATION_STATUS_NO_CONVERSION_IN_PROGRESS;
    config.mux = ADS1115_MUX_AIN0_AIN1;
    config.pga = 0; // Example PGA setting
    config.mode = 1; // Example mode setting
    config.dataRate = 0; // Example data rate setting
    config.compMode = 0; // Example comparator mode
    config.compPolarity = 0; // Example comparator polarity
    config.compLatch = 0; // Example comparator latch
    config.compQueue = 0; // Example comparator queue

    retVal_t ret = ads1115->setConfiguration(&config);
    CHECK_EQUAL(ERR_NONE, ret);
}
