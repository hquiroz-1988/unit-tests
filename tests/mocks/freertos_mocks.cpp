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


int xPortInIsrContext(void)
{
    return mock().actualCall("xPortInIsrContext").returnIntValueOrDefault(0);
}

void vTaskSuspend( TaskHandle_t xTaskToSuspend )
{
    mock().actualCall("vTaskSuspend")
        .withPointerParameter("xTaskToSuspend", xTaskToSuspend);
}

void vTaskResume( TaskHandle_t xTaskToResume )
{
    mock().actualCall("vTaskResume")
        .withPointerParameter("xTaskToResume", xTaskToResume);
}

void vTaskDelete( TaskHandle_t xTask )
{
    mock().actualCall("vTaskDelete")
        .withPointerParameter("xTask", xTask);
}

eTaskState eTaskGetState( TaskHandle_t xTask )
{
    return static_cast<eTaskState>(mock().actualCall("eTaskGetState")
        .withPointerParameter("xTask", xTask)
        .returnUnsignedIntValueOrDefault(eDeleted));
}

BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,
							const char * const pcName,		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask )
{
    return mock().actualCall("xTaskCreate")
        .withBoolParameter("pxTaskCode", (pxTaskCode != nullptr))
        .withStringParameter("pcName", pcName)
        .withBoolParameter("usStackDepth", (usStackDepth >= configMINIMAL_STACK_SIZE))
        .withPointerParameter("pvParameters", pvParameters)
        .withParameter("uxPriority", uxPriority)
        .withOutputParameter("pxCreatedTask", pxCreatedTask)
        .returnIntValueOrDefault(pdPASS);
}