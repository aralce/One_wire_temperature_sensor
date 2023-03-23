#pragma once
#include "OneWire.h"
#include "CppUTestExt/MockSupport.h"

// Model IDs
#define DS18S20MODEL 0x10  // also DS1820
#define DS18B20MODEL 0x28  // also MAX31820
#define DS1822MODEL  0x22
#define DS1825MODEL  0x3B  // also MAX31850
#define DS28EA00MODEL 0x42

// Error Codes
#define DEVICE_DISCONNECTED_C -127
#define DEVICE_DISCONNECTED_F -196.6
#define DEVICE_DISCONNECTED_RAW -7040

#define DEVICE_FAULT_OPEN_C -254
#define DEVICE_FAULT_OPEN_F -425.199982
#define DEVICE_FAULT_OPEN_RAW -32512

#define DEVICE_FAULT_SHORTGND_C -253
#define DEVICE_FAULT_SHORTGND_F -423.399994
#define DEVICE_FAULT_SHORTGND_RAW -32384

#define DEVICE_FAULT_SHORTVDD_C -252
#define DEVICE_FAULT_SHORTVDD_F -421.599976
#define DEVICE_FAULT_SHORTVDD_RAW -32256

typedef uint8_t DeviceAddress[8];

class DallasTemperature {
public:

	DallasTemperature(OneWire* oneWire) {
        mock().actualCall("DallasTemperature->constructor(OneWire*)")
              .withPointerParameter("OneWire", (void*)oneWire);
    }

	// initialise bus
	void begin(void) {
        mock().actualCall("DallasTemperature->begin");
    }

	// returns the number of devices found on the bus
	uint8_t getDeviceCount(void) {
        mock().actualCall("DallasTemperature->getDeviceCount");
        return mock().returnUnsignedIntValueOrDefault(0);
    }

	// returns the number of DS18xxx Family devices on bus
	uint8_t getDS18Count(void) {
        mock().actualCall("DallasTemperature->getDS18Count");
        return mock().returnUnsignedIntValueOrDefault(0);
    }

	// finds an address at a given index on the bus
	bool getAddress(uint8_t* deviceAddress, uint8_t index) {
        mock().actualCall("DallasTemperature->getAddress")
              .withOutputParameter("deviceAddress", (void*)deviceAddress)
              .withUnsignedIntParameter("index", index);
        return mock().returnBoolValueOrDefault(false);
    }

	// get global resolution
	uint8_t getResolution() {
        mock().actualCall("DallasTemperature->getResolution");
        return mock().returnUnsignedIntValueOrDefault(0);
    }

	// set global resolution to 9, 10, 11, or 12 bits
	void setResolution(uint8_t newResolution) {
        mock().actualCall("DallasTemperature->setResolution")
              .withUnsignedIntParameter("newResolution", newResolution);
    }

	// returns the device resolution: 9, 10, 11, or 12 bits
	uint8_t getResolution(const uint8_t* deviceAddress);

	// set resolution of a device to 9, 10, 11, or 12 bits
	bool setResolution(const uint8_t* deviceAddress, uint8_t newResolution, bool skipGlobalBitResolutionCalculation = false) {
        mock().actualCall("DallasTemperature->setResolution")
              .withMemoryBufferParameter("deviceAddress", deviceAddress, 8*sizeof(uint8_t))
              .withUnsignedIntParameter("newResolutionn", newResolution);
        return mock().returnBoolValueOrDefault(false);
    }

	// sets/gets the waitForConversion flag
	void setWaitForConversion(bool flag) {
        mock().actualCall("DallasTemperature->setWaitForConversion")
              .withBoolParameter("flag", flag);
    }
	bool getWaitForConversion(void) {
        mock().actualCall("DallasTemperature->getWaitForConversion");
        return mock().returnBoolValueOrDefault(false);
    }

	struct request_t {
		bool result;
		unsigned long timestamp;

		operator bool() {
			return result;
		}
	};

	// sends command for all devices on the bus to perform a temperature conversion
	request_t requestTemperatures(void) {
        mock().actualCall("DallasTemperature->requestTemperatures");
        request_t request;
        return request;
    }

	// returns temperature raw value (12 bit integer of 1/128 degrees C)
	int32_t getTemp(const uint8_t* deviceAddress) {
        mock().actualCall("DallasTemperature->getTemp")
              .withMemoryBufferParameter("deviceAddress", deviceAddress, 8*sizeof(uint8_t));
        return mock().returnLongIntValueOrDefault(0);
    }

	// returns temperature in degrees C
	float getTempC(const uint8_t* deviceAddress) {
        mock().actualCall("DallasTemperature->getTempC")
              .withMemoryBufferParameter("deviceAddress", deviceAddress, 8*sizeof(uint8_t));
        return mock().returnDoubleValueOrDefault(0);
    }

	// returns temperature in degrees F
	float getTempF(const uint8_t* deviceAddress) {
        mock().actualCall("DallasTemperature->getTempF")
              .withMemoryBufferParameter("deviceAddress", deviceAddress, 8*sizeof(uint8_t));
        return mock().returnDoubleValueOrDefault(0);
    }

	// returns true if the bus requires parasite power
	bool isParasitePowerMode(void) {
        mock().actualCall("DallasTemperature->isParasitePowerMode");
        return mock().returnBoolValueOrDefault(false);
    }

	static uint16_t millisToWaitForConversion(uint8_t bitResolution) {
        mock().actualCall("DallasTemperature->millisToWaitForConversion");
        switch (bitResolution) {
        case 9:
            return mock().returnUnsignedLongIntValueOrDefault(94);
        case 10:
            return mock().returnUnsignedLongIntValueOrDefault(188);
        case 11:
            return mock().returnUnsignedIntValueOrDefault(375);
        default:
            return mock().returnUnsignedIntValueOrDefault(750);
        }
    }

};