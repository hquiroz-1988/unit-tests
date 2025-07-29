#include "CppUTestExt/MockSupport.h"
#include "bus_voltage.hpp"

extern "C"
{

}

void BusVoltage::init(void)
{
    mock().actualCall("initBusVoltage");
}

Status_t BusVoltage::getFilteredVoltage(float * value)
{
    return static_cast<Status_t>(mock().actualCall("getFilteredVoltage")
          .withOutputParameter("value", value)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}
