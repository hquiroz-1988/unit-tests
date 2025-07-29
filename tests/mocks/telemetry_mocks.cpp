#include "CppUTestExt/MockSupport.h"
#include "networking.hpp"

extern "C"
{

}

void NetworkingModule::init(void)
{
    mock().actualCall("initNetworking");
}

Status_t NetworkingModule::queueNetworkingMessage(NetworkingMessage_t * message)
{
    return static_cast<Status_t>(mock().actualCall("queueNetworkingMessage")
          .withBoolParameter("message", message != nullptr)
          .returnIntValueOrDefault(STATUS_UNKNOWN));
}
