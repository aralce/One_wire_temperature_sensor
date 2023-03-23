#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "../../One_wire_temp_sensor.h"
#include "../mocks/ESP_IDF_driver/ds18x20.h"
#include <string.h>

#define TEMPERATURE_SENSOR_PIN 4
const int MAX_DEVICES = 10;
const size_t DEVICE_COUNT = 2;
const ds18x20_addr_t addr_list[DEVICE_COUNT] = { 560, 230};

One_wire_temp_sensor* temp_sensor = nullptr;

TEST_GROUP(One_wire_temperature_sensor_esp_idf)
{
    void setup ()
    {
        mock().expectOneCall("ds18x20_scan_devices")
            .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN)
            .withOutputParameterReturning("addr_list", (const void*)addr_list, sizeof(addr_list))
            .withUnsignedLongIntParameter("addr_count", MAX_DEVICES)
            .withOutputParameterReturning("found", (const void*)&DEVICE_COUNT, sizeof(DEVICE_COUNT))
            .andReturnValue(ESP_OK);

        temp_sensor = new One_wire_temp_sensor(TEMPERATURE_SENSOR_PIN);
    }
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
        delete temp_sensor;
    }
};

TEST(One_wire_temperature_sensor_esp_idf, get_device_count)
{
    const size_t DEVICE_COUNT = 2;

    mock().expectOneCall("ds18x20_scan_devices")
          .withOutputParameterReturning("found", &DEVICE_COUNT, sizeof(DEVICE_COUNT))
          .ignoreOtherParameters();
    CHECK_EQUAL(DEVICE_COUNT, temp_sensor->get_device_count());
}

#define BYTES_PER_ADDRESS 8

TEST(One_wire_temperature_sensor_esp_idf,  get_device_address_on_index)
{
    uint8_t address[BYTES_PER_ADDRESS];
    const uint8_t INDEX_TO_ASK_FOR_ADDRESS = 0;
    temp_sensor->get_device_address_on_index(address, INDEX_TO_ASK_FOR_ADDRESS);

    uint64_t raw_address = addr_list[INDEX_TO_ASK_FOR_ADDRESS];
    for (int i = 0; i < BYTES_PER_ADDRESS; ++i) {
        CHECK_EQUAL(raw_address & 0xFF, address[i]);
        raw_address >>= BYTES_PER_ADDRESS;
    }
}

TEST(One_wire_temperature_sensor_esp_idf, get_device_address_on_index_WHEN_index_is_invalid_THEN_address_does_not_change) {
    uint8_t address[BYTES_PER_ADDRESS] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t INDEX_TO_ASK_FOR_ADDRESS = MAX_DEVICES;
    temp_sensor->get_device_address_on_index(address, INDEX_TO_ASK_FOR_ADDRESS);

    for (int i = 0; i < BYTES_PER_ADDRESS; ++i)
        CHECK_EQUAL(i, address[i]);
}

// // #define bits
// // const uint8_t RESOLUTION = 14 bits;

// // TEST(One_wire_temperature_sensor_arduino, get_resolution)
// // {
// //     mock().expectOneCall("DallasTemperature->getResolution")
// //           .andReturnValue(RESOLUTION);
    
// //     CHECK_EQUAL(RESOLUTION, temp_sensor->get_resolution());
// // }

// // TEST(One_wire_temperature_sensor_arduino, set_resolution)
// // {
// //     mock().expectOneCall("DallasTemperature->setResolution")
// //           .withUnsignedIntParameter("newResolution", RESOLUTION);
    
// //     temp_sensor->set_resolution(RESOLUTION);
// // }

// // TEST(One_wire_temperature_sensor_arduino, request_temperatures)
// // {
// //     mock().expectOneCall("DallasTemperature->requestTemperatures");
// //     temp_sensor->request_temperatures();
// // }

// // TEST(One_wire_temperature_sensor_arduino, get_temperature_in_celsius)
// // {
// //     DeviceAddress device_address = {1, 2, 3, 4, 5, 6, 7, 8};
// //     float TEMPERATURE_IN_CELSIUS = 24.7;
// //     mock().expectOneCall("DallasTemperature->getTempC")
// //           .withMemoryBufferParameter("deviceAddress", device_address, sizeof(device_address))
// //           .andReturnValue(TEMPERATURE_IN_CELSIUS);
    
// //     Device_address address;
// //     memcpy(address, device_address, sizeof(device_address));
// //     DOUBLES_EQUAL(TEMPERATURE_IN_CELSIUS, temp_sensor->get_temperature_in_celsius(address), 0.000001f);
// // }

// // TEST(One_wire_temperature_sensor_arduino, get_millis_to_wait_for_conversion)
// // {
// //     mock().expectOneCall("DallasTemperature->millisToWaitForConversion");
// //     temp_sensor->get_millis_to_wait_for_conversion(RESOLUTION);
// // }