cmake_minimum_required(VERSION 3.10)
project(solar_meter_tests C CXX)

# Check for CPPUTEST_HOME
if(NOT DEFINED ENV{CPPUTEST_HOME})
    message(FATAL_ERROR "The environment variable CPPUTEST_HOME is not set. Set it to where cpputest is installed.")
endif()

set(CPPUTEST_HOME $ENV{CPPUTEST_HOME})

# Output directories
set(CPPUTEST_OBJS_DIR test-obj)
set(CPPUTEST_LIB_DIR test-lib)

# Component name
set(COMPONENT_NAME solar-meter)

# Source files
set(SRC_FILES "")
if(TEST_POWER_MONITOR)
    list(APPEND SRC_FILES
        ../solar_meter/components/application/power_monitor.cpp
        ../solar_meter/components/common/Task.cpp
    )
endif()
if(TEST_BUS_VOLTAGE)
    list(APPEND SRC_FILES
        ../solar_meter/components/application/bus_voltage.cpp
    )
endif()
if(TEST_ADS1115)
    list(APPEND SRC_FILES
        ../solar_meter/components/device_drivers/ads1115.cpp
    )
endif()

# Test source files
set(TEST_SRC_FILES tests/AllTests.cpp)
if(TEST_POWER_MONITOR)
    list(APPEND TEST_SRC_FILES tests/src/power_monitor_tests.cpp)
endif()
if(TEST_BUS_VOLTAGE)
    list(APPEND TEST_SRC_FILES tests/src/bus_voltage_tests.cpp)
endif()
if(TEST_ADS1115)
    list(APPEND TEST_SRC_FILES tests/src/ads1115_tests.cpp)
endif()

# Mocks
set(MOCKS_SRC_FILES
    tests/mocks/esp_log_mocks.cpp
    tests/mocks/freertos_mocks.cpp
    tests/mocks/helper_functions_mocks.cpp
    tests/mocks/i2c_task_mocks.cpp
    tests/mocks/telemetry_mocks.cpp
)
if(NOT TEST_BUS_VOLTAGE)
    list(APPEND MOCKS_SRC_FILES tests/mocks/bus_voltage_mocks.cpp)
endif()
if(NOT TEST_ADS1115)
    list(APPEND MOCKS_SRC_FILES tests/mocks/ads1115_mocks.cpp)
endif()

# Include directories
include_directories(
    ${CPPUTEST_HOME}/include
    ${CPPUTEST_HOME}/include/Platforms/Gcc
    example-include
    example-fff
    tests/exploding-fakes
    tests/fff
    tests/includes
    ../solar_meter/components/device_drivers/include
    ../solar_meter/components/peripheral_drivers/include
    ../solar_meter/components/common/include
    ../solar_meter/components/application/include
    ../ESP8266_RTOS_SDK/components/freertos/port/esp8266/include
    ../ESP8266_RTOS_SDK/components/freertos/port/esp8266/include/freertos
    ../ESP8266_RTOS_SDK/components/freertos/include
    ../ESP8266_RTOS_SDK/components/freertos/include/freertos
    ../ESP8266_RTOS_SDK/components/freertos/include/freertos/private
    ../ESP8266_RTOS_SDK/components/esp8266/include
    ../ESP8266_RTOS_SDK/components/esp_common/include
    ../ESP8266_RTOS_SDK/components/heap/include
    ../ESP8266_RTOS_SDK/components/heap/port/esp8266/include
    ../ESP8266_RTOS_SDK/components/log/include
)

# Compiler flags
set(CPPUTEST_WARNINGFLAGS
    -Wall -Werror -Wfatal-errors -Wswitch-default
    -Wno-format-nonliteral -Wno-sign-conversion -Wno-pedantic
    -Wno-shadow -Wno-missing-field-initializers -Wno-unused-parameter
)
set(CPPUTEST_CFLAGS
    -pedantic -Wno-missing-prototypes -Wno-strict-prototypes
    -D__ESP_FILE__=__FILE__ -DUNIT_TEST=1 -fprofile-arcs -ftest-coverage
)
set(CPPUTEST_CXXFLAGS
    -DUNIT_TEST=1 -Wno-c++14-compat --std=c++11
    -Wno-c++98-compat-pedantic -Wno-c++98-compat
)
set(CPPUTEST_EXE_FLAGS -c)

# Add executable
add_executable(${COMPONENT_NAME}_tests
    ${SRC_FILES}
    ${TEST_SRC_FILES}
    ${MOCKS_SRC_FILES}
)

# Set compile options
target_compile_options(${COMPONENT_NAME}_tests PRIVATE
    ${CPPUTEST_WARNINGFLAGS}
    $<$<COMPILE_LANGUAGE:C>:${CPPUTEST_CFLAGS}>
    $<$<COMPILE_LANGUAGE:CXX>:${CPPUTEST_CXXFLAGS}>
)

# Link CppUTest
target_link_libraries(${COMPONENT_NAME}_tests
    ${CPPUTEST_HOME}/lib/libCppUTest.a
    ${CPPUTEST_HOME}/lib/libCppUTestExt.a
    # Add other libraries as needed, e.g. m
)

# Set output directories
set_target_properties(${COMPONENT_NAME}_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CPPUTEST_LIB_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CPPUTEST_LIB_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CPPUTEST_LIB_DIR}
)

# Print mocks info
message(STATUS "MOCKS_SRC_FILES: ${MOCKS_SRC_FILES}")

# Optionally, add coverage and colorized output
# (You may need to add custom commands for coverage reporting)
