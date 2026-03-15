/**
 *******************************************************************************
 * @file    ads1115_mocks.cpp
 * @author  HQ
 * @date    2025-08-20 23:17:14
 * @brief   
 *******************************************************************************
 */

/*******************************************************************************
 * INCLUDES
*******************************************************************************/
#include "CppUTestExt/MockSupport.h"
#include "ads1115.hpp"

extern "C"
{

}

/*******************************************************************************
 * EXTERN VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE MACROS AND DEFINES
 *******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS
 *******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *******************************************************************************/

/*******************************************************************************
 * STATIC FUNCTIONS
 *******************************************************************************/

/*******************************************************************************
 * GLOBAL FUNCTIONS
 *******************************************************************************/
ADS1115::ADS1115(Gpio & _gpio) : I2CDevice(), alertPin(_gpio)
{
    mock().actualCall("ADS1115");
}

ADS1115::~ADS1115()
{
    mock().actualCall("~ADS1115");
}

 Status_t ADS1115::configure(const ADS1115_Config_t & configObj)
{
    return static_cast<Status_t>(mock()
            .actualCall("ADS1115::configure")
            .withIntParameter("configObj.opStatus", static_cast<int>(configObj.opStatus))
            .withIntParameter("configObj.mux", static_cast<int>(configObj.mux))
            .withIntParameter("configObj.pga", static_cast<int>(configObj.pga))
            .withIntParameter("configObj.mode", static_cast<int>(configObj.mode))
            .withIntParameter("configObj.dataRate", static_cast<int>(configObj.dataRate))
            .withIntParameter("configObj.compMode", static_cast<int>(configObj.compMode))
            .withIntParameter("configObj.compPolarity", static_cast<int>(configObj.compPolarity))
            .withIntParameter("configObj.compLatch", static_cast<int>(configObj.compLatch))
            .withIntParameter("configObj.compQueue", static_cast<int>(configObj.compQueue))
            .returnIntValueOrDefault(STATUS_UNKNOWN));
}

Status_t ADS1115::readADC_SingleEnded(ADS1115Channel & channel)
{
    return static_cast<Status_t>(mock()
            .actualCall("readADC_SingleEnded")
            .withParameter("channel", &channel)
            .returnIntValueOrDefault(STATUS_OKAY));
}

void ADS1115::HAL_GPIO_EXTI_Callback(void * arg)
{
    mock().actualCall("HAL_GPIO_EXTI_Callback")
        .withParameter("arg", arg);
}