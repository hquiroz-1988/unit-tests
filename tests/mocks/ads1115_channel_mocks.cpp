#include "CppUTestExt/MockSupport.h"
#include "ads1115_channel.hpp"

extern "C"
{

}

ADS1115Channel::ADS1115Channel(ADS1115 & _ads1115)
: ads1115(_ads1115)
{
    mock().actualCall("ADS1115Channel");
}

ADS1115Channel::~ADS1115Channel()
{
    mock().actualCall("~ADS1115Channel");
}

Status_t ADS1115Channel::init(void)
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel_init").returnIntValueOrDefault(STATUS_OKAY));
}

Status_t ADS1115Channel::startConversion(void)
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::startConversion").returnIntValueOrDefault(STATUS_OKAY));
}

Status_t ADS1115Channel::getConversion(float & value)
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::getConversion")
          .withOutputParameter("value", &value)
          .returnIntValueOrDefault(STATUS_OKAY));
}

Status_t ADS1115Channel::getFilteredVoltage(float * value)
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::getFilteredVoltage")
          .withOutputParameter("value", value)
          .returnIntValueOrDefault(STATUS_OKAY));
}

Status_t ADS1115Channel::setLowThreshold(int16_t value)
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::setLowThreshold")
          .withParameter("value", value)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}

Status_t ADS1115Channel::getLowThreshold(int16_t & value) const
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::getLowThreshold")
          .withOutputParameter("value", &value)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}

Status_t ADS1115Channel::setHighThreshold(int16_t value)
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::setHighThreshold")
          .withParameter("value", value)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}

Status_t ADS1115Channel::getHighThreshold(int16_t & value) const
{
    return static_cast<Status_t>(mock().actualCall("ADS1115Channel::getHighThreshold")
          .withOutputParameter("value", &value)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}

void ADS1115Channel::runAlertISR(void * arg)
{
    mock().actualCall("ADS1115Channel::runAlertISR")
          .withParameter("arg", arg);
}


