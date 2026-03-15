
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "power_monitor.hpp"



extern "C"
{
    #include "freertos/task.h"
    #include "freertos/FreeRTOS.h"
    #include "helper.h"   
}

TEST_GROUP(PowerMonitor_tests) 
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
        mock().expectOneCall("BusVoltage::BusVoltage")
        .ignoreOtherParameters();

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
        mock().expectOneCall("BusVoltage::~BusVoltage");
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
        /*  expect taskRun to be called */
        mock().expectNCalls(1, "FOREVER")
            .andReturnValue(true);
        mock().expectOneCall("FOREVER")
            .andReturnValue(false);
    }

    void expectTeardownHappyPath(void)
    {
        /* check power monitor is not null  */
        CHECK(powerMonitorModule != NULL);

        /*  unhappy path destructor */
        mock().expectOneCall("xPortInIsrContext").andReturnValue(0);

        mock().expectOneCall("CHECK_POINTER_VALID")
            /*  we dont care what is passed into check pointer valid */
            .ignoreOtherParameters()
            /* but well return true to proceed with other tests */
            .andReturnValue(true);

        mock().expectOneCall("eTaskGetState")
            .ignoreOtherParameters()
            .andReturnValue((eTaskState) eRunning);
        /* expect vTaskDelete to be called */
        mock().expectOneCall("vTaskDelete")
            .ignoreOtherParameters();
    }
};



TEST(PowerMonitor_tests, Constructor)
{
    /* check power monitor is not null  */
    CHECK(powerMonitorModule != NULL);


    /* Setup expectations for teardown  */
    /*  happy path destructor */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    mock().expectOneCall("CHECK_POINTER_VALID")
        .ignoreOtherParameters()
        .andReturnValue(true);
    mock().expectOneCall("eTaskGetState")
        .ignoreOtherParameters()
        .andReturnValue((eTaskState)eRunning);
    mock().expectOneCall("vTaskDelete")
        .ignoreOtherParameters();
}


TEST(PowerMonitor_tests, DestructorInISR)
{
    /* check power monitor is not null  */
    CHECK(powerMonitorModule != NULL);

    /*  unhappy path destructor */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);

    /* expect an os error to occur */
    sprintf(logBuffer, "Error: %i", STATUS_ISR_ERROR);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "Task")
        .withParameter("format", logBuffer);
    
}

TEST(PowerMonitor_tests, DestructorTaskHdlNull)
{
    /* check power monitor is not null  */
    CHECK(powerMonitorModule != NULL);

    /*  unhappy path destructor */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);

    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return false to proceed with other tests */
        .andReturnValue(false);

    /* expect a null pointer error to occur */
    sprintf(logBuffer, "Error: %i", STATUS_NULL_POINTER);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "Task")
        .withParameter("format", logBuffer);
}


TEST(PowerMonitor_tests, DestructorTaskStateDeleted)
{
    /* check power monitor is not null  */
    CHECK(powerMonitorModule != NULL);

    /*  unhappy path destructor */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);

    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return true to proceed with other tests */
        .andReturnValue(true);

    mock().expectOneCall("eTaskGetState")
        .ignoreOtherParameters()
        .andReturnValue((eTaskState)eDeleted);

    /* expect a null pointer error to occur */
    sprintf(logBuffer, "Error: %i", STATUS_OS_ERROR);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "Task")
        .withParameter("format", logBuffer);
}

TEST(PowerMonitor_tests, DestructorHappyPath)
{
    expectTeardownHappyPath();
}

