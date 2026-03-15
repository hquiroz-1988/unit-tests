
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "bus_voltage.hpp"
#include "bus_current.hpp"

extern "C"
{
    #include "freertos/task.h"
    #include "freertos/FreeRTOS.h"
    #include "helper.h"   
}

TEST_GROUP(BusVoltage_tests) 
{
    Gpio * gpioModule = nullptr;
    BusVoltage * busVoltageModule = nullptr;
    ADS1115 * ads1115Module = nullptr;
    INA219 * ina219Module = nullptr;
    PowerMonitor * powerMonitorModule = nullptr;
    NetworkingModule * networkingModule = nullptr;
    BusCurrent * busCurrentModule = nullptr;


    char logBuffer[256];

    void setup()
    {
        /* setup expectations for following constructors */
        mock().expectOneCall("Gpio").ignoreOtherParameters();
        mock().expectOneCall("InterruptBase").ignoreOtherParameters();
        mock().expectOneCall("I2CDevice").ignoreOtherParameters();
        mock().expectOneCall("ADS1115").ignoreOtherParameters();
        mock().expectOneCall("ADS1115Channel").ignoreOtherParameters();
        mock().expectOneCall("INA219").ignoreOtherParameters();
        mock().expectOneCall("BusCurrent").ignoreOtherParameters();
        mock().expectOneCall("NetworkingModule").ignoreOtherParameters();
        mock().expectOneCall("Task").ignoreOtherParameters();
        mock().expectOneCall("PowerMonitor").ignoreOtherParameters();

        gpioModule = new Gpio(GpioPin::GPIO_PIN_NONE);
        ads1115Module = new ADS1115(*gpioModule);
        busVoltageModule = new BusVoltage(*ads1115Module);
        ina219Module = new INA219();
        busCurrentModule = new BusCurrent(*ina219Module, powerMonitorModule);
        networkingModule = new NetworkingModule();
        powerMonitorModule = new PowerMonitor(*networkingModule, *busVoltageModule, *busCurrentModule);
    }

    void teardown()
    {
        /* teardown expectations */
        mock().expectOneCall("~PowerMonitor");
        mock().expectOneCall("~Task");
        mock().expectOneCall("~NetworkingModule");
        mock().expectOneCall("~ADS1115Channel");
        mock().expectOneCall("~ADS1115");
        mock().expectOneCall("~I2CDevice");
        mock().expectOneCall("~InterruptBase");
        mock().expectOneCall("~BusCurrent");
        mock().expectOneCall("~INA219");
        mock().expectOneCall("~Gpio");

        delete powerMonitorModule;
        delete networkingModule;
        delete busVoltageModule;
        delete ads1115Module;
        delete busCurrentModule;
        delete ina219Module;
        delete gpioModule;


        /* address dangling pointers */
        powerMonitorModule = nullptr;
        networkingModule = nullptr;
        busVoltageModule = nullptr;
        ads1115Module = nullptr;
        busCurrentModule = nullptr;
        ina219Module = nullptr;
        gpioModule = nullptr;

        CHECK(powerMonitorModule == nullptr);
        CHECK(networkingModule == nullptr);
        CHECK(busVoltageModule == nullptr);
        CHECK(ads1115Module == nullptr);
        CHECK(busCurrentModule == nullptr);
        CHECK(ina219Module == nullptr);
        CHECK(gpioModule == nullptr);

        mock().checkExpectations();
        mock().clear();
    }

    /* helper functions  */
    void expectRunTwice(void)
    {

    }

    void expectTeardownHappyPath(void)
    {

    }
};

TEST(BusVoltage_tests, Constructor)
{
    CHECK(busVoltageModule != NULL);
}

TEST(BusVoltage_tests, Destructor)
{

}

