#include "CppUTestExt/MockSupport.h"

extern "C"
{

}


#include <stdint.h>
#include <stddef.h>

extern "C" {

void* i2cQueueHdl = nullptr;

void* i2c_cmd_link_create() {
    return mock().actualCall("i2c_cmd_link_create").returnPointerValueOrDefault(nullptr);
}

void i2c_cmd_link_delete(void* cmd_handle) {
    mock().actualCall("i2c_cmd_link_delete").withPointerParameter("cmd_handle", cmd_handle);
}

int i2c_master_start(void* cmd_handle) {
    return mock().actualCall("i2c_master_start").withPointerParameter("cmd_handle", cmd_handle).returnIntValueOrDefault(0);
}

int i2c_master_stop(void* cmd_handle) {
    return mock().actualCall("i2c_master_stop").withPointerParameter("cmd_handle", cmd_handle).returnIntValueOrDefault(0);
}

int i2c_master_write_byte(void* cmd_handle, uint8_t data, bool ack_en) {
    return mock().actualCall("i2c_master_write_byte")
        .withPointerParameter("cmd_handle", cmd_handle)
        .withParameter("data", data)
        .withParameter("ack_en", ack_en)
        .returnIntValueOrDefault(0);
}

int i2c_master_write(void* cmd_handle, const uint8_t* data, size_t data_len, bool ack_en) {
    return mock().actualCall("i2c_master_write")
        .withPointerParameter("cmd_handle", cmd_handle)
        .withPointerParameter("data", (void*)data)
        .withParameter("data_len", (int)data_len)
        .withParameter("ack_en", ack_en)
        .returnIntValueOrDefault(0);
}

int i2c_master_read(void* cmd_handle, uint8_t* data, size_t data_len, int ack) {
    return mock().actualCall("i2c_master_read")
        .withPointerParameter("cmd_handle", cmd_handle)
        .withPointerParameter("data", data)
        .withParameter("data_len", (int)data_len)
        .withParameter("ack", ack)
        .returnIntValueOrDefault(0);
}

int i2c_master_cmd_begin(int i2c_num, void* cmd_handle, uint32_t ticks_to_wait) {
    return mock().actualCall("i2c_master_cmd_begin")
        .withParameter("i2c_num", i2c_num)
        .withPointerParameter("cmd_handle", cmd_handle)
        .withParameter("ticks_to_wait", ticks_to_wait)
        .returnIntValueOrDefault(0);
}


int esp_log_write(uint32_t level, const char* tag, const char* format, ...) {
    return mock().actualCall("esp_log_write")
        .withParameter("level", (int)level)
        .withStringParameter("tag", tag)
        .withStringParameter("format", format)
        .returnIntValueOrDefault(0);
}

}