TEST(PowerMonitor_tests, initTask)
{
    /*  Test with non null task handle  */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return true for testing purposes    */
        .andReturnValue(true);
    Status_t ret = powerMonitorModule->initTask();
    CHECK_EQUAL(STATUS_REINIT_ERROR, ret);
    

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return false to proceed with other tests */
        .andReturnValue(false);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    ret = powerMonitorModule->initTask();
    /* this should return fail but not call any other functions  */
    CHECK_EQUAL(STATUS_ISR_ERROR, ret);
    
    
    /*  expect call to check pointer valid */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return false to proceed with other tests */
        .andReturnValue(false);
    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    
    /* return a fail from xtaskcreate   */
    mock().expectOneCall("xTaskCreate")
    .withBoolParameter("pxTaskCode", true) /* this shoudl return true  since function ptr should not be nullptr */
    .withStringParameter("pcName", "PowerMonitor")
    .withBoolParameter("usStackDepth", true) /* this should be true if stack size greater than min */
    .withPointerParameter("pvParameters", powerMonitorModule)
    .withParameter("uxPriority", ESP_NORMAL_PRIORITY)
    .ignoreOtherParameters()
    .andReturnValue(pdFAIL);
    
    ret = powerMonitorModule->initTask();
    
    CHECK_EQUAL(STATUS_OS_ERROR, ret);


    /*  expect call to check pointer valid */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return false to proceed with other tests */
        .andReturnValue(false);
    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    
    /* return pass from xtaskcreate */
    mock().expectOneCall("xTaskCreate")
    .withBoolParameter("pxTaskCode", true) /* this shoudl return true  since function ptr should not be nullptr */
    .withStringParameter("pcName", "PowerMonitor")
    .withBoolParameter("usStackDepth", true) /* this should be true if stack size greater than min */
    .withPointerParameter("pvParameters", powerMonitorModule)
    .withParameter("uxPriority", ESP_NORMAL_PRIORITY)
    .ignoreOtherParameters()
    .andReturnValue(pdPASS);
    
    ret = powerMonitorModule->initTask();
    
    CHECK_EQUAL(STATUS_OKAY, ret);

    /* Setup expectations for teardown  */
    expectTeardownHappyPath();
}

TEST(PowerMonitor_tests, runInCurrent)
{
    /* expect taskRun to be called */
    expectRunTwice();  
    
    /*  notification doesnt return any useful value  */
    uint32_t notificationValue = 0;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdFALSE);

    /* expect an os error to occur */
    sprintf(logBuffer, "Error: %i", STATUS_OS_ERROR);

    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "PowerMonitor")
        .withParameter("format", logBuffer);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();

    /* run task run once then return    */
    expectRunTwice();

    /*  notification doesnt return any useful value  */
    notificationValue = GET_POWER_NOTIFY_BIT;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdTRUE);
    
    /*  expect bus voltage to be called and fail due to hal error*/
    mock().expectOneCall("getFilteredVoltage")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_HAL_ERROR);

    sprintf(logBuffer, "Error: %i", STATUS_HAL_ERROR);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "PowerMonitor")
        .withParameter("format", logBuffer);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();

    /* run task run once then return    */
    expectRunTwice();

    /*  notification doesnt return any useful value  */
    notificationValue = GET_POWER_NOTIFY_BIT;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdTRUE);
    
    /*  expect bus voltage to be called and fail due to queue full */
    mock().expectOneCall("getFilteredVoltage")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_QUEUE_FULL);

    /*  expect bus voltage to be called and fail due to queue full error */
    sprintf(logBuffer, "Error: %i", STATUS_QUEUE_FULL);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "PowerMonitor")
        .withParameter("format", logBuffer);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();


/******************************************************************************
 * Get Filtered Current HAL Error
 *****************************************************************************/
    /* run task run once then return    */
    expectRunTwice();

    /*  notification doesnt return any useful value  */
    notificationValue = GET_POWER_NOTIFY_BIT;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdTRUE);
    
    /*  expect bus voltage to be called and fail due to queue full */
    mock().expectOneCall("getFilteredVoltage")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);

    /*  expect bus current to be called and fail due to queue full error */
    mock().expectOneCall("getFilteredCurrent")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_HAL_ERROR);

    /*  expect log to return hal error */
    sprintf(logBuffer, "Error: %i", STATUS_HAL_ERROR);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "PowerMonitor")
        .withParameter("format", logBuffer);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();

/******************************************************************************
 * Get Filtered Current Queue Full
 *****************************************************************************/
    /* run task run once then return    */
    expectRunTwice();

    /*  notification doesnt return any useful value  */
    notificationValue = GET_POWER_NOTIFY_BIT;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdTRUE);
    
    /*  expect bus voltage to be called and fail due to queue full */
    mock().expectOneCall("getFilteredVoltage")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);

    /*  expect bus current to be called and fail due to queue full error */
    mock().expectOneCall("getFilteredCurrent")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_QUEUE_FULL);

    /*  expect log to return hal error */
    sprintf(logBuffer, "Error: %i", STATUS_QUEUE_FULL);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "PowerMonitor")
        .withParameter("format", logBuffer);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();

