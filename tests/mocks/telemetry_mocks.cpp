#include "CppUTestExt/MockSupport.h"
#include "telemetry.hpp"

extern "C"
{

}

void Telemetry::init(void)
{
    mock().actualCall("initTelemetry");
}

Status_t Telemetry::sendPacket(telemetry_packet_t * packet)
{
    return static_cast<Status_t>(mock().actualCall("sendPacket")
          .withParameter("packet", packet)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}
