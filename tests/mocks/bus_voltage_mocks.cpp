#include "CppUTestExt/MockSupport.h"

extern "C"
{

}

float getVoltage(void)
{
    mock("BusVoltage")
            .actualCall("getVoltage");
    return 0.0f;  // Placeholder return value
}
