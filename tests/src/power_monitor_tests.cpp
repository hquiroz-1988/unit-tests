
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
    /*  Test with non null task handle  */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return true for testing purposes    */
        .andReturnValue(true);
    retVal_t ret = powerMonitor->initTask();
    CHECK_EQUAL(ERR_FAIL, ret);
    

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /*  we dont care what is passed into check pointer valid */
        .ignoreOtherParameters()
        /* but well return false to proceed with other tests */
        .andReturnValue(false);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    ret = powerMonitor->initTask();
    /* this should return fail but not call any other functions  */
    CHECK_EQUAL(ERR_IN_ISR, ret);
    
    
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
    .withPointerParameter("pvParameters", powerMonitor)
    .withParameter("uxPriority", ESP_NORMAL_PRIORITY)
    .ignoreOtherParameters()
    .andReturnValue(pdFAIL);
    
    ret = powerMonitor->initTask();
    
    CHECK_EQUAL(ERR_OS_FAIL, ret);


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
    .withPointerParameter("pvParameters", powerMonitor)
    .withParameter("uxPriority", ESP_NORMAL_PRIORITY)
    .ignoreOtherParameters()
    .andReturnValue(pdPASS);
    
    ret = powerMonitor->initTask();
    
    CHECK_EQUAL(ERR_NONE, ret);
}

TEST(PowerMonitor_tests, runInCurrent)
{
    /* expect taskRun to be called */
    mock().expectNCalls(2, "FOREVER")
        .andReturnValue(true)
        .andReturnValue(false);
    
    powerMonitor->runInCurrent();
}

TEST(PowerMonitor_tests, SuspendAndResume)
{
    /*  Make a call to task suspend with an unitialized taskhandle  */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .withBoolParameter("ptr", false)
        /* we want this to return false to simulate error */
        .andReturnValue(false);
    retVal_t ret = powerMonitor->suspend();
    /* if task handle uninitialized this should return null pointer  */
    CHECK_EQUAL(ERR_NULL_POINTER, ret);


    /*  Make a call to task resume with an unitialized taskhandle  */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .withBoolParameter("ptr", false)
        /* we want this to return false to simulate error */
        .andReturnValue(false);
    /*  Test with null task handle    */
    ret = powerMonitor->resume();
    /* this should return null pointer  */
    CHECK_EQUAL(ERR_NULL_POINTER, ret);

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .withBoolParameter("ptr", false)
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    ret = powerMonitor->suspend();
    /* this should return fail but not call any other functions  */
    CHECK_EQUAL(ERR_IN_ISR, ret);

    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .withBoolParameter("ptr", false)
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    /* expect vTaskSuspend to be called */
    mock().expectOneCall("vTaskSuspend")
        /* we dont care about the value of xTaskToSuspend since we are mocking it */    
        .ignoreOtherParameters();
    ret = powerMonitor->suspend();
    CHECK_EQUAL(ERR_NONE, ret);

    /*  Test with xport returning in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .withBoolParameter("ptr", false)
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(1);
    ret = powerMonitor->resume();
    /* this should return in isr error */
    CHECK_EQUAL(ERR_IN_ISR, ret);

    /*  Test with xport returning not in ISR    */
    mock().expectOneCall("CHECK_POINTER_VALID")
        /* this might or might not be false, not important for this test */
        .withBoolParameter("ptr", false)
        /* we want this to return true so we can continue to other tests*/
        .andReturnValue(true);
    mock().expectOneCall("xPortInIsrContext").andReturnValue(0);
    /* expect vTaskResume to be called */
    mock().expectOneCall("vTaskResume")
        /* ignore value into vtask resume since we are mocking handle above */
        .ignoreOtherParameters();
    ret = powerMonitor->resume();
    CHECK_EQUAL(ERR_NONE, ret);
}