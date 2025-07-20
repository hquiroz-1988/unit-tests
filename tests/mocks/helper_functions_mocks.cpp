#include "CppUTestExt/MockSupport.h"

extern "C"
{
    #include "common.h"
}

#include <stdint.h>
#include <stddef.h>

#ifdef UNIT_TEST

bool FOREVER(void)
{
    return mock()
            .actualCall("FOREVER")
            .returnBoolValueOrDefault(true);
}

bool CHECK_POINTER_VALID(void *ptr)
{
    return mock()
            .actualCall("CHECK_POINTER_VALID")
            .withBoolParameter("ptr", (ptr != nullptr))/** this value might cause issues in thee future */
            .returnBoolValueOrDefault(ptr != nullptr);
}

#endif