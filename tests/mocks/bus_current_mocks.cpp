#include "CppUTestExt/MockSupport.h"
#include "bus_current.hpp"

extern "C"
{

}

void BusCurrent::init(void)
{
    mock().actualCall("initBusCurrent");
}

retVal_t BusCurrent::getFilteredCurrent(float * value)
{
    return static_cast<retVal_t>(mock().actualCall("getFilteredCurrent")
          .withOutputParameter("value", value)
          .returnIntValueOrDefault(ERR_FAIL));
}