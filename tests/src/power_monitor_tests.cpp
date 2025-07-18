
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "power_monitor.hpp"



extern "C"
{
    #include "freertos/task.h"
    #include "freertos/FreeRTOS.h"
    #include "common.h"   
}

TEST_GROUP(PowerMonitor_tests) 
{
    PowerMonitor * powerMonitor = nullptr;
    
    void setup()
    {
        // !TODO: create power monitor instance with appropriate parameters
        powerMonitor = new PowerMonitor();
    }

    void teardown()
    {
        mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
        mock().expectOneCall("eTaskStateGet").andReturnValue((eTaskState)eRunning);
        mock().expectOneCall("vTaskDelete");

        delete powerMonitor;

        powerMonitor = nullptr;

        CHECK(powerMonitor == nullptr);

        mock().clear();
    }
};

TEST(PowerMonitor_tests, Constructor)
{
    /* check power monitor is not null  */
    CHECK(powerMonitor != NULL);    
}

TEST(PowerMonitor_tests, initTask)
{
    

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    retVal_t ret = powerMonitor->initTask();
    /* this should return fail but not call any other functions  */
    CHECK_EQUAL(ERR_IN_ISR, ret);


    /* create real memory block, wont be used but needs to be a real location in memory */
    const StaticTask_t memBlock = {0};

    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    
    /* expect xTaskCreate to not run smoothly   */
    mock().expectOneCall("xTaskCreate")
    .withBoolParameter("pxTaskCode", true) /* this shoudl return true  since function ptr should not be nullptr */
    .withStringParameter("pcName", "PowerMonitor")
    .withBoolParameter("usStackDepth", true) /* this should be true if stack size greater than min */
    .withPointerParameter("pvParameters", powerMonitor)
    .withParameter("uxPriority", ESP_NORMAL_PRIORITY)
    .withOutputParameterReturning("pxCreatedTask", &memBlock, sizeof(StaticTask_t *))
    .ignoreOtherParameters()
    .andReturnValue(pdFAIL);
    
    ret = powerMonitor->initTask();
    
    CHECK_EQUAL(ERR_OS_FAIL, ret);

    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    
    /* expect xTaskCreate to not run smoothly   */
    mock().expectOneCall("xTaskCreate")
    .withBoolParameter("pxTaskCode", true) /* this shoudl return true  since function ptr should not be nullptr */
    .withStringParameter("pcName", "PowerMonitor")
    .withBoolParameter("usStackDepth", true) /* this should be true if stack size greater than min */
    .withPointerParameter("pvParameters", powerMonitor)
    .withParameter("uxPriority", ESP_NORMAL_PRIORITY)
    .withOutputParameterReturning("pxCreatedTask", &memBlock, sizeof(StaticTask_t *))
    .ignoreOtherParameters()
    .andReturnValue(pdPASS);
    
    ret = powerMonitor->initTask();
    
    CHECK_EQUAL(ERR_NONE, ret);
}

// TEST(PowerMonitor_tests, taskRun)
// {
//     // mock("BusVoltage")
//     //     .expectOneCall("getVoltage")
//     //     .andReturnValue(230.0f);

//     // taskRun();
// }
