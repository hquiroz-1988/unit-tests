#include "CppUTestExt/MockSupport.h"

#include "esp_log.h"


#include <stdint.h>
#include <stddef.h>


extern "C" void esp_log_write(esp_log_level_t level, const char* tag, const char* format, ...)
{
    /* print something on terminal   */
}

