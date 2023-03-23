#include "../../config.h"
#if defined(ESP32_WITH_ARDUINO) || defined(IS_RUNNING_TESTS)

#include "../../One_wire_temp_sensor.h"
#if defined(IS_RUNNING_TESTS)
    #include <mocks/Arduino_driver/OneWire.h>
    #include <mocks/Arduino_driver/DallasTemperature.h>
#else
    #include "driver/OneWire.h"
    #include "driver/DallasTemperature.h"
#endif

One_wire_temp_sensor::One_wire_temp_sensor(uint8_t pin) {
    one_wire = new OneWire(pin);
    temp_sensor = new DallasTemperature(one_wire);
    
    temp_sensor->begin();
    temp_sensor->setWaitForConversion(false);
}

One_wire_temp_sensor::~One_wire_temp_sensor() {
    delete temp_sensor;
    delete one_wire;
}

uint8_t One_wire_temp_sensor::get_device_count() {
    return temp_sensor->getDeviceCount();
}

void One_wire_temp_sensor::get_device_address_on_index(Device_address address_to_get, uint8_t index) const {
    temp_sensor->getAddress(address_to_get, index);
}

uint8_t One_wire_temp_sensor::get_resolution() const {
    return temp_sensor->getResolution();
}

void One_wire_temp_sensor::set_resolution(uint8_t new_resolution) {
    temp_sensor->setResolution(new_resolution);
}

void One_wire_temp_sensor::request_temperatures() const {
    temp_sensor->requestTemperatures();
}

float One_wire_temp_sensor::get_temperature_in_celsius(Device_address address) const {
    float temp = temp_sensor->getTempC(address);
    return temp;
}

uint16_t One_wire_temp_sensor::get_millis_to_wait_for_conversion(uint8_t resolution) const {
    return temp_sensor->millisToWaitForConversion(resolution);
}

#endif