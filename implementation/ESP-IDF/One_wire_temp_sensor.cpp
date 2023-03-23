#include "../../config.h"
#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#include "../../One_wire_temp_sensor.h"

#if defined(IS_RUNNING_TESTS)
    #include <mocks/ESP_IDF_driver/ds18x20.h>
#else
    #include "driver/ds18x20.h"
#endif

One_wire_temp_sensor::One_wire_temp_sensor(uint8_t pin) : pin_used(pin){
    ds18x20_scan_devices((gpio_num_t)pin_used, (ds18x20_addr_t*)address_list, MAX_NUMBER_OF_SENSORS, &devices_found);
}

One_wire_temp_sensor::~One_wire_temp_sensor() {

}

uint8_t One_wire_temp_sensor::get_device_count() {
    ds18x20_scan_devices((gpio_num_t)pin_used, (ds18x20_addr_t*)address_list, MAX_NUMBER_OF_SENSORS, &devices_found);
    return (uint8_t)devices_found;
}

#define BYTES_PER_ADDRESS 8

void One_wire_temp_sensor::get_device_address_on_index(Device_address address_to_get, uint8_t index) const {
    if (index >= devices_found)
        return;
    uint64_t raw_address = address_list[index];
    for (int i = 0; i < BYTES_PER_ADDRESS; ++i) {
        address_to_get[i] = raw_address & 0xFF;
        raw_address >>= 8;
    }
}

uint8_t One_wire_temp_sensor::get_resolution() const {
    return 0;
}

void One_wire_temp_sensor::set_resolution(uint8_t new_resolution) {

}

void One_wire_temp_sensor::request_temperatures() const {

}

float One_wire_temp_sensor::get_temperature_in_celsius(Device_address address) const {
    return 0;
}

uint16_t One_wire_temp_sensor::get_millis_to_wait_for_conversion(uint8_t resolution) const {
    return 0;
}

#endif