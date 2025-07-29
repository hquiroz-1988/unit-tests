#include "CppUTestExt/MockSupport.h"

#include "esp_log.h"


#include <stdint.h>
#include <stddef.h>
#include <string>

#define BUFFER_SIZE 256
extern "C" void esp_log_write(esp_log_level_t level, const char* tag, const char* format, ...)
{
    char buffer[BUFFER_SIZE]; // Declare a buffer to store the formatted string
    va_list args;             // Declare a va_list to handle variable arguments

    va_start(args, format);   // Initialize va_list
    vsnprintf(buffer, BUFFER_SIZE, format, args); // Format the string into the buffer
    va_end(args);             // Clean up va_list

    mock().actualCall("esp_log_write")
        .withParameter("level", level)
        .withParameter("tag", tag)
        .withParameter("format", buffer);
}

