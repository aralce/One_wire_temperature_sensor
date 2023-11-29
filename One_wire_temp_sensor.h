#pragma once
#include "config.h"
#include <stdint.h>
#include <stddef.h>

class OneWire;
class DallasTemperature;

typedef uint8_t Device_address[8];

class One_wire_temp_sensor {
public:
    explicit One_wire_temp_sensor(uint8_t pin);
    ~One_wire_temp_sensor();

    uint8_t get_device_count();
    void get_device_address_on_index(Device_address address_to_get, uint8_t index) const;
    
    uint8_t get_resolution() const;
    void set_resolution(uint8_t new_resolution);

    void request_temperatures();

    void request_temperature_BLOCKING();

    float get_temperature_in_celsius(Device_address address) const;

    uint16_t get_millis_to_wait_for_conversion(uint8_t resolution) const;

    bool is_sample_available();

private:
    OneWire* one_wire;
    DallasTemperature* temp_sensor;

    uint8_t pin_used;
    size_t devices_found;
    uint8_t resolution = 12;

    bool is_waiting_sample = false;
    bool _is_sample_available = false;
    int64_t microseconds_since_last_sample_request = 0;

    static const uint8_t MAX_NUMBER_OF_SENSORS = 10;
    uint64_t address_list[MAX_NUMBER_OF_SENSORS];

    bool is_time_to_enable_sample();
};