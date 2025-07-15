#include "CppUTestExt/MockSupport.h"

#include "driver/i2c.h"


#include <stdint.h>
#include <stddef.h>

extern "C" {

void* i2cQueueHdl = nullptr;

i2c_cmd_handle_t i2c_cmd_link_create(void) 
{
    return mock().actualCall("i2c_cmd_link_create").returnPointerValueOrDefault(nullptr);
}

void i2c_cmd_link_delete(i2c_cmd_handle_t cmd_handle)
{
    mock().actualCall("i2c_cmd_link_delete").withPointerParameter("cmd_handle", cmd_handle);
}

esp_err_t i2c_master_start(i2c_cmd_handle_t cmd_handle) 
{
    return mock().actualCall("i2c_master_start").withPointerParameter("cmd_handle", cmd_handle).returnIntValueOrDefault(0);
}

esp_err_t i2c_master_stop(i2c_cmd_handle_t cmd_handle)
{
    return mock().actualCall("i2c_master_stop").withPointerParameter("cmd_handle", cmd_handle).returnIntValueOrDefault(0);
}

esp_err_t i2c_master_write_byte(i2c_cmd_handle_t cmd_handle, uint8_t data, bool ack_en)
{
    return mock().actualCall("i2c_master_write_byte")
        .withPointerParameter("cmd_handle", cmd_handle)
        .withParameter("data", data)
        .withParameter("ack_en", ack_en)
        .returnIntValueOrDefault(0);
}

esp_err_t i2c_master_write(i2c_cmd_handle_t cmd_handle, uint8_t *data, size_t data_len, bool ack_en)
{
    return mock().actualCall("i2c_master_write")
        .withPointerParameter("cmd_handle", cmd_handle)
        .withPointerParameter("data", (void*)data)
        .withParameter("data_len", (int)data_len)
        .withParameter("ack_en", ack_en)
        .returnIntValueOrDefault(0);
}

esp_err_t i2c_master_read(i2c_cmd_handle_t cmd_handle, uint8_t *data, size_t data_len, i2c_ack_type_t ack)
{
    return mock().actualCall("i2c_master_read")
        .withPointerParameter("cmd_handle", cmd_handle)
        .withPointerParameter("data", data)
        .withParameter("data_len", (int)data_len)
        .withParameter("ack", ack)
        .returnIntValueOrDefault(0);
}

esp_err_t i2c_master_cmd_begin(i2c_port_t i2c_num, i2c_cmd_handle_t cmd_handle, TickType_t ticks_to_wait)
{
    return mock().actualCall("i2c_master_cmd_begin")
        .withParameter("i2c_num", i2c_num)
        .withPointerParameter("cmd_handle", cmd_handle)
        .withParameter("ticks_to_wait", ticks_to_wait)
        .returnIntValueOrDefault(0);
}

}
