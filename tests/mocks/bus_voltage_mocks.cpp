#include "CppUTestExt/MockSupport.h"
#include "bus_voltage.hpp"

extern "C"
{

}

BusVoltage::BusVoltage(ADS1115 & _ads1115) : ADS1115Channel(_ads1115)
{
    mock().actualCall("BusVoltage::BusVoltage")
    .withParameter("_ads1115", &_ads1115);
}

BusVoltage::~BusVoltage()
{
    mock().actualCall("BusVoltage::~BusVoltage");
}

Status_t BusVoltage::initialize(PowerMonitor *_pm)
{
    return static_cast<Status_t>(mock().actualCall("BusVoltage::initialize")
    .withPointerParameter("_pm", _pm)
    .returnIntValueOrDefault(STATUS_UNKNOWN));
}

void BusVoltage::runAlertISR(void * arg)
{
    mock().actualCall("BusVoltage::runAlertISR")
    .withParameter("arg", arg);
}
