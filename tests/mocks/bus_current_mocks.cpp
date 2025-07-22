#include "CppUTestExt/MockSupport.h"
#include "bus_current.hpp"

extern "C"
{

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