#include "../../config.h"
#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#include "../../One_wire_temp_sensor.h"

#if defined(IS_RUNNING_TESTS)
    #include <mocks/ESP_IDF_driver/ds18x20.h>
    #include <mocks/ESP_IDF_driver/esp_idf.h>
#else
    #include "driver/ds18x20.h"
    #include <esp_timer.h>
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

static void convert_ds18x20_addr_TO_device_address(Device_address address_got ,ds18x20_addr_t address_to_convert);

void One_wire_temp_sensor::get_device_address_on_index(Device_address address_to_get, uint8_t index) const {
    if (index >= devices_found)
        return;
    convert_ds18x20_addr_TO_device_address(address_to_get, address_list[index]);
}

#define BYTES_PER_ADDRESS 8
static void convert_ds18x20_addr_TO_device_address(Device_address address_got ,ds18x20_addr_t address_to_convert) {
    for (int i = 0; i < BYTES_PER_ADDRESS; ++i) {
        address_got[i] = address_to_convert & 0xFF;
        address_to_convert >>= 8;
    }
}

uint8_t One_wire_temp_sensor::get_resolution() const {
    return resolution;
}

#define MIN_RESOLUTION 9

void One_wire_temp_sensor::set_resolution(uint8_t new_resolution) {
    if (new_resolution < 9 || new_resolution > 12)
        return;

    uint8_t TH = 0x00;
    uint8_t TL = 0xFF;
    uint8_t config = (new_resolution - MIN_RESOLUTION) << 5;
    uint8_t bytes_to_write[3] = {TH, TL, config};

    for (size_t i = 0; i < devices_found; ++i) {
        ds18x20_write_scratchpad((gpio_num_t)pin_used, address_list[i], bytes_to_write);
        ds18x20_copy_scratchpad((gpio_num_t)pin_used, address_list[i]);
    }
    resolution = new_resolution;
}

#define DO_NOT_WAIT_FOR_CONVERSION false

void One_wire_temp_sensor::request_temperatures() {
    ds18x20_measure((gpio_num_t)pin_used, DS18X20_ANY, DO_NOT_WAIT_FOR_CONVERSION);
    microseconds_since_last_sample_request = esp_timer_get_time();
    is_waiting_sample = true;
}

#define WAIT_FOR_CONVERSION true

void One_wire_temp_sensor::request_temperature_BLOCKING()
{
    is_waiting_sample = false;
    _is_sample_available = true;
    ds18x20_measure((gpio_num_t)pin_used, DS18X20_ANY, WAIT_FOR_CONVERSION);
}

static ds18x20_addr_t get_ds18x20_addr_FROM_device_address(Device_address address);

float One_wire_temp_sensor::get_temperature_in_celsius(Device_address address) const {
    float temperature_to_read;
    ds18x20_addr_t address_to_use = get_ds18x20_addr_FROM_device_address(address);    
    ds18b20_read_temperature((gpio_num_t)pin_used, address_to_use, &temperature_to_read);
    
    return temperature_to_read;
}

#define BITS_PER_BYTE 8
static ds18x20_addr_t get_ds18x20_addr_FROM_device_address(Device_address address) {
    ds18x20_addr_t address_to_return = 0;
    for (int i = BYTES_PER_ADDRESS - 1; i >= 0; --i)
        address_to_return = (address_to_return << BITS_PER_BYTE) + address[i];
    return address_to_return;
}

#define bits
uint16_t One_wire_temp_sensor::get_millis_to_wait_for_conversion(uint8_t resolution) const {
	switch (resolution) {
	case 9 bits:
		return 94;
	case 10 bits:
		return 188;
	case 11 bits:
		return 375;
	default:
		return 750;
	}
}

bool One_wire_temp_sensor::is_sample_available() {
    if (is_waiting_sample)
        return is_time_to_enable_sample();
    else
        return _is_sample_available;
}

bool One_wire_temp_sensor::is_time_to_enable_sample()
{
    int64_t usecs_to_wait_sample = 1000*(int64_t)get_millis_to_wait_for_conversion(resolution);
    if (esp_timer_get_time() - microseconds_since_last_sample_request >= usecs_to_wait_sample)
    {
        is_waiting_sample = false;
        _is_sample_available = true;
        return true;
    }
    else
        return false;
}

#endif