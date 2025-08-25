#include "CppUTestExt/MockSupport.h"
#include "bus_current.hpp"

extern "C"
{

}


BusCurrent::BusCurrent(INA219 & _ina219, PowerMonitor * _pm)
{
    (void)_ina219;
    (void)_pm;
    mock().actualCall("BusCurrent");
}

BusCurrent::~BusCurrent()
{
    mock().actualCall("~BusCurrent");
}

void BusCurrent::init(void)
{
    mock().actualCall("initBusCurrent");
}

Status_t BusCurrent::getFilteredCurrent(float * value)
{
    return static_cast<Status_t>(mock().actualCall("getFilteredCurrent")
          .withOutputParameter("value", value)
          .returnIntValueOrDefault(STATUS_OKAY));
}