/******************************************************************************
 * Send Power Message Queue Full
 *****************************************************************************/
    /* run task run once then return    */
    expectRunTwice();

    /*  notification doesnt return any useful value  */
    notificationValue = GET_POWER_NOTIFY_BIT;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdTRUE);
    
    /*  expect bus voltage to be called and fail due to queue full */
    mock().expectOneCall("getFilteredVoltage")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);

    /*  expect bus current to be called and fail due to queue full error */
    mock().expectOneCall("getFilteredCurrent")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);
    
    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_QUEUE_FULL);

    /*  expect log to return hal error */
    sprintf(logBuffer, "Error: %i", STATUS_QUEUE_FULL);
    mock().expectOneCall("esp_log_write")
        .withParameter("level", ESP_LOG_ERROR)
        .withParameter("tag", "PowerMonitor")
        .withParameter("format", logBuffer);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();
/******************************************************************************
 * Send Power Message Queue Full
 *****************************************************************************/
    /* run task run once then return    */
    expectRunTwice();

    /*  notification doesnt return any useful value  */
    notificationValue = GET_POWER_NOTIFY_BIT;
    mock().expectOneCall("xTaskNotifyWait")
        .withOutputParameterReturning("pulNotificationValue", &notificationValue, sizeof(uint32_t))
        .ignoreOtherParameters()
        .andReturnValue(pdTRUE);
    
    /*  expect bus voltage to be called and fail due to queue full */
    mock().expectOneCall("getFilteredVoltage")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);

    /*  expect bus current to be called and fail due to queue full error */
    mock().expectOneCall("getFilteredCurrent")
        .ignoreOtherParameters()
        .andReturnValue(STATUS_OKAY);

    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);
    
    /* expect xtaskgettickcount to be called */
    mock().expectOneCall("xTaskGetTickCount")
        .ignoreOtherParameters()
        .andReturnValue(1000 / portTICK_PERIOD_MS);
    
    /* expect networking module queue message to be called */
    mock().expectOneCall("queueNetworkingMessage")
        .withParameter("message", true) /* this value should not be null */
        .andReturnValue(STATUS_OKAY);

    mock().expectOneCall("vTaskDelay")
        .withParameter("xTicksToDelay", 100 / portTICK_PERIOD_MS);
    
    powerMonitorModule->runInCurrent();

    /* Setup expectations for teardown  */
    expectTeardownHappyPath();
}

TEST(PowerMonitor_tests, SuspendAndResume)
{
    /*  Make a call to task suspend with an unitialized taskhandle  */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .ignoreOtherParameters()
        /* we want this to return false to simulate error */
        .andReturnValue(false);
    Status_t ret = powerMonitorModule->suspend();
    /* if task handle uninitialized this should return null pointer  */
    CHECK_EQUAL(STATUS_NULL_POINTER, ret);


    /*  Make a call to task resume with an unitialized taskhandle  */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .ignoreOtherParameters()
        /* we want this to return false to simulate error */
        .andReturnValue(false);
    /*  Test with null task handle    */
    ret = powerMonitorModule->resume();
    /* this should return null pointer  */
    CHECK_EQUAL(STATUS_NULL_POINTER, ret);

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .ignoreOtherParameters()
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    ret = powerMonitorModule->suspend();
    /* this should return fail but not call any other functions  */
    CHECK_EQUAL(STATUS_ISR_ERROR, ret);

    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .ignoreOtherParameters()
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    /* expect vTaskSuspend to be called */
    mock().expectOneCall("vTaskSuspend")
        /* we dont care about the value of xTaskToSuspend since we are mocking it */    
        .ignoreOtherParameters();
    ret = powerMonitorModule->suspend();
    CHECK_EQUAL(STATUS_OKAY, ret);

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .ignoreOtherParameters()
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    ret = powerMonitorModule->resume();
    /* this should return in isr error */
    CHECK_EQUAL(STATUS_ISR_ERROR, ret);

    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .ignoreOtherParameters()
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    /* expect vTaskResume to be called */
    mock().expectOneCall("vTaskResume")
        /* ignore value into vtask resume since we are mocking handle above */
        .ignoreOtherParameters();
    ret = powerMonitorModule->resume();
    CHECK_EQUAL(STATUS_OKAY, ret);


    /* Setup expectations for teardown  */
    expectTeardownHappyPath();
}