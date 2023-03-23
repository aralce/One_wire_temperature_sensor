#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "../../One_wire_temp_sensor.h"
#include "../mocks/Arduino_driver/DallasTemperature.h"
#include <string.h>

TEST_GROUP(One_wire_temperature_sensor_arduino_init)
{
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

#define TEMPERATURE_SENSOR_PIN 4

TEST(One_wire_temperature_sensor_arduino_init, WHEN_class_is_instanced_THEN_initialize_Dallas_library_and_begin_it)
{
    mock().expectOneCall("OneWire->constructor(uint8_t)")
          .withUnsignedIntParameter("pin", TEMPERATURE_SENSOR_PIN);

    mock().expectOneCall("DallasTemperature->constructor(OneWire*)")
          .ignoreOtherParameters();

    mock().expectOneCall("DallasTemperature->setWaitForConversion")
          .withBoolParameter("flag", false);

    One_wire_temp_sensor temp_sensor(TEMPERATURE_SENSOR_PIN);
}

One_wire_temp_sensor* temp_sensor = nullptr;

TEST_GROUP(One_wire_temperature_sensor_arduino)
{
    void setup()
    {
        mock().disable();
        temp_sensor = new One_wire_temp_sensor(TEMPERATURE_SENSOR_PIN);
        mock().enable();
    }
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
        delete temp_sensor;
    }
};

TEST(One_wire_temperature_sensor_arduino, get_device_count)
{
    const int DEVICE_COUNT = 2;
    mock().expectOneCall("DallasTemperature->getDeviceCount")
          .andReturnValue(DEVICE_COUNT);
    
    CHECK_EQUAL(DEVICE_COUNT, temp_sensor->get_device_count());
}

TEST(One_wire_temperature_sensor_arduino,  get_device_address_on_index)
{
    DeviceAddress device_address = {1, 2, 3, 4, 5, 6, 7, 8};
    const uint8_t INDEX_TO_ASK_FOR_ADDRESS = 0;
    mock().expectOneCall("DallasTemperature->getAddress")
          .withOutputParameterReturning("deviceAddress", (const void*)device_address, sizeof(device_address))
          .withUnsignedIntParameter("index", INDEX_TO_ASK_FOR_ADDRESS)
          .andReturnValue(true);
    
    Device_address address;
    memcpy(address, device_address, sizeof(device_address));
    temp_sensor->get_device_address_on_index(address, INDEX_TO_ASK_FOR_ADDRESS);
}

#define bits
const uint8_t RESOLUTION = 14 bits;

TEST(One_wire_temperature_sensor_arduino, get_resolution)
{
    mock().expectOneCall("DallasTemperature->getResolution")
          .andReturnValue(RESOLUTION);
    
    CHECK_EQUAL(RESOLUTION, temp_sensor->get_resolution());
}

TEST(One_wire_temperature_sensor_arduino, set_resolution)
{
    mock().expectOneCall("DallasTemperature->setResolution")
          .withUnsignedIntParameter("newResolution", RESOLUTION);
    
    temp_sensor->set_resolution(RESOLUTION);
}

TEST(One_wire_temperature_sensor_arduino, request_temperatures)
{
    mock().expectOneCall("DallasTemperature->requestTemperatures");
    temp_sensor->request_temperatures();
}

TEST(One_wire_temperature_sensor_arduino, get_temperature_in_celsius)
{
    DeviceAddress device_address = {1, 2, 3, 4, 5, 6, 7, 8};
    float TEMPERATURE_IN_CELSIUS = 24.7;
    mock().expectOneCall("DallasTemperature->getTempC")
          .withMemoryBufferParameter("deviceAddress", device_address, sizeof(device_address))
          .andReturnValue(TEMPERATURE_IN_CELSIUS);
    
    Device_address address;
    memcpy(address, device_address, sizeof(device_address));
    DOUBLES_EQUAL(TEMPERATURE_IN_CELSIUS, temp_sensor->get_temperature_in_celsius(address), 0.000001f);
}

TEST(One_wire_temperature_sensor_arduino, get_millis_to_wait_for_conversion)
{
    mock().expectOneCall("DallasTemperature->millisToWaitForConversion");
    temp_sensor->get_millis_to_wait_for_conversion(RESOLUTION);
}