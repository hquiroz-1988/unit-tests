#include "CppUTestExt/MockSupport.h"
#include "power_monitor.hpp"

extern "C"
{

}


PowerMonitor::PowerMonitor(NetworkingModule & _networking, BusVoltage & _busVoltage, BusCurrent & _busCurrent) 
: Task("test", 2048, ESP_NORMAL_PRIORITY), busVoltage(_busVoltage), busCurrent(_busCurrent), networkingModule(_networking)
{
    mock().actualCall("PowerMonitor");
}

PowerMonitor::~PowerMonitor()
{
    mock().actualCall("~PowerMonitor");
}

void PowerMonitor::taskRun()
{
    mock().actualCall("taskRun");
}

void PowerMonitor::notifyFromISR(void * arg)
{
    mock().actualCall("PowerMonitor::notifyFromISR")
          .withParameter("arg", arg);
}