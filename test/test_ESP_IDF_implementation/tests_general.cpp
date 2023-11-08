#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "../../One_wire_temp_sensor.h"
#include "../mocks/ESP_IDF_driver/ds18x20.h"
#include <string.h>

#define TEMPERATURE_SENSOR_PIN 4
const int MAX_DEVICES = 10;
const size_t DEVICE_COUNT = 2;
const ds18x20_addr_t addr_list[DEVICE_COUNT] = {560, 230};

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

#define bits

TEST(One_wire_temperature_sensor_esp_idf, GIVEN_resolution_was_not_set_WHEN_get_resolution_THEN_resolution_is_9_bits)
{
    const uint8_t DEFAULT_RESOLUTION = 12 bits;

    CHECK_EQUAL(DEFAULT_RESOLUTION, temp_sensor->get_resolution());
}

TEST(One_wire_temperature_sensor_esp_idf, set_resolution)
{
    const uint8_t RESOLUTION_TO_SET = 12 bits;

    uint8_t TH = 0x00;
    uint8_t TL = 0xFF;
    uint8_t config = 0 + ((RESOLUTION_TO_SET - 9) << 5 bits);
    uint8_t bytes_to_write[3] = { TH, TL, config };

    for (size_t i = 0; i < DEVICE_COUNT; ++i ) {
        mock().expectOneCall("ds18x20_write_scratchpad")
              .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN)
              .withUnsignedLongLongIntParameter("addr", addr_list[i])
              .withMemoryBufferParameter("buffer", bytes_to_write, sizeof(bytes_to_write))
              .andReturnValue(ESP_OK);

        mock().expectOneCall("ds18x20_copy_scratchpad")
              .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN)
              .withUnsignedLongLongIntParameter("addr", addr_list[i])
              .andReturnValue(ESP_OK);
    }
    
    temp_sensor->set_resolution(RESOLUTION_TO_SET);
}

TEST(One_wire_temperature_sensor_esp_idf, WHEN_invalid_resolution_is_set_THEN_set_resolution_has_no_effect)
{
    uint8_t invalid_resolution = 8 bits;
    mock().expectNoCall("ds18x20_write_scratchpad");
    mock().expectNoCall("ds18x20_copy_scratchpad");
    temp_sensor->set_resolution(invalid_resolution);

    invalid_resolution = 13 bits;
    mock().expectNoCall("ds18x20_write_scratchpad");
    mock().expectNoCall("ds18x20_copy_scratchpad");
    temp_sensor->set_resolution(invalid_resolution);
}

TEST(One_wire_temperature_sensor_esp_idf, GIVEN_resolution_is_set_to_12_bits_WHEN_get_resolution_THEN_it_is_12_bits)
{
    const uint8_t RESOLUTION_TO_GET = 12 bits;
    mock().disable();
    temp_sensor->set_resolution(RESOLUTION_TO_GET);
    mock().enable();

    CHECK_EQUAL(RESOLUTION_TO_GET, temp_sensor->get_resolution());
}

TEST(One_wire_temperature_sensor_esp_idf, request_temperatures)
{
    mock().expectOneCall("ds18x20_measure")
          .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN)
          .withUnsignedLongLongIntParameter("addr", DS18X20_ANY)
          .withBoolParameter("wait", false)
          .andReturnValue(ESP_OK);

    temp_sensor->request_temperatures();
}

#define MAX_NUMBERS_PER_BYTE 256
#define BITS_PER_BYTE 8
inline uint16_t get_representation_with_12_bits_and_3_fixed_decimals(float temp_to_convert) {
    return temp_to_convert*16;
}

TEST(One_wire_temperature_sensor_esp_idf, get_temperature_in_celsius)
{
    Device_address device_address = {0, 0, 0, 0, 0, 0, 0, 0};
    device_address[0] = addr_list[0] % MAX_NUMBERS_PER_BYTE;
    device_address[1] = (addr_list[0] >> BITS_PER_BYTE) % MAX_NUMBERS_PER_BYTE;
    
    float TEMPERATURE_IN_CELSIUS = 24.5;

    mock().expectOneCall("ds18b20_read_temperature")
          .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN)
          .withUnsignedLongLongIntParameter("addr", addr_list[0])
          .withOutputParameterReturning("temperature", &TEMPERATURE_IN_CELSIUS, sizeof(TEMPERATURE_IN_CELSIUS))
          .andReturnValue(ESP_OK);

    DOUBLES_EQUAL(TEMPERATURE_IN_CELSIUS, temp_sensor->get_temperature_in_celsius(device_address), 0.000001f);
}

TEST(One_wire_temperature_sensor_esp_idf, request_temperature_BLOCKING)
{
    mock().expectOneCall("ds18x20_measure")
          .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN)
          .withUnsignedLongLongIntParameter("addr", DS18X20_ANY)
          .withBoolParameter("wait", true);
    
    temp_sensor->request_temperature_BLOCKING();
}