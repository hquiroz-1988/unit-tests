#include "CppUTestExt/MockSupport.h"
#include "bus_voltage.hpp"

extern "C"
{

}

void BusVoltage::init(void)
{
    mock().actualCall("initBusVoltage");
}

retVal_t BusVoltage::getFilteredVoltage(float * value)
{
    return static_cast<retVal_t>(mock().actualCall("getFilteredVoltage")
          .withOutputParameter("value", value)
          .returnIntValueOrDefault(ERR_FAIL));
}
