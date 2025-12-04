
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "i2c_bus.hpp"

extern "C"
{
    
}

TEST_GROUP(I2CBus_tests)
{
    Gpio * sdaPin = nullptr;
    Gpio * sclPin = nullptr;
    I2CBus * i2cBusModule = nullptr;
    i2c_port_t i2cPort = I2C_NUM_0;
    void setup()
    {
        mock().expectOneCall("Gpio::Gpio").ignoreOtherParameters();
        mock().expectOneCall("Gpio::Gpio").ignoreOtherParameters();
        mock().expectOneCall("Mutex::Mutex").ignoreOtherParameters();

        sdaPin = new Gpio(GpioPin::GPIO_PIN_NONE);
        sclPin = new Gpio(GpioPin::GPIO_PIN_NONE);
        i2cBusModule = new I2CBus(*sdaPin, *sclPin, i2cPort);
    }

    void teardown()
    {
        mock().expectOneCall("Gpio::~Gpio").ignoreOtherParameters();
        mock().expectOneCall("Gpio::~Gpio").ignoreOtherParameters();
        mock().expectOneCall("Mutex::~Mutex").ignoreOtherParameters();

        delete i2cBusModule;
        delete sclPin;
        delete sdaPin;

        mock().checkExpectations();
        mock().clear();
    }
};


TEST(I2CBus_tests, Constructor)
{
    /* check i2c bus is not null  */
    CHECK(i2cBusModule != NULL);
}

TEST(I2CBus_tests, Initialize)
{
    Status_t status = STATUS_OKAY;

    /*  call gpio pins */
    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect an i2c_driver_install failure */
    mock().expectOneCall("i2c_driver_install")
    .ignoreOtherParameters()
    .andReturnValue(ESP_FAIL);

    /* expect no other calls if install fails*/    
    status = i2cBusModule->initialize();

    CHECK_EQUAL(STATUS_HAL_ERROR, status);

    /*  call gpio pins */
    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect an i2c_driver_install  */
    mock().expectOneCall("i2c_driver_install")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);

    mock().expectOneCall("i2c_param_config")
    .ignoreOtherParameters()
    .andReturnValue(ESP_FAIL);

    /* expect no other calls if install fails*/    
    status = i2cBusModule->initialize();

    CHECK_EQUAL(STATUS_HAL_ERROR, status);


        /*  call gpio pins */
    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect an i2c_driver_install  */
    mock().expectOneCall("i2c_driver_install")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);

    mock().expectOneCall("i2c_param_config")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);

    mock().expectOneCall("i2c_cmd_link_create")
    .ignoreOtherParameters()
    .andReturnValue((i2c_cmd_handle_t)nullptr);

    /* expect no other calls if install fails*/    
    status = i2cBusModule->initialize();

    CHECK_EQUAL(STATUS_HAL_ERROR, status);

        /*  call gpio pins */
    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect an i2c_driver_install  */
    mock().expectOneCall("i2c_driver_install")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);    

    mock().expectOneCall("i2c_param_config")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);

    uint8_t fakeCmdHandle[9];

    mock().expectOneCall("i2c_cmd_link_create")
    .ignoreOtherParameters()
    .andReturnValue((i2c_cmd_handle_t)&fakeCmdHandle);

    mock().expectOneCall("Mutex::create")
    .ignoreOtherParameters()
    .andReturnValue(STATUS_UNKNOWN);

    /* expect no other calls if install fails*/    
    status = i2cBusModule->initialize();

    CHECK_EQUAL(STATUS_UNKNOWN, status);


        /*  call gpio pins */
    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect call to get pin */
    mock().expectOneCall("Gpio::getPin")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_NUM_0);

    /* expect call to get pullup */
    mock().expectOneCall("Gpio::getPullup")
    .ignoreOtherParameters()
    .andReturnValue((int)GPIO_PULLUP_ENABLE);

    /* expect an i2c_driver_install  */
    mock().expectOneCall("i2c_driver_install")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);    

    mock().expectOneCall("i2c_param_config")
    .ignoreOtherParameters()
    .andReturnValue(ESP_OK);

    mock().expectOneCall("i2c_cmd_link_create")
    .ignoreOtherParameters()
    .andReturnValue((i2c_cmd_handle_t)&fakeCmdHandle);

    mock().expectOneCall("Mutex::create")
    .ignoreOtherParameters()
    .andReturnValue(STATUS_OKAY);

    /* expect no other calls if install fails*/    
    status = i2cBusModule->initialize();

    CHECK_EQUAL(STATUS_OKAY, status);
}
