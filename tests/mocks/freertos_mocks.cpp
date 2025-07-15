#include "FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{

}

#include <stdint.h>
#include <stddef.h>



TaskHandle_t xTaskGetCurrentTaskHandle( void )
{
    return mock().actualCall("xTaskGetCurrentTaskHandle").returnPointerValueOrDefault(nullptr);
}

uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait ) {
    return mock().actualCall("ulTaskNotifyTake")
        .withParameter("xClearCountOnExit", xClearCountOnExit)
        .withParameter("xTicksToWait", xTicksToWait)
        .returnUnsignedLongIntValueOrDefault(0);
}

BaseType_t xQueueGenericSend( QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition )
{
    return mock().actualCall("xQueueGenericSend")
        .withPointerParameter("xQueue", xQueue)
        .withConstPointerParameter("pvItemToQueue", pvItemToQueue)
        .withParameter("xTicksToWait", xTicksToWait)
        .withParameter("xCopyPosition", xCopyPosition)
        .returnIntValueOrDefault(0);
}   