TEST(BusVoltage_tests, Initialize)
{

    /* test hal error from configure path */
    mock().expectOneCall("ADS1115::configure")
        .withIntParameter("configObj.opStatus", static_cast<int>(ADS1115_OperationalStatus_t::Write_No_Effect))
        .withIntParameter("configObj.mux", static_cast<int>(ADS1115Mux_t::AIN0_GND))
        .withIntParameter("configObj.pga", static_cast<int>(ADS1115PGA_t::FSR_2_048V))
        .withIntParameter("configObj.mode", static_cast<int>(ADS1115Mode_t::SingleShot))
        .withIntParameter("configObj.dataRate", static_cast<int>(ADS1115DataRate_t::SPS_860))
        .withIntParameter("configObj.compMode", static_cast<int>(ADS1115CompMode_t::Window))
        .withIntParameter("configObj.compPolarity", static_cast<int>(ADS1115CompPolarity_t::ActiveLow))
        .withIntParameter("configObj.compLatch", static_cast<int>(ADS1115CompLatch_t::Latching))
        .withIntParameter("configObj.compQueue", static_cast<int>(ADS1115CompQueue_t::AssertAfterFourConversions))
        .andReturnValue(STATUS_HAL_ERROR);
    Status_t status = busVoltageModule->initialize(powerMonitorModule);
    CHECK_EQUAL(STATUS_HAL_ERROR, status);

    /* test low threshold out of bounds */
    mock().expectOneCall("ADS1115::configure")
        .withIntParameter("configObj.opStatus", static_cast<int>(ADS1115_OperationalStatus_t::Write_No_Effect))
        .withIntParameter("configObj.mux", static_cast<int>(ADS1115Mux_t::AIN0_GND))
        .withIntParameter("configObj.pga", static_cast<int>(ADS1115PGA_t::FSR_2_048V))
        .withIntParameter("configObj.mode", static_cast<int>(ADS1115Mode_t::SingleShot))
        .withIntParameter("configObj.dataRate", static_cast<int>(ADS1115DataRate_t::SPS_860))
        .withIntParameter("configObj.compMode", static_cast<int>(ADS1115CompMode_t::Window))
        .withIntParameter("configObj.compPolarity", static_cast<int>(ADS1115CompPolarity_t::ActiveLow))
        .withIntParameter("configObj.compLatch", static_cast<int>(ADS1115CompLatch_t::Latching))
        .withIntParameter("configObj.compQueue", static_cast<int>(ADS1115CompQueue_t::AssertAfterFourConversions))
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setLowThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_LO))
        .andReturnValue(STATUS_OUT_OF_BOUNDS);
    status = busVoltageModule->initialize(powerMonitorModule);
    CHECK_EQUAL(STATUS_OUT_OF_BOUNDS, status);

    /* test high threshold out of bounds */
    mock().expectOneCall("ADS1115::configure")
        .withIntParameter("configObj.opStatus", static_cast<int>(ADS1115_OperationalStatus_t::Write_No_Effect))
        .withIntParameter("configObj.mux", static_cast<int>(ADS1115Mux_t::AIN0_GND))
        .withIntParameter("configObj.pga", static_cast<int>(ADS1115PGA_t::FSR_2_048V))
        .withIntParameter("configObj.mode", static_cast<int>(ADS1115Mode_t::SingleShot))
        .withIntParameter("configObj.dataRate", static_cast<int>(ADS1115DataRate_t::SPS_860))
        .withIntParameter("configObj.compMode", static_cast<int>(ADS1115CompMode_t::Window))
        .withIntParameter("configObj.compPolarity", static_cast<int>(ADS1115CompPolarity_t::ActiveLow))
        .withIntParameter("configObj.compLatch", static_cast<int>(ADS1115CompLatch_t::Latching))
        .withIntParameter("configObj.compQueue", static_cast<int>(ADS1115CompQueue_t::AssertAfterFourConversions))
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setLowThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_LO))
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setHighThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_HI))
        .andReturnValue(STATUS_OUT_OF_BOUNDS);
    status = busVoltageModule->initialize(powerMonitorModule);
    CHECK_EQUAL(STATUS_OUT_OF_BOUNDS, status);

    /* test happy path */
    mock().expectOneCall("ADS1115::configure")
        .withIntParameter("configObj.opStatus", static_cast<int>(ADS1115_OperationalStatus_t::Write_No_Effect))
        .withIntParameter("configObj.mux", static_cast<int>(ADS1115Mux_t::AIN0_GND))
        .withIntParameter("configObj.pga", static_cast<int>(ADS1115PGA_t::FSR_2_048V))
        .withIntParameter("configObj.mode", static_cast<int>(ADS1115Mode_t::SingleShot))
        .withIntParameter("configObj.dataRate", static_cast<int>(ADS1115DataRate_t::SPS_860))
        .withIntParameter("configObj.compMode", static_cast<int>(ADS1115CompMode_t::Window))
        .withIntParameter("configObj.compPolarity", static_cast<int>(ADS1115CompPolarity_t::ActiveLow))
        .withIntParameter("configObj.compLatch", static_cast<int>(ADS1115CompLatch_t::Latching))
        .withIntParameter("configObj.compQueue", static_cast<int>(ADS1115CompQueue_t::AssertAfterFourConversions))
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setLowThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_LO))
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setHighThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_HI))
        .andReturnValue(STATUS_OKAY);
    status = busVoltageModule->initialize(powerMonitorModule);
    CHECK_EQUAL(STATUS_OKAY, status);

}
TEST(BusVoltage_tests, RunAlertISR)
{
    /* delete bus voltage module     */
    mock().expectOneCall("~ADS1115Channel");
    delete busVoltageModule;
    busVoltageModule = nullptr;
    CHECK(busVoltageModule == nullptr);

    /* now create busmodule again with a nullptr for power monitor  */
    mock().expectOneCall("ADS1115Channel")
        .ignoreOtherParameters();
    busVoltageModule = new BusVoltage(*ads1115Module);

    /* initialize bus voltage module with nullptr   */
    busVoltageModule->initialize(nullptr);

    /* with  nullptr power monitor reference, call isr */
    sprintf(logBuffer, "PowerMonitor is nullptr\n");
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "BusVoltage")
        .withParameter("format", logBuffer);
    (*busVoltageModule).runAlertISR(nullptr);


    /* delete bus voltage module     */
    mock().expectOneCall("~ADS1115Channel");
    delete busVoltageModule;
    busVoltageModule = nullptr;
    CHECK(busVoltageModule == nullptr);

    /* now create busmodule again with a nullptr for power monitor  */
    mock().expectOneCall("ADS1115Channel")
        .ignoreOtherParameters();
    busVoltageModule = new BusVoltage(*ads1115Module);

    /* initialize bus voltage module with nullptr   */
    mock().expectOneCall("ADS1115::configure")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setLowThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_LO))
        .andReturnValue(STATUS_OKAY);
    mock().expectOneCall("ADS1115Channel::setHighThreshold")
        .withParameter("value", static_cast<int16_t>(ADS1115_CONVERSION_COMPLETE_HI))
        .andReturnValue(STATUS_OKAY);
    busVoltageModule->initialize(powerMonitorModule);

    /* with  nullptr power monitor reference, call isr */
    mock().expectOneCall("PowerMonitor::notifyFromISR")
        .ignoreOtherParameters();
    (*busVoltageModule).runAlertISR(nullptr);
}